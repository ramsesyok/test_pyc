cmake_minimum_required(VERSION 3.10.0)
project(test_pyc VERSION 0.1.0 LANGUAGES C CXX)


add_executable(test_pyc main.cpp)
target_include_directories(test_pyc PRIVATE "C:/Program Files/Python312/include")
target_link_libraries(test_pyc PRIVATE "C:/Program Files/Python312/libs/python312.lib")
