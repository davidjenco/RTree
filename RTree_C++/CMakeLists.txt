cmake_minimum_required(VERSION 3.19)
project(RTree_C__)

set(CMAKE_CXX_STANDARD 20)

add_executable(RTree_C__ src/main.cpp src/RTree.cpp src/RTree.h src/DataGenerator.cpp src/DataGenerator.h src/CommandHandler.cpp src/CommandHandler.h src/Application.cpp src/Application.h src/Action.h src/Node.h src/RoutingEntry.h src/TreeConfig.h src/Node.cpp src/RoutingEntry.cpp src/TreeConfig.cpp src/RecurseInsertStruct.h src/InsertCandidate.h src/Tests.cpp)
add_executable(Test src/Tests.cpp src/RoutingEntry.h src/RoutingEntry.cpp)