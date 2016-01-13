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

namespace expression {

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

  std::string List::DebugInfo() const {
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

  bool EndOfFile::equal_to(Expression* other)  {
    if (const EndOfFile *p = static_cast<EndOfFile const*>(other)) {
      return true;
    }
    else {
      return false;
    }
  }

  bool Keyword::equal_to(Expression* other) {
    if (const Keyword *p = static_cast<Keyword const*>(other)) {
      return value == p->value;
    }
    else {
      return false;
    }
  }

  bool Integer::equal_to(Expression* other) {
    if (const Integer *p = static_cast<Integer const*>(other)) {
      return value == p->value;
    }
    else {
      return false;
    }
  }

  bool Float::equal_to(Expression* other) {
    if (const Float *p = static_cast<Float const*>(other)) {
      return  std::fabs(value - p->value) < std::numeric_limits<double>::epsilon();
    }
    else {
      return false;
    }
  }

  bool Ratio::equal_to(Expression* other) {
    if (const Ratio *p = static_cast<Ratio const*>(other)) {
      return numerator == p->numerator && denominator == p->denominator;
    }
    else {
      return false;
    }
  }

  bool Literal::equal_to(Expression* other) {
    if (const Literal *p = static_cast<Literal const*>(other)) {
      return value == p->value;
    }
    else {
      return false;
    }
  }

  bool List::equal_to(Expression* other) {
    if (const List *p = static_cast<List const*>(other)) {
      return inner == p->inner;
    }
    else {
      return false;
    }
  }

  bool Map::equal_to(Expression* other) {
    if (const Map *p = static_cast<Map const*>(other)) {
      return inner == p->inner;
    }
    else {
      return false;
    }
  }

  bool Set::equal_to(Expression* other) {
    if (const Set *p = static_cast<Set const*>(other)) {
      return inner == p->inner;
    }
    else {
      return false;
    }
  }

  bool String::equal_to(Expression* other) {
    if (const String *p = static_cast<String const*>(other)) {
      return value == p->value;
    }
    else {
      return false;
    }
  }

  bool Vector::equal_to(Expression* other) {
    if (const Vector *p = static_cast<Vector const*>(other)) {
      return inner == p->inner;
    }
    else {
      return false;
    }
  }
}