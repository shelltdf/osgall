###############################################################################
#
# OSGeo4W packaging
#
###############################################################################


set(OSGEO4W_DIR osgeo4w)
set(OSGEO4W_LIB_DIR ${OSGEO4W_DIR}/lib)
set(OSGEO4W_BIN_DIR ${OSGEO4W_DIR}/bin)
set(OSGEO4W_INCLUDE_DIR ${OSGEO4W_DIR}/include/laszip)

set(OSGEO4W_PACKAGES ${OSGEO4W_DIR}/packages)


set(OSGEO4W_DIRECTORIES
    ${OSGEO4W_DIR}
    ${OSGEO4W_LIB_DIR}
    ${OSGEO4W_INCLUDE_DIR}
    ${OSGEO4W_PACKAGES}
    ${OSGEO4W_BIN_DIR})



add_custom_target(make_osgeo4w_directories
  COMMAND ${CMAKE_COMMAND} -E echo "Building OSGeo4W install directories")

add_dependencies(  make_osgeo4w_directories  ${LASZIP_LIB_NAME} )


macro (make_directories)
    add_custom_command(
        TARGET make_osgeo4w_directories
        COMMAND ${CMAKE_COMMAND} -E  remove_directory  ${laszip_SOURCE_DIR}/osgeo4w DEPENDS osgeo4w
    )
    foreach(directory ${OSGEO4W_DIRECTORIES})

    STRING(REGEX REPLACE "/" "_" target "${directory}" )

    add_custom_command(
        TARGET make_osgeo4w_directories
        COMMAND ${CMAKE_COMMAND} -E make_directory ${directory}
    )

    endforeach()

endmacro(make_directories)



add_custom_target(copy ALL COMMENT "Copying OSGeo4W files")
add_dependencies( copy make_osgeo4w_directories  )


macro(copy_files GLOBPAT DESTINATION  )

    file(GLOB_RECURSE COPY_FILES
         RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
        ${GLOBPAT})
    foreach(FILENAME ${COPY_FILES})
        set(SRC "${FILENAME}")
        set(DST "${DESTINATION}")

        add_custom_command(
            TARGET copy
            COMMAND ${CMAKE_COMMAND} -E copy ${SRC} ${DST}
        )
    endforeach(FILENAME)
endmacro(copy_files)


macro(copy_directory SOURCE DESTINATION  )
        add_custom_command(
            TARGET copy
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${SOURCE} ${DESTINATION}
        )
endmacro(copy_directory)


add_custom_target(tar
  COMMAND ${CMAKE_COMMAND} -E echo "Tarring OSGeo4W install")
add_dependencies( tar copy  )

macro (tar_directories source destination base_paths)    
    add_custom_command(
        TARGET tar
        COMMAND ${CMAKE_COMMAND} -E chdir ${source} cmake -E tar cjf  ${destination} ${base_paths}
    )
endmacro(tar_directories)

make_directories()
copy_files(${LASZIP_BUILD_OUTPUT_DIRECTORY}/laszip.dll ${OSGEO4W_BIN_DIR}/  )
copy_files(${LASZIP_BUILD_OUTPUT_DIRECTORY}/laszip.lib ${OSGEO4W_LIB_DIR}/  )
copy_files(./include/laszip/*.hpp ${OSGEO4W_INCLUDE_DIR}/  )

tar_directories(${OSGEO4W_DIR} ${laszip_SOURCE_DIR}/${OSGEO4W_PACKAGES}/laszip-${VERSION}-${OSGEO4W_UPSTREAM_RELEASE}.tar.bz2 "bin/;lib/;include/")


add_custom_target(osgeo4w
    COMMAND ${CMAKE_COMMAND} -E echo "Making OSGeo4W build")

add_dependencies( osgeo4w tar   )




