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

namespace punch {
  namespace lang {
    class Reader;

    template<typename T>
    T const *as(const expression::Expression *e) {
      return as_type<expression::Expression, T>(e);
    }

    namespace expression {
      class EndOfFile : public Expression {
      public:
        EndOfFile() { }

        std::string DebugInfo() const override;

      protected:
        bool equal_to(const Expression *other) const override;
      };

      class Keyword : public Expression {
      public:
        Keyword(std::string value) : value(value) { }
        Keyword(Keyword &&other) : value(std::move(other.value)) { }

        static bool accepts(Token &);

        static UExpression create(Reader *, std::string &error);

        std::string DebugInfo() const override;

      protected:
        bool equal_to(const Expression *other) const override;

      private:
        std::string value;
      };

      class Integer : public Expression {
      public:
        Integer(long value) : value(value) { }
        Integer(Integer &&other) : value(std::move(other.value)) { }

        static bool accepts(Token &);

        static UExpression create(Reader *, std::string &error);

        std::string DebugInfo() const override;

      protected:
        bool equal_to(const Expression *other) const override;

      private:
        long value;
      };

      class Float : public Expression {
      public:
        Float(double value) : value(value) { }
        Float(Float &&other) : value(std::move(other.value)) { }

        static bool accepts(Token &);

        static UExpression create(Reader *, std::string &error);

        std::string DebugInfo() const override;

      protected:
        bool equal_to(const Expression *other) const override;

      private:
        double value;
      };

      class Ratio : public Expression {
      public:
        Ratio(long n, long d) : numerator(n), denominator(d) { }
        Ratio(Ratio &&other) : numerator(std::move(other.numerator)), denominator(std::move(other.denominator)) { }

        static bool accepts(Token &);

        static UExpression create(Reader *, std::string &error);

        std::string DebugInfo() const override;

      protected:
        bool equal_to(const Expression *other) const override;

      private:
        long numerator;
        long denominator;
      };

      class Literal : public Expression {
      public:
        Literal(std::string value) : value(value) { }
        Literal(Literal &&other) : value(std::move(other.value)) { }

        static bool accepts(Token &);

        static UExpression create(Reader *, std::string &error);

        std::string DebugInfo() const override;

      protected:
        bool equal_to(const Expression *other) const override;

      private:
        std::string value;
      };

      class Symbolic : public Expression {
      public:
        Symbolic(std::list<UExpression> &inner) : inner(std::move(inner)) { }
        Symbolic(Symbolic &&other) : inner(std::move(other.inner)) { }

        static bool accepts(Token &);

        static UExpression create(Reader *, std::string &error);

        std::string DebugInfo() const override;

        int n() const;

        std::list<UExpression> const &get_inner() const;

      protected:
        bool equal_to(const Expression *other) const override;

      private:
        std::list<UExpression> inner;
      };

      class Map : public Expression {
      public:
        Map(std::list<UExpression> &inner) : inner(std::move(inner)) { }
        Map(Map &&other) : inner(std::move(other.inner)) { }

        static bool accepts(Token &);

        static UExpression create(Reader *, std::string &error);

        std::string DebugInfo() const override;

      protected:
        bool equal_to(const Expression *other) const override;

      private:
        std::list<UExpression> inner;
      };

      class Set : public Expression {
      public:
        Set(std::list<UExpression> &inner) : inner(std::move(inner)) { }
        Set(Set &&other) : inner(std::move(other.inner)) { }

        static bool accepts(Token &);

        static UExpression create(Reader *, std::string &error);

        std::string DebugInfo() const override;

      protected:
        bool equal_to(const Expression *other) const override;

      private:
        std::list<UExpression> inner;
      };

      class String : public Expression {
      public:
        String(std::string value) : value(value) { }
        String(String &&other) : value(std::move(other.value)) { }

        static bool accepts(Token &);

        static UExpression create(Reader *, std::string &error);

        std::string DebugInfo() const override;

      protected:
        bool equal_to(const Expression *other) const override;

      private:
        std::string value;
      };

      class Vector : public Expression {
      public:
        Vector(std::list<UExpression> &inner) : inner(std::move(inner)) { }
        Vector(Vector &&other) : inner(std::move(other.inner)) { }

        static bool accepts(Token &);

        static UExpression create(Reader *, std::string &error);

        std::string DebugInfo() const override;

      protected:
        bool equal_to(const Expression *other) const override;

      private:
        std::list<UExpression> inner;
      };
    }
  }
}
#endif //PUNCH_LANG_READER_EXPRESSIONS_HPP
