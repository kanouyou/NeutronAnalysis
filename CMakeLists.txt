cmake_minimum_required(VERSION 2.8)
project(AnalysisCode CXX)

# set target
set(TARGET  PostAnalysis.exe)
set(MAINSRC run.cpp)

# set compiler
set(CXX clang++)

# set option
#add_compile_options(-stdlib=libstdc++)

# set flags
set(CMAKE_CXX_RELEASE "-O3")
set(CMAKE_CXX_DEBUG   "-Wall")

# ROOT lib
set(ROOTLIBS)
set(CMAKE_MODULE_PATH $ENV{ROOTSYS}/etc/cmake)
find_package(ROOT REQUIRED)
list(APPEND ROOTLIBS ${ROOT_LIBRARIES})
list(APPEND ROOTLIBS $ENV{ROOTSYS}/lib/root/libEG.so)
include_directories(${ROOT_INCLUDE_DIR})

# add include and src
include_directories(include)
add_subdirectory(src)

# build
add_executable(${TARGET} ${MAINSRC})

# link lib
target_link_libraries(${TARGET}  SimPHITSAnalysis  ${ROOTLIBS})

