
SET(INSTALL_DLL_FILE "" )
IF(HAVE_JPEG)
SET(INSTALL_DLL_FILE ${INSTALL_DLL_FILE} "bin/jpeg62.dll" )
ENDIF()
IF(HAVE_TIFF)
SET(INSTALL_DLL_FILE ${INSTALL_DLL_FILE} "bin/libtiff3.dll" )
ENDIF()
IF(HAVE_PNG)
SET(INSTALL_DLL_FILE ${INSTALL_DLL_FILE} "bin/libpng13.dll" "bin/zlib1.dll" )
ENDIF()


INSTALL(PROGRAMS 
	# bin/jpeg62.dll
	# bin/libpng13.dll
	# bin/libtiff3.dll
	# bin/zlib1.dll
    ${INSTALL_DLL_FILE}
	DESTINATION bin)
