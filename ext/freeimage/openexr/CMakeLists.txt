add_subdirectory(half)
add_subdirectory(iex)
add_subdirectory(ilmimf)
add_subdirectory(ilmthread)
add_subdirectory(imath)


ivw_project(openexr)

#--------------------------------------------------------------------
# Supress warnings
add_definitions( "/W0 /D_CRT_SECURE_NO_WARNINGS /wd4005 /wd4996 /nologo /IGNORE:4006" )

#--------------------------------------------------------------------
# Add header files

# file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

 set(HEADER_FILES 	${CMAKE_CURRENT_SOURCE_DIR}/IlmBaseConfig.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OpenEXRConfig.h)

ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add source files
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

ivw_group("Source Files" ${SOURCE_FILES} )



#--------------------------------------------------------------------
# Define defintions
ivw_define_standard_definitions(openexr)

#--------------------------------------------------------------------
# Create library
add_library(openexr ${SOURCE_FILES} ${HEADER_FILES})

#--------------------------------------------------------------------
# Creates VS folder structure
ivw_vs_folder(openexr ext/freeimagelib/openexrlib)

#--------------------------------------------------------------------
# Add dependencies
ivw_add_dependencies(Half
					Iex
					IlmImf
					IlmThread
					Imath)



#--------------------------------------------------------------------
# Make package (for other projects to find)
ivw_make_package(OpenEXR openexr)

