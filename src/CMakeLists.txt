add_executable(tetris)
target_sources(tetris PRIVATE
  "tetris.cpp"
  "Cell.cpp" "../include/Cell.h"
  "Grid.cpp" "../include/Grid.h"
  "TetrisApp.cpp" "../include/TetrisApp.h"
  "TetrisGame.cpp" "../include/TetrisGame.h"
  "Tetromino.cpp" "../include/Tetromino.h"
  "TitleScreen.cpp" "../include/TitleScreen.h"
  "../include/constants.h"
  "../include/flags.h"
  "../include/GridPoint.h")
target_include_directories(tetris PUBLIC "../include/")
target_link_libraries(tetris PRIVATE tetris_extern)
target_link_libraries(tetris PRIVATE utilityKnife)
target_link_libraries(tetris PRIVATE SDL3::SDL3)
target_link_libraries(tetris PRIVATE SDL3_ttf::SDL3_ttf)
target_link_options(tetris PRIVATE $<$<AND:$<PLATFORM_ID:Windows>,$<CONFIG:Release>>:-mwindows>)

set(is_CC "$<CXX_COMPILER_ID:Clang,AppleClang,GNU>")
set(is_MS "$<CXX_COMPILER_ID:MSVC>")
target_compile_options(${PROJECT_NAME} PRIVATE
	$<$<OR:${is_CC}>:-Wall>
	$<$<OR:${is_CC}>:-Wextra>
	$<$<OR:${is_CC}>:-Wsign-conversion>
	$<$<OR:${is_CC}>:-pedantic>
	$<$<OR:${is_CC}>:-std=c++20>
	$<$<OR:${is_CC}>:-Weffc++>
	$<${is_MS}:/W4>)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/data/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/data/)