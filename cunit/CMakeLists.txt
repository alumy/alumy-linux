set(target cunit)

FILE (GLOB_RECURSE ${target}_src
    ${CMAKE_CURRENT_SOURCE_DIR}/Automated/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/Basic/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/Console/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/Framework/*.c)

add_library(${target} OBJECT ${${target}_src})

target_compile_options(${target} PRIVATE 
	-Wno-format 
	-Wno-char-subscripts 
	-Wno-error=unused-result 
	-Wno-unused-result)

target_include_directories(${target}
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/letter-shell-3.1.2/src
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/mbedtls-3.2.1/library
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/mbedtls-3.2.1/include
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lwip-2.1.3/src/include)

