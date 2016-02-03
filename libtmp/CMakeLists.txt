#   Copyright (c) 2015 Raymond Kroon. All rights reserved.
#   The use and distribution terms for this software are covered by the
#   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
#   which can be found in the file LICENSE.txt at the root of this distribution.
#   By using this software in any fashion, you are agreeing to be bound by
#   the terms of this license.
#   You must not remove this notice, or any other, from this software.

FILE(GLOB libtmp_source "*.cpp")

# Declare the library
add_library(libtmp STATIC
        ${libtmp_source})

include_directories(include)

target_include_directories(libtmp PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
