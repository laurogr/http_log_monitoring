#include "gtest/gtest.h"

class BufferTest : public ::testing::Test {
 protected:
  void SetUp() override {
    //q1_.Enqueue(1);
  }

  // void TearDown() override {}
  //Queue<int> q1_;

};

TEST_F(BufferTest, IsEmptyInitially) {
  EXPECT_EQ(0, 0);
}
