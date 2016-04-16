/*
 *   Copyright (c) 2015 Raymond Kroon. All rights reserved.
 *   The use and distribution terms for this software are covered by the
 *   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
 *   which can be found in the file LICENSE.txt at the root of this distribution.
 *   By using this software in any fashion, you are agreeing to be bound by
 *   the terms of this license.
 *   You must not remove this notice, or any other, from this software.
 */


#ifndef PUNCH_LANG_EXPRESSION_HPP
#define PUNCH_LANG_EXPRESSION_HPP

#include <memory>
#include <lang/tokenizer.hpp>

namespace punch {
  namespace lang {

    template <bool isMutating>
    class TExpressionVisitor;

    class ExpressionVisitor;
    class UpgradingExpressionVisitor;

    namespace expressions {

      enum class ExpressionType {
        Keyword,
        Number,
//        Integer,
//        Float,
//        Ratio,
        Symbol,
        Symbolic,
        Map,
        Set,
        String,
        Vector
      };

      class Expression;

      typedef std::unique_ptr<Expression> UExpression;
      typedef std::shared_ptr<Expression> SharedExpression;

      class Expression : public std::enable_shared_from_this<Expression>{
      public:

        Expression() { }
        virtual ~Expression() { }

        virtual void accept(ExpressionVisitor&) const = 0;
        virtual void accept(UpgradingExpressionVisitor&) = 0;


        virtual ExpressionType type() const = 0;
        virtual bool equal_to(const Expression *e) const = 0;

      };

      inline bool operator==(const UExpression &lhs, const UExpression &rhs) {
        return lhs->type() == rhs->type() && lhs->equal_to(&*rhs);
      }

      inline bool operator!=(const UExpression &lhs, const UExpression &rhs) {
        return !(lhs == rhs);
      }

      inline bool operator==(const SharedExpression &lhs, const SharedExpression &rhs) {
        return lhs->equal_to(&*rhs);
      }

      inline bool operator!=(const SharedExpression &lhs, const SharedExpression &rhs) {
        return !(lhs == rhs);
      }

      inline ::std::ostream &operator<<(::std::ostream &os, const Token &token) {
        return os << token.DebugInfo();
      }

    }
  }
}

#endif //PUNCH_LANG_EXPRESSION_HPP
