/*
 *   Copyright (c) 2015 Raymond Kroon. All rights reserved.
 *   The use and distribution terms for this software are covered by the
 *   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
 *   which can be found in the file LICENSE.txt at the root of this distribution.
 *   By using this software in any fashion, you are agreeing to be bound by
 *   the terms of this license.
 *   You must not remove this notice, or any other, from this software.
 */

#ifndef FANCY_TOKENIZER_H
#define FANCY_TOKENIZER_H

#include <boost/assign/list_of.hpp>
#include <boost/unordered_map.hpp>
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

const std::set<char> whitespace = boost::assign::list_of(' ')(',')('\n')('\t')('\12');
const std::set<char> delimiters = boost::assign::list_of('{')('}')('[')(']')('(')(')')('\\')(';')('"');

typedef std::shared_ptr<std::string> shared_string;

class Token;
typedef std::shared_ptr<Token> SharedToken;

enum TokenType {
  None, // initialised value;
  Literal, RoundOpen, RoundClose, SquareOpen, SquareClose, CurlyOpen, CurlyClose,
  SetOpen, FunctionOpen, Dispatch, String, Regex, Char
};

const boost::unordered_map<TokenType, const char*> tokenTypeTranslations = boost::assign::map_list_of
    (None, "None")(Literal, "LIT")(RoundOpen, "(")(RoundClose, ")")(SquareOpen,"[")(SquareClose, "]")
    (CurlyOpen, "{")(CurlyClose, "}")(SetOpen,"#{")(FunctionOpen, "#(")(Dispatch, "#")(String, "STR")
    (Regex, "#\"")(Char, "CH");

class Token {
public:
  Token(const Token &other) : type(other.type), value(std::string(other.value)), pos(position(other.pos)) {}
  Token(const SharedToken &other) : Token(*other) { }
  ~Token() {};

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

  TokenType type;
  std::string value;
  position pos;

  static SharedToken None() {
    return SharedToken(new Token(TokenType::None, "", std::make_tuple(-1,-1)));
  }

  static SharedToken Literal(std::string value, position pos) {
    return SharedToken(new Token(TokenType::Literal, value, pos));
  }

  static SharedToken RoundOpen(position pos) {
    return SharedToken(new Token(TokenType::RoundOpen, "", pos));
  }

  static SharedToken RoundClose(position pos) {
    return SharedToken(new Token(TokenType::RoundClose, "", pos));
  }

  static SharedToken SquareOpen(position pos) {
    return SharedToken(new Token(TokenType::SquareOpen, "", pos));
  }

  static SharedToken SquareClose(position pos) {
    return SharedToken(new Token(TokenType::SquareClose, "", pos));
  }

  static SharedToken CurlyOpen(position pos) {
    return SharedToken(new Token(TokenType::CurlyOpen, "", pos));
  }

  static SharedToken CurlyClose(position pos) {
    return SharedToken(new Token(TokenType::CurlyClose, "", pos));
  }

  static SharedToken SetOpen(position pos) {
    return SharedToken(new Token(TokenType::SetOpen, "", pos));
  }

  static SharedToken FunctionOpen(position pos) {
    return SharedToken(new Token(TokenType::FunctionOpen, "", pos));
  }

  static SharedToken Dispatch(std::string value, position pos) {
    return SharedToken(new Token(TokenType::Dispatch, value, pos));
  }

  static SharedToken String(std::string value, position pos) {
    return SharedToken(new Token(TokenType::String, value, pos));
  }

  static SharedToken Regex(std::string value, position pos) {
    return SharedToken(new Token(TokenType::Regex, value, pos));
  }

  static SharedToken Char(std::string value, position pos) {
    return SharedToken(new Token(TokenType::Char, value, pos));
  }

private:
  Token(TokenType type, std::string value, position pos)
      : type(type), value(value), pos(pos) {}
};

inline ::std::ostream& operator<<(::std::ostream& os, const Token &token) {
  return os << token.DebugInfo();
}

class Tokenizer {
public:

  Tokenizer(std::unique_ptr<Scanner> &s) :
    scanner{std::move(s)}
  { next(); }

  ~Tokenizer() { };

  class iterator {
  public:
    typedef iterator self_type;
    typedef Token value_type;
    typedef Token& reference;
    typedef SharedToken pointer;
    typedef std::forward_iterator_tag iterator_category;
    typedef int difference_type;

    iterator(Tokenizer *parent, SharedToken current) : parent(parent), current(current) { }
    iterator(const self_type &other) : parent(other.parent), current(other.current) {}

    self_type operator++(int junk) { self_type i(*this); current = parent->next(); return i; }
    self_type operator++() { current = parent->next(); return *this; }
    reference operator*() { return *current; }

    bool operator==(const self_type& rhs) { return (parent == rhs.parent) && (current == rhs.current); }

    bool operator!=(const self_type& rhs) { return !(*this == rhs); }

  private:
    Tokenizer* parent;
    SharedToken current;
  };

  iterator begin()
  {
    return iterator(this, current);
  }

  iterator end()
  {
    return iterator(this, m_end);
  }

private:
  SharedToken next();
  void mark_ready();
  void ret(SharedToken);

  bool is_next(const char&);
  bool is_next(const std::set<char>);
  bool is_prev_whitespace();
  std::string slurp_until(const std::initializer_list<std::set<char>>&);
  std::string slurp_until(const char);
  void flush_line();

  bool ready = false;

  std::unique_ptr<Scanner> scanner;
  SharedToken m_end = Token::None();
  SharedToken current = m_end;
  SharedToken waiting = m_end;
};

#endif //FANCY_TOKENIZER_H
