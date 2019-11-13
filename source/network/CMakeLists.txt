set(target network)

file(GLOB ${target}_src
		${CMAKE_CURRENT_SOURCE_DIR}/wireless/*.c 
		${CMAKE_CURRENT_SOURCE_DIR}/*.c) 

add_library(${target} OBJECT ${${target}_src})

