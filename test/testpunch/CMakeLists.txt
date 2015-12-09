#   Copyright (c) 2015 Raymond Kroon. All rights reserved.
#   The use and distribution terms for this software are covered by the
#   Eclipse Public License 1.0 (http://opensource.org/licenses/eclipse-1.0.php)
#   which can be found in the file LICENSE.txt at the root of this distribution.
#   By using this software in any fashion, you are agreeing to be bound by
#   the terms of this license.
#   You must not remove this notice, or any other, from this software.

SET(CXX_LINK_FLAGS "-pthread")
SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${CXX_LINK_FLAGS}" )

file(GLOB resources "resources/*")
file(COPY ${resources} DESTINATION resources)

file(GLOB testpunch_source *.cpp)
add_executable(testpunch ${testpunch_source})

target_link_libraries(testpunch
    gtest gtest_main
    gmock gmock_main
    libpunch
    ${Boost_LIBRARIES}
)

add_test(NAME testpunch COMMAND testpunch)
