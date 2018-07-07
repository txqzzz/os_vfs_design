cmake_minimum_required(VERSION 3.10)
project(os_vfs_design)

set(CMAKE_CXX_STANDARD 11)

include_directories(vfs)

add_executable(os_vfs_design
        .idea/vcs.xml
        vfs/.idea/codeStyles/Project.xml
        vfs/.idea/dictionaries/dsay.xml
        vfs/.idea/misc.xml
        vfs/.idea/modules.xml
        vfs/.idea/vcs.xml
        vfs/.idea/vfs.iml
        vfs/.idea/workspace.xml
        vfs/cmake-build-debug/cmake_install.cmake
        vfs/cmake-build-debug/CMakeCache.txt
        vfs/cmake-build-debug/Makefile
        vfs/cmake-build-debug/vfs
        vfs/cmake-build-debug/vfs.cbp
        vfs/cmake-build-debug/vfs.layout
        vfs/cmake-build-debug/VFS.VHD
        vfs/CMakeLists.txt
        vfs/DataStructure.h
        vfs/dentryController.cpp
        vfs/dentryController.h
        vfs/dev_notes
        vfs/fsController.cpp
        vfs/fsController.h
        vfs/iNodeController.cpp
        vfs/iNodeController.h
        vfs/main.cpp
        vfs/Params.h
        vfs/sbController.cpp
        vfs/sbController.h
        vfs/TestController.cpp
        vfs/TestController.h
        vfs/vhdController.cpp
        vfs/vhdController.h)
