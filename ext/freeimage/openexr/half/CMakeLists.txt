
ivw_project(half)

#--------------------------------------------------------------------
# Supress warnings
if(WIN32)
add_definitions( "/W0 /D_CRT_SECURE_NO_WARNINGS /wd4005 /wd4996 /nologo /IGNORE:4006" )
endif()

#--------------------------------------------------------------------
# Add header files

file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

ivw_group("Half Header Files" ${HEADER_FILES} )

#--------------------------------------------------------------------
# Add source files

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

ivw_group("Half Source Files" ${SOURCE_FILES} )

#--------------------------------------------------------------------
# Define defintions
ivw_define_standard_definitions(half)

#--------------------------------------------------------------------
# Create library
add_library(half ${SOURCE_FILES} ${HEADER_FILES})

#--------------------------------------------------------------------
# Creates VS folder structure
ivw_vs_folder(half ext/freeimagelib/openexrlib)

#--------------------------------------------------------------------
# Make package (for other projects to find)
ivw_make_package(Half half)