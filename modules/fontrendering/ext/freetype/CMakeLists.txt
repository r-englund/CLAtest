cmake_minimum_required(VERSION 2.6)
project(freetype)
  
#--------------------------------------------------------------------
# Set source- and library name variables
if (NOT FREETYPE_SOURCE_NAME)
        set(FREETYPE_SOURCE_NAME freetype-2.4.12)
endif (NOT FREETYPE_SOURCE_NAME)
if (NOT FREETYPE_STATICLIB_OUTPUT_PATH)
        set(FREETYPE_STATICLIB_OUTPUT_PATH "." CACHE STRING
                "FT2 static lib output directory")
endif(NOT FREETYPE_STATICLIB_OUTPUT_PATH)
if (NOT FREETYPE_STATICLIB_OUTPUT_NAME)
        set(FREETYPE_STATICLIB_OUTPUT_NAME "freetype" CACHE STRING
                "FT2 static lib output name")
endif (NOT FREETYPE_STATICLIB_OUTPUT_NAME)
mark_as_advanced(FREETYPE_STATICLIB_OUTPUT_NAME)

#--------------------------------------------------------------------
# Set source directory variable
set(FREETYPE_SRC_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}")

#--------------------------------------------------------------------
# Create variables for include and library
set(FREETYPE_INCLUDE_DIRS "${FREETYPE_SRC_FOLDER}/include" CACHE STRING "FT2 include directory" FORCE)

set(FREETYPE_LIBRARIES freetype CACHE STRING "FT2 libraries" FORCE)
set(FREETYPE_LIBRARY freetype CACHE STRING "FREETYPE libraries" FORCE)
set(FREETYPE_src
        ${FREETYPE_SRC_FOLDER}/src/autofit/autofit.c
        ${FREETYPE_SRC_FOLDER}/src/bdf/bdf.c
        ${FREETYPE_SRC_FOLDER}/src/cff/cff.c
        ${FREETYPE_SRC_FOLDER}/src/base/ftbase.c
        ${FREETYPE_SRC_FOLDER}/src/base/ftbitmap.c
        ${FREETYPE_SRC_FOLDER}/src/cache/ftcache.c
        ${FREETYPE_SRC_FOLDER}/src/base/ftdebug.c
        ${FREETYPE_SRC_FOLDER}/src/base/ftfstype.c
        ${FREETYPE_SRC_FOLDER}/src/base/ftgasp.c
        ${FREETYPE_SRC_FOLDER}/src/base/ftglyph.c
        ${FREETYPE_SRC_FOLDER}/src/gzip/ftgzip.c
        ${FREETYPE_SRC_FOLDER}/src/base/ftinit.c
        ${FREETYPE_SRC_FOLDER}/src/lzw/ftlzw.c
        ${FREETYPE_SRC_FOLDER}/src/base/ftstroke.c
        ${FREETYPE_SRC_FOLDER}/src/base/ftsystem.c
        ${FREETYPE_SRC_FOLDER}/src/smooth/smooth.c
        ${FREETYPE_SRC_FOLDER}/src/base/ftbbox.c
        ${FREETYPE_SRC_FOLDER}/src/base/ftmm.c
        ${FREETYPE_SRC_FOLDER}/src/base/ftpfr.c
        ${FREETYPE_SRC_FOLDER}/src/base/ftsynth.c
        ${FREETYPE_SRC_FOLDER}/src/base/fttype1.c
        ${FREETYPE_SRC_FOLDER}/src/base/ftwinfnt.c
        ${FREETYPE_SRC_FOLDER}/src/pcf/pcf.c
        ${FREETYPE_SRC_FOLDER}/src/pfr/pfr.c
        ${FREETYPE_SRC_FOLDER}/src/psaux/psaux.c
        ${FREETYPE_SRC_FOLDER}/src/pshinter/pshinter.c
        ${FREETYPE_SRC_FOLDER}/src/psnames/psmodule.c
        ${FREETYPE_SRC_FOLDER}/src/raster/raster.c
        ${FREETYPE_SRC_FOLDER}/src/sfnt/sfnt.c
        ${FREETYPE_SRC_FOLDER}/src/truetype/truetype.c
        ${FREETYPE_SRC_FOLDER}/src/type1/type1.c
        ${FREETYPE_SRC_FOLDER}/src/cid/type1cid.c
        ${FREETYPE_SRC_FOLDER}/src/type42/type42.c
        ${FREETYPE_SRC_FOLDER}/src/winfonts/winfnt.c
)

SET(freetype_HDRS
    ${FREETYPE_SRC_FOLDER}/include/ft2build.h
    ${FREETYPE_SRC_FOLDER}/include/freetype/config/ftconfig.h
    ${FREETYPE_SRC_FOLDER}/include/freetype/config/ftheader.h
    ${FREETYPE_SRC_FOLDER}/include/freetype/config/ftmodule.h
    ${FREETYPE_SRC_FOLDER}/include/freetype/config/ftoption.h
    ${FREETYPE_SRC_FOLDER}/include/freetype/config/ftstdlib.h
)
  
#--------------------------------------------------------------------
# Add /include and subfolders as includes
include_directories(${FREETYPE_INCLUDE_DIRS})

add_library(freetype ${FREETYPE_src} ${freetype_HDRS})

set_target_properties(${FREETYPE_LIBRARIES} PROPERTIES
    COMPILE_DEFINITIONS "WIN32;_LIB;_CRT_SECURE_NO_WARNINGS;FT2_BUILD_LIBRARY")

#--------------------------------------------------------------------
# Creates VS folder structure
ivw_define_standard_definitions(freetype)
ivw_vs_folder(freetype ext)