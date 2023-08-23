
cmake_minimum_required(VERSION 2.8)

PROJECT(bzip2)
SET(LIBRARY_NAME bzip2)

OPTION(BZIP2_BUILD_STATIC "Enable build static library" OFF)

SET( LIBJPEG_HEADERS
    bzlib.h
)
SET( LIBJPEG_SOURCE 
    blocksort.c
	# bzip2.c
	# bzip2recover.c
	bzlib.c
	compress.c
	crctable.c
	decompress.c
	# dlltest.c
	huffman.c
	mk251.c
	randtable.c
	# spewG.c
	# unzcrash.c
	libbz2.def
)


if( BZIP2_BUILD_STATIC )
    add_definitions(-DBZ_STATIC)
    ADD_LIBRARY(${LIBRARY_NAME} STATIC ${LIBJPEG_HEADERS} ${LIBJPEG_SOURCE})
ELSE( BZIP2_BUILD_STATIC )
    add_definitions(-DBZ_EXPORT)
    ADD_LIBRARY(${LIBRARY_NAME} SHARED ${LIBJPEG_HEADERS} ${LIBJPEG_SOURCE})
ENDIF( BZIP2_BUILD_STATIC )


SET(INSTALL_INCDIR include)
SET(INSTALL_BINDIR bin)
IF(WIN32)
    SET(INSTALL_LIBDIR bin)
    SET(INSTALL_ARCHIVEDIR lib)
ELSE(WIN32)
    SET(INSTALL_LIBDIR lib)
    SET(INSTALL_ARCHIVEDIR lib)
ENDIF(WIN32)

INSTALL(TARGETS ${LIBRARY_NAME} 
        RUNTIME DESTINATION ${INSTALL_BINDIR}
		LIBRARY DESTINATION ${INSTALL_LIBDIR}
		ARCHIVE DESTINATION ${INSTALL_ARCHIVEDIR}
        )
INSTALL(
        FILES       ${LIBJPEG_HEADERS}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include
        )

        