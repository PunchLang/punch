/*
 *   Copyright (c) 2015 Raymond Kroon. All rights reserved.
 *   The use and distribution terms for this software are covered by the
 *   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
 *   which can be found in the file LICENSE.txt at the root of this distribution.
 *   By using this software in any fashion, you are agreeing to be bound by
 *   the terms of this license.
 *   You must not remove this notice, or any other, from this software.
 */

#include <gtest/gtest.h>
#include <lang/scanner.hpp>
#include <boost/optional/optional_io.hpp>

using namespace punch::lang;

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

TEST_F(ScannerTest, StringScannerPlusTest) {
  StringScanner scanner("+2");

  EXPECT_EQ('+', scanner.current_char());
  EXPECT_EQ(boost::none, scanner.previous_char());
  EXPECT_EQ('2', scanner.next_char());

  scanner.pop();
  EXPECT_EQ('2', scanner.current_char());
  EXPECT_EQ('+', scanner.previous_char());
  EXPECT_EQ(boost::none, scanner.next_char());
}

TEST_F(ScannerTest, StringScannerMinTest) {
  StringScanner scanner("-2");

  EXPECT_EQ('-', scanner.current_char());
  EXPECT_EQ(boost::none, scanner.previous_char());
  EXPECT_EQ('2', scanner.next_char());

  scanner.pop();
  EXPECT_EQ('2', scanner.current_char());
  EXPECT_EQ('-', scanner.previous_char());
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
