/*
 *   Copyright (c) 2015 Raymond Kroon. All rights reserved.
 *   The use and distribution terms for this software are covered by the
 *   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
 *   which can be found in the file LICENSE.txt at the root of this distribution.
 *   By using this software in any fashion, you are agreeing to be bound by
 *   the terms of this license.
 *   You must not remove this notice, or any other, from this software.
 */

#include <lang/visitors.hpp>
#include <lang/reader_expressions.hpp>
#include <lang/number_expressions.hpp>

namespace punch {
  namespace lang {

    template <typename Derived>
    void RecursiveVisitor<Derived>::handle_expression(Expression const * const expression) {
      if (expression->type() == ExpressionType::Keyword) {
        auto expr = _cast<Keyword>(expression);

        derived()->before(*expr);
        derived()->after(*expr);
      }
      else if (expression->type() == ExpressionType::Number) {
        auto expr = _cast<Number>(expression);

        derived()->before(*expr);
        derived()->after(*expr);
      }
      else if (expression->type() == ExpressionType::Integer) {
        auto expr = _cast<Integer>(expression);

        derived()->before(*expr);
        derived()->after(*expr);
      }
      else if (expression->type() == ExpressionType::Float) {
        auto expr = _cast<Float>(expression);

        derived()->before(*expr);
        derived()->after(*expr);
      }
      else if (expression->type() == ExpressionType::Ratio) {
        auto expr = _cast<Ratio>(expression);

        derived()->before(*expr);
        derived()->after(*expr);
      }
      else if (expression->type() == ExpressionType::Symbol) {
        auto expr = _cast<Symbol>(expression);

        derived()->before(*expr);
        derived()->after(*expr);
      }
      else if (expression->type() == ExpressionType::Symbolic) {
        level++;

        auto expr = _cast<Symbolic>(expression);

        derived()->before(*expr);

        auto inner = expr->get_inner();
        for (auto it = inner->begin(); it != inner->end(); ++it) {
          (*it)->accept(*this);
        }

        derived()->after(*expr);
        level--;
      }
      else if (expression->type() == ExpressionType::Map) {
        level++;

        auto expr = _cast<Map>(expression);

        derived()->before(*expr);

        auto inner = expr->get_inner();
        auto it = inner->begin();

        while (it != inner->end()) {

          (*it)->accept(*this);
          ++it;
          (*it)->accept(*this);

        }

        derived()->after(*expr);
        level--;
      }
      else if (expression->type() == ExpressionType::Set) {
        level++;

        auto expr = _cast<Set>(expression);

        derived()->before(*expr);

        auto inner = expr->get_inner();
        for (auto it = inner->begin(); it != inner->end(); ++it) {
          (*it)->accept(*this);
        }

        derived()->after(*expr);
        level--;
      }
      else if (expression->type() == ExpressionType::String) {
        auto expr = _cast<String>(expression);

        derived()->before(*expr);
        derived()->after(*expr);
      }
      else if (expression->type() == ExpressionType::Vector) {
        level++;

        auto expr = _cast<Vector>(expression);

        derived()->before(*expr);

        auto inner = expr->get_inner();
        for (auto it = inner->begin(); it != inner->end(); ++it) {
          (*it)->accept(*this);
        }

        derived()->after(*expr);
        level--;
      }
      else {
        assert(false);
      }
    }

    template <typename Derived>
    void RecursiveUpgradingVisitor<Derived>::handle_expression(SharedExpression & expression) {
      if (expression->type() == ExpressionType::Keyword) {
        auto kw = _cast<Keyword>(expression);

        derived()->handle(*kw, expression);
      }
      else if (expression->type() == ExpressionType::Number) {
        auto number = _cast<Number>(expression);

        derived()->handle(*number, expression);
      }
      else if (expression->type() == ExpressionType::Integer) {
        auto integer = _cast<Integer>(expression);

        derived()->handle(*integer, expression);
      }
      else if (expression->type() == ExpressionType::Float) {
        auto fl = _cast<Float>(expression);

        derived()->handle(*fl, expression);
      }
      else if (expression->type() == ExpressionType::Ratio) {
        auto ratio = _cast<Ratio>(expression);

        derived()->handle(*ratio, expression);
      }
      else if (expression->type() == ExpressionType::Symbol) {
        auto sym = _cast<Symbol>(expression);

        derived()->handle(*sym, expression);
      }
      else if (expression->type() == ExpressionType::Symbolic) {
        level++;

        auto symbolic = _cast<Symbolic>(expression);

        auto inner = symbolic->get_inner();
        this->upgrade(*inner);

        derived()->handle(*symbolic, expression);
        level--;
      }
      else if (expression->type() == ExpressionType::Map) {
        level++;

        auto map = _cast<Map>(expression);

        auto inner = map->get_inner();
        this->upgrade(*inner);

        derived()->handle(*map, expression);
        level--;
      }
      else if (expression->type() == ExpressionType::Set) {
        level++;

        auto set = _cast<Set>(expression);

        auto inner = set->get_inner();
        this->upgrade(*inner);

        derived()->handle(*set, expression);
        level--;
      }
      else if (expression->type() == ExpressionType::String) {
        auto s = _cast<String>(expression);

        derived()->handle(*s, expression);
      }
      else if (expression->type() == ExpressionType::Vector) {
        level++;

        auto vec = _cast<Vector>(expression);

        auto inner = vec->get_inner();
        this->upgrade(*inner);

        derived()->handle(*vec, expression);
        level--;
      }
      else {
        assert(false);
      }
    }

    void LoggingVisitor::before(Keyword const &v) {
      print(":" + v.value);
    }

    void LoggingVisitor::before(Number const &val) {
      print(val.value);
    }

    void LoggingVisitor::before(Integer const &v) {
      print("i:" + std::to_string(v.value));
    }

    void LoggingVisitor::before(Float const &v) {
      print(std::to_string(v.value));
    }

    void LoggingVisitor::before(Ratio const &v) {
      print(std::to_string(v.numerator) + "/" + std::to_string(v.denominator));
    }

    void LoggingVisitor::before(Symbol const &v) {
      print(v.value);
    }

    void LoggingVisitor::before(Symbolic const &v) {
      print("(");
      no_space = true;
    }

    void LoggingVisitor::after(Symbolic const &v) {
      no_space = true;
      print(")");
    }

    void LoggingVisitor::before(Map const &v) {
      print("{");
      no_space = true;
    }

    void LoggingVisitor::after(Map const &v) {
      no_space = true;
      print("}");
    }

    void LoggingVisitor::before(Set const &v) {
      print("#{");
      no_space = true;
    }

    void LoggingVisitor::after(Set const &val) {
      no_space = true;
      print("}");
    }

    void LoggingVisitor::before(expressions::String const &v) {
      print("\"" + v.value + "\"");
    }

    void LoggingVisitor::before(Vector const &v) {
      print("[");
      no_space = true;
    }

    void LoggingVisitor::after(Vector const &v) {
      no_space = true;
      print("]");
    }

    void NumberParser::handle(Number const &val, SharedExpression &original) {
      if (Integer::accepts(&*original)) {
        original = std::make_shared<Integer>(Integer::from_number(original));
      }
      else if (Float::accepts(&*original)) {
        original = std::make_shared<Float>(Float::from_number(original));
      }
      else if (Ratio::accepts(&*original)) {
        original = std::make_shared<Ratio>(Ratio::from_number(original));
      }
    }
  }
}


