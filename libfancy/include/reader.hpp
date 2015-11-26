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

class Reader;

namespace expression {

  class Expression;
  typedef std::shared_ptr<Expression> SharedExpression;

  class Expression {
  public:

    Expression() {}
    virtual ~Expression() {}

    virtual std::string DebugInfo() const = 0;

    virtual bool equal_to(Expression*) = 0;

  };

  inline ::std::ostream &operator<<(::std::ostream &os, const SharedExpression expression) {
    return os << expression->DebugInfo();
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
    static SharedExpression create(Reader*);

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

  class Literal : public Expression {
  public:
    Literal(std::string value) : value(value) {}
    Literal(Literal &&other) : value(std::move(other.value)) {}

    static bool accepts(Token&);
    static SharedExpression create(Reader*);

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
    List(std::list<SharedExpression> inner) : inner(std::move(inner)) {}
    List(List &&other) : inner(std::move(other.inner)) {}

    static bool accepts(Token&);
    static SharedExpression create(Reader*);

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
    std::list<SharedExpression> inner;
  };

  class Map : public Expression {
  public:
    Map(std::list<SharedExpression> inner) : inner(std::move(inner)) {}
    Map(Map &&other) : inner(std::move(other.inner)) {}

    static bool accepts(Token&);
    static SharedExpression create(Reader*);

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
    std::list<SharedExpression> inner;
  };

  class Set : public Expression {
  public:
    Set(std::list<SharedExpression> inner) : inner(std::move(inner)) {}
    Set(Set &&other) : inner(std::move(other.inner)) {}

    static bool accepts(Token&);
    static SharedExpression create(Reader*);

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
    std::list<SharedExpression> inner;
  };

  class String : public Expression {
  public:
    String(std::string value) : value(value) {}
    String(String &&other) : value(std::move(other.value)) {}

    static bool accepts(Token&);
    static SharedExpression create(Reader*);

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
    Vector(std::list<SharedExpression> inner) : inner(std::move(inner)) {}
    Vector(Vector &&other) : inner(std::move(other.inner)) {}

    static bool accepts(Token&);
    static SharedExpression create(Reader*);

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
    std::list<SharedExpression> inner;
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

  std::shared_ptr<Expression> next();

  void pop_token() {
    cur_tok = tokenizer->next();
  }

  Token current_token() {
    return cur_tok;
  }

private:

  void ret(SharedExpression expr) {
    current = expr;
  }

  std::unique_ptr<Tokenizer> tokenizer;
  Token cur_tok;

  SharedExpression m_end = SharedExpression{new EndOfFile()};
  SharedExpression current = m_end;
};

#endif //FANCY_READER_HPP
