# Source for code: https://www.reddit.com/r/cpp_questions/comments/ppxwpr/i_dont_get_how_to_structure_a_project_using_cmake/
cmake_minimum_required(VERSION 3.7)

project(Chess VERSION 1.0
                  DESCRIPTION "Chess"
                  LANGUAGES CXX)

# This sets the variable SOURCES to be the list of source files
set(SOURCES src/main.cpp )

# This defines your project to build an executable 'my_exe' from the sources
add_executable(main ${SOURCES})

# This adds the folder 'include' as a search path for the compiler
# target_include_directory(my_exe PRIVATE include)