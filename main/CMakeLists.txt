#   Copyright (c) 2015 Raymond Kroon. All rights reserved.
#   The use and distribution terms for this software are covered by the
#   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
#   which can be found in the file LICENSE.txt at the root of this distribution.
#   By using this software in any fashion, you are agreeing to be bound by
#   the terms of this license.
#   You must not remove this notice, or any other, from this software.

set(main_src main.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

add_executable(fancy ${main_src})

target_link_libraries(fancy
    libfancy
)
