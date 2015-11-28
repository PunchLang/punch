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
  std::unique_ptr<Scanner> scanner{new StringScanner(in)};
  std::unique_ptr<Tokenizer> tokenizer {new Tokenizer(std::move(scanner))};
  Reader reader(std::move(tokenizer));
  auto tokens = consume(reader);

  std::list<SharedExpression> required(std::begin(req), std::end(req));
  EXPECT_EQ(required, tokens);
}

void compare_file(std::string file, const std::initializer_list<SharedExpression>& req) {
  std::unique_ptr<Scanner> scanner{new LineScanner(file)};
  std::unique_ptr<Tokenizer> tokenizer {new Tokenizer(std::move(scanner))};
  Reader reader(std::move(tokenizer));
  auto tokens = consume(reader);

  std::list<SharedExpression> required(std::begin(req), std::end(req));
  EXPECT_EQ(required, tokens);
}

TEST_F(ReaderTest, Keyword) {
  compare(":test1",
          {std::make_shared<Keyword>("test1")
          });
}
