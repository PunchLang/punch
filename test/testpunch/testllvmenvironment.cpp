/*
 *   Copyright (c) 2015 Raymond Kroon. All rights reserved.
 *   The use and distribution terms for this software are covered by the
 *   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
 *   which can be found in the file LICENSE.txt at the root of this distribution.
 *   By using this software in any fashion, you are agreeing to be bound by
 *   the terms of this license.
 *   You must not remove this notice, or any other, from this software.
 */

#include <gtest/gtest.h>
#include <reader.hpp>
#include <environment.hpp>
#include <util.hpp>

class LLVMEnvironmentTest : public ::testing::Test {
public:
  LLVMEnvironmentTest() {}
  ~LLVMEnvironmentTest() {}

  void SetUp() {}
  void TearDown() {}
};

TEST_F(LLVMEnvironmentTest, Keyword) {
  LLVMEnvironment env;
  UExpression e = make_unique<expression::Keyword>("test");
  env.eval(e);
}

//compiler hpp, die gaat alles aan elkaar knopen. reader, ast runs, macro time etc.