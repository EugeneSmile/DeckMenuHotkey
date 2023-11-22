cmake_minimum_required(VERSION 3.5.0)
project(DeckMenuHotkey VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/out)

file(GLOB_RECURSE srcs src/*.cc src/*.h)

set(CROW_BUILD_EXAMPLES OFF)
set(CROW_BUILD_TESTS OFF)
add_subdirectory(deps/CrowCpp)
add_subdirectory(deps/toml11)

add_executable(deckmenuhotkey ${srcs})
add_dependencies(deckmenuhotkey Crow::Crow toml11::toml11)
target_link_libraries(deckmenuhotkey Crow::Crow toml11::toml11)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
