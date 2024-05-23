set(target network)

file(GLOB ${target}_src
		${CMAKE_CURRENT_SOURCE_DIR}/wireless/*.c 
		${CMAKE_CURRENT_SOURCE_DIR}/*.c) 

add_library(${target} OBJECT ${${target}_src})

target_compile_options(${target} PRIVATE
	-Wno-format
	-Wno-char-subscripts
	-Wno-error=unused-result
	-Wno-unused-result)

