cmake_minimum_required(VERSION 3.16)

project(ITKVTKDemo)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# add spdlog as sub project
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

set(ITKVTKlibs_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/ITKVTKlibs)
find_package(ITKVTKlibs REQUIRED)

# add VTK first cause itkVtkGlue depends on VTK
set(VTK_DIR ${VTK8.2_DIR})
message(STATUS "VTK_DIR: ${VTK_DIR}")
find_package(VTK REQUIRED
    vtkCommonCore
)

# add ITK
set(ITK_DIR ${ITK5.1_DIR})
message(STATUS "ITK_DIR: ${ITK_DIR}")
find_package(ITK REQUIRED COMPONENTS
    ITKCommon
)
include(${ITK_USE_FILE})




add_executable(${PROJECT_NAME}
    main.cpp
)

add_dependencies(${PROJECT_NAME} spdlog)

target_link_libraries(${PROJECT_NAME}
    ${ITK_LIBRARIES}
    ${VTK_LIBRARIES}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${STAGING_DIR}/include
)