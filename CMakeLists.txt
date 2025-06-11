# CMakeList.txt : CMake project for cmaking_cybercell, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
# Add source to this project's executable.

project(agarioplaybot)


find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable (cmaking_cybercell "agarioplaybot.cpp" "agarioplaybot.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET agarioplaybot PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
target_link_libraries( agarioplaybot ${OpenCV_LIBS} )
