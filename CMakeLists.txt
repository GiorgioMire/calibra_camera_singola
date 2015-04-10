cmake_minimum_required(VERSION 2.8)
project(calibra)
find_package(OpenCV REQUIRED)




include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(calibra ./src/calibra.cpp)
message("${OpenCV_LIBS}")
target_link_libraries(calibra ${OpenCV_LIBS} )
