
FIND_PATH(geotiff_INCLUDE_DIR geotiff.h
    PATHS
    ${CMAKE_SOURCE_DIR}../install/include/
    $ENV{PATH}
    /usr/include/
    /usr/local/include/
)

FIND_LIBRARY(geotiff_LIBRARY 
			NAMES geotiff
			PATHS ${geotiff_INCLUDE_DIR}/../lib/)
            
FIND_LIBRARY(geotiff_LIBRARY_DEBUG
			NAMES geotiffd
			PATHS ${geotiff_INCLUDE_DIR}/../lib/)
            
FIND_LIBRARY(xtiff_LIBRARY 
			NAMES xtiff
			PATHS ${geotiff_INCLUDE_DIR}/../lib/)

FIND_LIBRARY(xtiff_LIBRARY_DEBUG 
			NAMES xtiffd
			PATHS ${geotiff_INCLUDE_DIR}/../lib/)

            
SET(geotiff_FOUND "NO")
IF(geotiff_INCLUDE_DIR AND geotiff_LIBRARY)
    SET(geotiff_FOUND "YES")
ENDIF(geotiff_INCLUDE_DIR AND geotiff_LIBRARY)
