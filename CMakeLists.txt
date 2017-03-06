project(CGOGN-playground)

cmake_minimum_required (VERSION 2.6)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)

message(STATUS ${PROJECT_SOURCE_DIR})
message(STATUS ${CMAKE_MODULE_PATH})

find_package(Libunwind REQUIRED)

project(CGOGN-playground)
set(CGoGN_ROOT_DIR /home/kjetijo/3rdParty/CGoGN)
include(${CGoGN_ROOT_DIR}/apps_cmake.txt)

include_directories(${PROJECT_SOURCE_DIR}/provided ${LIBUNWIND_INCLUDE_DIR})
file(GLOB PROVIDED_SRCS ${PROJECT_SOURCE_DIR}/provided/*.cpp)

set(EXECUTABLE_OUTPUT_PATH bin)

add_executable(CGOGN-playground main.cpp ${PROVIDED_SRCS})
target_link_libraries(CGOGN-playground ${LIBUNWIND_LIBRARIES})
