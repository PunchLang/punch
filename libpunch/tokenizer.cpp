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
#include <reader.hpp>

Token Token::BeginOfFile = Token(TokenType::BeginOfFile, "", std::make_tuple(-1, -1));
Token Token::EndOfFile = Token(TokenType::EndOfFile, "", std::make_tuple(-1, -1));

bool Token::operator==(const Token other) const {
  return type == other.type && value == other.value && pos == other.pos;
}

bool Token::operator!=(const Token other) const {
  return !(*this == other);
}

void Tokenizer::mark_ready() {
  ready = true;
}

void Tokenizer::ret(Token token) {
  current = std::move(token);
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

bool Tokenizer::slurp_until(const std::initializer_list<std::set<char>>& stop_lists, std::string& result) {

  result = "";

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
  return true;
}

bool Tokenizer::slurp_until(const char c, std::string& result) {
  result = "";

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

  if (scanner->next_char()) {
    return true;
  }
  else {
    return false;
  }
}

void Tokenizer::flush_line() {
  scanner->flush_line();
}

bool Tokenizer::next(Token& token) {

  ready = false;

  if (!scanner->current_char()) {
    token = std::move(m_end);
    return false;
  }

  while (scanner->current_char()) {
    char c = *scanner->current_char();

    if (c == DOUBLE_QUOTE) {
      position pos = scanner->position();
      scanner->pop();

      std::string result;
      if (!slurp_until(DOUBLE_QUOTE, result)) {
        return false;
      }

      ret(Token::String(result, pos));
      scanner->pop();
    }
    else if (c == SEMICOLON || (c == DISPATCH && is_next(BANG))) {
      flush_line();
      continue;
    }
    else if (whitespace.find(c) != whitespace.end()) {
      //nothing
    }
    else if (c == BACKSLASH) {
      position pos = scanner->position();
      scanner->pop();

      std::string result;
      if (!slurp_until({whitespace, delimiters}, result)) {
        return false;
      }

      ret(Token::Char(result, pos));
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

      std::string result;
      if (!slurp_until(DOUBLE_QUOTE, result)) {
        return false;
      }

      ret(Token::Regex(result, pos));
      scanner->pop();
    }
    else if (c == DISPATCH) {
      position pos = scanner->position();
      scanner->pop();

      std::string result;
      if (!slurp_until({whitespace, delimiters}, result)) {
        return false;
      }

      ret(Token::Dispatch(result, pos));
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

      std::string result;
      if (!slurp_until({whitespace, delimiters}, result)) {
        return false;
      }

      ret(Token::Literal(result, pos));
    }

    scanner->pop();

    if (ready) {
      break;
    }

  }

  if (ready) {
    token = std::move(current);
    return true;
  }
  else {
    token = std::move(m_end);
    return false;
  }
}
