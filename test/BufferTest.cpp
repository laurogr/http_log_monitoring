#include "gtest/gtest.h"
#include "../src/Buffer.h"

class BufferTest : public ::testing::Test {
 protected:
  //void SetUp() override {}
  // void TearDown() override {}
  LogBuffer logBuffer {120};
};

TEST_F(BufferTest, IsEmptyInitially) {
  EXPECT_EQ(0, logBuffer.getAverageConnections());
}

TEST_F(BufferTest, IsUpdateAndAverageConnection) {
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

  EXPECT_EQ(1, logBuffer.getAverageConnections());
}
//TODO : Add tests to validate other functions



