/*
 *   Copyright (c) 2015 Raymond Kroon. All rights reserved.
 *   The use and distribution terms for this software are covered by the
 *   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
 *   which can be found in the file LICENSE.txt at the root of this distribution.
 *   By using this software in any fashion, you are agreeing to be bound by
 *   the terms of this license.
 *   You must not remove this notice, or any other, from this software.
 */


#ifndef PUNCH_ENVIRONMENT_HPP
#define PUNCH_ENVIRONMENT_HPP

#include <reader.hpp>
#include <llvm/IR/Value.h>

using namespace expression;

class Environment {

public:
  Environment() {}
  virtual void eval(Expression&) = 0;
};

class LLVMEnvironment : public Environment {
public:
  LLVMEnvironment() {}

  void eval(Expression&) override;
  llvm::Value codegen(List);
};

#endif //PUNCH_ENVIRONMENT_HPP
