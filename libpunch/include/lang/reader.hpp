/*
 *   Copyright (c) 2015 Raymond Kroon. All rights reserved.
 *   The use and distribution terms for this software are covered by the
 *   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
 *   which can be found in the file LICENSE.txt at the root of this distribution.
 *   By using this software in any fashion, you are agreeing to be bound by
 *   the terms of this license.
 *   You must not remove this notice, or any other, from this software.
 */


#ifndef PUNCH_LANG_READER_HPP
#define PUNCH_LANG_READER_HPP

#include <exception>
#include <algorithm>
#include <lang/tokenizer.hpp>
#include <lang/util.hpp>
#include <lang/reader_expressions.hpp>

namespace punch {
  namespace lang {

    namespace ple = expressions;

    class Reader;

    class ReaderResult {
    public:

      enum ResultType {
        OK, ERROR, END
      };

      ReaderResult() { }
      ReaderResult(ResultType result, std::string msg) : result(result), msg(std::move(msg)) { }
      ReaderResult(const ReaderResult &other) : result(other.result), msg(other.msg) { }
      ReaderResult(ReaderResult &&other) : result(std::move(other.result)), msg(std::move(other.msg)) { }

      ReaderResult &operator=(const ReaderResult &);

      bool is_ok() {
        return result == OK;
      }

      bool is_end() {
        return result == END;
      }

      bool is_error() {
        return result == ERROR;
      }

      ResultType result;
      std::string msg;
    };

    inline ReaderResult &ReaderResult::operator=(const ReaderResult &other) {
      result = other.result;
      msg = other.msg;

      return *this;
    }

    class Reader {

    public:
      Reader(Tokenizer *const t) : tokenizer{t} {
        tokenizer->next(cur_tok);
      }

      ~Reader() { }

      ReaderResult next(ple::SharedExpression &);

      bool pop_token() {
        return tokenizer->next(cur_tok);
      }

      Token const * const current_token() {
        if (has_token) {
          return &cur_tok;
        }
        else {
          return nullptr;
        }
      }

    private:

      void ret(ple::SharedExpression expr) {
        current = std::move(expr);
      }

      Tokenizer *const tokenizer;
      bool has_token = true;
      Token cur_tok;

      ple::SharedExpression m_end;
      ple::SharedExpression current;
    };
  }
}

#endif //PUNCH_LANG_READER_HPP
