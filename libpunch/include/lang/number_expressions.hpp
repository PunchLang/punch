/*
 *   Copyright (c) 2015 Raymond Kroon. All rights reserved.
 *   The use and distribution terms for this software are covered by the
 *   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
 *   which can be found in the file LICENSE.txt at the root of this distribution.
 *   By using this software in any fashion, you are agreeing to be bound by
 *   the terms of this license.
 *   You must not remove this notice, or any other, from this software.
 */


#ifndef PUNCH_NUMBER_EXPRESSIONS_HPP
#define PUNCH_NUMBER_EXPRESSIONS_HPP

#include <lang/expression.hpp>

namespace punch {
  namespace lang {
    namespace expressions {

      class Upgraded {
      public:
        Upgraded(SharedExpression original) : _original(std::move(original)) {}
        Upgraded(Upgraded&& other) : _original(std::move(other._original)) {}

        SharedExpression const & original() {return _original;}

      private:
        SharedExpression _original;
      };

      class Integer : public Expression, Upgraded {
      public:
        Integer(long value, SharedExpression original) : Upgraded(original), value(value) { }
        Integer(Integer &&other) : Upgraded(std::move(other)), value(std::move(other.value)) { }

        static bool accepts(Expression const * const e);
        static Integer from_number(SharedExpression const & number);

        void accept(ExpressionVisitor &) const override;
        void accept(UpgradingExpressionVisitor &) override;

        ExpressionType type() const override {
          return ExpressionType::Integer;
        }

        const long value;

      protected:
        bool equal_to(const Expression *other) const override;

      };

      class Float : public Expression, Upgraded {
      public:
        Float(double value, SharedExpression original) : Upgraded(original), value(value) { }
        Float(Float &&other) : Upgraded(std::move(other)), value(std::move(other.value)) { }

        static bool accepts(Expression const * const e);
        static Float from_number(SharedExpression const & number);

        void accept(ExpressionVisitor &) const override;
        void accept(UpgradingExpressionVisitor &) override;

        ExpressionType type() const override {
          return ExpressionType::Float;
        }

        const double value;

      protected:
        bool equal_to(const Expression *other) const override;

      };

      class Ratio : public Expression, Upgraded {
      public:
        Ratio(long n, long d, SharedExpression original) : Upgraded(original), numerator(n), denominator(d) { }
        Ratio(Ratio &&other) : Upgraded(std::move(other)),
            numerator(std::move(other.numerator)), denominator(std::move(other.denominator)) { }

        static bool accepts(Expression const * const e);
        static Ratio from_number(SharedExpression const & number);

        void accept(ExpressionVisitor &) const override;
        void accept(UpgradingExpressionVisitor &) override;

        ExpressionType type() const override {
          return ExpressionType::Ratio;
        }

        const long numerator;
        const long denominator;

      protected:
        bool equal_to(const Expression *other) const override;

      };
    }
  }
}

#endif //PUNCH_NUMBER_EXPRESSIONS_HPP
