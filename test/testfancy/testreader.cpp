/*
 *   Copyright (c) 2015 Raymond Kroon. All rights reserved.
 *   The use and distribution terms for this software are covered by the
 *   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
 *   which can be found in the file LICENSE.txt at the root of this distribution.
 *   By using this software in any fashion, you are agreeing to be bound by
 *   the terms of this license.
 *   You must not remove this notice, or any other, from this software.
 */

#include <boost/assign/list_of.hpp>
#include <list>
#include <gtest/gtest.h>
#include <reader.hpp>
#include <util.hpp>

using boost::assign::list_of;

UExpression eof = make_unique<EndOfFile>();

class ReaderTest : public ::testing::Test {
public:
  ReaderTest() {}
  ~ReaderTest() {}

  void SetUp() {}
  void TearDown() {}
};

std::list<SharedExpression> consume(Reader& reader) {
  std::list<SharedExpression> result;

  auto expr = reader.next();
  while (expr != eof) {
    result.push_back(std::move(expr));
    expr = reader.next();
  }

  return result;
}

void compare(std::string in, const std::initializer_list<SharedExpression>& req) {
  Reader reader(make_unique<Tokenizer>(make_unique<StringScanner>(in)));
  auto tokens = consume(reader);

  std::list<SharedExpression> required(std::begin(req), std::end(req));
  EXPECT_EQ(required, tokens);
}

void compare_file(std::string file, const std::initializer_list<SharedExpression>& req) {
  Reader reader(make_unique<Tokenizer>(make_unique<LineScanner>(file)));
  auto tokens = consume(reader);

  std::list<SharedExpression> required(std::begin(req), std::end(req));
  EXPECT_EQ(required, tokens);
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

  compare_file("resources/numbers.fc",
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