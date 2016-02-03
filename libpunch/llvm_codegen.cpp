/*
 *   Copyright (c) 2015 Raymond Kroon. All rights reserved.
 *   The use and distribution terms for this software are covered by the
 *   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
 *   which can be found in the file LICENSE.txt at the root of this distribution.
 *   By using this software in any fashion, you are agreeing to be bound by
 *   the terms of this license.
 *   You must not remove this notice, or any other, from this software.
 */

#include <expression.hpp>
#include <reader_expressions.hpp>


using namespace expression;

llvm::Value *Keyword::llvm_codegen() const {
  std::cout << "KEYWORD CODEGEN" << std::endl;
  return nullptr;
}

llvm::Value *Integer::llvm_codegen() const {
  std::cout << "KEYWORD CODEGEN" << std::endl;
  return nullptr;
}

llvm::Value *Float::llvm_codegen() const {
  std::cout << "FLOAT CODEGEN" << std::endl;
  return nullptr;
}

llvm::Value *Ratio::llvm_codegen() const {
  std::cout << "RATIO CODEGEN" << std::endl;
  return nullptr;
}

llvm::Value *Literal::llvm_codegen() const {
  std::cout << "LITERAL CODEGEN" << std::endl;
  return nullptr;
}

llvm::Value *Symbolic::llvm_codegen() const {
  std::cout << "SYMBOLIC CODEGEN" << std::endl;
  return nullptr;
}

llvm::Value *Map::llvm_codegen() const {
  std::cout << "MAP CODEGEN" << std::endl;
  return nullptr;
}

llvm::Value *Set::llvm_codegen() const {
  std::cout << "SET CODEGEN" << std::endl;
  return nullptr;
}

llvm::Value *String::llvm_codegen() const {
  std::cout << "STRING CODEGEN" << std::endl;
  return nullptr;
}

llvm::Value *Vector::llvm_codegen() const {
  std::cout << "VECTOR CODEGEN" << std::endl;
  return nullptr;
}
