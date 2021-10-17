
# IlmImfUtilTest

LINK_DIRECTORIES ( ${CMAKE_CURRENT_BINARY_DIR}/../IlmImf )


ADD_EXECUTABLE ( IlmImfUtilTest
  main.cpp
  testFlatImage.cpp
  testDeepImage.cpp
  testIO.cpp
 )


ADD_TEST ( TestIlmImfUtil IlmImfUtilTest )
TARGET_LINK_LIBRARIES ( IlmImfUtilTest 
        IlmImfUtil
        IlmImf  
        optimized Half${ILMBASE_LIBSUFFIX}                            debug Half${ILMBASE_LIBSUFFIX}d
        optimized Iex${ILMBASE_LIBSUFFIX}         debug Iex${ILMBASE_LIBSUFFIX}d
        optimized IlmThread${ILMBASE_LIBSUFFIX}   debug IlmThread${ILMBASE_LIBSUFFIX}d
        optimized Imath${ILMBASE_LIBSUFFIX}       debug Imath${ILMBASE_LIBSUFFIX}d
        ${PTHREAD_LIB} ${ZLIB_LIBRARIES}
        )

