set(target 3rd-party)

include(ExternalProject)

ExternalProject_Add(
	curl_proj
	SOURCE_DIR ${PROJECT_SOURCE_DIR}/3rd-party/curl
	DOWNLOAD_COMMAND ""
	UPDATE_COMMAND ""
	CMAKE_ARGS
		-DCMAKE_TOOLCHAIN_FILE:FILE_PATH=${CMAKE_TOOLCHAIN_FILE}
		-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/curl-release
		-DCURL_ENABLE_SSL=OFF)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} ${target}_src)

add_library(${target} OBJECT ${${target}_src})

add_dependencies(${target} curl_proj)

# curl 
target_include_directories(${target} PUBLIC ${CMAKE_BINARY_DIR}/curl-release/include) 
target_link_directories(${target} PUBLIC ${CMAKE_BINARY_DIR}/curl-release/lib)
target_link_libraries(${target} PUBLIC curl)
