/*
 *   Copyright (c) 2015 Raymond Kroon. All rights reserved.
 *   The use and distribution terms for this software are covered by the
 *   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
 *   which can be found in the file LICENSE.txt at the root of this distribution.
 *   By using this software in any fashion, you are agreeing to be bound by
 *   the terms of this license.
 *   You must not remove this notice, or any other, from this software.
 */

#include <reader.hpp>
#include <string>

bool Keyword::accepts(Token& tok) {
  return tok.type == TokenType::Literal && tok.value.find(":") == 0;
}

bool Literal::accepts(Token& tok) {
  return tok.type == TokenType::Literal;
}

bool List::accepts(Token& tok) {
  return tok.type == TokenType::RoundOpen;
}

bool Map::accepts(Token& tok) {
  return tok.type == TokenType::CurlyOpen;
}

bool Set::accepts(Token& tok) {
  return tok.type == TokenType::SetOpen;
}

bool String::accepts(Token& tok) {
  return tok.type == TokenType::String;
}

bool Vector::accepts(Token& tok) {
  return tok.type == TokenType::SquareOpen;
}

std::shared_ptr<Expression> Reader::next() {

  if (cur_tok == Token::EndOfFile) {
    return m_end;
  }

  if (Keyword::accepts(cur_tok)) {
    ret(Keyword::create(this));
  }
  else if (Literal::accepts(cur_tok)) {
    ret(Literal::create(this));
  }
  else if (List::accepts(cur_tok)) {
    ret(List::create(this));
  }
  else if (Map::accepts(cur_tok)) {
    ret(Map::create(this));
  }
  else if (Set::accepts(cur_tok)) {
    ret(Set::create(this));
  }
  else if (String::accepts(cur_tok)) {
    ret(String::create(this));
  }
  else if (Vector::accepts(cur_tok)) {
    ret(Vector::create(this));
  }
  else {
    //throw ReaderException();
  }

  cur_tok = tokenizer->next();
  return current;
}

std::list<SharedExpression> read_until(Reader* r, TokenType tt) {

  std::list<SharedExpression> inner;

  while (r->current_token().type != TokenType::RoundClose) {
    inner.push_back(r->next());
  }

  return inner;
}

SharedExpression Keyword::create(Reader *r) {
  return SharedExpression(new Keyword(r->current_token().value.substr(1)));
}

SharedExpression Literal::create(Reader *r) {
  return SharedExpression(new Literal(r->current_token().value));
}

SharedExpression List::create(Reader *r) {
  r->pop_token();

  return SharedExpression(new List(read_until(r, TokenType::RoundClose)));
}

SharedExpression Map::create(Reader *r) {
  r->pop_token();

  return SharedExpression(new Map(read_until(r, TokenType::RoundClose)));
}

SharedExpression Set::create(Reader *r) {
  r->pop_token();

  return SharedExpression(new Set(read_until(r, TokenType::RoundClose)));
}

SharedExpression String::create(Reader *r) {
  return SharedExpression(new String(r->current_token().value));
}

SharedExpression Vector::create(Reader *r) {
  r->pop_token();

  return SharedExpression(new Vector(read_until(r, TokenType::RoundClose)));
}

