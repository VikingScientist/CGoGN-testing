project(CGOGN-playground)
cmake_minimum_required (VERSION 2.8)

message(STATUS "${CMAKE_MODULE_PATH}")
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)

message(STATUS ${PROJECT_SOURCE_DIR})
message(STATUS ${CMAKE_MODULE_PATH})

find_package(Libunwind REQUIRED)

set(CGoGN_ROOT_DIR "/home/kjetijo/3rdParty/CGoGN")
include(${CGoGN_ROOT_DIR}/apps_cmake.txt)

include_directories(${PROJECT_SOURCE_DIR}/provided ${LIBUNWIND_INCLUDE_DIR})
file(GLOB PROVIDED_SRCS ${PROJECT_SOURCE_DIR}/provided/*.cpp)

set(EXECUTABLE_OUTPUT_PATH bin)

# TODO: Figure out how CGoGN likes me to fetch these libraries. Apperently they
#       are not in ${CGoGN_LIBRARIES}, so we'll add them manually
set(MORE_LIBS "-lalgo"
              "-lcontainer"
              "-lnl"
              "-lQGLViewer"
              "-ltetgen"
              "-ltinyxml2"
              "-ltopology"
              "-lutils"
              "-luuid")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

add_executable(CGOGN-playground main.cpp ${PROVIDED_SRCS})
target_link_libraries(CGOGN-playground ${LIBUNWIND_LIBRARIES} ${CGoGN_LIBRARIES} ${CGoGN_EXT_LIBS} ${MORE_LIBS})

