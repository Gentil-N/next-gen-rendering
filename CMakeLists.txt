cmake_minimum_required(VERSION 3.0.0)

project(ngr)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

set(NGR_SRC_DIR  ${PROJECT_SOURCE_DIR}/code)
set(NGR_SRC_FILES ${NGR_SRC_DIR}/main.cpp)

if(NGR_BUILD_SHARED)
       add_library(ngr SHARED ${NGR_SRC_FILES})
else()
       add_library(ngr STATIC ${NGR_SRC_FILES})
endif()