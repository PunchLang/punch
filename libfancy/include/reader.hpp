/*
 *   Copyright (c) 2015 Raymond Kroon. All rights reserved.
 *   The use and distribution terms for this software are covered by the
 *   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
 *   which can be found in the file LICENSE.txt at the root of this distribution.
 *   By using this software in any fashion, you are agreeing to be bound by
 *   the terms of this license.
 *   You must not remove this notice, or any other, from this software.
 */


#ifndef FANCY_READER_HPP
#define FANCY_READER_HPP

#include <tokenizer.hpp>

namespace expression {

  enum ExpressionType {
    EndOfFile, Keyword, Literal, List, Map, Set, String, Vector
  };

  const boost::unordered_map<ExpressionType, const char *> expressionTypeTranslations = boost::assign::map_list_of
      (EndOfFile,"EOF")(Keyword,":")(Literal, "LIT")(List, "()")(Map,"{}")(Set,"#{}")(String,"STR")(Vector,"[]");

  class Expression {
  public:
    Expression(ExpressionType type) : type(std::move(type)) {}
    Expression(const Expression &other) : type(other.type) {}
    Expression(Expression &&other) : type(std::move(other.type)) { }
    Expression &operator=(Expression &&other) {
      std::swap(this->type, other.type);

      return *this;
    }

    virtual ~Expression();

    virtual bool startsWith(Token&);
    virtual Expression read(std::shared_ptr<Tokenizer::iterator>);

    virtual std::string DebugInfo() const {
      return std::string(expressionTypeTranslations.at(type));
    }

    bool operator==(const Expression other) const {
      return this->type == other.type;
    }

    ExpressionType type;

  };

  inline ::std::ostream &operator<<(::std::ostream &os, const Expression &expression) {
    return os << expression.DebugInfo();
  }

  class Keyword : public Expression {
  public:
    Keyword(std::string value) : Expression(ExpressionType::Keyword), value(value) {}
    Keyword(Keyword &&other) : Expression(std::move(other)), value(std::move(other.value)) {}

    std::string DebugInfo() const {
      return Expression::DebugInfo() + " (" + value + ")";
    }

    bool operator==(const Keyword other) const {
      return Expression::operator==(other) && value == other.value;
    }

  private:
    std::string value;
  };

  class Literal : public Expression {
  public:
    Literal(std::string value) : Expression(ExpressionType::Literal), value(value) {}
    Literal(Literal &&other) : Expression(std::move(other)), value(std::move(other.value)) {}

    std::string DebugInfo() const {
      return Expression::DebugInfo() + " (" + value + ")";
    }

    bool operator==(const Literal other) const {
      return Expression::operator==(other) && value == other.value;
    }

  private:
    std::string value;
  };

  class List : public Expression {
  public:
    List(std::list<Expression> inner) : Expression(ExpressionType::List), inner(std::move(inner)) {}
    List(List &&other) : Expression(std::move(other)), inner(std::move(other.inner)) {}

    std::string DebugInfo() const {
      auto ret  = Expression::DebugInfo() + " (";

      for (auto it = inner.begin(); it != inner.end(); ++it) {
        ret += it->DebugInfo() + ", ";
      }

      ret += ")";
      return ret;
    }

    bool operator==(const List other) const {
      return Expression::operator==(other) && inner == other.inner;
    }

  private:
    std::list<Expression> inner;
  };

  class Map : public Expression {
  public:
    Map(std::list<Expression> inner) : Expression(ExpressionType::Map), inner(std::move(inner)) {}
    Map(Map &&other) : Expression(std::move(other)), inner(std::move(other.inner)) {}

    std::string DebugInfo() const {
      auto ret  = Expression::DebugInfo() + " (";

      for (auto it = inner.begin(); it != inner.end(); ++it) {
        ret += it->DebugInfo() + ", ";
      }

      ret += ")";
      return ret;
    }

    bool operator==(const Map other) const {
      return Expression::operator==(other) && inner == other.inner;
    }

  private:
    std::list<Expression> inner;
  };

  class Set : public Expression {
  public:
    Set(std::list<Expression> inner) : Expression(ExpressionType::Set), inner(std::move(inner)) {}
    Set(Set &&other) : Expression(std::move(other)), inner(std::move(other.inner)) {}

    std::string DebugInfo() const {
      auto ret  = Expression::DebugInfo() + " (";

      for (auto it = inner.begin(); it != inner.end(); ++it) {
        ret += it->DebugInfo() + ", ";
      }

      ret += ")";
      return ret;
    }

    bool operator==(const Set other) const {
      return Expression::operator==(other) && inner == other.inner;
    }

  private:
    std::list<Expression> inner;
  };

  class String : public Expression {
  public:
    String(std::string value) : Expression(ExpressionType::String), value(value) {}
    String(String &&other) : Expression(std::move(other)), value(std::move(other.value)) {}

    std::string DebugInfo() const {
      return Expression::DebugInfo() + " (" + value + ")";
    }

    bool operator==(const String other) const {
      return Expression::operator==(other) && value == other.value;
    }

  private:
    std::string value;
  };

  class Vector : public Expression {
  public:
    Vector(std::list<Expression> inner) : Expression(ExpressionType::Vector), inner(std::move(inner)) {}
    Vector(Vector &&other) : Expression(std::move(other)), inner(std::move(other.inner)) {}

    std::string DebugInfo() const {
      auto ret  = Expression::DebugInfo() + " (";

      for (auto it = inner.begin(); it != inner.end(); ++it) {
        ret += it->DebugInfo() + ", ";
      }

      ret += ")";
      return ret;
    }

    bool operator==(const Vector other) const {
      return Expression::operator==(other) && inner == other.inner;
    }

  private:
    std::list<Expression> inner;
  };

  inline ::std::ostream &operator<<(::std::ostream &os, const Token &token) {
    return os << token.DebugInfo();
  }
}

using namespace expression;

class Reader {

public:
  Reader(std::unique_ptr<Tokenizer> t) : tokens{std::move(t)} {}
  ~Reader() {}

  class iterator {
  public:
    typedef iterator self_type;
    typedef Expression value_type;
    typedef Expression& reference;
    typedef std::forward_iterator_tag iterator_category;
    typedef int difference_type;

    iterator(Reader *parent, Expression current) : parent(parent), current(std::move(current)) { }
    iterator(const self_type &other) : parent(other.parent), current(other.current) {}

    self_type operator++(int junk) { self_type i(*this); current = parent->read(); return i; }
    self_type operator++() { current = parent->read(); return *this; }
    reference operator*() { return current; }

    bool operator==(const self_type& rhs) { return (parent == rhs.parent) && (current == rhs.current); }

    bool operator!=(const self_type& rhs) { return !(*this == rhs); }

  private:
    Reader* parent;
    Expression current;
  };

  iterator begin() {
    return iterator(this, m_start);
  }

  iterator end() {
    return iterator(this, m_end);
  }

private:

  Expression read();
  std::unique_ptr<Tokenizer> tokens;

  Expression m_end = Expression(ExpressionType::EndOfFile);
  Expression m_start = m_end;
};

#endif //FANCY_READER_HPP
