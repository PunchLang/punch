/*
 *   Copyright (c) 2015 Raymond Kroon. All rights reserved.
 *   The use and distribution terms for this software are covered by the
 *   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
 *   which can be found in the file LICENSE.txt at the root of this distribution.
 *   By using this software in any fashion, you are agreeing to be bound by
 *   the terms of this license.
 *   You must not remove this notice, or any other, from this software.
 */

#include <list>
#include <gtest/gtest.h>
#include <lang/reader.hpp>
#include <lang/util.hpp>

using namespace punch::lang;
using namespace punch::lang::expression;

class ReaderTest : public ::testing::Test {

  UExpression eof = make_unique<EndOfFile>();
public:
  ReaderTest() {}
  ~ReaderTest() {}

  void SetUp() {}
  void TearDown() {}
};

ReaderResult consume(Reader& reader, std::list<SharedExpression>& result) {

  UExpression expr;
  ReaderResult rr;
  while ((rr= reader.next(expr)).is_ok()) {
    result.push_back(std::move(expr));
  }

  return rr;
}

void assert_reader_error(std::string in) {
  StringScanner scanner(in);
  Tokenizer tokenizer(&scanner);
  Reader reader(&tokenizer);
  std::list<SharedExpression> expressions;
  ReaderResult rr = consume(reader, expressions);

  EXPECT_EQ(true, !rr.is_ok());
}

void compare(std::string in, const std::initializer_list<SharedExpression>& req) {
  StringScanner scanner(in);
  Tokenizer tokenizer(&scanner);
  Reader reader(&tokenizer);
  std::list<SharedExpression> expressions;
  auto rr = consume(reader, expressions);

  std::list<SharedExpression> required(std::begin(req), std::end(req));
  EXPECT_EQ(required, expressions);
}

void compare_file(std::string file, const std::initializer_list<SharedExpression>& req) {
  LineScanner scanner(file);
  Tokenizer tokenizer(&scanner);
  Reader reader(&tokenizer);
  std::list<SharedExpression> expressions;
  auto rr = consume(reader, expressions);

  std::list<SharedExpression> required(std::begin(req), std::end(req));
  EXPECT_EQ(required, expressions);
}

TEST_F(ReaderTest, Keyword) {
  compare(":test1",
          {std::make_shared<Keyword>("test1")
          });
}

TEST_F(ReaderTest, Integers) {
  compare("0",
          {std::make_shared<Integer>(0)
          });

  compare("2",
          {std::make_shared<Integer>(2)
          });

  compare_file("resources/numbers.p",
               {std::make_shared<Integer>(2)
               });

  compare(std::string("+2"),
          {std::make_shared<Integer>(2)
          });

  compare("-2",
          {std::make_shared<Integer>(-2)
          });

  compare("12345",
          {std::make_shared<Integer>(12345)
          });

  compare("011",
          {std::make_shared<Integer>(9)
          });

  compare("0x11",
          {std::make_shared<Integer>(17)
          });

  compare("0xF",
          {std::make_shared<Integer>(15)
          });

  compare("2r100",
          {std::make_shared<Integer>(4)
          });

  compare("3r100",
          {std::make_shared<Integer>(9)
          });
}

TEST_F(ReaderTest, Float) {

  compare("0.",
          {std::make_shared<Float>(0)
          });

  compare("0.11",
          {std::make_shared<Float>(0.11)
          });

  compare("-1.11",
          {std::make_shared<Float>(-1.11)
          });

  compare("+1.11233",
          {std::make_shared<Float>(1.11233)
          });

  compare("12.23M",
          {std::make_shared<Float>(12.23)
          });
}

TEST_F(ReaderTest, Ratio) {

  compare("0/1",
          {std::make_shared<Ratio>(0,1)
          });

  compare("12/525",
          {std::make_shared<Ratio>(12,525)
          });
}

TEST_F(ReaderTest, Literal) {

  compare("a",
          {std::make_shared<Literal>("a")
          });

  compare("+",
          {std::make_shared<Literal>("+")
          });
}

TEST_F(ReaderTest, Symbolic) {

  std::list<UExpression> inner1;
  compare("()",
          {std::make_shared<Symbolic>(inner1)
          });

  std::list<UExpression> inner2;
  inner2.push_back(make_unique<Literal>("+"));
  inner2.push_back(make_unique<Literal>("a"));
  inner2.push_back(make_unique<Integer>(1));

  compare("(+ a 1)",
          {std::make_shared<Symbolic>(inner2)
          });
}

TEST_F(ReaderTest, IncompleteList) {
    assert_reader_error("( a a");
}

TEST_F(ReaderTest, WrongListClose) {
  assert_reader_error("( a a })");
  assert_reader_error("( a a ])");
  assert_reader_error(")");
  assert_reader_error("( a a ))");
}

TEST_F(ReaderTest, Map) {

  std::list<UExpression> inner1;
  compare("{}",
          {std::make_shared<Map>(inner1)
          });

  std::list<UExpression> inner2;
  inner2.push_back(make_unique<Integer>(1));
  inner2.push_back(make_unique<Literal>("a"));

  compare("{1 a}",
          {std::make_shared<Map>(inner2)
          });
}

TEST_F(ReaderTest, WrongMapClose) {
  assert_reader_error("{ a a )}");
  assert_reader_error("{ a a ]}");
  assert_reader_error("}");
  assert_reader_error("{ a a }}");
}

TEST_F(ReaderTest, UnevenMap) {
  assert_reader_error("{ a a a}");
}

TEST_F(ReaderTest, Set) {

  std::list<UExpression> inner1;
  compare("#{}",
          {std::make_shared<Set>(inner1)
          });

  std::list<UExpression> inner2;
  inner2.push_back(make_unique<Integer>(1));
  inner2.push_back(make_unique<Literal>("a"));

  compare("#{1 a}",
          {std::make_shared<Set>(inner2)
          });
}

TEST_F(ReaderTest, WrongSetClose) {
  assert_reader_error("#{ a a )}");
  assert_reader_error("#{ a a ]}");
  assert_reader_error("}");
  assert_reader_error("#{ a a }}");
}

TEST_F(ReaderTest, String) {
  compare("\"test1\"",
          {std::make_shared<String>("test1")
          });
}

TEST_F(ReaderTest, WrongStringClose) {
  assert_reader_error("\"open string");
}

TEST_F(ReaderTest, Vector) {

  std::list<UExpression> inner1;
  compare("[]",
          {std::make_shared<Vector>(inner1)
          });

  std::list<UExpression> inner2;
  inner2.push_back(make_unique<Integer>(1));
  inner2.push_back(make_unique<Literal>("a"));

  compare("[1 a]",
          {std::make_shared<Vector>(inner2)
          });
}

TEST_F(ReaderTest, WrongVectorClose) {
  assert_reader_error("[ a a )]");
  assert_reader_error("[ a a }]");
  assert_reader_error("]");
  assert_reader_error("[ a a ]]");
}

TEST_F(ReaderTest, InterleavedErrors) {
  assert_reader_error("([)]");
  assert_reader_error("({)}");
  assert_reader_error("[(])");
  assert_reader_error("#{[}]");
  assert_reader_error("#{(})");
  assert_reader_error("{({)}{][)");
}