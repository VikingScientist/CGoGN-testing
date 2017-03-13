project(CGOGN-playground)
cmake_minimum_required (VERSION 2.8)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)

message(STATUS ${PROJECT_SOURCE_DIR})
message(STATUS ${CMAKE_MODULE_PATH})

find_package(Libunwind REQUIRED)

set(CGoGN_ROOT_DIR /home/kjetijo/3rdParty/CGoGN)
include(${CGoGN_ROOT_DIR}/apps_cmake.txt)

include_directories(${PROJECT_SOURCE_DIR}/provided ${LIBUNWIND_INCLUDE_DIR})
file(GLOB PROVIDED_SRCS ${PROJECT_SOURCE_DIR}/provided/*.cpp)

set(EXECUTABLE_OUTPUT_PATH bin)

add_executable(CGOGN-playground main.cpp ${PROVIDED_SRCS})
target_link_libraries(CGOGN-playground ${LIBUNWIND_LIBRARIES} ${CGoGN_LIBRARIES} ${CGoGN_EXT_LIBS})

# qt_wrap_cpp(simpleGMap2_moc simpleGMap2.h)
# add_executable(sgm2 simpleGMap2.cpp ${PROVIDED_SRCS})
# target_link_libraries(sgm2 ${LIBUNWIND_LIBRARIES} ${CGoGN_LIBRARIES} ${CGoGN_EXT_LIBS})
# qt_use_cgogn_modules(sgm2)
