#include <gtest/gtest.h>
#include <scanner.hpp>
#include <iostream>
#include <boost/optional/optional_io.hpp>

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
  EXPECT_EQ(boost::none, scanner.previous_char());
  EXPECT_EQ('e', scanner.next_char());

  scanner.pop();
  EXPECT_EQ('e', scanner.current_char());
  EXPECT_EQ('T', scanner.previous_char());
  EXPECT_EQ('s', scanner.next_char());

  scanner.flush_line();
  EXPECT_EQ('g', scanner.previous_char());
  EXPECT_EQ(boost::none, scanner.current_char());
  EXPECT_EQ(boost::none, scanner.next_char());
}

TEST_F(ScannerTest, LineScannerTest) {
  LineScanner scanner("resources/scanner_test.txt");

  EXPECT_EQ('l', scanner.current_char());
  EXPECT_EQ(boost::none, scanner.previous_char());
  EXPECT_EQ('i', scanner.next_char());

  scanner.pop();
  EXPECT_EQ('i', scanner.current_char());
  EXPECT_EQ('l', scanner.previous_char());
  EXPECT_EQ('n', scanner.next_char());

  scanner.pop();
  scanner.pop();
  scanner.pop();
  scanner.pop();
  EXPECT_EQ('\n', scanner.next_char());

  scanner.flush_line();
  EXPECT_EQ('2', scanner.current_char());
  EXPECT_EQ(boost::none, scanner.previous_char());
  EXPECT_EQ('n', scanner.next_char());

  scanner.flush_line();
  scanner.pop();
  EXPECT_EQ('i', scanner.current_char());

  scanner.flush_line();
  EXPECT_EQ(boost::none, scanner.current_char());
  EXPECT_EQ(boost::none, scanner.previous_char());
  EXPECT_EQ(boost::none, scanner.next_char());
}
