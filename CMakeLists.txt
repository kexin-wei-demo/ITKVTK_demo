cmake_minimum_required(VERSION 3.16)

project(ITKVTKDemo)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

# ==============================================================================
set(ITKVTKlibs_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/ITKVTKlibs)
find_package(ITKVTKlibs REQUIRED)
set(Qt5_DIR "C:/Qt/Qt5.12.10/5.12.10/msvc2015_64/lib/cmake/Qt5")

# ==============================================================================

# add spdlog as sub project
include(spdlog_fetch)

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
    ITKVtkGlue
    ITKVTK
    ITKIOImageBase
    ITKIONRRD
)
include(${ITK_USE_FILE})

# add Qt
find_package(Qt5 REQUIRED COMPONENTS
    Core
)

# add src
file(GLOB SRC_FILES "*.cpp" "*.h" "*.hpp" "src/*.cpp" "src/*.h" "src/*.hpp")

add_executable(${PROJECT_NAME}
    ${SRC_FILES}
)

target_link_libraries(${PROJECT_NAME}
    ${ITK_LIBRARIES}
    ${VTK_LIBRARIES}
    spdlog
    Qt5::Core
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${STAGING_DIR}/include
    src
)

# find windeployqt
# get_target_property(qmake_exec_filepath Qt5::qmake IMPORTED_LOCATION)
# get_filename_component(qt_exec_bin_dir "${qmake_exec_filepath}" DIRECTORY)
# find_program(windeployqt_exec_filepath windeployqt HINTS "${qt_exec_bin_dir}")
# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
# COMMAND "${CMAKE_COMMAND}" -E
# env PATH="${qt_exec_bin_dir}" "${windeployqt_exec_filepath}"
# --verbose 0
# "$<TARGET_FILE:${PROJECT_NAME}>"
# COMMENT "Running windeployqt..."
# )