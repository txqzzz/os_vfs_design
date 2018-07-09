cmake_minimum_required(VERSION 3.10)
project(os_vfs_design)

set(CMAKE_CXX_STANDARD 11)

include_directories(vfs)

add_executable(os_vfs_design
        vfs/main.cpp
        vfs/CMakeLists.txt
        vfs/DataStructure.h
        vfs/dentryController.cpp
        vfs/dentryController.h
        vfs/dev_notes
        vfs/fsController.cpp
        vfs/fsController.h
        vfs/iNodeController.cpp
        vfs/iNodeController.h
        vfs/Params.h
        vfs/sbController.cpp
        vfs/sbController.h
        vfs/TestController.cpp
        vfs/TestController.h
        vfs/vhdController.cpp
        vfs/vhdController.h
        vfs/userController.cpp
        vfs/userController.h
        vfs/LGM.cpp vfs/dataBController.cpp vfs/dataBController.h)

