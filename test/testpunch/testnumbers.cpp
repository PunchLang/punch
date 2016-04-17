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
#include <lang/visitors.hpp>
#include <lang/reader_expressions.hpp>
#include <lang/number_expressions.hpp>

using namespace punch::lang;
using namespace punch::lang::expressions;

namespace punch {
  namespace lang {
    namespace expressions {
      inline ::std::ostream &operator<<(::std::ostream &os, const Expression &expression) {
        LoggingVisitor logger(os, 1);
        expression.accept(logger);
        return os;
      }
    }
  }
}

class NumberTest : public ::testing::Test {

public:
  NumberTest() {}
  ~NumberTest() {}

  void SetUp() {}
  void TearDown() {}
};

template <typename T>
void compare(Number number, T required) {

  NumberParser parser;

  SharedExpression target = std::make_shared<Number>(std::move(number));
  parser.upgrade(target);

  Expression * required_expression = & required;

  EXPECT_EQ(*target, *required_expression);
}

TEST_F(NumberTest, Integers) {
  compare(Number("0"), Integer(0, nullptr));
  compare(Number("2"), Integer(2, nullptr));
  compare(Number("+2"), Integer(2, nullptr));
  compare(Number("-2"), Integer(-2, nullptr));
  compare(Number("12345"), Integer(12345, nullptr));
  compare(Number("011"), Integer(9, nullptr));
  compare(Number("0x11"), Integer(17, nullptr));
  compare(Number("0xF"), Integer(15, nullptr));
  compare(Number("2r100"), Integer(4, nullptr));
  compare(Number("3r100"), Integer(9, nullptr));
}

TEST_F(NumberTest, Float) {
  compare(Number("0."), Float(0, nullptr));
  compare(Number("0.11"), Float(0.11, nullptr));
  compare(Number("-1.11"), Float(-1.11, nullptr));
  compare(Number("+1.11233"), Float(1.11233, nullptr));
  compare(Number("12.23M"), Float(12.23, nullptr));
}

TEST_F(NumberTest, Ratio) {

  compare(Number("0/1"),Ratio(0,1, nullptr));
  compare(Number("12/525"),Ratio(12,525, nullptr));
}