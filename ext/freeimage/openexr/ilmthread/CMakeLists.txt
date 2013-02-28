
ivw_project(ilmthread)

#--------------------------------------------------------------------
# Supress warnings
if(WIN32)
add_definitions( "/W0 /D_CRT_SECURE_NO_WARNINGS /wd4005 /wd4996 /nologo /IGNORE:4006" )
endif()

#--------------------------------------------------------------------
# Add header files

file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

ivw_group("Iex Header Files" ${HEADER_FILES} )

#--------------------------------------------------------------------
# Add source files

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

ivw_group("Iex Source Files" ${SOURCE_FILES} )

#--------------------------------------------------------------------
# Define defintions
ivw_define_standard_definitions(ilmthread)

#--------------------------------------------------------------------
# Create library
add_library(ilmthread ${SOURCE_FILES} ${HEADER_FILES})

#--------------------------------------------------------------------
# Creates VS folder structure
ivw_vs_folder(ilmthread ext/freeimagelib/openexrlib)

ivw_add_dependencies(Iex)
ivw_include_directories(${IVW_EXTENSIONS_DIR}/freeimage/openexr/)
ivw_include_directories(${IVW_EXTENSIONS_DIR}/freeimage/openexr/iex)

#--------------------------------------------------------------------
# Make package (for other projects to find)
ivw_make_package(IlmThread ilmthread)