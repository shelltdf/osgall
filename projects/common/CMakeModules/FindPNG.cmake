# Locate SQLITE3
# This module defines
# SQLITE3_LIBRARY
# SQLITE3_FOUND, if false, do not try to link to expat
# SQLITE3_INCLUDE_DIR, where to find the headers

FIND_PATH(PNG_INCLUDE_DIR png.h
    ${PNG_DIR}/include/sqlite
    $ENV{PNG_DIR}/include/sqlite
    $ENV{PNG_DIR}/Source/lib/sqlite #Windows Binary Installer
    $ENV{PNG_DIR}/sqlite
)

FIND_LIBRARY(PNG_LIBRARY
    NAMES libpng16_static
    PATHS
    ${PNG_DIR}/lib
    $ENV{PNG_DIR}/lib
    $ENV{PNG_DIR}/bin #Windows Binary Installer
    $ENV{PNG_DIR}
)

FIND_LIBRARY(PNG_LIBRARY_DEBUG
    NAMES libpng16_staticd
    PATHS
    ${PNG_DIR}/lib
    $ENV{PNG_DIR}/lib
    $ENV{PNG_DIR}/bin #Windows Binary Installer
    $ENV{PNG_DIR}
)


SET(PNG_FOUND "NO")
IF(PNG_INCLUDE_DIR)
    SET(PNG_FOUND "YES")
ENDIF(PNG_INCLUDE_DIR)


