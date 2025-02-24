#############################################################################
# Toolchain file
#############################################################################
message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}.")

cmake_minimum_required(VERSION 3.5)

project(alumy C ASM)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 1)
set(PROJECT_VERSION_TWEAK 10)
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

#############################################################################
# Check for linux
#############################################################################
include(${PROJECT_SOURCE_DIR}/cmake/toolchain/toolchain-flags.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/linux_check.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/generic_check.cmake)

#############################################################################
# Parameter for compile
#############################################################################

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(STATUS "64 bits cpu")
    set(CMAKE_INSTALL_LIBDIR lib64)
else()
    message(STATUS "32 bits cpu")
    set(CMAKE_INSTALL_LIBDIR lib)
endif()

#############################################################################
# Options
#############################################################################
option(BUILD_SHARED_LIBS "enable shared library" OFF)
option(BUILD_STATIC_LIBS "enable shared library" ON)

if(BUILD_STATIC_LIBS)
	set(BUILD_SHARED_LIBS OFF)
endif()

if(UNIT_TEST EQUAL 1)
	add_definitions(-DAL_UT)
endif()

include(GNUInstallDirs)
include(${PROJECT_SOURCE_DIR}/cmake/message.cmake)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

configure_file(
    ${PROJECT_SOURCE_DIR}/include/alumy/version.h.in
    ${PROJECT_SOURCE_DIR}/include/alumy/version.h)

#######################################################################
# get svn revision
#######################################################################
add_custom_command(
    OUTPUT ${PROJECT_SOURCE_DIR}/include/alumy/svn_revision.h
    ALL
    COMMAND ${CMAKE_COMMAND} -D TARGET_DIR=${CMAKE_CURRENT_BINARY_DIR} -P ${CMAKE_SOURCE_DIR}/cmake/svn-revision.cmake
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# rebuild svn_revision.h every time
add_custom_target(get_svn_revision
    ALL
    DEPENDS ${PROJECT_SOURCE_DIR}/include/alumy/svn_revision.h
)

#####################################################################
# get git hash
#####################################################################
add_custom_command(
    OUTPUT ${PROJECT_SOURCE_DIR}/include/alumy/git_hash.h
    ALL
    COMMAND ${CMAKE_COMMAND} -D TARGET_DIR=${CMAKE_CURRENT_BINARY_DIR} -P ${PROJECT_SOURCE_DIR}/cmake/git-hash.cmake
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# rebuild git_hash.h every time
add_custom_target(get_git_hash
    ALL
    DEPENDS ${PROJECT_SOURCE_DIR}/include/alumy/git_hash.h
)

add_definitions(-D__STDC_FORMAT_MACROS)
add_definitions(-DAL_USE_DEFAULT_CONFIG)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

if(${CMAKE_SYSTEM_NAME} STREQUAL Linux)
    link_libraries(rt)
endif()

link_libraries(m)

add_subdirectory(${PROJECT_SOURCE_DIR}/3rd-party)
add_subdirectory(${PROJECT_SOURCE_DIR}/crypto)
add_subdirectory(${PROJECT_SOURCE_DIR}/crc)
add_subdirectory(${PROJECT_SOURCE_DIR}/lrc)
add_subdirectory(${PROJECT_SOURCE_DIR}/mem)
add_subdirectory(${PROJECT_SOURCE_DIR}/math)
add_subdirectory(${PROJECT_SOURCE_DIR}/sys)
add_subdirectory(${PROJECT_SOURCE_DIR}/network)
add_subdirectory(${PROJECT_SOURCE_DIR}/filter)
add_subdirectory(${PROJECT_SOURCE_DIR}/string)
add_subdirectory(${PROJECT_SOURCE_DIR}/bcd)
add_subdirectory(${PROJECT_SOURCE_DIR}/qrcode)
add_subdirectory(${PROJECT_SOURCE_DIR}/bcc)
add_subdirectory(${PROJECT_SOURCE_DIR}/base64)
add_subdirectory(${PROJECT_SOURCE_DIR}/unity)
add_subdirectory(${PROJECT_SOURCE_DIR}/cmock)
add_subdirectory(${PROJECT_SOURCE_DIR}/cunit)
add_subdirectory(${PROJECT_SOURCE_DIR}/osal)
add_subdirectory(${PROJECT_SOURCE_DIR}/cjson)
add_subdirectory(${PROJECT_SOURCE_DIR}/libcsv)
add_subdirectory(${PROJECT_SOURCE_DIR}/protobuf-c)
add_subdirectory(${PROJECT_SOURCE_DIR}/pid)
add_subdirectory(${PROJECT_SOURCE_DIR}/lzma)
add_subdirectory(${PROJECT_SOURCE_DIR}/xyzmodem)
add_subdirectory(${PROJECT_SOURCE_DIR}/time)
add_subdirectory(${PROJECT_SOURCE_DIR}/net)
add_subdirectory(${PROJECT_SOURCE_DIR}/expect)
add_subdirectory(${PROJECT_SOURCE_DIR}/bitops)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib)

#############################################################################
# libalumy
#############################################################################

set(objs 
		$<TARGET_OBJECTS:3rd-party>
		$<TARGET_OBJECTS:sys>
		$<TARGET_OBJECTS:crypto>
		$<TARGET_OBJECTS:cunit>
		$<TARGET_OBJECTS:crc>
		$<TARGET_OBJECTS:lrc>
		$<TARGET_OBJECTS:mem>
		$<TARGET_OBJECTS:math>
		$<TARGET_OBJECTS:filter>
		$<TARGET_OBJECTS:network>
		$<TARGET_OBJECTS:string>
		$<TARGET_OBJECTS:bcd>
		$<TARGET_OBJECTS:qrcode>
		$<TARGET_OBJECTS:bcc>
		$<TARGET_OBJECTS:base64>
		$<TARGET_OBJECTS:osal>
		$<TARGET_OBJECTS:cjson>
		$<TARGET_OBJECTS:libcsv>
		$<TARGET_OBJECTS:protobuf-c>
		$<TARGET_OBJECTS:pid>
		$<TARGET_OBJECTS:lzma>
		$<TARGET_OBJECTS:xyzmodem>
		$<TARGET_OBJECTS:time>
		$<TARGET_OBJECTS:net>
		$<TARGET_OBJECTS:expect>
		$<TARGET_OBJECTS:lib>
		$<TARGET_OBJECTS:bitops>)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_NAME}_src)

include(${PROJECT_SOURCE_DIR}/cmake/check_type_size.cmake)

if(${CMAKE_SYSTEM_NAME} STREQUAL Linux)
    if(BUILD_SHARED_LIBS)
    	message(STATUS "Building shared library for Linux")

    	add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_src} ${objs})
    	target_link_libraries(${PROJECT_NAME} 3rd-party)

    	add_dependencies(${PROJECT_NAME} get_git_hash)
    	add_dependencies(${PROJECT_NAME} get_svn_revision)

    	set_target_properties(${PROJECT_NAME} PROPERTIES
           	VERSION "${PROJECT_VERSION}"
           	SOVERSION "${PROJECT_VERSION_MAJOR}")

    	install(TARGETS "${PROJECT_NAME}"
            	DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            	EXPORT "${PROJECT_NAME}")
    endif()
endif()

if(BUILD_STATIC_LIBS)
    add_library("${PROJECT_NAME}-static" STATIC ${${PROJECT_NAME}_src} ${objs})
    target_link_libraries("${PROJECT_NAME}-static" 3rd-party)
    
    add_dependencies("${PROJECT_NAME}-static" get_git_hash)
    add_dependencies("${PROJECT_NAME}-static" get_svn_revision)
    add_dependencies("${PROJECT_NAME}-static" 3rd-party)
    
    # target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR})
    # target_compile_options(<target> [BEFORE] <INTERFACE|PUBLIC|PRIVATE> [items1...] [<INTERFACE|PUBLIC|PRIVATE> [items2...] ...])
    
    set_target_properties("${PROJECT_NAME}-static" PROPERTIES OUTPUT_NAME "${PROJECT_NAME}")
    set_target_properties("${PROJECT_NAME}-static" PROPERTIES PREFIX "lib")
    
    install(TARGETS "${PROJECT_NAME}-static"
	    DESTINATION "${CMAKE_INSTALL_LIBDIR}")
endif()
    
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
	FILES_MATCHING PATTERN "*.h")

include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_VENDOR alumy)
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CHECKSUM "SHA256")
set(CPACK_GENERATOR "STGZ;TGZ;TZ")

set(CPACK_SOURCE_IGNORE_FILES
    ${PROJECT_SOURCE_DIR}/build
    ${PROJECT_SOURCE_DIR}/pack
    ${PROJECT_SOURCE_DIR}/.svn)
set(CPACK_SOURCE_GENERATOR "TGZ")

include(CPack)

if(UNIT_TEST EQUAL 1)
	if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
		enable_testing()
		add_subdirectory(${PROJECT_SOURCE_DIR}/test)
	endif()
endif()


