/*
 *   Copyright (c) 2015 Raymond Kroon. All rights reserved.
 *   The use and distribution terms for this software are covered by the
 *   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
 *   which can be found in the file LICENSE.txt at the root of this distribution.
 *   By using this software in any fashion, you are agreeing to be bound by
 *   the terms of this license.
 *   You must not remove this notice, or any other, from this software.
 */

#include <reader_expressions.hpp>
#include <reader.hpp>
#include <util.hpp>

namespace expression {

  template <typename T>
  T const *as(const Expression* e) {
    return as_type<Expression, T>(e);
  }

  int Symbolic::n() const {
    return inner.size();
  }

  std::list<UExpression> const & Symbolic::get_inner() const {
    return inner;
  }

  /* DEBUGINFO */

  std::string EndOfFile::DebugInfo() const {
    return "EOF";
  }

  std::string Keyword::DebugInfo() const {
    return ":" + value ;
  }

  std::string Integer::DebugInfo() const {
    return "INT(" + std::to_string(value) + ")";
  }

  std::string Float::DebugInfo() const {
    return "FLOAT(" + std::to_string(value) + ")";
  }

  std::string Ratio::DebugInfo() const {
    return "RATIO(" + std::to_string(numerator) + "/" + std::to_string(denominator) + ")";
  }

  std::string Literal::DebugInfo() const {
    return "LIT(" + value + ")";
  }

  std::string Symbolic::DebugInfo() const {
    auto ret  = std::string("LIST[ ");

    for (auto it = inner.begin(); it != inner.end(); ++it) {
      ret += (*it)->DebugInfo() + " ";
    }

    ret += "]";
    return ret;
  }

  std::string Map::DebugInfo() const {
    auto ret  = std::string("MAP{");

    for (auto it = inner.begin(); it != inner.end(); ++it) {
      ret += (*it)->DebugInfo();
    }

    ret += "}";
    return ret;
  }

  std::string Set::DebugInfo() const {
    auto ret  = std::string("SET{");

    for (auto it = inner.begin(); it != inner.end(); ++it) {
      ret += (*it)->DebugInfo();
    }

    ret += "}";
    return ret;
  }

  std::string String::DebugInfo() const {
    return "\"" + value + "\"";
  }

  std::string Vector::DebugInfo() const {
    auto ret  = std::string("VEC[");

    for (auto it = inner.begin(); it != inner.end(); ++it) {
      ret += (*it)->DebugInfo();
    }

    ret += "]";
    return ret;
  }

  /* EQUALITY */

  bool EndOfFile::equal_to(const Expression* other) const {
    if (auto p = as<EndOfFile>(other)) {
      return true;
    }
    else {
      return false;
    }
  }

  bool Keyword::equal_to(const Expression* other) const {
    if (auto p = as<Keyword>(other)) {
      return value == p->value;
    }
    else {
      return false;
    }
  }

  bool Integer::equal_to(const Expression* other) const {
    if (auto p = as<Integer>(other)) {
      return value == p->value;
    }
    else {
      return false;
    }
  }

  bool Float::equal_to(const Expression* other) const {
    if (auto p = as<Float>(other)) {
      return  std::fabs(value - p->value) < std::numeric_limits<double>::epsilon();
    }
    else {
      return false;
    }
  }

  bool Ratio::equal_to(const Expression* other) const {
    if (auto p = as<Ratio>(other)) {
      return numerator == p->numerator && denominator == p->denominator;
    }
    else {
      return false;
    }
  }

  bool Literal::equal_to(const Expression* other) const {
    if (auto p = as<Literal>(other)) {
      return value == p->value;
    }
    else {
      return false;
    }
  }

  bool Symbolic::equal_to(const Expression* other) const {
    if (auto p = as<Symbolic>(other)) {
      return inner == p->inner;
    }
    else {
      return false;
    }
  }

  bool Map::equal_to(const Expression* other) const {
    if (auto p = as<Map>(other)) {
      return inner == p->inner;
    }
    else {
      return false;
    }
  }

  bool Set::equal_to(const Expression* other) const {
    if (auto p = as<Set>(other)) {
      return inner == p->inner;
    }
    else {
      return false;
    }
  }

  bool String::equal_to(const Expression* other) const {
    if (const String *p = static_cast<String const*>(other)) {
      return value == p->value;
    }
    else {
      return false;
    }
  }

  bool Vector::equal_to(const Expression* other) const {
    if (const Vector *p = static_cast<Vector const*>(other)) {
      return inner == p->inner;
    }
    else {
      return false;
    }
  }
}