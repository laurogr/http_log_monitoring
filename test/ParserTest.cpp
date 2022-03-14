#include "../src/Parser.h"
#include "gtest/gtest.h"

TEST(ParserTest, ParseFunction) {
  auto result = Parser::parse(
      "\"10.0.0.2\",\"-\",\"apache\",1549573860,\"GET /api/user "
      "HTTP/1.0\",200,1234");

  EXPECT_EQ(result.date, 1549573860);
  EXPECT_EQ(result.request, "\"GET /api/user HTTP/1.0\"");
}

TEST(ParserTest, ParseSection) {
  auto resource = "\"GET /api/user HTTP/1.0\"";
  auto resultSection = Parser::parseSectionFromResource(resource);

  EXPECT_EQ(resultSection, "api");
}

//TODO : Add tests to validate other functions
