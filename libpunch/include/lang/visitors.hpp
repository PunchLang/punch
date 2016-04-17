/*
 *   Copyright (c) 2015 Raymond Kroon. All rights reserved.
 *   The use and distribution terms for this software are covered by the
 *   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
 *   which can be found in the file LICENSE.txt at the root of this distribution.
 *   By using this software in any fashion, you are agreeing to be bound by
 *   the terms of this license.
 *   You must not remove this notice, or any other, from this software.
 */


#ifndef PUNCH_LANG_VISITORS_HPP
#define PUNCH_LANG_VISITORS_HPP

#include <type_traits>
#include <lang/expression.hpp>
#include <lang/util.hpp>

namespace ple = punch::lang::expressions;

namespace punch{
  namespace lang {

    namespace expressions {
      class Keyword;
      class Number;
      class Integer;
      class Float;
      class Ratio;
      class Symbol;
      class Symbolic;
      class Map;
      class Set;
      class String;
      class Vector;
    }

    using namespace expressions;


    class ExpressionVisitor {
    public:
      virtual ~ExpressionVisitor() { };

      virtual void visit(Expression const * const) = 0;

      virtual void before(Keyword const &val) {};
      virtual void after(Keyword const &val) {};

      virtual void before(Number const &val) {};
      virtual void after(Number const &val) {};

      virtual void before(Integer const &val) {};
      virtual void after(Integer const &val) {};

      virtual void before(Float const &val) {};
      virtual void after(Float const &val) {};

      virtual void before(Ratio const &val) {};
      virtual void after(Ratio const &val) {};

      virtual void before(Symbol const &val) {};
      virtual void after(Symbol const &val) {};

      virtual void before(Symbolic const &val) {};
      virtual void after(Symbolic const &val) {};

      virtual void before(Map const &val) {};
      virtual void after(Map const &val) {};

      virtual void before(Set const &val) {};
      virtual void after(Set const &val) {};

      virtual void before(String const &val) {};
      virtual void after(String const &val) {};

      virtual void before(Vector const &val) {};
      virtual void after(Vector const &val) {};

    private:
      virtual ExpressionVisitor* derived()  = 0;
    };

    template <typename Derived>
    class RecursiveVisitor : public ExpressionVisitor {
    public:

      RecursiveVisitor(int starting_level) : level(starting_level) {}

      void visit(Expression const * expression) override {
        if (expression) {
          handle_expression(expression);
        }
      }

    protected:
      int level = 0;

    private:

      ExpressionVisitor* derived() override {
        return static_cast<Derived*>(this);
      }

      template <typename e_type>
      e_type const * _cast(Expression const * expression) {
        return as_type<Expression, e_type>(expression);
      }

      void handle_expression(Expression const * const expression);
    };

    #define Derive_RecursiveVisitor(Type) class Type: public RecursiveVisitor<Type>

    class UpgradingExpressionVisitor {
    public:
      virtual ~UpgradingExpressionVisitor() { };

      virtual void upgrade(SharedExpression &) = 0;

      virtual void upgrade(std::list<SharedExpression>& expressions) = 0;

      virtual void handle(Keyword const &val, SharedExpression& original) {}
      virtual void handle(Number const &val, SharedExpression& original) {}
      virtual void handle(Integer const &val, SharedExpression& original) {}
      virtual void handle(Float const &val, SharedExpression& original) {}
      virtual void handle(Ratio const &val, SharedExpression& original) {}
      virtual void handle(Symbol const &val, SharedExpression& original) {}
      virtual void handle(Symbolic const &val, SharedExpression& original) {}
      virtual void handle(Map const &val, SharedExpression& original) {}
      virtual void handle(Set const &val, SharedExpression& original) {}
      virtual void handle(String const &val, SharedExpression& original) {}
      virtual void handle(Vector const &val, SharedExpression& original) {}

    private:
      virtual UpgradingExpressionVisitor* derived() = 0;
    };

    template <typename Derived>
    class RecursiveUpgradingVisitor : public UpgradingExpressionVisitor {
    public :
      RecursiveUpgradingVisitor() { }

      void upgrade(SharedExpression &expression) override {
        handle_expression(expression);
      }

      void upgrade(std::list<SharedExpression> &expressions) override {
        for (auto it = expressions.begin(); it != expressions.end(); ++it) {
          upgrade(*it);
        }
      }

    protected:
      int level = 0;

      template<typename e_type>
      e_type *_cast(SharedExpression expression) {
        return const_cast<e_type *>(as_type<Expression, e_type>(&*expression));
      }

    private:

      UpgradingExpressionVisitor *derived() override {
        return static_cast<Derived *>(this);
      }

      void handle_expression(SharedExpression &expression);
    };

    #define Derive_RecursiveMutatingVisitor(Type) class Type: public RecursiveMutatingVisitor<Type>

    Derive_RecursiveVisitor(LoggingVisitor) {

    public:

      LoggingVisitor() : LoggingVisitor(std::cout, 0){}
      LoggingVisitor(std::ostream &os, int starting_level) : RecursiveVisitor(starting_level), os(os) {}

      void before(Keyword const &val) override;
      void before(Number const &val) override;
      void before(Integer const &val) override;
      void before(Float const &val) override;
      void before(Ratio const &val) override;
      void before(Symbol const &val) override;
      void before(Symbolic const &val) override;
      void after(Symbolic const &val) override;
      void before(Map const &val) override;
      void after(Map const &val) override;
      void before(Set const &val) override;
      void after(Set const &val) override;
      void before(String const &val) override;
      void before(Vector const &val) override;
      void after(Vector const &val) override;

    private:

      void print(std::string s) {
        if (!no_space) {
          os << " ";
        }

        if (no_space) {
          no_space = false;
        }

        os << s;
        if (level == 0) {
          os << std::endl;
        }
      }

      bool no_space = false;
      std::ostream& os;

    };

    class NumberParser : public RecursiveUpgradingVisitor<NumberParser> {
    public:
      NumberParser() {}

      void handle(Number const &val, SharedExpression& original) override;
    };
  }
}


#endif //PUNCH_LANG_VISITORS_HPP
