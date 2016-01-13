/*
 *   Copyright (c) 2015 Raymond Kroon. All rights reserved.
 *   The use and distribution terms for this software are covered by the
 *   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
 *   which can be found in the file LICENSE.txt at the root of this distribution.
 *   By using this software in any fashion, you are agreeing to be bound by
 *   the terms of this license.
 *   You must not remove this notice, or any other, from this software.
 */


#ifndef PUNCH_EXPRESSION_HPP
#define PUNCH_EXPRESSION_HPP

#include <llvm/IR/Value.h>
#include <memory>
#include <tokenizer.hpp>

namespace expression {

  class Expression;

  typedef std::unique_ptr <Expression> UExpression;
  typedef std::shared_ptr <Expression> SharedExpression;

  class Expression {
  public:

    Expression() { }
    virtual ~Expression() { }

    virtual std::string DebugInfo() const = 0;

    virtual llvm::Value *llvm_codegen() = 0;

    virtual bool equal_to(Expression *e) = 0;

  };

  inline ::std::ostream &operator<<(::std::ostream &os, const SharedExpression &expression) {
    return os << expression->DebugInfo();
  }

  inline ::std::ostream &operator<<(::std::ostream &os, const Expression *expression) {
    return os << expression->DebugInfo();
  }

  inline bool operator==(const UExpression &lhs, const UExpression &rhs) {
    return lhs->equal_to(&*rhs);
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

#endif //PUNCH_EXPRESSION_HPP
