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
    void LoggingVisitor::visit(endoffile_type&) {
      print("EOF");
    }

    void LoggingVisitor::visit(keyword_type& v) {
      print(":" + v.value);
    }

    void LoggingVisitor::visit(integer_type& v) {
      print(std::to_string(v.value));
    }

    void LoggingVisitor::visit(float_type& v) {
      print(std::to_string(v.value));
    }

    void LoggingVisitor::visit(ratio_type& v) {
      print(std::to_string(v.numerator) + "/" + std::to_string(v.denominator));
    }

    void LoggingVisitor::visit(literal_type& v) {
      print(v.value);
    }

    void LoggingVisitor::visit(symbolic_type& v) {
      level++;
      print("(");

      bool first = true;
      auto inner = v.get_inner();
      for (auto it = inner->begin(); it != inner->end(); ++it) {
        if (!first) {
          print(", ");
        }

        (*it)->accept(*this);
        first = false;
      }

      level--;
      print(")");
    }

    void LoggingVisitor::visit(map_type& v) {
      level++;
      print("{");

      bool first = true;
      auto inner = v.get_inner();
      auto it = inner->begin();

      while (it != inner->end()) {
        if (!first) {
          print(", ");
        }

        (*it)->accept(*this);
        ++it;
        (*it)->accept(*this);

        first = false;
      }

      level--;
      print("}");
    }

    void LoggingVisitor::visit(set_type& v) {
      level++;
      print("#{");

      bool first = true;
      auto inner = v.get_inner();
      for (auto it = inner->begin(); it != inner->end(); ++it) {
        if (!first) {
          print(", ");
        }

        (*it)->accept(*this);
        first = false;
      }

      level--;
      print("}");
    }

    void LoggingVisitor::visit(string_type& v) {
      print("\"" + v.value + "\"");
    }

    void LoggingVisitor::visit(vector_type& v) {
      level++;
      print("[");

      bool first = true;
      auto inner = v.get_inner();
      for (auto it = inner->begin(); it != inner->end(); ++it) {
        if (!first) {
          print(", ");
        }

        (*it)->accept(*this);
        first = false;
      }

      level--;
      print("]");
    }
  }
}


