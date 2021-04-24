
cmake_minimum_required(VERSION 2.8)

PROJECT(imgui)
SET(LIBRARY_NAME imgui)

OPTION(IMGUI_BUILD_STATIC "Enable build static library" ON)

SET( LIBJPEG_HEADERS
    imconfig.h
    imgui.h
)
SET( LIBJPEG_SOURCE 
    imgui.cpp
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_widgets.cpp
    
)

IF(MSVC)
    SET(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")
    SET(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")
ELSEIF(MSYS AND MINGW)
    SET(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")
    SET(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")
ELSEIF(ANDROID) #android
ELSE(MSVC)
ENDIF(MSVC)

if( IMGUI_BUILD_STATIC )
    add_definitions(-DIMGUI_LIBRARY_STATIC)
    ADD_LIBRARY(${LIBRARY_NAME} STATIC ${LIBJPEG_HEADERS} ${LIBJPEG_SOURCE})
ELSE( IMGUI_BUILD_STATIC )
    add_definitions(-DIMGUI_LIBRARY)
    ADD_LIBRARY(${LIBRARY_NAME} SHARED ${LIBJPEG_HEADERS} ${LIBJPEG_SOURCE})
ENDIF( IMGUI_BUILD_STATIC )

INSTALL(TARGETS ${LIBRARY_NAME} 
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        )
INSTALL(
        FILES       ${LIBJPEG_HEADERS}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include
        )

        