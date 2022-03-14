#pragma once

struct Line {
  std::string remotehost;
  std::string rfc931;
  std::string authuser;
  long int date;
  std::string request;
  std::string status;
  std::string bytes;
};


class Parser {
 private:
  enum headerIndex {
    remotehost,
    rfc931,
    authuser,
    date,
    request,
    status,
    bytes
  };

  enum resourceIndex {
    httpMethod,
    section,
    httpVersion
  };

  enum sectionIndex {
    beforeSection,
    mainSection,
    subSection
  };

  static void parseLine(const std::string& line,
                        std::vector<std::string>& parsedString,
                        char delimiter = ',') {
    boost::split(parsedString, line,
                 [delimiter](char c) { return c == delimiter; });
  }

  static long int parseDate(std::vector<std::string>& parsedString) {
    return std::stol(parsedString[headerIndex::date]);
  }

  static std::string parseRequest(std::vector<std::string>& parsedString) {
    return parsedString[headerIndex::request];
  }

  static void parseString(const std::string& line,
                          std::vector<std::string>& parsedString,
                          char delimiter) {
    boost::split(parsedString, line,
                 [delimiter](char c) { return c == delimiter; });
  }

 public:
  //TODO : improve parsing strategy
  static std::string parseSectionFromRequest(const std::string& resource) {
    std::vector<std::string> parsedString;

    //splitting resource: "GET /api/user HTTP/1.0" -> /api/user
    Parser::parseString(resource, parsedString, ' ');

    //splitting section : /api/user -> api
    auto section = std::move(parsedString[resourceIndex::section]);
    Parser::parseString(section, parsedString, '/');

    return parsedString[sectionIndex::mainSection];
  }

  static struct Line parse(const std::string& line) {
    struct Line retLine;
    std::vector<std::string> parsedString;
    Parser::parseLine(line, parsedString);
    retLine.date = Parser::parseDate(parsedString);
    retLine.request = std::move(Parser::parseRequest(parsedString));

    return retLine;
  }
};