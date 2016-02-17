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
      class EndOfFile;
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

    template <bool isMutating>
    class TExpressionVisitor {

    protected:
      typedef typename std::conditional<isMutating,
          Expression*, Expression const * const>::type expression_type;

      typedef typename std::conditional<isMutating,
          EndOfFile, EndOfFile const>::type endoffile_type;

      typedef typename std::conditional<isMutating,
          Keyword, Keyword const>::type keyword_type;

      typedef typename std::conditional<isMutating,
          Integer, Integer const>::type integer_type;

      typedef typename std::conditional<isMutating,
          Float, Float const>::type float_type;

      typedef typename std::conditional<isMutating,
          Ratio, Ratio const>::type ratio_type;

      typedef typename std::conditional<isMutating,
          Literal, Literal const>::type literal_type;

      typedef typename std::conditional<isMutating,
          Symbolic, Symbolic const>::type symbolic_type;

      typedef typename std::conditional<isMutating,
          Map, Map const>::type map_type;

      typedef typename std::conditional<isMutating,
          Set, Set const>::type set_type;

      typedef typename std::conditional<isMutating,
          String, String const>::type string_type;

      typedef typename std::conditional<isMutating,
          Vector, Vector const>::type vector_type;

    public:
      virtual void visit(endoffile_type&) = 0;
      virtual void visit(keyword_type&) = 0;
      virtual void visit(integer_type&) = 0;
      virtual void visit(float_type&) = 0;
      virtual void visit(ratio_type&) = 0;
      virtual void visit(literal_type&) = 0;
      virtual void visit(symbolic_type&) = 0;
      virtual void visit(map_type&) = 0;
      virtual void visit(set_type&) = 0;
      virtual void visit(string_type&) = 0;
      virtual void visit(vector_type&) = 0;
    };

    typedef TExpressionVisitor<false> ExpressionVisitor;
    typedef TExpressionVisitor<true> MutatingExpressionVisitor;

    class LoggingVisitor : public ExpressionVisitor {
    public:

      LoggingVisitor() : LoggingVisitor(std::cout, 0){}
      LoggingVisitor(std::ostream &os, int start_level) : os(os), level(start_level) {}

      void visit(endoffile_type&) override;
      void visit(keyword_type&) override;
      void visit(integer_type&) override;
      void visit(float_type&) override;
      void visit(ratio_type&) override;
      void visit(literal_type&) override;
      void visit(symbolic_type&) override;
      void visit(map_type&) override;
      void visit(set_type&) override;
      void visit(string_type&) override;
      void visit(vector_type&) override;

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
