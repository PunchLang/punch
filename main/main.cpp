// -*- compile-command: "cd ../build && make"; -*-
/*
 *   Copyright (c) 2015 Raymond Kroon. All rights reserved.
 *   The use and distribution terms for this software are covered by the
 *   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
 *   which can be found in the file LICENSE.txt at the root of this distribution.
 *   By using this software in any fashion, you are agreeing to be bound by
 *   the terms of this license.
 *   You must not remove this notice, or any other, from this software.
 */

#include <iostream>
#include <string>
#include <tokenizer.hpp>
#include <reader.hpp>
#include <util.hpp>

int main(int argc, char *argv[]) {

  if (argc > 1) {
    Tokenizer tokenizer(make_unique<LineScanner>(argv[1]));
    Token token = Token::EndOfFile;
    while (tokenizer.next(token)) {
      std::cout << token.DebugInfo() << ", ";
    }
  }


  return 0;
}
