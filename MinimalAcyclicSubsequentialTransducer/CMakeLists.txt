cmake_minimum_required(VERSION 3.13)
project("MAST")

add_library(MastClass STATIC AcyclicSubsequentialTransducer/SubsequentialTransducer.cpp)
add_library(MastBuilder STATIC Builder/MinimalSubsequentialTransducerBuilder.cpp)
add_executable(${PROJECT_NAME}  Source.cpp)

target_link_libraries(${PROJECT_NAME} MastClass)
target_link_libraries(${PROJECT_NAME}  MastBuilder)
