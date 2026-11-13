cmake_minimum_required(VERSION 3.10)
project(homework_2)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

include_directories(include)

add_executable(main src/main.cpp src/Detector.cpp)

target_link_libraries(main ${OpenCV_LIBS})
add_executable(tuner src/tuner.cpp)
target_link_libraries(tuner ${OpenCV_LIBS})
