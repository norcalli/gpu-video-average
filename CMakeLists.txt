cmake_minimum_required(VERSION 2.8.8)

project(sample_project C CXX)
SET(CMAKE_SYSTEM_NAME Darwin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++ -std=c++11")
# Add MacPorts
INCLUDE_DIRECTORIES(/opt/local/include)
LINK_DIRECTORIES(/opt/local/lib)


find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

message(STATUS "${OpenCV_LIBS}")
message(STATUS "${OpenCV_INCLUDE_DIRS}")

file(GLOB srcs ./*.cc ./*.h*)
# set(srcs main.cc)

add_executable(gpu_average gpu.cc)
target_link_libraries(gpu_average "${OpenCV_LIBS}")

add_executable(cpu_average cpu.cc)
target_link_libraries(cpu_average "${OpenCV_LIBS}")