cmake_minimum_required(VERSION 3.8)

project(SmartSignals)
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/std:c++latest>)

#add_executable(SmartSignalsTest test.cpp)

add_library(SmartSignals INTERFACE)
target_include_directories(SmartSignals INTERFACE include)

install(
DIRECTORY include/
DESTINATION include/SmartSignals
)

install(
TARGET SmartSignals
)