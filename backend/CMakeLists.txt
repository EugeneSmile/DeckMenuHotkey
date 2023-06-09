cmake_minimum_required(VERSION 3.0.0)
project(DeckMenuHotkey VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/out)

file(GLOB_RECURSE srcs src/*.cc src/*.h)

add_subdirectory(deps/cpprestsdk)

add_executable(deckmenuhotkey ${srcs})
add_dependencies(deckmenuhotkey cpprest)
target_link_libraries(deckmenuhotkey PRIVATE cpprest)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
