cmake_minimum_required(VERSION 3.10)
project(vfs)

set(CMAKE_CXX_STANDARD 11)

add_executable(vfs main.cpp vhdController.h Params.h vhdController.cpp sbController.cpp sbController.h fsController.cpp fsController.h TestController.cpp TestController.h dentryController.cpp dentryController.h iNodeController.cpp iNodeController.h "DataStructure.h")