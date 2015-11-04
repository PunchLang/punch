#include <gtest/gtest.h>
#include <scanner.hpp>


class ScannerTest : public ::testing::Test {
public:
  ScannerTest() {}
  ~ScannerTest() {}

  void SetUp() {}
  void TearDown() {}
};

TEST_F(ScannerTest, StringScannerTest) {
  StringScanner scanner("Test string");

  EXPECT_EQ('T', scanner.current_char());

  scanner.pop();
  EXPECT_EQ('e', scanner.current_char());
  EXPECT_EQ('T', scanner.previous_char());
  EXPECT_EQ('s', scanner.next_char());

  scanner.flush_line();
  EXPECT_EQ('g', scanner.previous_char());
  EXPECT_EQ(boost::none, scanner.current_char());
  EXPECT_EQ(boost::none, scanner.next_char());
}
