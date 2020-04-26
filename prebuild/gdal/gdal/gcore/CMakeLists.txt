# ******************************************************************************
# * Project:  CMake4GDAL
# * Purpose:  CMake build scripts
# * Author: Dmitriy Baryshnikov (aka Bishop), polimax@mail.ru
# ******************************************************************************
# * Copyright (C) 2012,2013 Bishop
# * 
# * Permission is hereby granted, free of charge, to any person obtaining a
# * copy of this software and associated documentation files (the "Software"),
# * to deal in the Software without restriction, including without limitation
# * the rights to use, copy, modify, merge, publish, distribute, sublicense,
# * and/or sell copies of the Software, and to permit persons to whom the
# * Software is furnished to do so, subject to the following conditions:
# *
# * The above copyright notice and this permission notice shall be included
# * in all copies or substantial portions of the Software.
# *
# * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
# * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# * DEALINGS IN THE SOFTWARE.
# ******************************************************************************

cmake_minimum_required (VERSION 2.8.10)

set(LIB_NAME "gcore")
project ("lib${LIB_NAME}")

if(GDAL_USE_CPL)
	add_definitions(-DUSE_CPL)
endif(GDAL_USE_CPL)

option(GDAL_PAM_ENABLED "Set ON to enable pam" ON)
if(GDAL_PAM_ENABLED)
	add_definitions(-DPAM_ENABLED)
endif(GDAL_PAM_ENABLED)

include_directories(${GDAL_ROOT_SOURCE_DIR}/gcore)
include_directories(${GDAL_ROOT_SOURCE_DIR}/ogr)
include_directories(${GDAL_ROOT_SOURCE_DIR}/ogr/ogrsf_frmts)
include_directories(${GDAL_ROOT_SOURCE_DIR}/ogr/ogrsf_frmts/geojson)
include_directories(${GDAL_ROOT_SOURCE_DIR}/ogr/ogrsf_frmts/generic)
include_directories(${GDAL_ROOT_SOURCE_DIR}/alg)
include_directories(${GDAL_ROOT_SOURCE_DIR}/frmts/gtiff)
include_directories(${GDAL_ROOT_SOURCE_DIR}/frmts/vrt)
include_directories(${GDAL_ROOT_SOURCE_DIR}/frmts/mem)

configure_file(${GDAL_ROOT_SOURCE_DIR}/gcore/gdal_version.h.in 
 ${GDAL_ROOT_SOURCE_DIR}/gcore/gdal_version.h @ONLY)

set(LIB_HHEADERS
    gdal_version.h
    
    gdal.h
    gdalexif.h
    gdalgeorefpamdataset.h
    gdaljp2abstractdataset.h
    gdaljp2metadata.h
    gdaljp2metadatagenerator.h
    gdalsse_priv.h
    gdal_def.h
    gdal_frmts.h
    gdal_mdreader.h
    # gdal_pam.h
    gdal_priv.h
    gdal_proxy.h
    gdal_rat.h
    rawdataset.h
)

set(LIB_CSOURCES
    gdalabstractbandblockcache.cpp
    gdalallvalidmaskband.cpp
    gdalarraybandblockcache.cpp
    # gdalclientserver.cpp
    gdalcolortable.cpp
    gdaldataset.cpp
    gdaldefaultasync.cpp
    gdaldefaultoverviews.cpp
    gdaldllmain.cpp
    gdaldriver.cpp
    gdaldrivermanager.cpp
    gdalexif.cpp
    gdalgeorefpamdataset.cpp
    gdalhashsetbandblockcache.cpp
    gdaljp2abstractdataset.cpp
    gdaljp2box.cpp
    gdaljp2metadata.cpp
    gdaljp2metadatagenerator.cpp
    gdaljp2structure.cpp
    gdalmajorobject.cpp
    gdalmultidim.cpp
    gdalmultidomainmetadata.cpp
    gdalnodatamaskband.cpp
    gdalnodatavaluesmaskband.cpp
    gdalopeninfo.cpp
    gdaloverviewdataset.cpp
    # gdalpamdataset.cpp
    # gdalpamproxydb.cpp
    # gdalpamrasterband.cpp
    gdalproxydataset.cpp
    gdalproxypool.cpp
    gdalrasterband.cpp
    gdalrasterblock.cpp
    gdalrescaledalphaband.cpp
    gdalvirtualmem.cpp
    gdal_mdreader.cpp
    gdal_misc.cpp
    gdal_rat.cpp
    overview.cpp
    rasterio.cpp
    rasterio_ssse3.cpp
    rawdataset.cpp
)

if(NOT NACL)
set(LIB_CSOURCES ${LIB_CSOURCES} gdalclientserver.cpp)
endif(NOT NACL)


if(GDAL_PAM_ENABLED)
	set(LIB_HHEADERS ${LIB_HHEADERS}
		gdal_pam.h
	)
	set(LIB_CSOURCES ${LIB_CSOURCES}
		gdalpamdataset.cpp
		gdalpamrasterband.cpp
		gdalpamproxydb.cpp
	)
endif(GDAL_PAM_ENABLED)

if(WIN32)
    set(GDAL_TARGET_LINK_LIB ${GDAL_TARGET_LINK_LIB} Ws2_32.lib PARENT_SCOPE)
else(WIN32)
    set(GDAL_TARGET_LINK_LIB ${GDAL_TARGET_LINK_LIB} PARENT_SCOPE)
endif(WIN32)

add_subdirectory(mdreader)

set(GDAL_TARGET_OBJECTS ${GDAL_TARGET_OBJECTS} $<TARGET_OBJECTS:${LIB_NAME}> PARENT_SCOPE)
add_library(${LIB_NAME} OBJECT ${LIB_HHEADERS} ${LIB_CSOURCES})

install(FILES ${LIB_HHEADERS} DESTINATION include/gdal)



