/*
 *   Copyright (c) 2015 Raymond Kroon. All rights reserved.
 *   The use and distribution terms for this software are covered by the
 *   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
 *   which can be found in the file LICENSE.txt at the root of this distribution.
 *   By using this software in any fashion, you are agreeing to be bound by
 *   the terms of this license.
 *   You must not remove this notice, or any other, from this software.
 */

#include <lang/number_expressions.hpp>
#include <lang/reader_expressions.hpp>
#include <lang/visitors.hpp>
#include <boost/regex.hpp>

using namespace punch::lang;
using namespace punch::lang::expressions;

void Integer::accept(ExpressionVisitor &v) const {
  v.visit(this);
}

void Integer::accept(UpgradingExpressionVisitor &v) {
  auto sh = shared_from_this();
  v.upgrade(sh);
}

void Float::accept(ExpressionVisitor &v) const {
  v.visit(this);
}

void Float::accept(UpgradingExpressionVisitor &v) {
  auto sh = shared_from_this();
  v.upgrade(sh);
}

void Ratio::accept(ExpressionVisitor &v) const {
  v.visit(this);
}

void Ratio::accept(UpgradingExpressionVisitor &v) {
  auto sh = shared_from_this();
  v.upgrade(sh);
}

bool Integer::equal_to(const Expression *other) const {
  if (auto p = as<Integer>(other)) {
    return value == p->value;
  }
  else {
    return false;
  }
}

bool Float::equal_to(const Expression *other) const {
  if (auto p = as<Float>(other)) {
    return std::fabs(value - p->value) < std::numeric_limits<double>::epsilon();
  }
  else {
    return false;
  }
}

bool Ratio::equal_to(const Expression *other) const {
  if (auto p = as<Ratio>(other)) {
    return numerator == p->numerator && denominator == p->denominator;
  }
  else {
    return false;
  }
}

const boost::regex int_pattern(
    "([-+]?)(([0]?)|([1-9][0-9]*)|0[xX]([0-9A-Fa-f]+)|0([0-7]+)|([1-9][0-9]?)[rR]([0-9A-Za-z]+)|0[0-9]+)(N)?");
const boost::regex float_pattern("([-+]?[0-9]+(\\.[0-9]*)?([eE][-+]?[0-9]+)?)(M)?");
const boost::regex ratio_pattern("([-+]?[0-9]+)/([0-9]+)");

bool Integer::accepts(Expression const *const e) {
  if (auto p = as<Number>(e)) {
    boost::smatch match;
    return boost::regex_match(p->value, match, int_pattern);
  }

  return false;
}

bool Float::accepts(Expression const *const e) {
  if (auto p = as<Number>(e)) {
    boost::smatch match;
    return boost::regex_match(p->value, match, float_pattern);
  }

  return false;
}

bool Ratio::accepts(Expression const *const e) {
  if (auto p = as<Number>(e)) {
    boost::smatch match;
    return boost::regex_match(p->value, match, ratio_pattern);
  }

  return false;
}

const std::set<char> sign_start = {'-', '+'};

Integer Integer::from_number(SharedExpression const & expr) {

  /* regex breaks first number after [+-], so we check for it and remove it*/

  auto number = as<Number>(&*expr);

  bool negate = false;
  std::string input = number->value;
  if (sign_start.find(input.at(0)) != sign_start.end()) {
    if (input.find("-") == 0) {
      negate = true;
    }
    input = input.substr(1);
  }

  boost::smatch match;
  boost::regex_match(input, match, int_pattern);

  //bool negate = (n.matched && std::string(n.first, n.second).compare("-") == 0);

  if (match[3].matched) {
    if (match[9].matched) {
      // not yet bigint thingies.
      return Integer(0, expr);
    }

    return Integer(0, expr);
  }

  auto n = match[1];

  int group = 0;
  int radix = 10;

  if (match[4].matched) {
    radix = 10;
    group = 4;
  }
  else if (match[5].matched) {
    radix = 16;
    group = 5;
  }
  else if (match[6].matched) {
    radix = 8;
    group = 6;
  }
  else if (match[8].matched) {
    auto r = match[7];
    radix = std::stoi(std::string(r.first, r.second));
    group = 8;
  }

  if (group == 0) {
    //error = "Invalid integer";
  }

  auto m = match[group];
  //std::cout << m.length() << " INT! " << group << " : [" << std::string(m.first, m.second)  << "] : " <<  radix << std::endl;

  long value = std::stol(std::string(m.first, m.second), nullptr, radix);
  if (negate) {
    value = -value;
  }

//  if(match[9].matched) {
//    // bigint not supported
//  }

  return Integer(value, expr);
}

Float Float::from_number(SharedExpression const & expr) {

  auto number = as<Number>(&*expr);

  boost::smatch match;
  boost::regex_match(number->value, match, float_pattern);

  std::string value = number->value;

  if (match[4].matched) {
    // no decimals yet
    value = value.substr(0, value.size() - 1);
  }

  double d = std::stod(value);

  return Float(d, expr);

}

Ratio Ratio::from_number(SharedExpression const & expr) {
  auto number = as<Number>(&*expr);

  boost::smatch match;
  std::string input = number->value;
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

  return Ratio(numerator, denominator, expr);

}