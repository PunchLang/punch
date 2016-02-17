/*
 *   Copyright (c) 2015 Raymond Kroon. All rights reserved.
 *   The use and distribution terms for this software are covered by the
 *   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
 *   which can be found in the file LICENSE.txt at the root of this distribution.
 *   By using this software in any fashion, you are agreeing to be bound by
 *   the terms of this license.
 *   You must not remove this notice, or any other, from this software.
 */

#include <lang/compiler.hpp>
#include <lang/reader.hpp>

using namespace punch::lang;
using namespace punch::lang::expressions;

ReaderResult consume(Reader& reader, std::list<UExpression>& result) {

  UExpression expr;
  ReaderResult rr;
  while ((rr= reader.next(expr)).is_ok()) {
    result.push_back(std::move(expr));
  }

  return rr;
}

void Compiler::compile(std::string v) { }
void Compiler::compile_file(std::string filename) {

  LineScanner scanner(filename);
  Tokenizer tokenizer(&scanner);
  Reader reader(&tokenizer);
  std::list<UExpression> expressions;
  auto rr = consume(reader, expressions);

  if (!rr.is_error()) {
    LoggingVisitor logger;
    for (UExpression &expression : expressions) {
      expression->accept(logger);
    }
    std::cout << std::endl;

  }
  else {
    std::cout << "ERR"  << rr.msg << std::endl;
  }

}