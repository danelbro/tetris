cmake_minimum_required(VERSION 3.27)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}" CACHE INTERNAL "")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(tetris)

if(MSVC)
	if(NOT CMAKE_GENERATOR STREQUAL "Ninja")
		add_definitions(/MP)
	endif()
endif()

set(SRC_PATH "src")
set(INC_PATH "include")
file(GLOB_RECURSE SRCS "${SRC_PATH}/*.cpp")
file(GLOB_RECURSE HEADERS "${INC_PATH}/*.h")

add_executable(${PROJECT_NAME} ${SRCS} ${HEADERS})
set_target_properties(${PROJECT_NAME}
	PROPERTIES WIN32_EXECUTABLE $<AND:$<PLATFORM_ID:Windows>,$<CONFIG:Release>>)
source_group("Source Files" FILES ${SRCS})
source_group("Header Files" FILES ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC ${INC_PATH})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

if(MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE
		/W4
		/permissive-
		/w44365
		/external:W3
	)
else()
	target_compile_options(${PROJECT_NAME} PRIVATE
		-pedantic
		-pedantic-errors
		-Wall
		-Wextra
		-Wconversion
		-Wsign-conversion
		-Werror
		-std=c++20
	)
endif()

add_subdirectory(extern/utilityKnife)
target_link_libraries(tetris PUBLIC utilityKnife)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "${PROJECT_NAME}")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/data/ ${CMAKE_BINARY_DIR}/data/)
