/*
 *   Copyright (c) 2015 Raymond Kroon. All rights reserved.
 *   The use and distribution terms for this software are covered by the
 *   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
 *   which can be found in the file LICENSE.txt at the root of this distribution.
 *   By using this software in any fashion, you are agreeing to be bound by
 *   the terms of this license.
 *   You must not remove this notice, or any other, from this software.
 */

#include <lang/reader_expressions.hpp>
#include <lang/reader.hpp>
#include <lang/util.hpp>

using namespace punch::lang;
using namespace punch::lang::expressions;

template <typename T>
T const *as(const Expression* e) {
  return as_type<Expression, T>(e);
}

int Symbolic::n() const {
  return inner.size();
}

std::list<UExpression> const * const Symbolic::get_inner() const {
  return &inner;
}

std::list<UExpression> const * const Map::get_inner() const {
  return &inner;
}

std::list<UExpression> const * const Vector::get_inner() const {
  return &inner;
}

std::list<UExpression> const * const Set::get_inner() const {
  return &inner;
}

void Keyword::accept(ExpressionVisitor &v) const {
  v.visit(this);
}

void Keyword::accept(MutatingExpressionVisitor &v) {
  v.visit(this);
}

void Integer::accept(ExpressionVisitor &v) const {
  v.visit(this);
}

void Integer::accept(MutatingExpressionVisitor &v) {
  v.visit(this);
}

void Float::accept(ExpressionVisitor &v) const {
  v.visit(this);
}

void Float::accept(MutatingExpressionVisitor &v) {
  v.visit(this);
}

void Ratio::accept(ExpressionVisitor &v) const {
  v.visit(this);
}

void Ratio::accept(MutatingExpressionVisitor &v) {
  v.visit(this);
}

void Literal::accept(ExpressionVisitor &v) const {
  v.visit(this);
}

void Literal::accept(MutatingExpressionVisitor &v) {
  v.visit(this);
}

void Symbolic::accept(ExpressionVisitor &v) const {
  v.visit(this);
}

void Symbolic::accept(MutatingExpressionVisitor &v) {
  v.visit(this);
}

void Map::accept(ExpressionVisitor &v) const {
  v.visit(this);
}

void Map::accept(MutatingExpressionVisitor &v) {
  v.visit(this);
}

void Set::accept(ExpressionVisitor &v) const {
  v.visit(this);
}

void Set::accept(MutatingExpressionVisitor &v) {
  v.visit(this);
}

void String::accept(ExpressionVisitor &v) const {
  v.visit(this);
}

void String::accept(MutatingExpressionVisitor &v) {
  v.visit(this);
}

void Vector::accept(ExpressionVisitor &v) const {
  v.visit(this);
}

void Vector::accept(MutatingExpressionVisitor &v) {
  v.visit(this);
}

  /* EQUALITY */

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
