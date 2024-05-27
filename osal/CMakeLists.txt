set(target osal)

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
	file(GLOB_RECURSE ${target}_src ${CMAKE_CURRENT_SOURCE_DIR}/linux/*.c)
else()
	file(GLOB_RECURSE ${target}_src ${CMAKE_CURRENT_SOURCE_DIR}/none/*.c)
endif()

add_library(${target} OBJECT ${osal_src})
target_link_libraries(${target} 3rd-party)

target_compile_options(${target} PRIVATE
	-Wno-format
	-Wno-char-subscripts
	-Wno-error=unused-result
	-Wno-unused-result)

