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
#include <lang/tokenizer.hpp>
#include <lang/util.hpp>

using namespace punch::lang;

class TokenizerTest : public ::testing::Test {
public:
  TokenizerTest() {}
  ~TokenizerTest() {}

  void SetUp() {}
  void TearDown() {}
};

std::list<Token> consume(Tokenizer& tokenizer) {
  std::list<Token> result;

  Token cur_tok;
  while (tokenizer.next(cur_tok)) {
    result.push_back(cur_tok);
  }

  return result;
}

void compare(std::string in, const std::initializer_list<Token>& req) {
  StringScanner scanner(in);
  Tokenizer tokenizer(&scanner);
  auto tokens = consume(tokenizer);

  std::list<Token> required(std::begin(req), std::end(req));
  EXPECT_EQ(required, tokens);
}

void compare_file(std::string file, const std::initializer_list<Token>& req) {
  LineScanner scanner(file);
  Tokenizer tokenizer(&scanner);
  auto tokens = consume(tokenizer);

  std::list<Token> required(std::begin(req), std::end(req));
  EXPECT_EQ(required, tokens);
}

position pos(int l, int c) {
  return std::make_tuple(l,c);
}

TEST_F(TokenizerTest, SimpleForms) {
  compare("(1 12 a ab)",
          {Token::RoundOpen(pos(1,1)),
           Token::Literal("1", pos(1,2)),
           Token::Literal("12", pos(1,4)),
           Token::Literal("a", pos(1,7)),
           Token::Literal("ab", pos(1,9)),
           Token::RoundClose(pos(1,11))
          });

  compare("[a1 1a a ab]",
          {Token::SquareOpen(pos(1,1)),
           Token::Literal("a1", pos(1,2)),
           Token::Literal("1a", pos(1,5)),
           Token::Literal("a", pos(1,8)),
           Token::Literal("ab", pos(1,10)),
           Token::SquareClose(pos(1,12))
          });

  compare("{:a 1 :b 2}",
          {Token::CurlyOpen(pos(1,1)),
           Token::Literal(":a", pos(1,2)),
           Token::Literal("1", pos(1,5)),
           Token::Literal(":b", pos(1,7)),
           Token::Literal("2", pos(1,10)),
           Token::CurlyClose(pos(1,11))
          });
}

TEST_F(TokenizerTest, IgnoreWhitespace) {
  compare("(1,,, 2    3 \n\t 4)",
          {Token::RoundOpen(pos(1,1)),
           Token::Literal("1", pos(1,2)),
           Token::Literal("2", pos(1,7)),
           Token::Literal("3", pos(1,12)),
           Token::Literal("4", pos(2,3)),
           Token::RoundClose(pos(2,4))
          });
}

TEST_F(TokenizerTest, ParseStrings) {
  compare("\"multi word string { }\"",
          {Token::String("multi word string { }", pos(1,1))
          });

  compare("\"with line-breaks\n Second\r\nThird\"",
          {Token::String("with line-breaks\n Second\r\nThird", pos(1,1))
          });
}

TEST_F(TokenizerTest, MultiForms) {
  compare("(1) [2] 12   \";;12\" {",
          {Token::RoundOpen(pos(1,1)),
           Token::Literal("1", pos(1,2)),
           Token::RoundClose(pos(1,3)),
           Token::SquareOpen(pos(1,5)),
           Token::Literal("2", pos(1,6)),
           Token::SquareClose(pos(1,7)),
           Token::Literal("12", pos(1,9)),
           Token::String(";;12", pos(1,14)),
           Token::CurlyOpen(pos(1,21))
          });

  compare("\"str 1\" \"str 2\" ",
          {Token::String("str 1", pos(1,1)),
           Token::String("str 2", pos(1,9))
          });

  compare("(1 2)\n(3 4)",
          {Token::RoundOpen(pos(1,1)),
           Token::Literal("1", pos(1,2)),
           Token::Literal("2", pos(1,4)),
           Token::RoundClose(pos(1,5)),
           Token::RoundOpen(pos(2,1)),
           Token::Literal("3", pos(2,2)),
           Token::Literal("4", pos(2,4)),
           Token::RoundClose(pos(2,5)),
          });
}

TEST_F(TokenizerTest, Comment) {
  compare("[[]] ;; comment here",
          {Token::SquareOpen(pos(1,1)),
           Token::SquareOpen(pos(1,2)),
           Token::SquareClose(pos(1,3)),
           Token::SquareClose(pos(1,4))
          });

  compare("\"with ;; in string too\"",
          {Token::String("with ;; in string too", pos(1,1))
          });

  compare("fixed;comment here",
          {Token::Literal("fixed", pos(1,1))
          });

  compare("before line; comment etc\n (new)",
          {Token::Literal("before", pos(1,1)),
           Token::Literal("line", pos(1,8)),
           Token::RoundOpen(pos(2,2)),
           Token::Literal("new", pos(2,3)),
           Token::RoundClose(pos(2,6))
          });
}

TEST_F(TokenizerTest, Dispatch) {
  compare("#{1 12}",
          {Token::SetOpen(pos(1,1)),
           Token::Literal("1", pos(1,3)),
           Token::Literal("12", pos(1,5)),
           Token::CurlyClose(pos(1,7))
          });

  compare("#(+ 2 %1)",
          {Token::FunctionOpen(pos(1,1)),
           Token::Literal("+", pos(1,3)),
           Token::Literal("2", pos(1,5)),
           Token::Literal("%1", pos(1,7)),
           Token::RoundClose(pos(1,9))
          });

  compare("#\".*(^a-z)+\"",
          {Token::Regex(".*(^a-z)+", pos(1,1))
          });

  compare("#tag{1 2}",
          {Token::Dispatch("tag", pos(1,1)),
           Token::CurlyOpen(pos(1,5)),
           Token::Literal("1", pos(1,6)),
           Token::Literal("2", pos(1,8)),
           Token::CurlyClose(pos(1,9))
          });

  compare("#ns/tag 1234",
          {Token::Dispatch("ns/tag", pos(1,1)),
           Token::Literal("1234", pos(1,9))
          });

  compare("aa#ns 1234",
          {Token::Literal("aa#ns", pos(1,1)),
           Token::Literal("1234", pos(1,7))
          });

  compare("#!ns/tag 1234",
          {});

  compare("a #!ns/tag 1234",
          {Token::Literal("a", pos(1,1))});

  compare("a#!tag 1234",
          {Token::Literal("a#!tag", pos(1,1)),
           Token::Literal("1234", pos(1,8))
          });
}

TEST_F(TokenizerTest, Characters) {
  compare("\\a\\b \\z \\aa[1]",
          {Token::Char("a", pos(1,1)),
           Token::Char("b", pos(1,3)),
           Token::Char("z", pos(1,6)),
           Token::Char("aa", pos(1,9)),
           Token::SquareOpen(pos(1,12)),
           Token::Literal("1", pos(1,13)),
           Token::SquareClose(pos(1,14))
          });
}

TEST_F(TokenizerTest, Numbers) {
  compare("+2",
          {Token::Literal("+2", pos(1,1))
          });
}

TEST_F(TokenizerTest, FromFile) {
  compare_file("resources/simple.p",
               {Token::RoundOpen(pos(1,1)),
                Token::Literal("test", pos(1,2)),
                Token::Literal("1", pos(1,7)),
                Token::Literal("2", pos(1,9)),
                Token::Literal("3", pos(1,11)),
                Token::RoundClose(pos(1,12))
               });

  compare_file("resources/multiline.p",
               {Token::RoundOpen(pos(1,1)),
                Token::Literal("test", pos(1,2)),
                Token::Literal("1", pos(2,8)),
                Token::Literal("2", pos(2,10)),
                Token::Literal("3", pos(2,12)),
                Token::RoundClose(pos(3,1))
               });

  compare_file("resources/withcomments.p",
               {Token::RoundOpen(pos(3,1)),
                Token::Literal("defn", pos(3,2)),
                Token::Literal("test", pos(3,7)),
                Token::SquareOpen(pos(3,12)),
                Token::Literal("a", pos(3,13)),
                Token::SquareClose(pos(3,14)),
                Token::RoundOpen(pos(5,4)),
                Token::Literal("+", pos(5,5)),
                Token::Literal("1", pos(5,7)),
                Token::Literal("a", pos(5,9)),
                Token::RoundClose(pos(5,10)),
                Token::RoundClose(pos(5,11))
               });
}