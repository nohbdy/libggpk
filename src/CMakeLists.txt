include_directories(${LIBGGPK_SOURCE_DIR}/include/ggpk)
add_library(libggpk Node.cpp Archive.cpp)

# libggpk must link with Qt4
target_link_libraries(libggpk ${QT_LIBRARIES})
