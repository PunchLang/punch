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

#include <exception>
#include <tokenizer.hpp>
#include <util.hpp>

class Reader;

namespace expression {

  class Expression;
  typedef std::unique_ptr<Expression> UExpression;
  typedef std::shared_ptr<Expression> SharedExpression;

  class Expression {
  public:

    Expression() {}
    virtual ~Expression() {}

    virtual std::string DebugInfo() const = 0;

    virtual bool equal_to(Expression*) = 0;

  };

  inline ::std::ostream &operator<<(::std::ostream &os, const SharedExpression& expression) {
    return os << expression->DebugInfo();
  }

  inline ::std::ostream &operator<<(::std::ostream &os, const Expression* expression) {
    return os << expression->DebugInfo();
  }

  inline bool operator==(UExpression &lhs, UExpression &rhs) {
    return lhs->equal_to(&*rhs);
  }

  inline bool operator!=(UExpression& lhs, UExpression& rhs) {
    return !(lhs == rhs);
  }

  inline bool operator==(SharedExpression lhs, SharedExpression rhs) {
    return lhs->equal_to(&*rhs);
  }

  inline bool operator!=(SharedExpression lhs, SharedExpression rhs) {
    return !(lhs == rhs);
  }

  class EndOfFile : public Expression {
  public:
    EndOfFile() {}

    std::string DebugInfo() const override {
      return "EOF";
    }

  protected:
    bool equal_to(Expression* other) override {
      if (const EndOfFile *p = dynamic_cast<EndOfFile const*>(other)) {
        return true;
      }
      else {
        return false;
      }
    }
  };

  class Keyword : public Expression {
  public:
    Keyword(std::string value) : value(value) {}
    Keyword(Keyword &&other) : value(std::move(other.value)) {}

    static bool accepts(Token&);
    static UExpression create(Reader*);

    std::string DebugInfo() const override {
      return "KW (" + value + ")";
    }

  protected:
    bool equal_to(Expression* other) override {
      if (const Keyword *p = dynamic_cast<Keyword const*>(other)) {
        return value == p->value;
      }
      else {
        return false;
      }
    }

  private:
    std::string value;
  };

  class Integer : public Expression {
  public:
    Integer(long value) : value(value) {}
    Integer(Integer &&other) : value(std::move(other.value)) {}

    static bool accepts(Token&);
    static UExpression create(Reader*);

    std::string DebugInfo() const override {
      return "INT (" + std::to_string(value) + ")";
    }

  protected:
    bool equal_to(Expression* other) override {
      if (const Integer *p = dynamic_cast<Integer const*>(other)) {
        return value == p->value;
      }
      else {
        return false;
      }
    }

  private:
    long value;
  };

  class Float : public Expression {
  public:
    Float(double value) : value(value) {}
    Float(Float &&other) : value(std::move(other.value)) {}

    static bool accepts(Token&);
    static UExpression create(Reader*);

    std::string DebugInfo() const override {
      return "FLOAT (" + std::to_string(value) + ")";
    }

  protected:
    bool equal_to(Expression* other) override {
      if (const Float *p = dynamic_cast<Float const*>(other)) {
        return  std::fabs(value - p->value) < std::numeric_limits<double>::epsilon();
      }
      else {
        return false;
      }
    }

  private:
    double value;
  };

  class Ratio : public Expression {
  public:
    Ratio(long n, long d) : numerator(n), denominator(d) {}
    Ratio(Ratio &&other) : numerator(std::move(other.numerator)), denominator(std::move(other.denominator)) {}

    static bool accepts(Token&);
    static UExpression create(Reader*);

    std::string DebugInfo() const override {
      return "RATIO (" + std::to_string(numerator) + "/" + std::to_string(denominator) + ")";
    }

  protected:
    bool equal_to(Expression* other) override {
      if (const Ratio *p = dynamic_cast<Ratio const*>(other)) {
        return numerator == p->numerator && denominator == p->denominator;
      }
      else {
        return false;
      }
    }

  private:
    long numerator;
    long denominator;
  };

  class Literal : public Expression {
  public:
    Literal(std::string value) : value(value) {}
    Literal(Literal &&other) : value(std::move(other.value)) {}

    static bool accepts(Token&);
    static UExpression create(Reader*);

    std::string DebugInfo() const override {
      return "LIT (" + value + ")";
    }

  protected:
    bool equal_to(Expression* other) override {
      if (const Literal *p = dynamic_cast<Literal const*>(other)) {
        return value == p->value;
      }
      else {
        return false;
      }
    }

  private:
    std::string value;
  };

  class List : public Expression {
  public:
    List(std::list<UExpression> inner) : inner(std::move(inner)) {}
    List(List &&other) : inner(std::move(other.inner)) {}

    static bool accepts(Token&);
    static UExpression create(Reader*);

    std::string DebugInfo() const override {
      auto ret  = std::string("LIST (");

      for (auto it = inner.begin(); it != inner.end(); ++it) {
        ret += (*it)->DebugInfo() + ", ";
      }

      ret += ")";
      return ret;
    }

  protected:
    bool equal_to(Expression* other) override {
      if (const List *p = dynamic_cast<List const*>(other)) {
        return inner == p->inner;
      }
      else {
        return false;
      }
    }

  private:
    std::list<UExpression> inner;
  };

  class Map : public Expression {
  public:
    Map(std::list<UExpression> inner) : inner(std::move(inner)) {}
    Map(Map &&other) : inner(std::move(other.inner)) {}

    static bool accepts(Token&);
    static UExpression create(Reader*);

    std::string DebugInfo() const override {
      auto ret  = std::string("MAP (");

      for (auto it = inner.begin(); it != inner.end(); ++it) {
        ret += (*it)->DebugInfo() + ", ";
      }

      ret += ")";
      return ret;
    }

  protected:
    bool equal_to(Expression* other) override {
      if (const Map *p = dynamic_cast<Map const*>(other)) {
        return inner == p->inner;
      }
      else {
        return false;
      }
    }

  private:
    std::list<UExpression> inner;
  };

  class Set : public Expression {
  public:
    Set(std::list<UExpression> inner) : inner(std::move(inner)) {}
    Set(Set &&other) : inner(std::move(other.inner)) {}

    static bool accepts(Token&);
    static UExpression create(Reader*);

    std::string DebugInfo() const override{
      auto ret  = std::string("SET (");

      for (auto it = inner.begin(); it != inner.end(); ++it) {
        ret += (*it)->DebugInfo() + ", ";
      }

      ret += ")";
      return ret;
    }

  protected:
    bool equal_to(Expression* other) override {
      if (const Set *p = dynamic_cast<Set const*>(other)) {
        return inner == p->inner;
      }
      else {
        return false;
      }
    }

  private:
    std::list<UExpression> inner;
  };

  class String : public Expression {
  public:
    String(std::string value) : value(value) {}
    String(String &&other) : value(std::move(other.value)) {}

    static bool accepts(Token&);
    static UExpression create(Reader*);

    std::string DebugInfo() const override {
      return "STR (" + value + ")";
    }

  protected:
    bool equal_to(Expression* other) override {
      if (const String *p = dynamic_cast<String const*>(other)) {
        return value == p->value;
      }
      else {
        return false;
      }
    }

  private:
    std::string value;
  };

  class Vector : public Expression {
  public:
    Vector(std::list<UExpression> inner) : inner(std::move(inner)) {}
    Vector(Vector &&other) : inner(std::move(other.inner)) {}

    static bool accepts(Token&);
    static UExpression create(Reader*);

    std::string DebugInfo() const override{
      auto ret  = std::string("VEC (");

      for (auto it = inner.begin(); it != inner.end(); ++it) {
        ret += (*it)->DebugInfo() + ", ";
      }

      ret += ")";
      return ret;
    }

  protected:
    bool equal_to(Expression* other) override {
      if (const Vector *p = dynamic_cast<Vector const*>(other)) {
        return inner == p->inner;
      }
      else {
        return false;
      }
    }

  private:
    std::list<UExpression> inner;
  };

  inline ::std::ostream &operator<<(::std::ostream &os, const Token &token) {
    return os << token.DebugInfo();
  }
}

using namespace expression;

class ReaderException : public std::exception {

public:
  ReaderException(std::string msg = "Could not read token") : msg(msg) {}

private:
  virtual const char* what() const throw()
  {
    return msg.c_str();
  }

  std::string msg;
};

class Reader {

public:
  Reader(std::unique_ptr<Tokenizer> t) : tokenizer{std::move(t)}, cur_tok(tokenizer->next()) {
  }

  ~Reader() {}

  UExpression next();

  void pop_token() {
    cur_tok = tokenizer->next();
  }

  Token current_token() {
    return cur_tok;
  }

private:

  void ret(UExpression expr) {
    current = std::move(expr);
  }

  std::unique_ptr<Tokenizer> tokenizer;
  Token cur_tok;

  UExpression m_end = make_unique<EndOfFile>();
  UExpression current = make_unique<EndOfFile>();
};

#endif //FANCY_READER_HPP
