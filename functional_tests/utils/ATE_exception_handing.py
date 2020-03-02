import logging
from functools import wraps
from sys import exit as sys_exit
from time import sleep

from pytest import exit as pytest_exit

from vhat_client import VideoStreamingError, LookupError
from functional_tests.config_reader import read_configuration


SECTION_NAME = 'RUN_MODE'
PARAM_NAME = 'TEST_RUN_MODE'
TEXT_FOR_EXIT_WHEN_VideoStreamingError = "Cancel pytest execution because VideoStreamingError after {} tries"

try:
    section_name = read_configuration(SECTION_NAME)[PARAM_NAME]
    exc_conf = read_configuration(section_name)
    logging.debug('Reading handing exception configuration for {} MODE'.format(exc_conf))
except KeyError:
    raise KeyError('Configuration is missing. Check that config [{}].{}'.format(SECTION_NAME, PARAM_NAME))


def get_bool_config_value(name):
    if exc_conf[name].lower() == 'true':
        return True
    elif exc_conf[name].lower() == 'false':
        return False
    else:
        raise KeyError("Wrong value. Correct values for [{}] are: [True or False]".format(name))


try:
    IS_EXIT_WHEN_VideoStreamingError = get_bool_config_value('IS_EXIT_WHEN_VideoStreamingError')
    MAX_CNT_TRIES = int(exc_conf['MAX_CNT_TRIES'])
    SLEEP_BETWEEN_TRIES_SEC = float(exc_conf['SLEEP_BETWEEN_TRIES_SEC'])
    TEXT_FOR_EXIT_WHEN_VideoStreamingError = TEXT_FOR_EXIT_WHEN_VideoStreamingError.format(MAX_CNT_TRIES)
    IS_DEV_MODE_FOR_LookupError = get_bool_config_value('IS_DEV_MODE_FOR_LookupError')
    MAX_CNT_TRIES_LookupError = int(exc_conf['MAX_CNT_TRIES_LookupError'])
    WAIT_FOR_DEV_MODE_SEC = float(exc_conf['WAIT_FOR_DEV_MODE_SEC'])
except KeyError as e:
    raise KeyError("Configuration is missing. Check [{}] - param [{}]".format(exc_conf, e))


def finish_pytest_runs(text):
    logging.info(text)
    pytest_exit(text)


def catch_exception_video(function):
    @wraps(function)
    def wrapper(*args, **kwargs):
        for attempt in range(MAX_CNT_TRIES):
            try:
                return function(*args, **kwargs)
            except VideoStreamingError:
                logging.info("got {}, after attempt {} from {}. Going to sleep {} sec"
                             .format(str(VideoStreamingError), attempt, MAX_CNT_TRIES, SLEEP_BETWEEN_TRIES_SEC))
                sleep(SLEEP_BETWEEN_TRIES_SEC)
        logging.info("still getting {}, after {} attempts.".format(str(VideoStreamingError), MAX_CNT_TRIES))
        if IS_EXIT_WHEN_VideoStreamingError:
            finish_pytest_runs(TEXT_FOR_EXIT_WHEN_VideoStreamingError)
        else:
            logging.info("Exit from test using sys.exit(1) because of {}".format(str(VideoStreamingError)))
            sys_exit(1)

    return wrapper


def catch_exception_lookup(function):
    @wraps(function)
    def wrapper(*args, **kwargs):
        for attempt in range(MAX_CNT_TRIES_LookupError):
            try:
                return function(*args, **kwargs)
            except LookupError:
                if IS_DEV_MODE_FOR_LookupError and attempt == 0:
                    logging.debug("DEV MODE is turned ON. GOT LookupError so we are waiting {} sec.".format(WAIT_FOR_DEV_MODE_SEC))
                    sleep(WAIT_FOR_DEV_MODE_SEC)
                elif not IS_DEV_MODE_FOR_LookupError:
                    logging.info("DEV MODE is turned OFF. Can`t found object")
                    raise LookupError
                else:
                    logging.info("Can`t found object")
                    raise LookupError
    return wrapper
