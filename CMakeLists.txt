cmake_minimum_required(VERSION 3.16)

project(ArithmeticParser)

set(CMAKE_CXX_STANDARD 17)
set(PROJECT_DIR ${CMAKE_SOURCE_DIR})
set(PROJECT_INCLUDE_DIR ${PROJECT_DIR}/ArithmeticParser)
set(PROJECT_SOURCE_DIR ${PROJECT_DIR}/ArithmeticParser)

set(PROJECT_SOURCES 
        ${PROJECT_SOURCE_DIR}/main.cpp 
    )

set(PROJECT_HEADERS
        ${PROJECT_INCLUDE_DIR}/ArithmeticParser.h
    )

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} )
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE_DIR})

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE "/Zc:__cplusplus")
endif()
