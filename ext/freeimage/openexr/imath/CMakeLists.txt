ivw_project(imath)

#--------------------------------------------------------------------
# Supress warnings
add_definitions( "/W0 /D_CRT_SECURE_NO_WARNINGS /wd4005 /wd4996 /nologo /IGNORE:4006" )

#--------------------------------------------------------------------
# Add header files

file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

ivw_group("IMath Header Files" ${HEADER_FILES} )

#--------------------------------------------------------------------
# Add source files

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

ivw_group("IMath Source Files" ${SOURCE_FILES} )

#--------------------------------------------------------------------
# Define defintions
ivw_define_standard_definitions(imath)

#--------------------------------------------------------------------
# Create library
add_library(imath ${SOURCE_FILES} ${HEADER_FILES})

#--------------------------------------------------------------------
# Creates VS folder structure
ivw_vs_folder(imath ext/freeimagelib/openexrlib)

#--------------------------------------------------------------------
# Add dependancies
ivw_include_directories(${IVW_EXTENSIONS_DIR}/freeimage/openexr/half)
ivw_include_directories(${IVW_EXTENSIONS_DIR}/freeimage/openexr/iex)



# ivw_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#--------------------------------------------------------------------
# Make package (for other projects to find)
ivw_make_package(IMath imath)