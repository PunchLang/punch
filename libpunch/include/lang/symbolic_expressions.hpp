/*
 *   Copyright (c) 2015 Raymond Kroon. All rights reserved.
 *   The use and distribution terms for this software are covered by the
 *   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
 *   which can be found in the file LICENSE.txt at the root of this distribution.
 *   By using this software in any fashion, you are agreeing to be bound by
 *   the terms of this license.
 *   You must not remove this notice, or any other, from this software.
 */


#ifndef PUNCH_LANG_SYMBOLIC_EXPRESSIONS_HPP
#define PUNCH_LANG_SYMBOLIC_EXPRESSIONS_HPP

#include <lang/expression.hpp>

namespace punch {
  namespace lang {
    namespace expressions {

      class Def : public Expression {
      public:
        Def() { }

        static bool accepts(const Expression &expression);

        static UExpression create(const Expression &expression);

        void accept(ExpressionVisitor&) const override;
        void accept(MutatingExpressionVisitor&) override;

      protected:
        bool equal_to(const Expression *other) const override;
      };

      class Fn : public Expression {
      public:
        Fn() { }

        static bool accepts(const Expression &expression);

        static UExpression create(const Expression &expression);

        void accept(ExpressionVisitor&) const override;
        void accept(MutatingExpressionVisitor&) override;

      protected:
        bool equal_to(const Expression *other) const override;
      };


      // (. punch.lang add 1 2), struct, static/instanced
      // (. punch.lang/add 1 2), namespaced

      class Interop : public Expression {
      public:
        Interop() { }

        static bool accepts(const Expression &expression);

        static UExpression create(const Expression &expression);

        void accept(ExpressionVisitor&) const override;
        void accept(MutatingExpressionVisitor&) override;

      protected:
        bool equal_to(const Expression *other) const override;
      };
    }
  }
}

#endif //PUNCH_LANG_SYMBOLIC_EXPRESSIONS_HPP
