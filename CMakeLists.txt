cmake_minimum_required(VERSION 3.8)

project(
  GLFWTEST
  VERSION 0.1
  DESCRIPTION "test"
  LANGUAGES CXX)


include(FetchContent)

##glfw
FetchContent_Declare(
  glfw
  GIT_REPOSITORY "https://github.com/glfw/glfw.git"
  GIT_TAG "3.4"
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
  assimp
  GIT_REPOSITORY "https://github.com/assimp/assimp.git"
)
FetchContent_MakeAvailable(assimp)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
  ${CMAKE_SOURCE_DIR}/RenderingEngine/*.cpp
)

add_executable(program ${SRC_FILES})
add_compile_definitions(GLFW_INCLUDE_NONE)

target_include_directories(program PUBLIC ${CMAKE_SOURCE_DIR}/RenderingEngine)
target_include_directories(program PUBLIC ${CMAKE_SOURCE_DIR}/Libs/glm)
target_include_directories(program PUBLIC ${CMAKE_SOURCE_DIR}/Libs/imgui)
target_include_directories(program PUBLIC ${CMAKE_SOURCE_DIR}/Libs/entt)
target_include_directories(program PUBLIC ${CMAKE_SOURCE_DIR}/Libs/stb_image)
target_include_directories(program PUBLIC ${CMAKE_SOURCE_DIR}/Libs/spdlog/include)

target_link_libraries(program PRIVATE glfw glad imgui assimp)

add_subdirectory(Libs/glad)
add_subdirectory(Libs/imgui)

target_compile_options(program PUBLIC -w)
target_compile_features(program PUBLIC cxx_std_17)