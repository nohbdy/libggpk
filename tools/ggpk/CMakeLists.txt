include_directories(${LIBGGPK_SOURCE_DIR}/include)
link_directories(${LIBGGPK_BINARY_DIR}/bin)
add_executable(ggpk ggpk.cpp ggpk_extract.cpp ggpk_info.cpp ggpk_list.cpp ggpk_none.cpp)
target_link_libraries(ggpk libggpk)
