#include "interaction/ATE/sig_connection_manager.h"

#include <signal.h>

#include "ate_message_adapter.h"
#include "logger/logger.h"
#include "message_factory/message_factory.h"

namespace {

volatile std::atomic_bool sig_term;
volatile std::atomic_bool sig_int;

void HandleSigTerm(int signum) { SIGTERM == signum ? sig_term.store(true) : sig_int.store(true); }

void HandleSigAbort(int signum) {
  logger::info("[signal] Signal {} has been caught", signum);

  fflush(nullptr);
  struct sigaction act;
  std::memset(&act, 0, sizeof(act));

  act.sa_handler = SIG_DFL;
  sigaction(SIGABRT, &act, nullptr);
  raise(SIGABRT);
  exit(1);  // never called, just in case
}

}  // namespace

namespace interaction {

SigConnectionManager::SigConnectionManager(boost::asio::io_context& io_context, IMessageAdapter& ate_message_adapter)
    : io_context_{io_context}, ate_message_adapter_{ate_message_adapter}, running_{false} {}

SigConnectionManager::~SigConnectionManager() { Stop(); }

void SigConnectionManager::Start() {
  running_ = true;
  sig_term = sig_int = false;

  loop_ = std::thread([this]() {
    timespec const kTimeWait{1, 0};
    timespec time_remain;

    while (true) {
      while (running_ && !sig_term && !sig_int) {
        nanosleep(&kTimeWait, &time_remain);  // sleep 1s
      }

      if (!running_) {
        break;
      }

      if (sig_term || sig_int) {
        this->Terminate();

        if (sig_term) {
          sig_term.store(false);
        }
        if (sig_int) {
          sig_int.store(false);
        }

        break;
      }
    }
  });

  const int kSignals[] = {SIGHUP, SIGTERM, SIGINT, SIGABRT};
  for (int signum : kSignals) {
    struct sigaction act;
    std::memset(&act, 0, sizeof(act));
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, signum);

    switch (signum) {
      case SIGABRT:
        act.sa_handler = HandleSigAbort;
        break;
      default:
        act.sa_handler = HandleSigTerm;
        break;
    }
    sigaction(signum, &act, nullptr);
  }
}

void SigConnectionManager::Stop() {
  running_ = false;
  if (loop_.joinable()) {
    loop_.join();
  }
}

void SigConnectionManager::Terminate() {
  logger::info("[signal] Signal {} has been caught", sig_term ? SIGTERM : SIGINT);
  if (!io_context_.stopped()) {
    io_context_.stop();
  }
}

}  // namespace interaction
