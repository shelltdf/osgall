# yue.nicholas@gmail.com

ADD_EXECUTABLE ( IlmImfExamples
  drawImage.cpp
  generalInterfaceExamples.cpp
  generalInterfaceTiledExamples.cpp
  lowLevelIoExamples.cpp
  main.cpp
  previewImageExamples.cpp
  rgbaInterfaceExamples.cpp
  rgbaInterfaceTiledExamples.cpp
 )


TARGET_LINK_LIBRARIES ( IlmImfExamples 
        IlmImf
        optimized Half                            debug Halfd
        optimized Iex${ILMBASE_LIBSUFFIX}         debug Iex${ILMBASE_LIBSUFFIX}d
        optimized IlmThread${ILMBASE_LIBSUFFIX}   debug IlmThread${ILMBASE_LIBSUFFIX}d
        optimized Imath${ILMBASE_LIBSUFFIX}       debug Imath${ILMBASE_LIBSUFFIX}d
        ${PTHREAD_LIB} ${ZLIB_LIBRARIES}
        )
  
