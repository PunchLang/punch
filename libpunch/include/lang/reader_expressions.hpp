/*
 *   Copyright (c) 2015 Raymond Kroon. All rights reserved.
 *   The use and distribution terms for this software are covered by the
 *   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
 *   which can be found in the file LICENSE.txt at the root of this distribution.
 *   By using this software in any fashion, you are agreeing to be bound by
 *   the terms of this license.
 *   You must not remove this notice, or any other, from this software.
 */


#ifndef PUNCH_LANG_READER_EXPRESSIONS_HPP
#define PUNCH_LANG_READER_EXPRESSIONS_HPP

#include <lang/expression.hpp>
#include <lang/visitors.hpp>

namespace punch {
  namespace lang {
    class Reader;

    template<typename T>
    T const *as(const expressions::Expression *e) {
      return as_type<expressions::Expression, T>(e);
    }

    namespace expressions {

      class Keyword : public Expression {
      public:
        Keyword(std::string value) : value(value) { }
        Keyword(Keyword &&other) : value(std::move(other.value)) { }

        static bool accepts(Token &);

        static UExpression create(Reader *, std::string &error);

        void accept(ExpressionVisitor &) const override;
        void accept(UpgradingExpressionVisitor &) override;

        ExpressionType type() const override {
          return ExpressionType::Keyword;
        }

        const std::string value;

      protected:
        bool equal_to(const Expression *other) const override;
      };

      class Number : public Expression {
      public:
        Number(std::string value) : value (value) {}
        Number(Number &&other) : value(std::move(other.value)) {}

        static bool accepts(Token &tok);
        static UExpression create(Reader *r, std::string &error);

        void accept(ExpressionVisitor &) const override;
        void accept(UpgradingExpressionVisitor &) override;

        ExpressionType type() const override {
          return ExpressionType::Number;
        };

        const std::string value;

      protected:
        bool equal_to(const Expression *other) const override;
      };

//      class Integer : public Expression {
//      public:
//        Integer(long value) : value(value) { }
//        Integer(Integer &&other) : value(std::move(other.value)) { }
//
//        static bool accepts(Token &);
//
//        static UExpression create(Reader *, std::string &error);
//
//        void accept(ExpressionVisitor &) const override;
//        void accept(UpgradingExpressionVisitor &) override;
//
//        ExpressionType type() const override {
//          return ExpressionType::Integer;
//        }
//
//        const long value;
//
//      protected:
//        bool equal_to(const Expression *other) const override;
//
//      };
//
//      class Float : public Expression {
//      public:
//        Float(double value) : value(value) { }
//        Float(Float &&other) : value(std::move(other.value)) { }
//
//        static bool accepts(Token &);
//
//        static UExpression create(Reader *, std::string &error);
//
//        void accept(ExpressionVisitor &) const override;
//        void accept(UpgradingExpressionVisitor &) override;
//
//        ExpressionType type() const override {
//          return ExpressionType::Float;
//        }
//
//        const double value;
//
//      protected:
//        bool equal_to(const Expression *other) const override;
//
//      };
//
//      class Ratio : public Expression {
//      public:
//        Ratio(long n, long d) : numerator(n), denominator(d) { }
//        Ratio(Ratio &&other) : numerator(std::move(other.numerator)), denominator(std::move(other.denominator)) { }
//
//        static bool accepts(Token &);
//
//        static UExpression create(Reader *, std::string &error);
//
//        void accept(ExpressionVisitor &) const override;
//        void accept(UpgradingExpressionVisitor &) override;
//
//        ExpressionType type() const override {
//          return ExpressionType::Ratio;
//        }
//
//        const long numerator;
//        const long denominator;
//
//      protected:
//        bool equal_to(const Expression *other) const override;
//
//      };

      class Symbol : public Expression {
      public:
        Symbol(std::string value) : value(value) { }
        Symbol(Symbol &&other) : value(std::move(other.value)) { }

        static bool accepts(Token &);

        static SharedExpression create(Reader *, std::string &error);

        void accept(ExpressionVisitor &) const override;
        void accept(UpgradingExpressionVisitor &) override;

        ExpressionType type() const override {
          return ExpressionType::Symbol;
        }

        const std::string value;

      protected:
        bool equal_to(const Expression *other) const override;

      };

      class Symbolic : public Expression {
      public:
        Symbolic() {}
        Symbolic(std::list<SharedExpression> &inner) : inner(std::move(inner)) { }
        Symbolic(Symbolic &&other) : inner(std::move(other.inner)) { }

        static bool accepts(Token &);

        static SharedExpression create(Reader *, std::string &error);

        virtual void accept(ExpressionVisitor &) const override;
        virtual void accept(UpgradingExpressionVisitor &) override;

        int n() const;

        ExpressionType type() const override {
          return ExpressionType::Symbolic;
        }

        std::list<SharedExpression> const * const get_inner() const;
        std::list<SharedExpression> * get_inner();

      protected:
        bool equal_to(const Expression *other) const override;

      private:
        std::list<SharedExpression> inner;
      };

      class Map : public Expression {
      public:
        Map(std::list<SharedExpression> &inner) : inner(std::move(inner)) { }
        Map(Map &&other) : inner(std::move(other.inner)) { }

        static bool accepts(Token &);

        static SharedExpression create(Reader *, std::string &error);

        void accept(ExpressionVisitor &) const override;
        void accept(UpgradingExpressionVisitor &) override;

        ExpressionType type() const override {
          return ExpressionType::Map;
        }

        std::list<SharedExpression> const * const get_inner() const;
        std::list<SharedExpression> * get_inner();

      protected:
        bool equal_to(const Expression *other) const override;

      private:
        std::list<SharedExpression> inner;
      };

      class Set : public Expression {
      public:
        Set(std::list<SharedExpression> &inner) : inner(std::move(inner)) { }
        Set(Set &&other) : inner(std::move(other.inner)) { }

        static bool accepts(Token &);

        static SharedExpression create(Reader *, std::string &error);

        void accept(ExpressionVisitor &) const override;
        void accept(UpgradingExpressionVisitor &) override;

        ExpressionType type() const override {
          return ExpressionType::Set;
        }

        std::list<SharedExpression> const * const get_inner() const;
        std::list<SharedExpression> * get_inner();

      protected:
        bool equal_to(const Expression *other) const override;

      private:
        std::list<SharedExpression> inner;
      };

      class String : public Expression {
      public:
        String(std::string value) : value(value) { }
        String(String &&other) : value(std::move(other.value)) { }

        static bool accepts(Token &);

        static SharedExpression create(Reader *, std::string &error);

        void accept(ExpressionVisitor &) const override;
        void accept(UpgradingExpressionVisitor &) override;

        ExpressionType type() const override {
          return ExpressionType::String;
        }

        const std::string value;

      protected:
        bool equal_to(const Expression *other) const override;
      };

      class Vector : public Expression {
      public:
        Vector(std::list<SharedExpression> &inner) : inner(std::move(inner)) { }
        Vector(Vector &&other) : inner(std::move(other.inner)) { }

        static bool accepts(Token &);

        static SharedExpression create(Reader *, std::string &error);

        void accept(ExpressionVisitor &) const override;
        void accept(UpgradingExpressionVisitor &) override;

        ExpressionType type() const override {
          return ExpressionType::Vector;
        }

        std::list<SharedExpression> const * const get_inner() const;
        std::list<SharedExpression> * get_inner();

      protected:
        bool equal_to(const Expression *other) const override;

      private:
        std::list<SharedExpression> inner;
      };
    }
  }
}
#endif //PUNCH_LANG_READER_EXPRESSIONS_HPP
