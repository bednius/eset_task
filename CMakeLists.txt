cmake_minimum_required(VERSION 3.9)
project(eset_test)

set(CMAKE_CXX_STANDARD 11)

add_executable(eset_test main.cpp ArgumentsParser.cpp ArgumentsParser.h Architecture/EvMachine.cpp Architecture/EvMachine.h Architecture/EvThread.cpp Architecture/EvThread.h Architecture/InstructionType.h Architecture/ArgumentType.h Architecture/InstructionInfo.cpp Architecture/InstructionInfo.h Architecture/InstructionSet.cpp Architecture/InstructionSet.h Architecture/Code.cpp Architecture/Code.h Architecture/EvMemory.cpp Architecture/EvMemory.h Architecture/ThreadsManager.cpp Architecture/ThreadsManager.h)

target_link_libraries(eset_test pthread)