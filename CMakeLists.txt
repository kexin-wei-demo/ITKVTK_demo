cmake_minimum_required(VERSION 3.16)

project(ITKVTKDemo)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(ExternalProject)
ExternalProject_Add(spdlog
    PREFIX spdlog
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/lib/spdlog
    CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
    -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
    -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
    -DCMAKE_INSTALL_PREFIX=${STAGING_DIR}
    -DSPDLOG_BUILD_SHARED=OFF
)

add_executable(${PROJECT_NAME}
    main.cpp
)

add_dependencies(${PROJECT_NAME} spdlog)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${STAGING_DIR}/include
)