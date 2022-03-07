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

struct new_metrics {
  std::unordered_map<std::string, int> hashmap;
  int countLines{0};
  long int start{0};
  int duration;

  explicit new_metrics(int duration) : duration{duration} {}

  void updateMetrics(long int value, std::string section) {
    if (start == 0) this->start = value;

    if (this->hashmap.find(section) == this->hashmap.end())
      this->hashmap[section] = 1;
    else
      this->hashmap[section] += 1;

    ++countLines;
  }

  int getStart() { return this->start; }

  int getDuration() { return this->duration; }

  int getCountLines() { return this->countLines; }

  void resetMetrics() {
    this->start = 0;
    this->hashmap.clear();
    countLines = 0;
  }
};

struct Alert {
  int threshold;
  bool highTraffic{false};

  Alert(int threshold) : threshold{threshold} {}

  bool isThresholdReached(struct new_metrics m, int currentTime) {
    return ((currentTime - m.getStart()) > m.getDuration());
  }

  void countOcurrences(struct new_metrics m) {
    for(const auto val : m.hashmap) {
      std::cout << val.first << " " << val.second << std::endl;
    }

  }

  void generateAlert(struct new_metrics m, int currentTime) {
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

int main() {
  std::ifstream myFile(
      "/Users/lauro/Documents/workspace/http_log_monitoring/src/"
      "sample_csv.txt");

  //variables
  std::string line;
  std::string header;
  std::vector<std::string> parsedLine;
  std::string request;
  long int timestamp;
  struct new_metrics crono(60);
  struct Alert alert {10 * 120}; // 10 requests per second, 2min

  // readHeader
  getline(myFile, header);
  auto idxTime = 3;
  auto idxRequest = 4;
  std::unordered_multimap<int,std::string> myMultimap;

  while (getline(myFile, line)) {
    Parser::parseString(line, parsedLine,',');
    timestamp = std::stol(parsedLine[idxTime]);
    request = std::move(parsedLine[idxRequest]);


    myMultimap.insert({timestamp,request});


    crono.updateMetrics(timestamp, request);

    if (alert.isThresholdReached(crono, timestamp)) {
      alert.generateAlert(crono, timestamp);
      crono.resetMetrics();
    }
  }
  std::cout << "multimap size " << myMultimap.size() << std::endl;
  auto range = myMultimap.equal_range(1549573860);

  for_each (
      range.first,
      range.second,
      [](std::unordered_multimap<int,std::string> ::value_type& x){std::cout << " " << x.second;}
  );

  //1549573860

  /*
  for(auto v : myMultimap) {

  }*/

  return 0;
}




//std::chrono::steady_clock::time_point begin =
//    std::chrono::steady_clock::now();


//Parser::parseRequest(parsedLine[idxRequest], parsedRequest);
//resource = std::move(parsedRequest[1]);
//Parser::parseResource(resource, parsedResources);
//section = std::move(parsedResources[1]);




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