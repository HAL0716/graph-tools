cmake_minimum_required(VERSION 3.10)
project(HelloWorld)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    /opt/homebrew/include/eigen3
)

add_executable(main src/main.cpp)

file(GLOB OTHER_SRC_FILES src/*.cpp)
list(REMOVE_ITEM OTHER_SRC_FILES ${CMAKE_SOURCE_DIR}/src/main.cpp)
add_library(objects OBJECT ${OTHER_SRC_FILES})
target_link_libraries(main PRIVATE $<TARGET_OBJECTS:objects>)

set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)