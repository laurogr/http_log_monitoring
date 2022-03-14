#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>
#include <unordered_map>

#include "Alerts.h"
#include "Buffer.h"
#include "Parser.h"

const std::string filePath{
    "/Users/lauro/Documents/workspace/http_log_monitoring/src/"
    "sample_csv.txt"};

int main() {
  LogBuffer logBuffer(120);
  std::vector<std::unique_ptr<Alert>> alerts;
  alerts.push_back(std::make_unique<AlertAverage>(10));
  alerts.push_back(std::make_unique<AlertRecurrent>(10));
  std::string logLine, header;

  std::ifstream httpLogFile(filePath);

  if (httpLogFile.is_open()) {
    getline(httpLogFile, header);

    while (getline(httpLogFile, logLine)) {
      auto logLineObj = Parser::parse(logLine);
      logBuffer.update(logLineObj.date, std::move(logLineObj.request));

      for (const auto& alert : alerts) alert->check(logBuffer, logLineObj.date);
    }
  }else {
    std::cout << "ERROR : Not able to open the file\n";
  }

  return 0;
}
