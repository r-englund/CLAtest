#--------------------------------------------------------------------
# Add subdirectories
add_subdirectory(zlib)
add_subdirectory(libjpeg)
add_subdirectory(libmng)
add_subdirectory(libopenjpeg)
add_subdirectory(libpng)
add_subdirectory(librawlite)
add_subdirectory(libtiff)
add_subdirectory(libtiff4)
add_subdirectory(openexr)
#--------------------------------------------------------------------
# FreeImage Package
ivw_project(freeimage)


#--------------------------------------------------------------------
# Supress warnings
add_definitions( "/W0 /D_CRT_SECURE_NO_WARNINGS /wd4005 /wd4996 /nologo /IGNORE:4006" )

#--------------------------------------------------------------------
# Add header files

file(GLOB_RECURSE DeprecationManager_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/DeprecationManager/*.h)
file(GLOB_RECURSE FreeImage_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/FreeImage/*.h)
file(GLOB_RECURSE FreeImageToolkit_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/FreeImageToolkit/*.h)
file(GLOB_RECURSE Metadata_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Metadata/*.h)


set(General_HEADER_FILES 
				CacheFile.h
				FreeImage.h
				FreeImageIO.h
				Plugin.h
				Quantizers.h
				ToneMapping.h
				Utilities.h)

LIST(APPEND HEADER_FILES 	${DeprecationManager_HEADER_FILES} 
							${FreeImage_HEADER_FILES} 
							${FreeImageToolkit_HEADER_FILES} 
							${Metadata_HEADER_FILES}
							${General_HEADER_FILES}	)


ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add source files

file(GLOB_RECURSE DeprecationManager_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/DeprecationManager/*.cpp)
file(GLOB_RECURSE FreeImage_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/FreeImage/*.cpp)
file(GLOB_RECURSE FreeImageToolkit_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/FreeImageToolkit/*.cpp)
file(GLOB_RECURSE Metadata_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Metadata/*.cpp)


LIST(APPEND SOURCE_FILES  
					${DeprecationManager_SOURCE_FILES} 
					${FreeImage_SOURCE_FILES}
					${FreeImageToolkit_SOURCE_FILES}
					${Metadata_SOURCE_FILES}
		)

ivw_group("Source Files" ${SOURCE_FILES} )

#--------------------------------------------------------------------
# Add all folders as includes
list_subdirectories(FREEIMAGE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} FALSE FALSE)

include_directories(${FREEIMAGE_DIRS})

#--------------------------------------------------------------------
# Define defintions
ivw_define_standard_definitions(freeimage)

#--------------------------------------------------------------------
# Create library
add_library(freeimage ${SOURCE_FILES} ${HEADER_FILES})

#--------------------------------------------------------------------
# Add dependencies
ivw_add_dependencies(
	LibJPEG 
	LibMNG 
	LibOpenJPEG
	LibPNG  
	LibRawLite 
	LibTiff4 
	LibTiff 
	OpenEXR
	ZLib
	)

ivw_include_directories(${IVW_EXTENSIONS_DIR}/freeimage/)
ivw_include_directories(${IVW_EXTENSIONS_DIR}/freeimage/FreeImage)



#--------------------------------------------------------------------
# This is to use .lib file, should be deleted when real linker is working 
# link_directories(${IVW_EXTENSIONS_DIR}/freeimage/FreeImageLib/)

# ivw_add_dependency_libraries(FreeImage)

# SET_TARGET_PROPERTIES( freeimage PROPERTIES LINK_FLAGS
# 					"/LIBPATH:${IVW_EXTENSIONS_DIR}/freeimage/FreeImageLib/" )

# LIST(APPEND _allLibsDir "${IVW_EXTENSIONS_DIR}/freeimage/FreeImageLib" )

#---------------------------------------------------------------------

#--------------------------------------------------------------------
# Creates VS folder structure
ivw_vs_folder(freeimage ext/freeimagelib)

#--------------------------------------------------------------------
# Make package (for other projects to find)
ivw_make_package(FreeImage freeimage)


	