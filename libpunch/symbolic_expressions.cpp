/*
 *   Copyright (c) 2015 Raymond Kroon. All rights reserved.
 *   The use and distribution terms for this software are covered by the
 *   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
 *   which can be found in the file LICENSE.txt at the root of this distribution.
 *   By using this software in any fashion, you are agreeing to be bound by
 *   the terms of this license.
 *   You must not remove this notice, or any other, from this software.
 */

#include <lang/util.hpp>
#include <lang/symbolic_expressions.hpp>
#include <lang/reader_expressions.hpp>

using namespace punch::lang;
using namespace punch::lang::expressions;


  const String def("def");
  const String fn("fn");
  const String dot(".");

  UExpression Def::create(const Expression &expression) {
    return make_unique<Def>();
  }

  UExpression Fn::create(const Expression &expression) {
    return make_unique<Fn>();
  }

  UExpression Interop::create(const Expression &expression) {
    return make_unique<Interop>();
  }