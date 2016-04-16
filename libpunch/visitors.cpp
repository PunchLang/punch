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

namespace punch {
  namespace lang {

    void LoggingVisitor::before(Keyword const &v) {
      print(":" + v.value);
    }

    void LoggingVisitor::before(Number const &val) {
      print(val.value);
    }

//    void LoggingVisitor::before(Integer const &v) {
//      print(std::to_string(v.value));
//    }
//
//    void LoggingVisitor::before(Float const &v) {
//      print(std::to_string(v.value));
//    }
//
//    void LoggingVisitor::before(Ratio const &v) {
//      print(std::to_string(v.numerator) + "/" + std::to_string(v.denominator));
//    }

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

    void NumberParser::handle(Symbol const &val, SharedExpression& original) {
      original = std::shared_ptr<Expression> (new Symbol("hihi"));
    };
  }
}


