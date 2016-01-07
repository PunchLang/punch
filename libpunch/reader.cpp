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
#include <boost/regex.hpp>

bool Keyword::accepts(Token& tok) {
  return tok.type == TokenType::Literal && tok.value.find(":") == 0;
}

const std::set<char> number_start = boost::assign::list_of('0')('1')('2')('3')('4')('5')('6')('7')('8')('9');
const std::set<char> sign_start = boost::assign::list_of('-')('+');

bool might_be_number(Token& tok) {
  return (tok.type == TokenType::Literal) &&
      (number_start.find(tok.value.at(0)) != number_start.end() ||
          (tok.value.length() > 1 && sign_start.find(tok.value.at(0)) != sign_start.end()));
}

const boost::regex int_pattern("([-+]?)(([0]?)|([1-9][0-9]*)|0[xX]([0-9A-Fa-f]+)|0([0-7]+)|([1-9][0-9]?)[rR]([0-9A-Za-z]+)|0[0-9]+)(N)?");
const boost::regex float_pattern("([-+]?[0-9]+(\\.[0-9]*)?([eE][-+]?[0-9]+)?)(M)?");
const boost::regex ratio_pattern("([-+]?[0-9]+)/([0-9]+)");

bool Integer::accepts(Token& tok) {

  if (might_be_number(tok)) {
    boost::smatch match;
    return boost::regex_match(tok.value, match, int_pattern);
  }

  return false;
}

bool Float::accepts(Token& tok) {

  const boost::regex float_regex(float_pattern);

  if (might_be_number(tok)) {
    return boost::regex_match(tok.value, float_regex);
  }

  return false;
}

bool Ratio::accepts(Token& tok) {
  const boost::regex ratio_regex(ratio_pattern);

  if (might_be_number(tok)) {
    return boost::regex_match(tok.value, ratio_regex);
  }

  return false;
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

ReaderResult OK(ReaderResult::ResultType::OK, "");
ReaderResult END(ReaderResult::ResultType::END, "");
ReaderResult ERR(std::string reason) {
  return ReaderResult(ReaderResult::ResultType::ERROR, reason);
}

ReaderResult Reader::next(UExpression& expr) {

  if (cur_tok == Token::EndOfFile) {
    expr = std::move(m_end);
    return END;
  }

  std::string msg;

  if (Keyword::accepts(cur_tok)) {
    ret(Keyword::create(this, msg));
  }
  else if (Integer::accepts(cur_tok)) {
    ret(Integer::create(this, msg));
  }
  else if (Float::accepts(cur_tok)) {
    ret(Float::create(this, msg));
  }
  else if (Ratio::accepts(cur_tok)) {
    ret(Ratio::create(this, msg));
  }
  else if (Literal::accepts(cur_tok)) {
    ret(Literal::create(this, msg));
  }
  else if (List::accepts(cur_tok)) {
    ret(List::create(this, msg));
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

  tokenizer->next(cur_tok);

  if (current) {
    expr = std::move(current);
    return OK;
  }
  else {
    return ERR(msg);
  }
}

template <class T>
std::set<T> without(const std::set<T>& orig, const T& value) {
  std::set<T> result(orig);
  result.erase(value);

  return result;
}

ReaderResult read_until(Reader* r, TokenType tt, const std::set<TokenType>& not_in, std::list<UExpression>& l) {

  while (r->current_token().type != tt) {
    if (r->current_token() == Token::EndOfFile) {
      return ERR(std::string("EOF, expected ") + tokenTypeTranslations.at(tt));
    }

    if (not_in.find(r->current_token().type) != not_in.end()) {
      return ERR(std::string("Expected ") + tokenTypeTranslations.at(tt) + " got " + tokenTypeTranslations.at(r->current_token().type));
    }

    UExpression expr;
    auto rr = r->next(expr);

    if (rr.is_ok()) {
      l.push_back(std::move(expr));
    }
    else {
      return rr;
    }
  }

  return OK;
}

UExpression Keyword::create(Reader *r, std::string& error) {
  return make_unique<Keyword>(r->current_token().value.substr(1));
}

UExpression Integer::create(Reader *r, std::string& error) {

  /* regex breaks first number after [+-], so we check for it and remove it*/

  bool negate = false;
  std::string input = r->current_token().value;
  if (sign_start.find(input.at(0)) != sign_start.end()) {
    if (input.find("-") == 0) {
      negate = true;
    }
    input = input.substr(1);
  }

  boost::smatch match;
  boost::regex_match(input, match, int_pattern);

  //bool negate = (n.matched && std::string(n.first, n.second).compare("-") == 0);

  if(match[3].matched)
  {
    if(match[9].matched) {
      // not yet bigint thingies.
      return make_unique<Integer>(0);
    }

    return make_unique<Integer>(0);
  }

  auto n = match[1];

  int group = 0;
  int radix = 10;

  if(match[4].matched) {
    radix = 10;
    group = 4;
  }
  else if(match[5].matched) {
    radix = 16;
    group = 5;
  }
  else if(match[6].matched) {
    radix = 8;
    group = 6;
  }
  else if(match[8].matched) {
    auto r = match[7];
    radix = std::stoi(std::string(r.first, r.second));
    group = 8;
  }

  if(group == 0) {
    error = "Invalid integer";
    return make_unique<Integer>();
  }

  auto m = match[group];
  //std::cout << m.length() << " INT! " << group << " : [" << std::string(m.first, m.second)  << "] : " <<  radix << std::endl;

  long value = std::stol(std::string(m.first, m.second), nullptr, radix);
  if(negate) {
    value = -value;
  }

//  if(match[9].matched) {
//    // bigint not supported
//  }

  return make_unique<Integer>(value);
}

UExpression Float::create(Reader *r, std::string& error) {
  boost::smatch match;
  boost::regex_match(r->current_token().value, match, float_pattern);

  std::string value = r->current_token().value;

  if (match[4].matched) {
    // no decimals yet
    value = value.substr(0, value.size() - 1);
  }

  double d = std::stod(value);

  return make_unique<Float>(d);
}

UExpression Ratio::create(Reader *r, std::string& error) {
  boost::smatch match;
  std::string input = r->current_token().value;
  boost::regex_match(input, match, ratio_pattern);

  auto n = match[1];
  std::string n_str(n.first, n.second);

  if (n_str.find('+') == 0) {
    n_str = n_str.substr(1);
  }

  auto d = match[2];
  std::string d_str(d.first, d.second);

  long numerator = stol(n_str, 0, 10);
  long denominator = stol(d_str, 0, 10);

  return make_unique<Ratio>(numerator,denominator);
}

UExpression Literal::create(Reader *r, std::string& error) {
  return make_unique<Literal>(r->current_token().value);
}

UExpression List::create(Reader *r, std::string& error) {
  r->pop_token();

  auto without_round_close = without(closeTypes, TokenType::RoundClose);

  std::list<UExpression> l;
  ReaderResult result = read_until(r, TokenType::RoundClose, without_round_close, l);

  if (result.is_ok()) {
    return make_unique<List>(l);
  }
  else {
    error = result.msg;
    return make_unique<List>();
  }
}

UExpression Map::create(Reader *r, std::string& error) {
  r->pop_token();

  auto without_curly_close = without(closeTypes, TokenType::CurlyClose);

  std::list<UExpression> l;
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

UExpression Set::create(Reader *r, std::string& error) {
  r->pop_token();

  auto without_curly_close = without(closeTypes, TokenType::CurlyClose);

  std::list<UExpression> l;
  ReaderResult result = read_until(r, TokenType::CurlyClose, without_curly_close, l);

  if (result.is_ok()) {
    return make_unique<Set>(l);
  }
  else {
    error = result.msg;
    return make_unique<Set>();
  }
}

UExpression String::create(Reader *r, std::string& error) {
  return make_unique<String>(r->current_token().value);
}

UExpression Vector::create(Reader *r, std::string& error) {
  r->pop_token();

  auto without_square_close = without(closeTypes, TokenType::SquareClose);

  std::list<UExpression> l;
  ReaderResult result = read_until(r, TokenType::SquareClose, without_square_close, l);

  if (result.is_ok()) {
    return make_unique<Vector>(l);
  }
  else {
    error = result.msg;
    return make_unique<Vector>();
  }
}

