cmake_minimum_required(VERSION 2.8)
project(png_converter)

## gnu++11 feature
set(CMAKE_CXX_FLAGS "-std=gnu++14 -Wall -Wextra ${CMAKE_CXX_FLAGS}")

## Set OpenCV environment variables
set(OpenCV_DIR /usr/local/share/OpenCV)

## Set Boost environment variables
set(Boost_INCLUDE_DIR /usr/include)
set(Boost_LIBRARY_DIR /usr/lib/x86_64-linux-gnu)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ..)

find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem system program_options)

include_directories(../include
    ${Boost_INCLUDE_DIR}
    ${OpenCV_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME}
               ../src/png_converter.cpp
)

target_link_libraries(${PROJECT_NAME}
                      ${OpenCV_LIBRARIES}
                      ${Boost_LIBRARIES}
)
