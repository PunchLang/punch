#   Copyright (c) 2015 Raymond Kroon. All rights reserved.
#   The use and distribution terms for this software are covered by the
#   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
#   which can be found in the file LICENSE.txt at the root of this distribution.
#   By using this software in any fashion, you are agreeing to be bound by
#   the terms of this license.
#   You must not remove this notice, or any other, from this software.

cmake_minimum_required(VERSION 3.2)
SET(CMAKE_COLOR_MAKEFILE ON)
project(punch)

enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") # -fsanitize=address")

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Boost 1.59.0  COMPONENTS program_options filesystem system regex REQUIRED)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

add_subdirectory(libpunch)
add_subdirectory(main)
add_subdirectory(test)
