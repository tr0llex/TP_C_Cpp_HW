cmake_minimum_required(VERSION 3.16)

set (PROJECT hw1)

project(${PROJECT})

set(CMAKE_C_FLAGS "-pedantic -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS "-pedantic -fprofile-arcs -ftest-coverage")

configure_file(CMakeLists.txt.in
        googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )

add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
        ${CMAKE_BINARY_DIR}/googletest-build)

enable_testing()
add_subdirectory(tests)

include_directories("${PROJECT_SOURCE_DIR}/include")
add_executable(${PROJECT} src/main.c src/my_string.c src/email_parser.c)
