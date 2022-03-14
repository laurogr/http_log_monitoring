#include "gtest/gtest.h"
#include "../src/Alert.h"

class AlertTest : public ::testing::Test {
 protected:
  void SetUp() override {
    logBuffer.update(1549573860,"\"GET /api/user HTTP/1.0\"");
    logBuffer.update(1549573861,"\"GET /api/user HTTP/1.0\"");
    logBuffer.update(1549573862,"\"GET /api/user HTTP/1.0\"");
    logBuffer.update(1549573863,"\"GET /api/user HTTP/1.0\"");
    logBuffer.update(1549573864,"\"GET /api/user HTTP/1.0\"");
    logBuffer.update(1549573865,"\"GET /api/user HTTP/1.0\"");
    logBuffer.update(1549573866,"\"GET /api/user HTTP/1.0\"");
    logBuffer.update(1549573867,"\"GET /api/user HTTP/1.0\"");
    logBuffer.update(1549573868,"\"GET /api/user HTTP/1.0\"");
    logBuffer.update(1549573869,"\"GET /api/user HTTP/1.0\"");
    logBuffer.update(1549573870,"\"GET /api/user HTTP/1.0\"");
  }

  // void TearDown() override {}
  AlertAverage alertAverage{10};
  AlertRecurrent alertRecurrent{10};
  LogBuffer logBuffer {120};

};

//TODO : Add tests to validate algorithm
TEST_F(AlertTest, IsEmptyInitially) {
  //alertRecurrent.check(logBuffer, 1549573870);
  //EXPECT_EQ(1, 1);
}
