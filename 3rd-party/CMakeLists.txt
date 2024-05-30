set(target 3rd-party)

include(ExternalProject)

ExternalProject_Add(
	curl_proj
	SOURCE_DIR ${PROJECT_SOURCE_DIR}/3rd-party/curl
	DOWNLOAD_COMMAND ""
	UPDATE_COMMAND ""
	CMAKE_ARGS
		-DCMAKE_TOOLCHAIN_FILE:FILE_PATH=${CMAKE_TOOLCHAIN_FILE}
		-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/install
		-DBUILD_STATIC_LIBS=${BUILD_STATIC_LIBS}
		-DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
		-DBUILD_STATIC_CURL=${BUILD_STATIC_LIBS}
		-DBUILD_CURL_EXE=OFF
		-DCURL_ENABLE_SSL=OFF)

ExternalProject_Add(
	iniparser_proj
	SOURCE_DIR ${PROJECT_SOURCE_DIR}/3rd-party/iniparser
	DOWNLOAD_COMMAND ""
	UPDATE_COMMAND ""
	CMAKE_ARGS
		-DCMAKE_TOOLCHAIN_FILE:FILE_PATH=${CMAKE_TOOLCHAIN_FILE}
		-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/install
		-DBUILD_STATIC_LIBS=${BUILD_STATIC_LIBS}
		-DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS})

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} ${target}_src)

add_library(${target} OBJECT ${${target}_src})

add_dependencies(${target} curl_proj)
add_dependencies(${target} iniparser_proj)

target_include_directories(${target} PUBLIC
	${CMAKE_CURRENT_BINARY_DIR}/install/include)

target_link_directories(${target} PUBLIC 
	${CMAKE_CURRENT_BINARY_DIR}/install/lib)

if(BUILD_STATIC_LIBS)
    target_compile_options(${target} PUBLIC -fPIC)
    target_link_libraries(${target} PUBLIC pthread)
    target_link_libraries(${target} PUBLIC 
	    ${CMAKE_CURRENT_BINARY_DIR}/install/lib/libcurl.a
	    ${CMAKE_CURRENT_BINARY_DIR}/install/lib/libiniparser.a)
endif()

if(BUILD_SHARED_LIBS)
    target_link_libraries(${target} PUBLIC 
	    ${CMAKE_CURRENT_BINARY_DIR}/install/lib/libcurl.so
	    ${CMAKE_CURRENT_BINARY_DIR}/install/lib/libiniparser.so)
endif()

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/install/include/
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	USE_SOURCE_PERMISSIONS)

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/install/lib/
	DESTINATION ${CMAKE_INSTALL_LIBDIR}
	USE_SOURCE_PERMISSIONS)

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/install/bin/
	DESTINATION ${CMAKE_INSTALL_BINDIR}
	USE_SOURCE_PERMISSIONS)

