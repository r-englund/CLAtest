
ivw_project(libjpeg)

#--------------------------------------------------------------------
# Supress warnings
add_definitions( "/W0 /D_CRT_SECURE_NO_WARNINGS /wd4005 /wd4996 /nologo /IGNORE:4006" )

#--------------------------------------------------------------------
# Add header files

file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

ivw_group("Header Files" ${HEADER_FILES} )

#--------------------------------------------------------------------
# Add source files

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

list(REMOVE_ITEM SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/jmemmac.c)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/jmemdos.c)


ivw_group("Source Files" ${SOURCE_FILES} )

#--------------------------------------------------------------------
# Define defintions
ivw_define_standard_definitions(libjpeg)

#--------------------------------------------------------------------
# Create library
add_library(libjpeg ${SOURCE_FILES} ${HEADER_FILES})

# ivw_add_dependencies(ZLib)

#--------------------------------------------------------------------
# Creates VS folder structure
ivw_vs_folder(libjpeg ext/freeimagelib)

#--------------------------------------------------------------------
# Make package (for other projects to find)
ivw_make_package(LibJPEG libjpeg)