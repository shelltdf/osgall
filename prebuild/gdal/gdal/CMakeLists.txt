# ******************************************************************************
# * Project:  CMake4GDAL
# * Purpose:  CMake build scripts
# * Author: Dmitriy Baryshnikov (aka Bishop), polimax@mail.ru
# ******************************************************************************
# * Copyright (C) 2012,2013 Bishop
# * 
# * Permission is hereby granted, free of charge, to any person obtaining a
# * copy of this software and associated documentation files (the "Software"),
# * to deal in the Software without restriction, including without limitation
# * the rights to use, copy, modify, merge, publish, distribute, sublicense,
# * and/or sell copies of the Software, and to permit persons to whom the
# * Software is furnished to do so, subject to the following conditions:
# *
# * The above copyright notice and this permission notice shall be included
# * in all copies or substantial portions of the Software.
# *
# * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
# * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# * DEALINGS IN THE SOFTWARE.
# ******************************************************************************

cmake_minimum_required (VERSION 2.8.10)

set(PROJECT_NAME "gdallib")
project(${PROJECT_NAME})

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
set(CMAKE_COLOR_MAKEFILE ON)

# parse the version number from gdal_version.h and include in GDAL_MAJOR_VERSION and GDAL_MINOR_VERSION
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/gcore/gdal_version.h.in GDAL_VERSION_H_CONTENTS)
string(REGEX MATCH "GDAL_VERSION_MAJOR[ \t]+([0-9]+)"
  GDAL_MAJOR_VERSION ${GDAL_VERSION_H_CONTENTS})
string (REGEX MATCH "([0-9]+)"
  GDAL_MAJOR_VERSION ${GDAL_MAJOR_VERSION})
string(REGEX MATCH "GDAL_VERSION_MINOR[ \t]+([0-9]+)"
  GDAL_MINOR_VERSION ${GDAL_VERSION_H_CONTENTS})
string (REGEX MATCH "([0-9]+)"
  GDAL_MINOR_VERSION ${GDAL_MINOR_VERSION})
string(REGEX MATCH "GDAL_VERSION_REV[ \t]+([0-9]+)"
  GDAL_REV_VERSION ${GDAL_VERSION_H_CONTENTS})  
string (REGEX MATCH "([0-9]+)"
  GDAL_REV_VERSION ${GDAL_REV_VERSION})
    
# Setup package meta-data
set(GDAL_VERSION ${GDAL_MAJOR_VERSION}.${GDAL_MINOR_VERSION}.${GDAL_REV_VERSION})
message(STATUS "gdal version=[${GDAL_VERSION}]")
    
set(GDAL_LIB_NAME gdal${GDAL_MAJOR_VERSION}${GDAL_MINOR_VERSION})
message(STATUS "GDAL lib name ... " ${GDAL_LIB_NAME})

#diagnosing info
message(STATUS "c++ compiler ... " ${CMAKE_CXX_COMPILER})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(GDAL_ROOT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(GDAL_ROOT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})


if(NACL)
SET(CMAKE_C_USE_RESPONSE_FILE_FOR_LIBRARIES 1)
SET(CMAKE_CXX_USE_RESPONSE_FILE_FOR_LIBRARIES 1)
SET(CMAKE_C_USE_RESPONSE_FILE_FOR_OBJECTS 1)
SET(CMAKE_CXX_USE_RESPONSE_FILE_FOR_OBJECTS 1)
SET(CMAKE_C_USE_RESPONSE_FILE_FOR_INCLUDES 1)
SET(CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES 1)
set(CMAKE_C_RESPONSE_FILE_LINK_FLAG "@")
set(CMAKE_CXX_RESPONSE_FILE_LINK_FLAG "@")
SET(CMAKE_C_CREATE_STATIC_LIBRARY "<CMAKE_AR> rc <TARGET> <LINK_FLAGS> <OBJECTS>")
SET(CMAKE_CXX_CREATE_STATIC_LIBRARY "<CMAKE_AR> rc <TARGET> <LINK_FLAGS> <OBJECTS>") 
endif(NACL)

if (EMSCRIPTEN)
     set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-pthread")
     set(CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS} "-pthread")
endif (EMSCRIPTEN)

if(ANDROID)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-psabi ")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-psabi ")
elseif(NACL)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-psabi ")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-psabi ")
elseif(MINGW)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-psabi ")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-psabi -fpermissive")
elseif(EMSCRIPTEN)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-psabi ")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-psabi ")
elseif(WIN32)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -W4 -wd4127 -wd4251 -wd4275 -wd4786 -wd4100 -wd4245 -wd4206 -wd4018 -wd4389")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W4 -wd4127 -wd4251 -wd4275 -wd4786 -wd4100 -wd4245 -wd4206 -wd4018 -wd4389")
else( ANDROID)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -W")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -fpermissive")
endif( ANDROID)

# set path to additional CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# uninstall target
configure_file(${CMAKE_MODULE_PATH}/cmake_uninstall.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake IMMEDIATE @ONLY)
   
# For windows, do not allow the compiler to use default target (Vista).
if(WIN32)
  add_definitions(-D_WIN32_WINNT=0x0501 -D_USRDLL)
endif(WIN32)

if(MSVC)
  set(CMAKE_DEBUG_POSTFIX "d")
  add_definitions(-D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE)
  #add_definitions(-D_MBCS)
if( MSVC_VERSION LESS 1900)
  set(GDAL_CSOURCES ${GDAL_CSOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/gcore/Version.rc)
  source_group("Resource Files" FILES ${CMAKE_CURRENT_SOURCE_DIR}/gcore/Version.rc)  
endif( MSVC_VERSION LESS 1900)
endif(MSVC)

if(NOT EMSCRIPTEN AND NOT ANDROID AND NOT NACL AND NOT MINGW)
add_definitions(-DSTRICT -DHAVE_SSE_AT_COMPILE_TIME)
endif(NOT EMSCRIPTEN AND NOT ANDROID AND NOT NACL AND NOT MINGW)

if(UNIX)
    add_definitions(-DCPL_LSB)
endif(UNIX)

if(MSVC)
    if(CMAKE_CL_64)
        # set_target_properties(${GDAL_LIB_NAME} PROPERTIES STATIC_LIBRARY_FLAGS "/machine:x64")
        add_definitions(-D_WIN64)
    endif(CMAKE_CL_64)
endif(MSVC)

if(UNIX)
     if(CMAKE_COMPILER_IS_GNUCXX OR CV_ICC)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -fno-strict-aliasing")#-Wextra -Wall -W -pthread -O2 -fno-strict-aliasing -pthrea
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fno-strict-aliasing")
     endif()
endif(UNIX)

option(GDAL_USE_CPL "Set ON to use cpl" ON)
option(GDAL_USE_OPENCL "" OFF)

option(GDAL_ENABLE_OGR "Build OGR module" ON)
if(GDAL_ENABLE_OGR)
	add_definitions(-DOGR_ENABLED)
endif()

include_directories(${GDAL_ROOT_SOURCE_DIR}/port)

set(GDAL_TARGET_OBJECTS "")
set(GDAL_TARGET_LINK_LIB "")



list(REMOVE_DUPLICATES GDAL_TARGET_OBJECTS)
foreach(TO ${GDAL_TARGET_OBJECTS})
	message(STATUS "gdal link=${TO}")
endforeach(TO)

option(GDAL_BUILD_SHARED "Set ON to build shared library" ON)

if(GDAL_BUILD_SHARED)

else(GDAL_BUILD_SHARED)
    ADD_DEFINITIONS(-DXML_STATIC)
    ADD_DEFINITIONS(-DCPL_DISABLE_DLL)
    ADD_DEFINITIONS(-DCPL_DISABLE_STDCALL)
endif(GDAL_BUILD_SHARED)

ADD_DEFINITIONS(-DNOMINMAX)
add_definitions(-DGDAL_COMPILATION)

# option(GDAL_USE_JSONC "Set ON to use json-c" ON)
# if(GDAL_USE_JSONC)
    # find_package(JSONC REQUIRED)
    # include_directories(${JSONC_INCLUDE_DIR})
# endif(GDAL_USE_JSONC)

option(OGR_USE_JSONC_INTERNAL "Set ON to use internal libjson-c" ON)
if(OGR_USE_JSONC_INTERNAL)
    include_directories(${GDAL_ROOT_SOURCE_DIR}/ogr/ogrsf_frmts/geojson/libjson)
    # set(GDAL_TARGET_LINK_LIB ${GDAL_TARGET_LINK_LIB} ogrsf_geojson_jsonc)
else(OGR_USE_JSONC_INTERNAL)
endif(OGR_USE_JSONC_INTERNAL)
    
    
add_subdirectory(port)
add_subdirectory(gcore)
add_subdirectory(alg)
add_subdirectory(ogr)
add_subdirectory(frmts)
add_subdirectory(gnm)
add_subdirectory(apps)


if(GDAL_BUILD_SHARED)
	add_library(${GDAL_LIB_NAME} SHARED ${GDAL_CSOURCES} ${GDAL_TARGET_OBJECTS})#${GDAL_HHEADERS} 
else(GDAL_BUILD_SHARED)
	add_library(${GDAL_LIB_NAME} STATIC ${GDAL_CSOURCES} ${GDAL_TARGET_OBJECTS})#${GDAL_HHEADERS} 
endif(GDAL_BUILD_SHARED)


if(MSVC)
    if(CMAKE_CL_64)
        set_target_properties(${GDAL_LIB_NAME} PROPERTIES STATIC_LIBRARY_FLAGS "/machine:x64")
        # add_definitions(-D_WIN64)
    endif(CMAKE_CL_64)
endif(MSVC)

configure_file(${CMAKE_MODULE_PATH}/gdal_def.cmake ${GDAL_ROOT_SOURCE_DIR}/gcore/gdal_def.h @ONLY)

list(REMOVE_DUPLICATES GDAL_TARGET_LINK_LIB)
foreach(TL ${GDAL_TARGET_LINK_LIB})
	message(STATUS "gdal target link lib=${TL}")
endforeach(TL)

SET(GDAL_TARGET_LINK_LIB "")
# if(POSTGRES_FOUND)
    if(WIN32)
		set(GDAL_TARGET_LINK_LIB ${GDAL_TARGET_LINK_LIB} Ws2_32.lib Secur32.lib Psapi.lib)
    endif(WIN32)
# endif(POSTGRES_FOUND)

# if(GDAL_TARGET_LINK_LIB)
	target_link_libraries(${GDAL_LIB_NAME} 
    ${GDAL_TARGET_LINK_LIB}
    # ws2_32.lib
    optimized ${TIFF_LIBRARY} debug ${TIFF_LIBRARY_DEBUG}
    optimized ${GEOTIFF_LIBRARY} debug ${GEOTIFF_LIBRARY_DEBUG}
    #optimized ${XTIFF_LIBRARY} debug ${XTIFF_LIBRARY_DEBUG}
    optimized ${GEOS_LIBRARY} debug ${GEOS_LIBRARY_DEBUG}
    optimized ${SQLITE3_LIBRARY} debug ${SQLITE3_LIBRARY_DEBUG}
    optimized ${PROJ_LIBRARY} debug ${PROJ_LIBRARY_DEBUG}
    optimized ${EXPAT_LIBRARY} debug ${EXPAT_LIBRARY_DEBUG}
    optimized ${ZLIB_LIBRARY} debug ${ZLIB_LIBRARY_DEBUG}
    optimized ${PNG_LIBRARY_RELEASE} debug ${PNG_LIBRARY_DEBUG}
    optimized ${JPEG_LIBRARY_RELEASE} debug ${JPEG_LIBRARY_DEBUG}
    ${ICONV_LIBRARIES}
    )
# endif(GDAL_TARGET_LINK_LIB)
 
# if(PROJ_FOUND)
    # target_link_libraries(${GDAL_LIB_NAME} ${PROJ_LIBRARY})
# endif(PROJ_FOUND)

set_target_properties(${GDAL_LIB_NAME}
	PROPERTIES PROJECT_LABEL ${PROJECT_NAME}
    VERSION ${GDAL_VERSION}
    SOVERSION 1
	ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
	

if(WIN32)
    if(CMAKE_CL_64)
        install(TARGETS ${GDAL_LIB_NAME}
			RUNTIME DESTINATION bin
			ARCHIVE DESTINATION lib
			LIBRARY DESTINATION lib)
    else(CMAKE_CL_64)
        install(TARGETS ${GDAL_LIB_NAME}
			RUNTIME DESTINATION bin
			ARCHIVE DESTINATION lib
			LIBRARY DESTINATION lib)
    endif(CMAKE_CL_64)
	install(FILES ${GDAL_ROOT_SOURCE_DIR}/gcore/gdal_def.h DESTINATION include/gdal)
else(WIN32)
    install(TARGETS ${GDAL_LIB_NAME}
        RUNTIME DESTINATION bin
        # ARCHIVE DESTINATION lib/gdal
        # LIBRARY DESTINATION lib/gdal)
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib)
	install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/gcore/gdal_def.h DESTINATION include/gdal)
endif(WIN32) 
    
add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
