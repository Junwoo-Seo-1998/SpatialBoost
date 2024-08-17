cmake_minimum_required(VERSION 3.11)
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)
project(glad C)

add_library(glad STATIC ${SRC_FILES})
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_property(TARGET glad PROPERTY C_STANDARD 99)