#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <boost/algorithm/string.hpp>


//https://httpd.apache.org/docs/2.4/logs.html

void readHeader();

void printDebug(std::vector<std::string>& vec) {
  for(int i = 0; i < vec.size(); ++i) {
    std::cout << vec[i] << " ";
  }
  std::cout << "\n";
}

void parseString(const std::string& line, std::vector<std::string>& parsedString, char delimiter) {
  boost::split(parsedString, line, [delimiter](char c){return c == delimiter;});
}

void parseLine(const std::string& line, std::vector<std::string>& parsedLine, char delimiter = ',') {
  parseString(line,parsedLine,delimiter);
}

void parseRequest(const std::string& line, std::vector<std::string>& parsedRequest, char delimiter = ' ') {
  parseString(line,parsedRequest,delimiter);
}

void parseResource(const std::string& line, std::vector<std::string>& parsedResource, char delimiter = '/') {
  parseString(line,parsedResource,delimiter);
}

//ingest level metrics

int main() {
  std::string header;
  std::string line;
  std::ifstream myFile("/Users/lauro/Documents/workspace/http_log_monitoring/src/sample_small.txt");

  std::unordered_map<std::string,int> hashmap;
  std::vector<std::string> parsedLine;
  std::vector<std::string> parsedRequest;
  std::vector<std::string> parsedResources;

  std::string request;
  std::string resource;
  std::string section;
  long int timestamp;

  //readHeader
  getline(myFile, header);
  auto idxTime = 3;
  auto idxRequest = 4;
  auto timer = 0;

  //process File
  while (getline(myFile, line)) {

    parseLine(line,parsedLine);
    timestamp = std::stol(parsedLine[idxTime]);

    parseRequest(parsedLine[idxRequest],parsedRequest);
    resource = std::move(parsedRequest[1]);

    parseResource(resource,parsedResources);
    section = std::move(parsedResources[1]);

    std::cout<< resource << " " << parsedResources[1] << std::endl; //TODO : if only /, what to do?
    if(hashmap.find(section) == hashmap.end()) hashmap[section] = 1;
    else hashmap[section] += 1;

  }

  for(auto val : hashmap) {
    std::cout << val.first << " " << val.second << std::endl;
  }

  return 0;
}

/*
int main() {
  std::string header;
  std::string line;
  std::ifstream myfile("/Users/lauro/Documents/workspace/http_log_monitoring/src/sample_small.txt");
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