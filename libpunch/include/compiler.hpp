/*
 *   Copyright (c) 2015 Raymond Kroon. All rights reserved.
 *   The use and distribution terms for this software are covered by the
 *   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
 *   which can be found in the file LICENSE.txt at the root of this distribution.
 *   By using this software in any fashion, you are agreeing to be bound by
 *   the terms of this license.
 *   You must not remove this notice, or any other, from this software.
 */


#ifndef PUNCH_COMPILER_HPP
#define PUNCH_COMPILER_HPP

#include <string>
#include <environment.hpp>

class Compiler {
public:
  Compiler() {};

  void compile_file(std::string);
  void compile(std::string);

private:
  LLVMEnvironment env;
};

#endif //PUNCH_COMPILER_HPP
