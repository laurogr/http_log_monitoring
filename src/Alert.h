#pragma once

#include "Buffer.h"
#include "Parser.h"

// Base class for 2 type of alerts
class Alert {
 public:
  virtual void check(const struct LogBuffer& logBuffer, long int timestamp) = 0;

  virtual void printAlert(const struct LogBuffer&, long int timestamp,
                          const std::string& msg) = 0;

  static std::string getReadableFormatTime(long int timestamp) {
    std::time_t temp = timestamp;
    std::tm* t = std::gmtime(&temp);
    std::stringstream ss;
    ss << std::put_time(t, "%Y-%m-%d %I:%M:%S %p");
    return ss.str();
  }
  virtual ~Alert() = default;
  long int lastAlertTimestamp{0};
};

// Alert Based on average numbers of connections per second.
class AlertAverage : public Alert {
 private:
  enum class AlertStates { normalTraffic, highTraffic };
  AlertStates alertState{AlertStates::normalTraffic};
  int thresholdConnectionPerSecond;

 public:
  explicit AlertAverage(int thresholdConnectionPerSecond)
      : thresholdConnectionPerSecond{thresholdConnectionPerSecond} {}
  ~AlertAverage() override = default;

  // TODO : check design pattern to avoid if conditions on the state
  void check(const struct LogBuffer& logBuffer, long int timestamp) override {
    auto averageConnections = logBuffer.getAverageConnections();

    // detecting high traffic: checking threshold, state and if is not a late
    // timestamp already analyzed
    if (averageConnections > thresholdConnectionPerSecond &&
        alertState == AlertStates::normalTraffic &&
        timestamp > lastAlertTimestamp) {
      alertState = AlertStates::highTraffic;
      lastAlertTimestamp = timestamp;
      printAlert(logBuffer, timestamp, "High Traffic");
    }

    // detecting normal traffic after a hightraffic event
    if (alertState == AlertStates::highTraffic &&
        averageConnections <= thresholdConnectionPerSecond &&
        timestamp > lastAlertTimestamp) {
      alertState = AlertStates::normalTraffic;
      lastAlertTimestamp = timestamp;
      printAlert(logBuffer, timestamp, "Normal Traffic");
    }
  }

  void printAlert(const struct LogBuffer& logBuffer, long int timestamp,
                  const std::string& msg) override {
    // TODO : use string builder
    std::cout << "[" << getReadableFormatTime(timestamp) << "] " << msg << "\n";
  }
};

// Alert Based on recurrent time period, for instance, 10s
class AlertRecurrent : public Alert {
 public:
  explicit AlertRecurrent(int recurrence) : recurrence{recurrence} {}
  ~AlertRecurrent() override = default;

  void check(const struct LogBuffer& logBuffer, long int timestamp) override {
    if (timestamp % recurrence == 0 && timestamp > lastAlertTimestamp) {
      lastAlertTimestamp = timestamp;
      printAlert(logBuffer, timestamp,
                 std::to_string(getConnectionNumber(logBuffer, timestamp)));
    }
  }

  void printAlert(const struct LogBuffer& logBuffer, long int timestamp,
                  const std::string& msg) override {
    // TODO : use string builder
    std::cout << "[" << getReadableFormatTime(timestamp)
              << "][recurrent] # connections in the last " << recurrence
              << " seconds : " << msg << "\n";

    // TODO : WIP, check below
    printConnectionInformation(logBuffer, timestamp);
  }

  [[nodiscard]] int getConnectionNumber(const LogBuffer& logBuffer,
                                        long timestamp) const {
    auto totalNumberConnections{0};
    for (long int i = timestamp - recurrence; i < timestamp; ++i)
      totalNumberConnections += logBuffer.getLineBuffer(i).getCount();

    return totalNumberConnections;
  }

  void printConnectionInformation(const LogBuffer& logBuffer,
                                  long timestamp) const {
    auto map = getSectionCounterHashmap(logBuffer, timestamp);
    for (const auto& v : map)
      std::cout << "/" << v.first << " " << v.second << " - hits\n";
  }

  // TODO : improve aggregation/calculation of information, simplify it
  std::unordered_map<std::string, int> getSectionCounterHashmap(
      const LogBuffer& logBuffer, long timestamp) const {
    std::unordered_map<std::string, int> sectionCounterHashmap;

    // getting the information of each second and aggregating
    for (long int i = timestamp - recurrence; i < timestamp; ++i) {
      auto hash = logBuffer.getLineBuffer(i).getHashmapRequest();
      for (const auto& v : hash) {
        auto section = Parser::parseSectionFromResource(v.first);
        if (sectionCounterHashmap.find(section) ==
            sectionCounterHashmap.end()) {
          sectionCounterHashmap[section] = v.second;
        } else {
          sectionCounterHashmap[section] += v.second;
        }
      }
    }
    return sectionCounterHashmap;
  }

 private:
  std::unordered_map<long int, bool> checkedTimestamps;
  int recurrence;
};
