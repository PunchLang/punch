/*
 *   Copyright (c) 2015 Raymond Kroon. All rights reserved.
 *   The use and distribution terms for this software are covered by the
 *   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
 *   which can be found in the file LICENSE.txt at the root of this distribution.
 *   By using this software in any fashion, you are agreeing to be bound by
 *   the terms of this license.
 *   You must not remove this notice, or any other, from this software.
 */

#include <lang/reader.hpp>

namespace punch {
  namespace lang {

    using namespace expressions;

    ReaderResult OK(ReaderResult::ResultType::OK, "");
    ReaderResult END(ReaderResult::ResultType::END, "");
    ReaderResult ERR(std::string reason) {
      return ReaderResult(ReaderResult::ResultType::ERROR, reason);
    }

    ReaderResult Reader::next(SharedExpression &expr) {

      if (!has_token) {
        return END;
      }

      std::string msg;

      if (Keyword::accepts(cur_tok)) {
        ret(Keyword::create(this, msg));
      }
      else if (Number::accepts(cur_tok)) {
        ret(Number::create(this, msg));
      }
      else if (Symbol::accepts(cur_tok)) {
        ret(Symbol::create(this, msg));
      }
      else if (Symbolic::accepts(cur_tok)) {
        ret(Symbolic::create(this, msg));
      }
      else if (Map::accepts(cur_tok)) {
        ret(Map::create(this, msg));
      }
      else if (Set::accepts(cur_tok)) {
        ret(Set::create(this, msg));
      }
      else if (String::accepts(cur_tok)) {
        ret(String::create(this, msg));
      }
      else if (Vector::accepts(cur_tok)) {
        ret(Vector::create(this, msg));
      }
      else if (closeTypes.find(cur_tok.type) != closeTypes.end()) {
        return ERR("Closing tag without open");
      }
      else {
        return ERR("");
      }

      has_token = tokenizer->next(cur_tok);

      if (current) {
        expr = std::move(current);
        return OK;
      }
      else {
        return ERR(msg);
      }
    }


    template<class T>
    std::set<T> without(const std::set<T> &orig, const T &value) {
      std::set<T> result(orig);
      result.erase(value);

      return result;
    }

    ReaderResult read_until(Reader *r, TokenType tt, const std::set<TokenType> &not_in, std::list<SharedExpression> &l) {

      while (r->current_token() && r->current_token()->type != tt) {

        if (not_in.find(r->current_token()->type) != not_in.end()) {
          return ERR(std::string("Expected ") + tokenTypeTranslations.at(tt) + " got " +
                     tokenTypeTranslations.at(r->current_token()->type));
        }

        SharedExpression expr;
        auto rr = r->next(expr);

        if (rr.is_ok()) {
          l.push_back(std::move(expr));
        }
        else {
          return rr;
        }
      }

      if (!r->current_token()) {
        return ERR(std::string("EOF, expected ") + tokenTypeTranslations.at(tt));
      }

      return OK;
    }

/* ACCEPTS */

    bool Keyword::accepts(Token &tok) {
      return tok.type == TokenType::Literal && tok.value.find(":") == 0;
    }

    const std::set<char> number_start = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    const std::set<char> sign_start = {'-', '+'};

    bool might_be_number(Token &tok) {
      return (tok.type == TokenType::Literal) &&
             (number_start.find(tok.value.at(0)) != number_start.end() ||
              (tok.value.length() > 1 && sign_start.find(tok.value.at(0)) != sign_start.end()));
    }

    bool Number::accepts(Token &tok) {
      return might_be_number(tok);
    }

    bool Symbol::accepts(Token &tok) {
      return tok.type == TokenType::Literal;
    }

    bool Symbolic::accepts(Token &tok) {
      return tok.type == TokenType::RoundOpen;
    }

    bool Map::accepts(Token &tok) {
      return tok.type == TokenType::CurlyOpen;
    }

    bool Set::accepts(Token &tok) {
      return tok.type == TokenType::SetOpen;
    }

    bool String::accepts(Token &tok) {
      return tok.type == TokenType::String;
    }

    bool Vector::accepts(Token &tok) {
      return tok.type == TokenType::SquareOpen;
    }

/* FACTORY METHODS */

    UExpression Keyword::create(Reader *r, std::string &error) {
      return make_unique<Keyword>(r->current_token()->value.substr(1));
    }

    UExpression Number::create(Reader *r, std::string &error) {
      return make_unique<Number>(r->current_token()->value);
    }

    SharedExpression Symbol::create(Reader *r, std::string &error) {
      return make_unique<Symbol>(r->current_token()->value);
    }

    SharedExpression Symbolic::create(Reader *r, std::string &error) {
      r->pop_token();

      auto without_round_close = without(closeTypes, TokenType::RoundClose);

      std::list<SharedExpression> l;
      ReaderResult result = read_until(r, TokenType::RoundClose, without_round_close, l);

      if (result.is_ok()) {
        return make_unique<Symbolic>(l);
      }
      else {
        error = result.msg;
        return make_unique<Symbolic>();
      }
    }

    SharedExpression Map::create(Reader *r, std::string &error) {
      r->pop_token();

      auto without_curly_close = without(closeTypes, TokenType::CurlyClose);

      std::list<SharedExpression> l;
      ReaderResult result = read_until(r, TokenType::CurlyClose, without_curly_close, l);

      if (l.size() % 2 == 1) {
        error = "Map entries should be even";
        return make_unique<Map>();
      }

      if (result.is_ok()) {
        return make_unique<Map>(l);
      }
      else {
        error = result.msg;
        return make_unique<Map>();
      }
    }

    SharedExpression Set::create(Reader *r, std::string &error) {
      r->pop_token();

      auto without_curly_close = without(closeTypes, TokenType::CurlyClose);

      std::list<SharedExpression> l;
      ReaderResult result = read_until(r, TokenType::CurlyClose, without_curly_close, l);

      if (result.is_ok()) {
        return make_unique<Set>(l);
      }
      else {
        error = result.msg;
        return make_unique<Set>();
      }
    }


    SharedExpression String::create(Reader *r, std::string &error) {
      return make_unique<String>(r->current_token()->value);
    }

    SharedExpression Vector::create(Reader *r, std::string &error) {
      r->pop_token();

      auto without_square_close = without(closeTypes, TokenType::SquareClose);

      std::list<SharedExpression> l;
      ReaderResult result = read_until(r, TokenType::SquareClose, without_square_close, l);

      if (result.is_ok()) {
        return make_unique<Vector>(l);
      }
      else {
        error = result.msg;
        return make_unique<Vector>();
      }
    }
  }
}
