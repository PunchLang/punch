/*
 *   Copyright (c) 2015 Raymond Kroon. All rights reserved.
 *   The use and distribution terms for this software are covered by the
 *   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
 *   which can be found in the file LICENSE.txt at the root of this distribution.
 *   By using this software in any fashion, you are agreeing to be bound by
 *   the terms of this license.
 *   You must not remove this notice, or any other, from this software.
 */

#include <tokenizer.hpp>

bool Token::operator==(const Token other) const {
  return type == other.type && value == other.value && pos == other.pos;
}

void Tokenizer::mark_ready() {
  ready = true;
}

void Tokenizer::ret(SharedToken token) {
  current = token;
  mark_ready();
}

bool Tokenizer::is_next(const char &c) {
  if (scanner->next_char()) {
    return scanner->next_char() == c;
  }

  return false;
}

bool Tokenizer::is_next(const std::set<char> cs) {
  if (scanner->next_char()) {
    return cs.find(*scanner->next_char()) != cs.end();
  }

  return false;
}

std::string Tokenizer::slurp_until(const std::initializer_list<std::set<char>>& stop_lists) {

  std::string result;

  while (scanner->current_char()) {
    result += *scanner->current_char();

    if (scanner->next_char())
    {
      for (auto it = stop_lists.begin(); it != stop_lists.end(); ++it) {
        if (is_next(*it)) {
          goto ret;
        }
      }

      scanner->pop();
    }
    else {
      break;
    }
  }
  ret:
  return result;
}

std::string Tokenizer::slurp_until(const char c) {
  std::string result;

  while (scanner->current_char()) {
    result += *scanner->current_char();

    if (scanner->next_char() && !is_next(c))
    {
      scanner->pop();
    }
    else {
      break;
    }
  }

  return result;
}

void Tokenizer::flush_line() {
  scanner->flush_line();
}

SharedToken Tokenizer::next() {

  ready = false;

  if (!scanner->current_char()) {
    return m_end;
  }

  while (scanner->current_char()) {
    char c = *scanner->current_char();

    if (c == DOUBLE_QUOTE) {
      position pos = scanner->position();
      scanner->pop();
      ret(Token::String(slurp_until(DOUBLE_QUOTE), pos));
      scanner->pop();
    }
    else if (c == SEMICOLON) {
      flush_line();
      continue;
    }
    else if (whitespace.find(c) != whitespace.end()) {
      //nothing
    }
    else if (c == BACKSLASH) {
      position pos = scanner->position();
      scanner->pop();
      ret(Token::Char(slurp_until({whitespace, delimiters}), pos));
    }
    else if (c == DISPATCH && is_next(CURLY_OPEN)) {
      ret(Token::SetOpen(scanner->position()));
      scanner->pop();
    }
    else if (c == DISPATCH && is_next(ROUND_OPEN)) {
      ret(Token::FunctionOpen(scanner->position()));
      scanner->pop();
    }
    else if (c == DISPATCH && is_next(DOUBLE_QUOTE)) {
      position pos = scanner->position();
      scanner->pop();
      scanner->pop();
      ret(Token::Regex(slurp_until(DOUBLE_QUOTE), pos));
      scanner->pop();
    }
    else if (c == DISPATCH) {
      position pos = scanner->position();
      scanner->pop();
      ret(Token::Dispatch(slurp_until({whitespace, delimiters}), pos));
    }
    else if (c == ROUND_OPEN) {
      ret(Token::RoundOpen(scanner->position()));
    }
    else if (c == ROUND_CLOSE) {
      ret(Token::RoundClose(scanner->position()));
    }
    else if (c == CURLY_OPEN) {
      ret(Token::CurlyOpen(scanner->position()));
    }
    else if (c == CURLY_CLOSE) {
      ret(Token::CurlyClose(scanner->position()));
    }
    else if (c == SQUARE_OPEN) {
      ret(Token::SquareOpen(scanner->position()));
    }
    else if (c == SQUARE_CLOSE) {
      ret(Token::SquareClose(scanner->position()));
    }
    else {
      position pos = scanner->position();
      ret(Token::Literal(slurp_until({whitespace, delimiters}), pos));
    }

    scanner->pop();

    if (ready) {
      break;
    }

  }

  if (ready) {
    return current;
  }
  else {
    return m_end;
  }
}
