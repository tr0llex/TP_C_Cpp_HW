cmake_minimum_required(VERSION 3.16)

set (PROJECT hw2)
project(${PROJECT})

find_package(Threads REQUIRED)

set(CMAKE_C_FLAGS "-Wall -Werror -Wpedantic -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS "-Wall -Werror -Wpedantic -fprofile-arcs -ftest-coverage")

# --- <Libraries> ---

set(NUMSEQ_PATH ${PROJECT_SOURCE_DIR}/lib_num_sequence)


set(NUMSEQ_P "_num_sequence_parallel")
set(NUMSEQ_PARALLEL_LIB
        ${NUMSEQ_PATH}/include/numseq.h
        ${NUMSEQ_PATH}/src_parallel/numseq.c)

add_library(${NUMSEQ_P} MODULE ${NUMSEQ_PARALLEL_LIB})
target_link_libraries(${NUMSEQ_P} Threads::Threads)

set(NUMSEQ_C "_num_sequence_consistent")
set(NUMSEQ_CONSISTENT_LIB
        ${NUMSEQ_PATH}/include/numseq.h
        ${NUMSEQ_PATH}/src_consistent/numseq.c)

add_library(${NUMSEQ_C} STATIC ${NUMSEQ_CONSISTENT_LIB})


target_include_directories(${NUMSEQ_C} PUBLIC ${NUMSEQ_PATH}/include)
target_include_directories(${NUMSEQ_P} PUBLIC ${NUMSEQ_PATH}/include)
# --- </Libraries> ---


# --- <Tests> ---
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
# --- </Tests> ---


include_directories("${PROJECT_SOURCE_DIR}/include")
add_executable(${PROJECT} src/main.c)

# --- <Link libraries> ---
target_link_libraries(${PROJECT_NAME} PRIVATE ${NUMSEQ_C} ${CMAKE_DL_LIBS})
# --- </Link libraries> ---
