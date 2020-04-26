
PROJECT(osgXI)
SET(PROJECT_NAME osgXI)

SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../common/CMakeModules/")
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.7)

IF(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

SET(OSGXI_MAJOR_VERSION 0)
SET(OSGXI_MINOR_VERSION 1)
SET(OSGXI_PATCH_VERSION 0)
SET(OSGXI_VERSION ${OSGXI_MAJOR_VERSION}.${OSGXI_MINOR_VERSION}.${OSGXI_PATCH_VERSION})


FIND_PACKAGE(Doxygen)
IF( NOT ANDROID )
    FIND_PACKAGE(OpenGL)
ENDIF( NOT ANDROID )
FIND_PACKAGE(WOSG)


SET(OSG_DEBUG_POSTFIX "d" CACHE STRING "use postfix to link with osg with debug")

IF(WIN32)
    IF(MSVC)
        OPTION(WIN32_USE_MP "Set to ON to build OpenSceneGraph with the /MP option (Visual Studio 2005 and above)." OFF)
        MARK_AS_ADVANCED(WIN32_USE_MP)
        IF(WIN32_USE_MP)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
        ENDIF(WIN32_USE_MP)

        # More MSVC specific compilation flags
        ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
        ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
    ENDIF(MSVC)
ENDIF(WIN32)

SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} CACHE STRING "You may add additional search paths here. Use ; to separate multiple paths.")
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} CACHE STRING "You may add additional search paths here. Use ; to separate multiple paths.")


IF (NOT WIN32)
    SET(CMAKE_CXX_FLAGS "-W -Wall -Wno-unused")
ENDIF(NOT WIN32)


# Here we apparantly do some funky stuff with making the bin/ and lib/
# folders which is probably needed to work around a very old CMake bug?

#SET(OUTPUT_BINDIR ${PROJECT_BINARY_DIR}/bin/${CMAKE_SYSTEM_NAME})
SET(OUTPUT_BINDIR ${PROJECT_BINARY_DIR}/bin)
MAKE_DIRECTORY(${OUTPUT_BINDIR})
IF(MSVC AND NOT MSVC_IDE)
    MAKE_DIRECTORY(${OUTPUT_BINDIR}/${OSG_PLUGINS})
ENDIF(MSVC AND NOT MSVC_IDE)

#SET(OUTPUT_LIBDIR ${PROJECT_BINARY_DIR}/lib/${CMAKE_SYSTEM_NAME})
SET(OUTPUT_LIBDIR ${PROJECT_BINARY_DIR}/lib)
MAKE_DIRECTORY(${OUTPUT_LIBDIR})
IF(NOT MSVC OR MSVC_IDE)
    MAKE_DIRECTORY(${OUTPUT_LIBDIR}/${OSG_PLUGINS})
ENDIF(NOT MSVC OR MSVC_IDE)

# On CMake 2.4.x use EXECUTABLE_OUTPUT_PATH and LIBRARY_OUTPUT_PATH and later
# we work around the DLL placement by use of the PREFIX target property hack
#
# On CMake 2.6.x use the newly minted CMAKE_LIBRARY_OUTPUT_DIRECTORY,
# CMAKE_ARCHIVE_OUTPUT_DIRECTORY & CMAKE_RUNTIME_OUTPUT_DIRECTORY

IF(CMAKE_MAJOR_VERSION EQUAL 2 AND CMAKE_MINOR_VERSION LESS 5)
    SET(EXECUTABLE_OUTPUT_PATH ${OUTPUT_BINDIR})
    SET(LIBRARY_OUTPUT_PATH    ${OUTPUT_LIBDIR})
ELSE()
    # If CMake >= 2.6.0
    SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_LIBDIR})
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_BINDIR})
    IF(WIN32)
        SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_BINDIR})
    ELSE(WIN32)
        SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_LIBDIR})
    ENDIF(WIN32)
ENDIF()


# set postfix
SET(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")
SET(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")
SET(CMAKE_RELWITHDEBINFO_POSTFIX "rd" CACHE STRING "add a postfix, usually empty on windows")
SET(CMAKE_MINSIZEREL_POSTFIX "s" CACHE STRING "add a postfix, usually empty on windows")


INCLUDE_DIRECTORIES(include ${CMAKE_INCLUDE_PATH} ${OPENSCENEGRAPH_INCLUDE_DIR})
LINK_DIRECTORIES(${CMAKE_LIBRARY_PATH} ${OPENSCENEGRAPH_LIB_DIR})

# OPTION(BUILD_OSGCG "Enable to build new osgCg library and related examples" ON)
# OPTION(BUILD_OSGINPUT "Enable to build osgInput library and related examples" OFF)
# OPTION(BUILD_OSGCSNET "Enable to build osgCSNet library and related examples" ON)
# OPTION(BUILD_OSGPHYSX "Build osgPhysx in test, the physics engine based on nVIDIA PhysX" OFF)

# OPTION(BUILD_OSGPOSTEFFECTS_WITH_OSGCG "Build osgPostEffects with osgCg support" OFF)

# OPTION(BUILD_APPLICATION     "Enable to build osgXI application"  ON)
# OPTION(BUILD_TOOLS     "Enable to build osgXI tools"  ON)
# OPTION(BUILD_DEMO_SCENES "Enable to build osgXI demo scenes" ON)
# OPTION(BUILD_EXAMPLES "Enable to build osgXI examples, some of which may be incompatible with old devices" ON)
# OPTION(BUILD_PLUGINS "Enable to build osgXI support for .osg/.ive format" ON)
# OPTION(BUILD_DOCUMENTATION "Build osgXI documentation" ON)

# OPTION(BUILD_OSGXI_STATIC_LIBRARY "Build osgXI static library" OFF)


ADD_SUBDIRECTORY(example)


IF(BUILD_DOCUMENTATION)
    OPTION(BUILD_HTMLHELP "Also build HTML help file (.chm)" OFF)
    IF(BUILD_HTMLHELP)
        SET(GENERATE_HTMLHELP YES)
        
        FIND_PROGRAM(HHC_FILE hhc.exe
          PATHS
          $ENV{PATH}
        )
    ELSE(BUILD_HTMLHELP)
        SET(GENERATE_HTMLHELP NO)
    ENDIF(BUILD_HTMLHELP)

    IF(DOT)
        SET(HAVE_DOT YES)
    ELSE(DOT)
        SET(HAVE_DOT NO)
    ENDIF(DOT)
    
    CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/doc/Doxyfiles/doxyfile.cmake
        ${PROJECT_BINARY_DIR}/doc/doxyfile
    )
    ADD_CUSTOM_TARGET(DoxygenDoc ${DOXYGEN}
        ${PROJECT_BINARY_DIR}/doc/doxyfile
    )
ENDIF(BUILD_DOCUMENTATION)

CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/../common/CMakeModules/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)
ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
