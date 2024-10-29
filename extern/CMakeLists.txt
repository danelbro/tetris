add_subdirectory(utilityKnife)

add_library(tetris_extern INTERFACE)
target_link_libraries(tetris_extern INTERFACE utilityKnife)
target_link_libraries(tetris_extern INTERFACE SDL3::SDL3)
target_link_libraries(tetris_extern INTERFACE SDL3_ttf::SDL3_ttf)
