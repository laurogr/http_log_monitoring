#include "gtest/gtest.h"

class AlertTest : public ::testing::Test {
 protected:
  void SetUp() override {
    //q1_.Enqueue(1);
  }

  // void TearDown() override {}
  //Queue<int> q1_;

};

TEST_F(AlertTest, IsEmptyInitially) {
  EXPECT_EQ(0, 0);
}
