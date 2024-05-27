set(target 3rd-party)

include(ExternalProject)

ExternalProject_Add(
	curl_proj
	SOURCE_DIR ${PROJECT_SOURCE_DIR}/3rd-party/curl
	DOWNLOAD_COMMAND ""
	UPDATE_COMMAND ""
	CMAKE_ARGS
		-DCMAKE_TOOLCHAIN_FILE:FILE_PATH=${CMAKE_TOOLCHAIN_FILE}
		-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/curl
		-DCURL_ENABLE_SSL=OFF)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} ${target}_src)
add_library(${target} OBJECT ${${target}_src})
add_dependencies(${target} curl_proj)

# curl 
include_directories(${CMAKE_CURRENT_BINARY_DIR}/curl/include) 
link_directories(${CMAKE_CURRENT_BINARY_DIR}/curl/lib)
link_libraries(curl)
