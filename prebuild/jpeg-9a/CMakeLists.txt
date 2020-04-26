
cmake_minimum_required(VERSION 2.8)

PROJECT(jpeg)
SET(LIBRARY_NAME jpeg)

OPTION(JPEG_BUILD_STATIC "Enable build static library" OFF)

SET( LIBJPEG_HEADERS
    jconfig.h
    cderror.h
    cdjpeg.h
    jdct.h
    jerror.h
    jinclude.h
    jmemsys.h
    jmorecfg.h
    jpegint.h
    jpeglib.h
    jversion.h
    transupp.h
)
SET( LIBJPEG_SOURCE 
    jaricom.c jcapimin.c jcapistd.c jcarith.c jccoefct.c jccolor.c 
    jcdctmgr.c jchuff.c jcinit.c jcmainct.c jcmarker.c jcmaster.c 
    jcomapi.c jcparam.c jcprepct.c jcsample.c jctrans.c jdapimin.c 
    jdapistd.c jdarith.c jdatadst.c jdatasrc.c jdcoefct.c jdcolor.c 
    jddctmgr.c jdhuff.c jdinput.c jdmainct.c jdmarker.c jdmaster.c 
    jdmerge.c jdpostct.c jdsample.c jdtrans.c jerror.c jfdctflt.c 
    jfdctfst.c jfdctint.c jidctflt.c jidctfst.c jidctint.c jquant1.c 
    jquant2.c jutils.c jmemmgr.c
    
    # jmemansi.c jmemname.c jmemnobs.c jmemdos.c jmemmac.c
    jmemnobs.c
)

IF(MSVC)
    SET(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")
    SET(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")
    CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/jconfig.vc ${PROJECT_SOURCE_DIR}/jconfig.h COPYONLY)
ELSEIF(MSYS AND MINGW)
    SET(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")
    SET(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")
    CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/jconfig.vc ${PROJECT_SOURCE_DIR}/jconfig.h COPYONLY)
ELSEIF(ANDROID) #android
    CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/jconfig.vc ${PROJECT_SOURCE_DIR}/jconfig.h COPYONLY)
ELSE(MSVC)
    CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/jconfig.cfg ${PROJECT_SOURCE_DIR}/jconfig.h COPYONLY)
ENDIF(MSVC)

if( JPEG_BUILD_STATIC )
    add_definitions(-DJPEG_LIBRARY_STATIC)
    ADD_LIBRARY(${LIBRARY_NAME} STATIC ${LIBJPEG_HEADERS} ${LIBJPEG_SOURCE})
ELSE( JPEG_BUILD_STATIC )
    add_definitions(-DJPEG_LIBRARY)
    ADD_LIBRARY(${LIBRARY_NAME} SHARED ${LIBJPEG_HEADERS} ${LIBJPEG_SOURCE})
ENDIF( JPEG_BUILD_STATIC )

INSTALL(TARGETS ${LIBRARY_NAME} 
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        )
INSTALL(
        FILES       ${LIBJPEG_HEADERS}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include
        )

        