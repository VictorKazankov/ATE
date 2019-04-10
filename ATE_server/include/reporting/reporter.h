#ifndef ATE_SERVER_REPORTING_REPORTER_H_
#define ATE_SERVER_REPORTING_REPORTER_H_

namespace reporter {
/**
 * @brief Interface class for reporting test results
 **/
class Reporter {
 public:
  virtual ~Reporter() = default;

  /**
   * @brief Record data to the report
   **/
  virtual void Log() = 0;

  /**
   * @brief Close report descriptor
   **/
  virtual void Close() = 0;
};
}  // namespace reporter

#endif  // ATE_SERVER_REPORTING_REPORTER_H_
