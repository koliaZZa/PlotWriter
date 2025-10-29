cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

project(PlotWriter)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# if we don't set build_type
if(NOT DEFINED CMAKE_BUILD_TYPE OR "${CMAKE_BUILD_TYPE}" STREQUAL "")
      set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()
message(STATUS "Build type set to: ${CMAKE_BUILD_TYPE}")

# do not generate ZERO_CHECK
set(CMAKE_SUPPRESS_REGENERATION true)

# declare build structures
add_definitions("-DLIB=\"lib${LIB_SUFFIX}\"")
set(APP_BINARY_BIN_DIR "${CMAKE_BINARY_DIR}/bin")
set(APP_BINARY_LIB_DIR "${CMAKE_BINARY_DIR}/lib${LIB_SUFFIX}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${APP_BINARY_BIN_DIR}")
if(WIN32)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
#else()
  #set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
endif()


set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Configurator Headers
set(
HEADERS
Include/Model/Runs/Model.Quantity.h
Include/Model/Runs/Model.RunResult.h
Include/Model/Runs/Model.RunsInfo.h
Include/Model/Plots/Model.PlotCollection.h
Include/Model/Plots/Model.Graph.h
Include/Model/Common/Model.ErrorReporter.h
Include/Model/Common/Model.Functions.h
Include/Model/Common/Model.Object.h
Include/Model/Common/Model.RJUtilities.h
Include/Model/Common/Model.Utilities.h
Include/Model/NewRun.h
)

# Configurator Source file
set(
SOURCES
Source/Model/Runs/Model.Quantity.cpp
Source/Model/Runs/Model.RunResult.cpp
Source/Model/Runs/Model.RunsInfo.cpp
Source/Model/Plots/Model.PlotCollection.cpp
Source/Model/Plots/Model.Graph.cpp
Source/Model/Common/Model.ErrorReporter.cpp
Source/Model/Common/Model.Functions.cpp
Source/Model/Common/Model.Object.cpp
Source/Model/Common/Model.RJUtilities.cpp
Source/Model/Common/Model.Utilities.cpp
Source/Model/NewRun.cpp
main.cpp
)

source_group(TREE "${CMAKE_SOURCE_DIR}" FILES ${HEADERS} ${SOURCES})

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})

#add_executable(PlotWriter )
target_link_libraries(PlotWriter ${ROOT_LIBRARIES})

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>  
)