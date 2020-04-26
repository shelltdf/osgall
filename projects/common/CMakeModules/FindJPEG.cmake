# Locate SQLITE3
# This module defines
# SQLITE3_LIBRARY
# SQLITE3_FOUND, if false, do not try to link to expat
# SQLITE3_INCLUDE_DIR, where to find the headers

FIND_PATH(JPEG_INCLUDE_DIR jconfig.h
    ${JPEG_DIR}/include/sqlite
    $ENV{JPEG_DIR}/include/sqlite
    $ENV{JPEG_DIR}/Source/lib/sqlite #Windows Binary Installer
    $ENV{JPEG_DIR}/sqlite
)

FIND_LIBRARY(JPEG_LIBRARY
    NAMES jpeg
    PATHS
    ${ZLIB_DIR}/lib
    $ENV{JPEG_DIR}/lib
    $ENV{JPEG_DIR}/bin #Windows Binary Installer
    $ENV{JPEG_DIR}
)

FIND_LIBRARY(JPEG_LIBRARY_DEBUG
    NAMES jpegd
    PATHS
    ${ZLIB_DIR}/lib
    $ENV{JPEG_DIR}/lib
    $ENV{JPEG_DIR}/bin #Windows Binary Installer
    $ENV{JPEG_DIR}
)


SET(JPEG_FOUND "NO")
IF(JPEG_INCLUDE_DIR)
    SET(JPEG_FOUND "YES")
ENDIF(JPEG_INCLUDE_DIR)


