
FIND_PATH(GEOTIFF_INCLUDE_DIR geotiff.h
    PATHS
    ${CMAKE_SOURCE_DIR}../install/include/
    $ENV{PATH}
    /usr/include/
    /usr/local/include/
)

FIND_LIBRARY(GEOTIFF_LIBRARY 
			NAMES geotiff
			PATHS ${GEOTIFF_INCLUDE_DIR}/../lib/)
            
FIND_LIBRARY(GEOTIFF_LIBRARY_DEBUG
			NAMES geotiffd
			PATHS ${GEOTIFF_INCLUDE_DIR}/../lib/)
            
# FIND_LIBRARY(xtiff_LIBRARY 
			# NAMES xtiff
			# PATHS ${GEOTIFF_INCLUDE_DIR}/../lib/)

# FIND_LIBRARY(xtiff_LIBRARY_DEBUG 
			# NAMES xtiffd
			# PATHS ${GEOTIFF_INCLUDE_DIR}/../lib/)

            
SET(GEOTIFF_FOUND "NO")
IF(GEOTIFF_INCLUDE_DIR AND geotiff_LIBRARY)
    SET(GEOTIFF_FOUND "YES")
ENDIF(GEOTIFF_INCLUDE_DIR AND geotiff_LIBRARY)
