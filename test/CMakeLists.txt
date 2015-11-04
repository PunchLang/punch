#   Copyright (c) 2015 Raymond Kroon. All rights reserved.
#   The use and distribution terms for this software are covered by the
#   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
#   which can be found in the file LICENSE.txt at the root of this distribution.
#   By using this software in any fashion, you are agreeing to be bound by
#   the terms of this license.
#   You must not remove this notice, or any other, from this software.

find_package(Threads REQUIRED)
include(ExternalProject)

ExternalProject_Add(
    gtest
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/external/googletest/googletest/"
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
    # Disable install step
    INSTALL_COMMAND   ""
    LOG_CONFIGURE ON
    LOG_BUILD ON
)

ExternalProject_Get_Property(gtest source_dir binary_dir)
add_library(libgtest      IMPORTED STATIC GLOBAL)
set_target_properties(libgtest PROPERTIES
  IMPORTED_LOCATION ${binary_dir}/libgtest.a
)

add_dependencies(libgtest      gtest)

include_directories("${source_dir}/include")

ExternalProject_Add(
    gmock
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/external/googletest/googlemock/"
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gmock
    # Disable install step
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(gmock source_dir binary_dir)
add_library(libgmock      IMPORTED STATIC GLOBAL)
set_target_properties(libgmock PROPERTIES
  IMPORTED_LOCATION ${binary_dir}/libgmock.a
)

add_dependencies(libgmock      googletest)

include_directories("${source_dir}/include")

add_subdirectory(testfancy)
