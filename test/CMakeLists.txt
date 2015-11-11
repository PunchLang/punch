#   Copyright (c) 2015 Raymond Kroon. All rights reserved.
#   The use and distribution terms for this software are covered by the
#   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
#   which can be found in the file LICENSE.txt at the root of this distribution.
#   By using this software in any fashion, you are agreeing to be bound by
#   the terms of this license.
#   You must not remove this notice, or any other, from this software.

find_package(Threads REQUIRED)

set(gtest_SOURCE_DIR "${CMAKE_SOURCE_DIR}/test/lib/googletest/googletest/")
include_directories(${gtest_SOURCE_DIR}/include)

# Skip googletest because googlemock is an extension of it
#add_subdirectory(${gtest_SOURCE_DIR})

set(gmock_SOURCE_DIR "${CMAKE_SOURCE_DIR}/test/lib/googletest/googlemock/")
include_directories(${gmock_SOURCE_DIR}/include)
add_subdirectory(${gmock_SOURCE_DIR})

add_subdirectory(testfancy)
