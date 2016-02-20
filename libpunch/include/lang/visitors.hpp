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
      class Integer;
      class Float;
      class Ratio;
      class Literal;
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

      virtual void visit(Expression const *) = 0;

      virtual void handle_keyword(Keyword const &val) = 0;

      virtual void handle_integer(Integer const &val) = 0;

      virtual void handle_float(Float const &val) = 0;

      virtual void handle_ratio(Ratio const &val) = 0;

      virtual void handle_literal(Literal const &val) = 0;

      virtual void handle_symbolic(Symbolic const &val) = 0;

      virtual void handle_map(Map const &val) = 0;

      virtual void handle_set(Set const &val) = 0;

      virtual void handle_string(String const &val) = 0;

      virtual void handle_vector(Vector const &val) = 0;

    private:
      virtual ExpressionVisitor* derived()  = 0;
    };


    class MutatingExpressionVisitor {
    public:
      virtual ~MutatingExpressionVisitor() { };

      virtual void visit(Expression const *) = 0;
    };

    template <typename Derived>
    class RecursiveVisitor : public ExpressionVisitor {
    public:
      void visit(Expression const * expression) {
        if (expression) {
          handle_expression(expression);
        }
      }

    private:
      ExpressionVisitor* derived() {
        return static_cast<Derived*>(this);
      }

      template <typename e_type>
      e_type const * _cast(Expression const * expression) {
        return as_type<Expression, e_type>(expression);
      }

      void handle_expression(Expression const * expression) {
        if (expression->type() == ExpressionType::Keyword) {
          auto expr = _cast<Keyword>(expression);
          derived()->handle_keyword(*expr);
        }
        else if (expression->type() == ExpressionType::Integer) {
          auto expr = _cast<Integer>(expression);
          derived()->handle_integer(*expr);
        }
        else if (expression->type() == ExpressionType::Float) {
          auto expr = _cast<Float>(expression);
          derived()->handle_float(*expr);
        }
        else if (expression->type() == ExpressionType::Ratio) {
          auto expr = _cast<Ratio>(expression);
          derived()->handle_ratio(*expr);
        }
        else if (expression->type() == ExpressionType::Literal) {
          auto expr = _cast<Literal>(expression);
          derived()->handle_literal(*expr);
        }
        else if (expression->type() == ExpressionType::Symbolic) {
          auto expr = _cast<Symbolic>(expression);
          derived()->handle_symbolic(*expr);
        }
        else if (expression->type() == ExpressionType::Map) {
          auto expr = _cast<Map>(expression);
          derived()->handle_map(*expr);
        }
        else if (expression->type() == ExpressionType::Set) {
          auto expr = _cast<Set>(expression);
          derived()->handle_set(*expr);
        }
        else if (expression->type() == ExpressionType::String) {
          auto expr = _cast<String>(expression);
          derived()->handle_string(*expr);
        }
        else if (expression->type() == ExpressionType::Vector) {
          auto expr = _cast<Vector>(expression);
          derived()->handle_vector(*expr);
        }
      }
    };

    #define Derive_RecursiveVisitor(Type) class Type: public RecursiveVisitor<Type>

    Derive_RecursiveVisitor(LoggingVisitor) {

    public:

      LoggingVisitor() : LoggingVisitor(std::cout, 0){}
      LoggingVisitor(std::ostream &os, int start_level) : os(os), level(start_level) {}

      void handle_keyword(Keyword const &val) override;
      void handle_integer(Integer const &val) override;
      void handle_float(Float const &val) override;
      void handle_ratio(Ratio const &val) override;
      void handle_literal(Literal const &val) override;
      void handle_symbolic(Symbolic const &val) override;
      void handle_map(Map const &val) override;
      void handle_set(Set const &val) override;
      void handle_string(String const &val) override;
      void handle_vector(Vector const &val) override;

    private:
      void print(std::string s) {
        os << s;
        if (level == 0) {
          os << std::endl;
        }
      }

      int level = 0;
      std::ostream& os;

    };
  }
}

#endif //PUNCH_LANG_VISITORS_HPP
