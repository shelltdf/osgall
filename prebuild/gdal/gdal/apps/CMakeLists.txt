# ******************************************************************************
# * Project:  CMake4GDAL
# * Purpose:  CMake build scripts
# * Author: Dmitriy Baryshnikov (aka Bishop), polimax@mail.ru
# ******************************************************************************
# * Copyright (C) 2012 Bishop
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

include_directories(${GDAL_ROOT_SOURCE_DIR}/gcore/)
include_directories(${GDAL_ROOT_SOURCE_DIR}/alg/)
include_directories(${GDAL_ROOT_SOURCE_DIR}/ogr/)
include_directories(${GDAL_ROOT_SOURCE_DIR}/ogr/ogrsf_frmts/geojson/)
include_directories(${GDAL_ROOT_SOURCE_DIR}/ogr/ogrsf_frmts/generic/)
include_directories(${GDAL_ROOT_SOURCE_DIR}/frmts/vrt/)

include_directories(${PROJ_INCLUDE_DIR})
include_directories(${GDAL_ROOT_SOURCE_DIR}/gnm/)

set(LIB_NAME "applib")
set(LIB_HHEADERS

)
set(LIB_CSOURCES
    commonutils.cpp
    gdalinfo_lib.cpp
    gdal_translate_lib.cpp 
    gdalwarp_lib.cpp
    ogr2ogr_lib.cpp
    gdaldem_lib.cpp
    nearblack_lib.cpp
    gdal_grid_lib.cpp 
    gdal_rasterize_lib.cpp
    gdalbuildvrt_lib.cpp
    gdalmdiminfo_lib.cpp
    gdalmdimtranslate_lib.cpp
)
set(GDAL_TARGET_LINK_LIB ${GDAL_TARGET_LINK_LIB} PARENT_SCOPE)
set(GDAL_TARGET_OBJECTS ${GDAL_TARGET_OBJECTS} $<TARGET_OBJECTS:${LIB_NAME}> PARENT_SCOPE)
add_library(${LIB_NAME} OBJECT ${LIB_HHEADERS} ${LIB_CSOURCES})
install(FILES ${LIB_HHEADERS} DESTINATION include/gdal)




option(GDAL_BUILD_APPS "Set ON to build apps" OFF)
if(GDAL_BUILD_APPS)

	include(app)
    
#gdalinfo
	set(GDALINFO_HHEADERS
	)
	set(GDALINFO_CSOURCES
		gdalinfo_bin.cpp
	)
	apps_macro("gdalinfo" "${GDALINFO_CSOURCES}" "${GDALINFO_HHEADERS}")
    
#gdalmdiminfo
	set(GDALINFO_HHEADERS
	)
	set(GDALINFO_CSOURCES
		gdalmdiminfo_bin.cpp
	)
	apps_macro("gdalmdiminfo" "${GDALINFO_CSOURCES}" "${GDALINFO_HHEADERS}")   
    
#gdalmdimtranslate
	set(GDALINFO_HHEADERS
	)
	set(GDALINFO_CSOURCES
		gdalmdimtranslate_bin.cpp
	)
	apps_macro("gdalmdimtranslate" "${GDALINFO_CSOURCES}" "${GDALINFO_HHEADERS}") 

#gdalserver
	set(GDALINFO_HHEADERS
	)
	set(GDALINFO_CSOURCES
		gdalserver.cpp
	)
	apps_macro("gdalserver" "${GDALINFO_CSOURCES}" "${GDALINFO_HHEADERS}") 

#gdalwarpsimple
	set(GDALINFO_HHEADERS
	)
	set(GDALINFO_CSOURCES
		gdalwarpsimple.c
	)
	apps_macro("gdalwarpsimple" "${GDALINFO_CSOURCES}" "${GDALINFO_HHEADERS}") 
    
#gdaltransform
	set(GDALINFO_HHEADERS
	)
	set(GDALINFO_CSOURCES
		gdaltransform.cpp
	)
	apps_macro("gdaltransform" "${GDALINFO_CSOURCES}" "${GDALINFO_HHEADERS}") 

#gdalsrsinfo
	set(GDALINFO_HHEADERS
	)
	set(GDALINFO_CSOURCES
		gdalsrsinfo.cpp
	)
	apps_macro("gdalsrsinfo" "${GDALINFO_CSOURCES}" "${GDALINFO_HHEADERS}") 
   
#nearblack
	set(GDALINFO_HHEADERS
	)
	set(GDALINFO_CSOURCES
		nearblack_bin.cpp
	)
	apps_macro("nearblack" "${GDALINFO_CSOURCES}" "${GDALINFO_HHEADERS}") 
   
#gdalenhance
	set(GDALINFO_HHEADERS
	)
	set(GDALINFO_CSOURCES
		gdalenhance.cpp
	)
	apps_macro("gdalenhance" "${GDALINFO_CSOURCES}" "${GDALINFO_HHEADERS}") 
   
#gdaldem
	set(GDALINFO_HHEADERS
	)
	set(GDALINFO_CSOURCES
		gdaldem_bin.cpp
	)
	apps_macro("gdaldem" "${GDALINFO_CSOURCES}" "${GDALINFO_HHEADERS}") 
   
#gdalmanage
	set(GDALINFO_HHEADERS
	)
	set(GDALINFO_CSOURCES
		gdalmanage.cpp
	)
	apps_macro("gdalmanage" "${GDALINFO_CSOURCES}" "${GDALINFO_HHEADERS}") 
   
#gdaltindex
	set(GDALINFO_HHEADERS
	)
	set(GDALINFO_CSOURCES
		gdaltindex.cpp
	)
	apps_macro("gdaltindex" "${GDALINFO_CSOURCES}" "${GDALINFO_HHEADERS}") 
      
#gdal_rasterize
	set(GDALINFO_HHEADERS
	)
	set(GDALINFO_CSOURCES
		gdal_rasterize_bin.cpp
	)
	apps_macro("gdal_rasterize" "${GDALINFO_CSOURCES}" "${GDALINFO_HHEADERS}") 
         
#multireadtest
	set(GDALINFO_HHEADERS
	)
	set(GDALINFO_CSOURCES
		gdalasyncread.cpp
	)
	apps_macro("multireadtest" "${GDALINFO_CSOURCES}" "${GDALINFO_HHEADERS}") 

#gdalasyncread
	set(GDALINFO_HHEADERS
	)
	set(GDALINFO_CSOURCES
		gdalasyncread.cpp
	)
	apps_macro("gdalasyncread" "${GDALINFO_CSOURCES}" "${GDALINFO_HHEADERS}")   
   
#testreprojmulti
	set(GDALINFO_HHEADERS
	)
	set(GDALINFO_CSOURCES
		testreprojmulti.cpp
	)
	# apps_macro("testreprojmulti" "${GDALINFO_CSOURCES}" "${GDALINFO_HHEADERS}")   
       
#ogrtindex
	set(GDALINFO_HHEADERS
	)
	set(GDALINFO_CSOURCES
		ogrtindex.cpp
	)
	apps_macro("ogrtindex" "${GDALINFO_CSOURCES}" "${GDALINFO_HHEADERS}")   
          
#ogrlineref
	set(GDALINFO_HHEADERS
	)
	set(GDALINFO_CSOURCES
		ogrlineref.cpp
	)
	apps_macro("ogrlineref" "${GDALINFO_CSOURCES}" "${GDALINFO_HHEADERS}")   
              
#testepsg
	set(GDALINFO_HHEADERS
	)
	set(GDALINFO_CSOURCES
		testepsg.cpp
	)
	apps_macro("testepsg" "${GDALINFO_CSOURCES}" "${GDALINFO_HHEADERS}")   

#dumpoverviews
	set(GDALINFO_HHEADERS
	)
	set(GDALINFO_CSOURCES
		dumpoverviews.cpp
	)
	apps_macro("dumpoverviews" "${GDALINFO_CSOURCES}" "${GDALINFO_HHEADERS}")   
    
#gdalflattenmask
	set(GDALINFO_HHEADERS
	)
	set(GDALINFO_CSOURCES
		gdalflattenmask.c
	)
	apps_macro("gdalflattenmask" "${GDALINFO_CSOURCES}" "${GDALINFO_HHEADERS}")   
        
#gdaltorture
	set(GDALINFO_HHEADERS
	)
	set(GDALINFO_CSOURCES
		gdaltorture.cpp
	)
	apps_macro("gdaltorture" "${GDALINFO_CSOURCES}" "${GDALINFO_HHEADERS}")   
           
#gdal2ogr
	set(GDALINFO_HHEADERS
	)
	set(GDALINFO_CSOURCES
		gdal2ogr.c
	)
	apps_macro("gdal2ogr" "${GDALINFO_CSOURCES}" "${GDALINFO_HHEADERS}")   

#gnmmanage
	set(GDALINFO_HHEADERS
	)
	set(GDALINFO_CSOURCES
		gnmmanage.cpp
	)
	apps_macro("gnmmanage" "${GDALINFO_CSOURCES}" "${GDALINFO_HHEADERS}")   

#gnmanalyse
	set(GDALINFO_HHEADERS
	)
	set(GDALINFO_CSOURCES
		gnmanalyse.cpp
	)
	apps_macro("gnmanalyse" "${GDALINFO_CSOURCES}" "${GDALINFO_HHEADERS}")   


#gdalbuildvrt    
	set(gdalbuildvrt_HHEADERS
	)

	set(gdalbuildvrt_CSOURCES
		gdalbuildvrt_bin.cpp
	)

	apps_macro("gdalbuildvrt" "${gdalbuildvrt_CSOURCES}" "${gdalbuildvrt_HHEADERS}")	
	
#gdaladdo    
	set(GDALADDO_HHEADERS
	)

	set(GDALADDO_CSOURCES
		gdaladdo.cpp
	)

	apps_macro("gdaladdo" "${GDALADDO_CSOURCES}" "${GDALADDO_HHEADERS}")	
    
#gdal_grid   
    set(gdal_grid_HHEADERS
    )

    set(gdal_grid_CSOURCES
        gdal_grid_bin.cpp
    )

apps_macro("gdal_grid" "${gdal_grid_CSOURCES}" "${gdal_grid_HHEADERS}")
    
#gdal_translate    
	include_directories(${GDAL_ROOT_SOURCE_DIR}/frmts/)
	set(gdal_translate_HHEADERS
	)

	set(gdal_translate_CSOURCES
		gdal_translate_bin.cpp
	)

	apps_macro("gdal_translate" "${gdal_translate_CSOURCES}" "${gdal_translate_HHEADERS}")    
    
#gdalwarp
	set(gdalwarp_HHEADERS  
	)

	set(gdalwarp_CSOURCES
		gdalwarp_bin.cpp        
	)

	apps_macro("gdalwarp" "${gdalwarp_CSOURCES}" "${gdalwarp_HHEADERS}")  
    
#gdal_contour
	set(gdal_contour_HHEADERS
	)

	set(gdal_contour_CSOURCES
		gdal_contour.cpp
	)

	apps_macro("gdal_contour" "${gdal_contour_CSOURCES}" "${gdal_contour_HHEADERS}") 
    
#gdallocationinfo   
	include_directories(${GDAL_ROOT_SOURCE_DIR}/ogr/ogrsf_frmts/)

	set(gdallocationinfo_HHEADERS
	)

	set(gdallocationinfo_CSOURCES
		gdallocationinfo.cpp
	)
	apps_macro("gdallocationinfo" "${gdallocationinfo_CSOURCES}" "${gdallocationinfo_HHEADERS}")
	
#ogrinfo    
	set(ogrinfo_HHEADERS
	)

	set(ogrinfo_CSOURCES
		ogrinfo.cpp
	)
	apps_macro("ogrinfo" "${ogrinfo_CSOURCES}" "${ogrinfo_HHEADERS}")
    
#ogr2ogr    
	set(ogr2ogr_HHEADERS
	)

	set(ogr2ogr_CSOURCES
		ogr2ogr_bin.cpp
	)
	apps_macro("ogr2ogr" "${ogr2ogr_CSOURCES}" "${ogr2ogr_HHEADERS}")    

endif(GDAL_BUILD_APPS)

option(GDAL_BUILD_TESTAPPS "Set ON to build test apps" OFF)
if(GDAL_BUILD_TESTAPPS)

	include(app)
    
#test_ogrsf.cpp
	set(TEST_OGRSF_HHEADERS
	)

	set(TEST_OGRSF_CSOURCES
		test_ogrsf.cpp
	)

	apps_macro("test_ogrsf" "${TEST_OGRSF_CSOURCES}" "${TEST_OGRSF_HHEADERS}")

endif(GDAL_BUILD_TESTAPPS)
