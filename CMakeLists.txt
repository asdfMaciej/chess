# Source for code: https://www.reddit.com/r/cpp_questions/comments/ppxwpr/i_dont_get_how_to_structure_a_project_using_cmake/
cmake_minimum_required(VERSION 3.7)
project(Chess VERSION 1.0
                  DESCRIPTION "Chess"
                  LANGUAGES CXX)


include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)




# This sets the variable SOURCES to be the list of source files
set(SOURCES src/main.cpp src/pieces.cpp src/board.cpp src/renderer.cpp src/constants.cpp)

# This defines your project to build an executable 'my_exe' from the sources
add_executable(Chess ${SOURCES})

target_link_libraries(Chess PRIVATE sfml-graphics)
target_compile_features(Chess PRIVATE cxx_std_17)

#if (WIN32 AND BUILD_SHARED_LIBS)
#    add_custom_command(TARGET Chess POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:Chess> $<TARGET_FILE_DIR:Chess> COMMAND_EXPAND_LISTS)
#endif()

#install(TARGETS Chess)