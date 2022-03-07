#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// https://httpd.apache.org/docs/2.4/logs.html

void readHeader();

void printDebug(std::vector<std::string>& vec) {
  for (int i = 0; i < vec.size(); ++i) {
    std::cout << vec[i] << " ";
  }
  std::cout << "\n";
}

class Parser {
  static void parseString(const std::string& line,
                          std::vector<std::string>& parsedString,
                          char delimiter) {
    boost::split(parsedString, line,
                 [delimiter](char c) { return c == delimiter; });
  }

 public:
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
};
// ingest level metrics

struct field {
  std::string name;
  int index;
  std::string value;
};

struct line {
  std::vector<struct field> fields;
};

struct metrics {
  std::unordered_map<std::string, int> hashmap;
  long int start{0};
  int secondsRule;
  metrics(int secondsRule) : secondsRule{secondsRule} {}

  void updateMetrics(long int value, std::string section) {
    if (start == 0)
      this->start = value;

    if (this->hashmap.find(section) == this->hashmap.end())
      this->hashmap[section] = 1;
    else
      this->hashmap[section] += 1;

    checkMetricsRule(value);
  }

  void checkMetricsRule(long int value) {
    if ((value - this->start) > this->secondsRule) {
      this->start = 0;
      saveMetrics(value);
      this->hashmap.clear();
    }
  }

  void saveMetrics(long int value) {
    std::cout << "Rule of " << secondsRule << " | " << value << " " << start
              << std::endl;

    for (auto val : this->hashmap) {
      std::cout << val.first << " " << val.second << std::endl;
    }
  }

};

struct warning_metrics : metrics {
  int alertThreshold;

  warning_metrics(int secondsRule, int alertThreshold)
      : metrics(secondsRule), alertThreshold{alertThreshold} {}


};

int main() {
  std::string header;
  std::string line;
  // std::ifstream
  // myFile("/Users/lauro/Documents/workspace/http_log_monitoring/src/sample_small.txt");
  std::ifstream myFile(
      "/Users/lauro/Documents/workspace/http_log_monitoring/src/"
      "sample_csv.txt");
  // std::ifstream
  // myFile("/Users/lauro/Documents/workspace/http_log_monitoring/src/sample_huge.txt");

  std::unordered_map<std::string, int> hashmap;
  std::vector<std::string> parsedLine;
  std::vector<std::string> parsedRequest;
  std::vector<std::string> parsedResources;

  std::string request;
  std::string resource;
  std::string section;
  long int timestamp;

  // readHeader
  getline(myFile, header);
  auto idxTime = 3;
  auto idxRequest = 4;
  auto timer = 0;

  struct metrics crono {
    60
  };

  std::chrono::steady_clock::time_point begin =
      std::chrono::steady_clock::now();
  // process File
  while (getline(myFile, line)) {
    Parser::parseLine(line, parsedLine);

    Parser::parseRequest(parsedLine[idxRequest], parsedRequest);
    resource = std::move(parsedRequest[1]);

    Parser::parseResource(resource, parsedResources);
    section = std::move(parsedResources[1]);

    timestamp = std::stol(parsedLine[idxTime]);
    crono.updateMetrics(timestamp, section);
    if (hashmap.find(section) == hashmap.end())
      hashmap[section] = 1;
    else
      hashmap[section] += 1;
  }

  for (auto val : hashmap) {
    std::cout << val.first << " " << val.second << std::endl;
  }

  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

  std::cout
      << "Time difference = "
      << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count()
      << "[ns]" << std::endl;

  return 0;
}

/*
int main() {
  std::string header;
  std::string line;
  std::ifstream
myfile("/Users/lauro/Documents/workspace/http_log_monitoring/src/sample_small.txt");
  std::vector<std::string> parsedLine;

  if (myfile.is_open()) {

    getline(myfile, header);
    //parse header
    std::unordered_map <std::string,int> hashmap;
    int lineTokens = 7;
    int requestTokens = 3;
    int counter = 0;
    int indexTime = 3;
    int indexRequest = 4;
    long int startTenSecondWindow;
    std::vector<std::string> parsedLine;
    std::vector<std::string> parsedRequest;

    parsedLine.reserve(lineTokens);
    parsedRequest.reserve(requestTokens);

    while (getline(myfile, line)) {
      parseLine(line,parsedLine);
      for(int i = 0; i < parsedLine.size(); ++i) {
        std::cout << parsedLine[i] << " ";
      }
      std::cout << "\n";
    }

    myfile.close();

  } else
      std::cout << "Unable to open file";
}
*/

/*
while (getline(myfile, line,',')) {
  if(counter == lineTokens) {
    auto currentRequest = parsedLine[indexRequest];
    auto currentTime = parsedLine[indexTime];

std::cout << "Begin of for \n" ;
for(auto v : parsedLine)
  std::cout << v << " ";
std::cout << "End of for \n" ;

counter = 0;
parsedLine.clear();
}

parsedLine.push_back(std::move(line));
++counter;
}*/