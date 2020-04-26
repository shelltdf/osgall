
SET ( ILMIMFUTIL_SRCS
  ImfImageChannel.cpp
  ImfFlatImageChannel.cpp
  ImfDeepImageChannel.cpp
  ImfSampleCountChannel.cpp
  ImfImageLevel.cpp
  ImfFlatImageLevel.cpp
  ImfDeepImageLevel.cpp
  ImfImage.cpp
  ImfFlatImage.cpp
  ImfDeepImage.cpp
  ImfImageIO.cpp
  ImfFlatImageIO.cpp
  ImfDeepImageIO.cpp
  ImfImageDataWindow.cpp
)

IF(BUILD_SHARED_LIBS)
    ADD_DEFINITIONS(-DILMIMF_EXPORTS)
ENDIF()

LINK_DIRECTORIES ( ${CMAKE_CURRENT_BINARY_DIR}/../IlmImf )

ADD_LIBRARY ( IlmImfUtil ${LIB_TYPE}
  ${ILMIMFUTIL_SRCS}
)

TARGET_LINK_LIBRARIES ( IlmImfUtil 
  optimized Half                            debug Halfd
  optimized Iex${ILMBASE_LIBSUFFIX}         debug Iex${ILMBASE_LIBSUFFIX}d
  optimized IlmThread${ILMBASE_LIBSUFFIX}   debug IlmThread${ILMBASE_LIBSUFFIX}d
  optimized Imath${ILMBASE_LIBSUFFIX}       debug Imath${ILMBASE_LIBSUFFIX}d
  IlmImf
  ${PTHREAD_LIB} ${ZLIB_LIBRARIES}
)


# Libraries

# INSTALL ( TARGETS
  # IlmImfUtil
  # DESTINATION
  # ${CMAKE_INSTALL_PREFIX}/lib
# )

IF(WIN32)
INSTALL ( TARGETS IlmImfUtil
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION bin
    ARCHIVE DESTINATION lib
)
ELSE(WIN32)
INSTALL ( TARGETS IlmImfUtil
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
ENDIF(WIN32)


# Headers
INSTALL ( FILES
  ImfImageChannel.h
  ImfFlatImageChannel.h
  ImfDeepImageChannel.h
  ImfSampleCountChannel.h
  ImfImageLevel.h
  ImfFlatImageLevel.h
  ImfDeepImageLevel.h
  ImfImage.h
  ImfFlatImage.h
  ImfDeepImage.h
  ImfImageIO.h
  ImfFlatImageIO.h
  ImfDeepImageIO.h
  ImfImageDataWindow.h
  ImfImageChannelRenaming.h
  DESTINATION
  ${CMAKE_INSTALL_PREFIX}/include/OpenEXR
  )

