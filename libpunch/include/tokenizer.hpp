/*
 *   Copyright (c) 2015 Raymond Kroon. All rights reserved.
 *   The use and distribution terms for this software are covered by the
 *   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
 *   which can be found in the file LICENSE.txt at the root of this distribution.
 *   By using this software in any fashion, you are agreeing to be bound by
 *   the terms of this license.
 *   You must not remove this notice, or any other, from this software.
 */

#ifndef PUNCH_TOKENIZER_H
#define PUNCH_TOKENIZER_H

#include <util.hpp>
#include <unordered_map>
#include <memory>
#include <iterator>
#include <list>
#include <set>
#include <string>
#include <scanner.hpp>
#include <iostream>

const char DOUBLE_QUOTE = '"';
const char DISPATCH = '#';
const char SEMICOLON = ';';
const char BACKSLASH = '\\';
const char BANG = '!';

const char CURLY_OPEN = '{';
const char CURLY_CLOSE = '}';
const char ROUND_OPEN = '(';
const char ROUND_CLOSE = ')';
const char SQUARE_OPEN = '[';
const char SQUARE_CLOSE = ']';

const std::set<char> whitespace = {' ', ',', '\n', '\t', '\12'};
const std::set<char> delimiters = {'{', '}', '[', ']', '(', ')', '\\', ';', '"'};

namespace token {

  enum class TokenType {
    BeginOfFile, EndOfFile,
    Literal, RoundOpen, RoundClose, SquareOpen, SquareClose, CurlyOpen, CurlyClose,
    SetOpen, FunctionOpen, Dispatch, String, Regex, Char
  };

  const std::set<TokenType> closeTypes = { TokenType::RoundClose, TokenType::CurlyClose, TokenType::SquareClose };

  const std::unordered_map<TokenType, const char *, EnumClassHash> tokenTypeTranslations ({
      {TokenType::BeginOfFile, "BOF"},
      {TokenType::EndOfFile, "EOF"},
      {TokenType::Literal, "LIT"},
      {TokenType::RoundOpen, "("},
      {TokenType::RoundClose, ")"},
      {TokenType::SquareOpen, "["},
      {TokenType::SquareClose, "]"},
      {TokenType::CurlyOpen, "{"},
      {TokenType::CurlyClose, "}"},
      {TokenType::SetOpen, "#{"},
      {TokenType::FunctionOpen, "#("},
      {TokenType::Dispatch, "#"},
      {TokenType::String, "STR"},
      {TokenType::Regex, "#\""},
      {TokenType::Char, "CH"}});

  class Token {
  public:
    Token(const Token &other) : type(other.type), value(std::string(other.value)), pos(position(other.pos)) { }

    Token(Token &&other) : type(std::move(other.type)), value(std::move(other.value)), pos(std::move(other.pos)) { }
    Token &operator=(Token &&other) {
      std::swap(this->type, other.type);
      std::swap(this->value, other.value);
      std::swap(this->pos, other.pos);

      return *this;
    }

    ~Token() { };

    std::string DebugInfo() const {
      if (value.empty()) {
        return std::string(tokenTypeTranslations.at(type))
               + " (" + std::to_string(std::get<0>(pos)) + ", "
               + std::to_string(std::get<1>(pos)) + ")";
      }
      else {
        return std::string(tokenTypeTranslations.at(type)) + " " + value
               + " (" + std::to_string(std::get<0>(pos)) + ", "
               + std::to_string(std::get<1>(pos)) + ")";
      }
    }

    bool operator==(const Token other) const;
    bool operator!=(const Token other) const;

    TokenType type;
    std::string value;
    position pos;

    static Token BeginOfFile;
    static Token EndOfFile;

    static Token Literal(std::string value, position pos) {
      return Token(TokenType::Literal, value, pos);
    }

    static Token RoundOpen(position pos) {
      return Token(TokenType::RoundOpen, "", pos);
    }

    static Token RoundClose(position pos) {
      return Token(TokenType::RoundClose, "", pos);
    }

    static Token SquareOpen(position pos) {
      return Token(TokenType::SquareOpen, "", pos);
    }

    static Token SquareClose(position pos) {
      return Token(TokenType::SquareClose, "", pos);
    }

    static Token CurlyOpen(position pos) {
      return Token(TokenType::CurlyOpen, "", pos);
    }

    static Token CurlyClose(position pos) {
      return Token(TokenType::CurlyClose, "", pos);
    }

    static Token SetOpen(position pos) {
      return Token(TokenType::SetOpen, "", pos);
    }

    static Token FunctionOpen(position pos) {
      return Token(TokenType::FunctionOpen, "", pos);
    }

    static Token Dispatch(std::string value, position pos) {
      return Token(TokenType::Dispatch, value, pos);
    }

    static Token String(std::string value, position pos) {
      return Token(TokenType::String, value, pos);
    }

    static Token Regex(std::string value, position pos) {
      return Token(TokenType::Regex, value, pos);
    }

    static Token Char(std::string value, position pos) {
      return Token(TokenType::Char, value, pos);
    }

  private:
    Token(TokenType type, std::string value, position pos)
        : type(type), value(value), pos(pos) { }
  };

  inline ::std::ostream &operator<<(::std::ostream &os, const Token &token) {
    return os << token.DebugInfo();
  }
}

using namespace token;

class Tokenizer {

public:

  Tokenizer(Scanner * const s) :
    scanner{s}
  { }

  ~Tokenizer() { };

  bool next(Token&);

private:
  void mark_ready();
  void ret(Token);

  bool is_next(const char&);
  bool is_next(const std::set<char>);
  bool is_prev_whitespace();
  bool slurp_until(const std::initializer_list<std::set<char>>&, std::string&);
  bool slurp_until(const char, std::string&);
  void flush_line();

  bool ready = false;

  Scanner * const scanner;
  Token m_end = Token::EndOfFile;
  Token current = m_end;
  Token waiting = m_end;
};

#endif //PUNCH_TOKENIZER_H
