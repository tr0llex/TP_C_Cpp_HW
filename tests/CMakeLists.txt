include_directories("${PROJECT_SOURCE_DIR}/lib_num_sequence/include")

file(GLOB sources "${PROJECT_SOURCE_DIR}/lib_num_sequence/src_consistent/*.c")
list(REMOVE_ITEM sources "${PROJECT_SOURCE_DIR}/src/main.c")

file(GLOB tests "${PROJECT_SOURCE_DIR}/tests/*.cpp")
list(REMOVE_ITEM tests "${PROJECT_SOURCE_DIR}/test/main.cpp")

foreach(file ${tests})
    set(name)
    get_filename_component(name ${file} NAME_WE)
    add_executable("${name}_tests"
            ${sources}
            ${file}
            "${PROJECT_SOURCE_DIR}/tests/main.cpp")
    target_link_libraries("${name}_tests" gtest_main ${CMAKE_DL_LIBS})
    add_test(NAME ${name} COMMAND "${name}_tests")
endforeach()
