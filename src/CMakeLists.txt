cmake_minimum_required(VERSION 2.8)

# locate all file with .cpp
file(GLOB SOURCES *.cpp)

add_library(SimPHITSAnalysis  STATIC  ${SOURCES})
