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
#include <lang/visitors.hpp>
#include <lang/reader.hpp>

using namespace punch::lang;
using namespace punch::lang::expressions;

int Symbolic::n() const {
  return inner.size();
}

std::list<SharedExpression> const * Symbolic::get_inner() const {
  return &inner;
}

std::list<SharedExpression> * Symbolic::get_inner() {
  return &inner;
}

std::list<SharedExpression> const * Map::get_inner() const {
  return &inner;
}

std::list<SharedExpression> * Map::get_inner() {
  return &inner;
}

std::list<SharedExpression> const * Vector::get_inner() const {
  return &inner;
}

std::list<SharedExpression> * Vector::get_inner() {
  return &inner;
}

std::list<SharedExpression> const * Set::get_inner() const {
  return &inner;
}

std::list<SharedExpression> * Set::get_inner() {
  return &inner;
}

void Keyword::accept(ExpressionVisitor &v) const {
  v.visit(this);
}

void Keyword::accept(UpgradingExpressionVisitor &v) {
  auto sh = shared_from_this();
  v.upgrade(sh);
}

void Number::accept(ExpressionVisitor &v) const {
  v.visit(this);
}

void Number::accept(UpgradingExpressionVisitor &v) {
  auto sh = shared_from_this();
  v.upgrade(sh);
}

void Symbol::accept(ExpressionVisitor &v) const {
  v.visit(this);
}

void Symbol::accept(UpgradingExpressionVisitor &v) {
  auto sh = shared_from_this();
  v.upgrade(sh);
}

void Symbolic::accept(ExpressionVisitor &v) const {
  v.visit(this);
}

void Symbolic::accept(UpgradingExpressionVisitor &v) {
  auto sh = shared_from_this();
  v.upgrade(sh);
}

void Map::accept(ExpressionVisitor &v) const {
  v.visit(this);
}

void Map::accept(UpgradingExpressionVisitor &v) {
  auto sh = shared_from_this();
  v.upgrade(sh);
}

void Set::accept(ExpressionVisitor &v) const {
  v.visit(this);
}

void Set::accept(UpgradingExpressionVisitor &v) {
  auto sh = shared_from_this();
  v.upgrade(sh);
}

void String::accept(ExpressionVisitor &v) const {
  v.visit(this);
}

void String::accept(UpgradingExpressionVisitor &v) {
  auto sh = shared_from_this();
  v.upgrade(sh);
}

void Vector::accept(ExpressionVisitor &v) const {
  v.visit(this);
}

void Vector::accept(UpgradingExpressionVisitor &v) {
  auto sh = shared_from_this();
  v.upgrade(sh);
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

  bool Number::equal_to(const Expression* other) const {
    if (auto p = as<Number>(other)) {
      return value == p->value;
    }
    else {
      return false;
    }
  }

  bool Symbol::equal_to(const Expression* other) const {
    if (auto p = as<Symbol>(other)) {
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
