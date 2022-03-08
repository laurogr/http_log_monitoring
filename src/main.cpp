#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// https://httpd.apache.org/docs/2.4/logs.html
// https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/

class Parser {
 public:
  static void parseString(const std::string& line,
                          std::vector<std::string>& parsedString,
                          char delimiter) {
    boost::split(parsedString, line,
                 [delimiter](char c) { return c == delimiter; });
  }
};


struct LogLines {
  std::unordered_map<long int, std::vector<std::string>>
      hashmap;  // every second will have an element
  int countLines{0};
  long int start{0};
  int duration;

  explicit LogLines(int duration) : duration{duration} {}

  void update(long int timestamp, std::string section) {
    if (start == 0) this->start = timestamp;

    if (this->hashmap.find(timestamp) == this->hashmap.end())
      this->hashmap[timestamp] = std::move(std::vector<std::string>{section});
    else
      this->hashmap[timestamp].push_back(section);

    ++countLines;
  }

  long int getStart() const { return this->start; }

  int getDuration() const { return this->duration; }

  int getCountLines() const { return this->countLines; }
};

/*
void alert2min() {
  int eventCounter = 0;
  max_timestamp = (max_timestamp > timestamp)
                      ? max_timestamp
                      : timestamp;  // bug unordered log lines

 //sum of evenst for each second in the period
  for (int i = max_timestamp - 120; i < max_timestamp; ++i) {
    if (logLine.hashmap.find(i) != logLine.hashmap.end()) {
      eventCounter += logLine.hashmap[i].count;
    }
  }

  //if it tis bigger than the threshold
  if (eventCounter >= 1200 && !hightraffic) {
    if (alertmap.find(timestamp) == alertmap.end()) {
      std::cout << "high traffic : " << eventCounter
                << " problem at : " << timestamp << std::endl;
      hightraffic = true;
      alertmap[timestamp] = true;
    }
  }

  if (hightraffic && eventCounter < 1200) {
    std::cout << "normal traffic : " << eventCounter << " recovered at : "
              << " " << timestamp << std::endl;
    hightraffic = false;
  }
}
*/

// vector of alerts

struct req {
  std::string httpMethod;
  std::string resource;
};

int main() {
  std::ifstream myFile(
      "/Users/lauro/Documents/workspace/http_log_monitoring/src/"
      "sample_small.txt");

  // variables
  std::string line;
  std::string header;
  std::vector<std::string> parsedLine;
  std::string request;
  std::string resource;
  std::string section;
  long int timestamp;
  struct LogLines logLine(60);

  // readHeader
  getline(myFile, header);
  auto idxTime = 3;
  auto idxRequest = 4;
  std::unordered_multimap<int, std::string> myMultimap;

  std::unordered_map<int, int> ct;
  int lastTimeStamp;

  bool hightraffic = false;
  std::unordered_map<int, bool> alertmap;
  std::unordered_map<int, bool> alertmap10s;
  int max_timestamp = 0;

  while (getline(myFile, line)) {
    Parser::parseString(line, parsedLine, ',');
    timestamp = std::stol(parsedLine[idxTime]);
    request = std::move(parsedLine[idxRequest]);

    logLine.update(timestamp, request);

    // loop to check the alerts (could be another thread)

    // ****************** ALERT 10s  ******************
    // set lastTimestap, when it is greater than 10 or here, just to % 10
    int myCount = 0;
    if (timestamp % 10 == 0) {
      if (alertmap.find(timestamp) == alertmap.end()) {
        std::unordered_map<std::string,int> sectionCounter;

        for (int i = timestamp - 10; i < timestamp; ++i) { //looping all the seconds here
          myCount += logLine.hashmap[i].size(); //getting the number of connection int 10s windows

          //for (auto section : logLine.hashmap[i]) { //parse the section
          //  std::cout << section << std::endl;
          //}

        }
        alertmap[timestamp] = true;
        std::cout << "INFO : " << myCount << " connections between " << timestamp - 10
                  << " et " << timestamp  << std::endl;
      }
    }

    // ****************** ALERT 2 MIN ******************

    // ****************** ALERT 2 MIN ******************

    //}

    /*
    auto val = timestamp % 120;
    if( val == 0 && timestamp != lastTimeStamp) {
      lastTimeStamp = timestamp;
      std::cout << "timestamp = " << timestamp << std::endl;

      int events = 0;
      for(int i = timestamp - 120; i < timestamp; ++i) {
        if(logLine.hashmap.find(i) != logLine.hashmap.end()) {
          events += logLine.hashmap[i].count;
        }
      }
      std::cout << "events timestamp -120 " << (timestamp - 120) << " a
    timestamp " << timestamp << " events : "<< events << std::endl; if(events >
    1200) std::cout << "high traffic" << std::endl;
    }*/
  }

  /*for(auto v : ct) {
    std::cout << v.first << " " << v.second << std::endl;
  }*/

  /*
  for(auto v : logLine.hashmap) {
    std::cout << v.first << " " << v.second.count << "\n";
    for(auto section : v.second.hashmapSection) {
      std::cout << section.first << " " << section.second <<"\n";
    }
  }*/

  // 1549573860

  return 0;
}

struct Alert {
  int threshold;
  bool highTraffic{false};

  Alert(int threshold) : threshold{threshold} {}

  bool isThresholdReached(struct LogLines m, int currentTime) {
    return ((currentTime - m.getStart()) > m.getDuration());
  }

  void countOcurrences(struct LogLines m) {
    for (const auto val : m.hashmap) {
      //std::cout << val.first << " " << val.second.count << std::endl;
    }
  }

  void generateAlert(struct LogLines m, int currentTime) {
    std::cout << "Rule of " << m.getDuration() << " | " << currentTime << " "
              << m.getStart() << " Number of events : " << m.getCountLines()
              << std::endl;
    this->countOcurrences(m);

    auto countLines{m.getCountLines()};

    if (countLines > threshold && !highTraffic) {
      std::cout << "High Traffic" << std::endl;
      highTraffic = true;
    }

    if (highTraffic && countLines < threshold) {
      std::cout << "Traffic ok" << std::endl;
      highTraffic = false;
    }
  }
};

/*
 *


  std::cout << "multimap size " << myMultimap.size() << std::endl;
  auto range = myMultimap.equal_range(1549573860);

for_each (
    range.first,
    range.second,
    [](std::unordered_multimap<int,std::string> ::value_type& x){std::cout << "
" << x.second;}
);


 * */
// std::chrono::steady_clock::time_point begin =
//     std::chrono::steady_clock::now();

// Parser::parseRequest(parsedLine[idxRequest], parsedRequest);
// resource = std::move(parsedRequest[1]);
// Parser::parseResource(resource, parsedResources);
// section = std::move(parsedResources[1]);

/*

  static void parseLine(const std::string& line,
                        std::vector<std::string>& parsedLine,
                        char delimiter = ',') {
    parseString(line, parsedLine, delimiter);
  }

static void parseRequest(const std::string& line,
                         std::vector<std::string>& parsedRequest,
                         char delimiter = ' ') {
  parseString(line, parsedRequest, delimiter);
}

static void parseResource(const std::string& line,
                          std::vector<std::string>& parsedResource,
                          char delimiter = '/') {
  parseString(line, parsedResource, delimiter);
}


 * */

struct metrics {
  std::unordered_map<std::string, int> hashmap;
  int countLines{0};
  long int start{0};
  int secondsRule;
  metrics(int secondsRule) : secondsRule{secondsRule} {}

  void updateMetrics(long int value, std::string section) {
    if (start == 0) this->start = value;

    if (this->hashmap.find(section) == this->hashmap.end())
      this->hashmap[section] = 1;
    else
      this->hashmap[section] += 1;

    ++countLines;
    resetMetrics(value);
  }

  // Reset the metrics if the delta of time is bigger than the rule defined
  // leave this function only with the start = 0 and hashmap clear
  // the alert business logic will check when to call it using the if
  // if ((value - this->start) > this->secondsRule) {
  void resetMetrics(long int value) {
    if ((value - this->start) > this->secondsRule) {
      this->start = 0;
      printMetrics(value);
      this->hashmap.clear();
      countLines = 0;
    }
  }

  virtual void printMetrics(long int value) {
    std::cout << "Rule of " << secondsRule << " | " << value << " " << start
              << std::endl;

    for (auto val : this->hashmap) {
      std::cout << val.first << " " << val.second << std::endl;
    }
  }
};

struct warning_metrics : metrics {
  int alertThreshold;
  int lastCountLines;
  // data structure (circular maybe, to save the metrics)

  warning_metrics(int secondsRule, int alertThreshold)
      : metrics(secondsRule), alertThreshold{alertThreshold} {}

  void printMetrics(long value) override {
    std::cout << "WARNINF METRICS : Rule of " << secondsRule << " | " << value
              << " " << start << std::endl;

    for (auto val : this->hashmap) {
      std::cout << "WARNINF METRICS : " << val.first << " " << val.second
                << std::endl;
    }

    std::cout << "WARNINF METRICS : total of connections " << countLines
              << std::endl;
    // 10 requests per second
    // average = countConnections();
    alertThreshold = 120 * 10;
    if (countLines > alertThreshold) {
      auto msg =
          "High traffic generated an alert - hits = {value}, triggered at "
          "{time}";
      std::cout << msg << std::endl;
    }

    if (lastCountLines > alertThreshold && countLines < alertThreshold)
      std::cout << "ALL GOOD NOW" << std::endl;

    lastCountLines = countLines;
  }
};