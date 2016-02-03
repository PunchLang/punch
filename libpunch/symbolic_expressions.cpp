/*
 *   Copyright (c) 2015 Raymond Kroon. All rights reserved.
 *   The use and distribution terms for this software are covered by the
 *   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
 *   which can be found in the file LICENSE.txt at the root of this distribution.
 *   By using this software in any fashion, you are agreeing to be bound by
 *   the terms of this license.
 *   You must not remove this notice, or any other, from this software.
 */

#include <util.hpp>
#include <symbolic_expressions.hpp>
#include <reader_expressions.hpp>

expression::UExpression upgrade_expression(const expression::Expression& e) {
  using namespace expression;
  if (Def::accepts(e)) {
    return Def::create(e);
  }
  else if (Fn::accepts(e)) {
    return Fn::create(e);
  }
  else if (Interop::accepts(e)) {
    return Interop::create(e);
  }
  else {
    return make_unique<Expression>();
  }
}

namespace expression {

  const String def("def");
  const String fn("fn");
  const String dot(".");

  UExpression Def::create(const Expression &expression) {
    return make_unique<Def>();
  }

  bool Def::accepts(const Expression& e) {
    if (auto s = as<Symbolic>(&e)) {
      if (s->n() == 2) {
        auto &inner = s->get_inner();
        if ((*inner.front()).equal_to(&def)) {
          return true;
        }
      }
    }
    return false;
  }

  UExpression Fn::create(const Expression &expression) {
    return make_unique<Fn>();
  }

  bool Fn::accepts(const Expression& e) {
    if (auto s = as<Symbolic>(&e)) {
      if (s->n() >= 3) {
        auto &inner = s->get_inner();
        if ((*inner.front()).equal_to(&fn)) {
          auto iter = inner.begin();
          iter++;
          if (auto v = as<Vector>(&**iter)) {
            return true;
          }
        }
      }
    }
    return false;
  }

  UExpression Interop::create(const Expression &expression) {
    return make_unique<Interop>();
  }

  bool Interop::accepts(const Expression& e) {
    if (auto s = as<Symbolic>(&e)) {
      if (s->n() >= 2) {
        auto &inner = s->get_inner();
        if ((*inner.front()).equal_to(&dot)) {
          auto iter = inner.begin();
          iter++;
          if (auto v = as<String>(&**iter)) {
            return true;
          }
        }
      }
    }
    return false;
  }
}