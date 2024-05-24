get_filename_component(toolchain ${CMAKE_TOOLCHAIN_FILE} NAME ABSOLUTE)

if(${toolchain} STREQUAL "gcc.cmake")
	include(${PROJECT_SOURCE_DIR}/cmake/toolchain/gcc-flags.cmake)
elseif(${toolchain} STREQUAL "arm-linux-gnueabi.cmake")
	include(${PROJECT_SOURCE_DIR}/cmake/toolchain/arm-linux-gnueabi-flags.cmake)
endif()
