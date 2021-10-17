# yue.nicholas@gmail.com

ADD_EXECUTABLE ( exrheader
  main.cpp
)

TARGET_LINK_LIBRARIES ( exrheader
  IlmImf
  optimized Half${ILMBASE_LIBSUFFIX}                            debug Half${ILMBASE_LIBSUFFIX}d
  optimized Iex${ILMBASE_LIBSUFFIX}         debug Iex${ILMBASE_LIBSUFFIX}d
  optimized IlmThread${ILMBASE_LIBSUFFIX}   debug IlmThread${ILMBASE_LIBSUFFIX}d
  ${PTHREAD_LIB}
  ${ZLIB_LIBRARIES}
)

INSTALL ( TARGETS
  exrheader
  DESTINATION
  ${CMAKE_INSTALL_PREFIX}/bin
)
