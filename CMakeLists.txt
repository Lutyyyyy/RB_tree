cmake_minimum_required (VERSION 3.14)

project(RB_tree_t VERSION 1.0)

if(NOT BUILD_TYPE)
	set(BUILD_TYPE RELEASE)
endif()

if ("${BUILD_TYPE}" STREQUAL "DEBUG")
    set (OPTIONS -Wall -Wextra -Wpedantic -Werror -DDEBUG_MODE)
else ()
    set (OPTIONS -Wall -Wextra -Wpedantic -Werror -O2)
endif ()
add_compile_options(${OPTIONS})

#apps
add_executable (main apps/main.cpp)
target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(main PRIVATE cxx_std_20)

#tests
enable_testing()
find_package (GTest REQUIRED)

add_executable(gtest test/test.cpp)
target_include_directories(gtest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries (gtest PRIVATE ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)
target_compile_features(gtest PRIVATE cxx_std_20)
