cmake_minimum_required(VERSION 3.8)

project(SmartSignals)
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/std:c++latest>)

#add_executable(SmartSignalsTest test.cpp)

install(
DIRECTORY src/
DESTINATION "include/SmartSignals"
)
