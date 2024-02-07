cmake_minimum_required(VERSION 3.14)
project(BoomstationUpdater LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network)

include(CMake/DefaultCXX.cmake)

qt_standard_project_setup()

add_subdirectory(src)
