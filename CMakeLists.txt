cmake_minimum_required(VERSION 2.8)
project(LIBGGPK)

option(LIBGGPK_BUILD_TOOLS "Build the ggpk tool(s)." ON)

find_package(Qt4 COMPONENTS QtCore REQUIRED)
include(${QT_USE_FILE})

add_subdirectory(src)
add_subdirectory(tools)
