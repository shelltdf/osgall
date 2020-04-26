
cmake_minimum_required(VERSION 2.8)

PROJECT(libxml2)
SET(LIBRARY_NAME xml2)

SET(FOO_H)
FILE(GLOB FOO_H include/libxml/*.h)

SET( HEADER_FILES
    # acconfig.h 
    # elfgcchack.h 
    libxml.h 
    # <ClInclude Include="config.h 
    # ${FOO_H}
    
    include/libxml/c14n.h
    include/libxml/chvalid.h
    include/libxml/debugXML.h
    include/libxml/dict.h
    include/libxml/DOCBparser.h
    include/libxml/encoding.h
    include/libxml/entities.h
    include/libxml/globals.h
    include/libxml/hash.h
    include/libxml/HTMLparser.h
    include/libxml/HTMLtree.h
    include/libxml/list.h
    include/libxml/nanoftp.h
    include/libxml/nanohttp.h
    include/libxml/parser.h
    include/libxml/parserInternals.h
    include/libxml/pattern.h
    include/libxml/relaxng.h
    include/libxml/SAX.h
    include/libxml/SAX2.h
    include/libxml/schemasInternals.h
    include/libxml/schematron.h
    include/libxml/threads.h
    include/libxml/tree.h
    include/libxml/uri.h
    include/libxml/valid.h
    include/libxml/xinclude.h
    include/libxml/xlink.h
    include/libxml/xmlautomata.h
    include/libxml/xmlerror.h
    include/libxml/xmlexports.h
    include/libxml/xmlIO.h
    include/libxml/xmlmemory.h
    include/libxml/xmlmodule.h
    include/libxml/xmlreader.h
    include/libxml/xmlregexp.h
    include/libxml/xmlsave.h
    include/libxml/xmlschemas.h
    include/libxml/xmlschemastypes.h
    include/libxml/xmlstring.h
    include/libxml/xmlunicode.h
    include/libxml/xmlversion.h
    # include/libxml/xmlwin32version.h
    include/libxml/xmlwriter.h
    include/libxml/xpath.h
    include/libxml/xpathInternals.h
    include/libxml/xpointer.h
)
SET( SOURCE_FILES 
    buf.c 
    c14n.c 
    catalog.c 
    chvalid.c 
    debugXML.c 
    dict.c 
    DOCBparser.c 
    encoding.c 
    entities.c 
    error.c 
    globals.c 
    hash.c 
    HTMLparser.c 
    HTMLtree.c 
    legacy.c 
    list.c 
    # nanoftp.c 
    # nanohttp.c 
    parser.c 
    parserInternals.c 
    pattern.c 
    relaxng.c 
    SAX.c 
    SAX2.c 
    schematron.c 
    threads.c 
    tree.c 
    uri.c 
    valid.c 
    xinclude.c 
    xlink.c 
    xmlcatalog.c 
    xmlIO.c 
    xmlmemory.c 
    xmlmodule.c 
    xmlreader.c 
    xmlregexp.c 
    xmlsave.c 
    xmlschemas.c 
    xmlschemastypes.c 
    xmlstring.c 
    xmlunicode.c 
    xmlwriter.c 
    xpath.c 
    xpointer.c 
)

include_directories(AFTER ${CMAKE_SOURCE_DIR}/include/ )

IF(MSVC)
    SET(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")
    SET(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")
    CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/include/libxml/xmlwin32version.h.in ${PROJECT_SOURCE_DIR}/include/libxml/xmlversion.h COPYONLY)
    include_directories(AFTER ${CMAKE_SOURCE_DIR}/win32/VC10/ )
ELSEIF(MSYS AND MINGW)
    SET(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")
    SET(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")
    CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/include/libxml/xmlwin32version.h.in ${PROJECT_SOURCE_DIR}/include/libxml/xmlversion.h COPYONLY)
    include_directories(AFTER ${CMAKE_SOURCE_DIR}/win32/VC10/ )
ElSE(MSVC) #android
    # CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/jconfig.cfg ${PROJECT_SOURCE_DIR}/jconfig.h COPYONLY)
    CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/include/libxml/xmlwin32version.h.in ${PROJECT_SOURCE_DIR}/include/libxml/xmlversion.h COPYONLY)
    include_directories(AFTER ${CMAKE_SOURCE_DIR}/ndk/ )
ENDIF(MSVC)


# ADD_LIBRARY(${LIBRARY_NAME} ${HEADER_FILES} ${SOURCE_FILES})

OPTION(LIBXML_BUILD_STATIC "Enable build static library" ON)

if( LIBXML_BUILD_STATIC )
    add_definitions(-DLIBXML_STATIC)
    ADD_LIBRARY(${LIBRARY_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})
ELSE( LIBXML_BUILD_STATIC )
    add_definitions(-DLIBXML_LIBRARY)
    # add_definitions(-DLIBXML_PUSH_ENABLED)
    # add_definitions(-DLIBXML_SAX1_ENABLED)
    ADD_LIBRARY(${LIBRARY_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})
ENDIF( LIBXML_BUILD_STATIC )



INSTALL(TARGETS ${LIBRARY_NAME} 
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        )
INSTALL(
        FILES       ${HEADER_FILES}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include
        )
INSTALL(
        FILES       ${FOO_H}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/libxml
        )
        