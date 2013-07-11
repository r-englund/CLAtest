set_property(GLOBAL PROPERTY USE_FOLDERS On)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER ext)

#--------------------------------------------------------------------
# Only output error messages
function(message)
  list(GET ARGV 0 MessageType)
  if(MessageType STREQUAL FATAL_ERROR OR
     MessageType STREQUAL SEND_ERROR OR
     MessageType STREQUAL WARNING OR
     MessageType STREQUAL AUTHOR_WARNING)
    list(REMOVE_AT ARGV 0)
    _message(STATUS "${ARGV}")
  endif()
endfunction()
function(ivw_message)
    _message(${ARGV})
endfunction()

#--------------------------------------------------------------------
# Add own cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_BINARY_DIR}/cmake/")

include(globalmacros)

#--------------------------------------------------------------------
# Output paths for the executables, runtimes, archives and libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin CACHE PATH
   "Single Directory for all Executables.")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib CACHE PATH
   "Single Directory for all static libraries.")
   
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib CACHE PATH
   "Single Directory for all Libraries")
   
mark_as_advanced(CMAKE_INSTALL_PREFIX CMAKE_RUNTIME_OUTPUT_DIRECTORY CMAKE_BUNDLE_OUTPUT_DIRECTORY CMAKE_ARCHIVE_OUTPUT_DIRECTORY CMAKE_LIBRARY_OUTPUT_DIRECTORY)

#--------------------------------------------------------------------
# Path for this solution
if(NOT EXECUTABLE_OUTPUT_PATH)
  set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin CACHE PATH 
    "Single output directory for building all executables.")
endif(NOT EXECUTABLE_OUTPUT_PATH)

if(NOT LIBRARY_OUTPUT_PATH)
  set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib CACHE PATH
    "Single output directory for building all libraries.")
endif(NOT LIBRARY_OUTPUT_PATH)

mark_as_advanced(EXECUTABLE_OUTPUT_PATH LIBRARY_OUTPUT_PATH)

# Set Common Variables
set(IVW_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(IVW_MODULE_DIR ${CMAKE_SOURCE_DIR}/modules)
set(IVW_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(IVW_EXTENSIONS_DIR ${CMAKE_SOURCE_DIR}/ext)
set(IVW_BINARY_DIR ${CMAKE_BINARY_DIR})
set(IVW_LIBRARY_DIR ${LIBRARY_OUTPUT_PATH})
set(IVW_EXECUTABLE_DIR ${EXECUTABLE_OUTPUT_PATH})
set(IVW_CMAKE_SOURCE_MODULE_DIR ${CMAKE_SOURCE_DIR}/cmake)
set(IVW_CMAKE_BINARY_MODULE_DIR ${CMAKE_BINARY_DIR}/cmake)

configure_file(${IVW_CMAKE_SOURCE_MODULE_DIR}/inviwocommondefines_template.h ${IVW_INCLUDE_DIR}/inviwo/core/common/inviwocommondefines.h @ONLY IMMEDIATE)

# Set ignored libs
set(VS_MULTITHREADED_DEBUG_DLL_IGNORE_LIBRARY_FLAGS
    "/NODEFAULTLIB:libc.lib
     /NODEFAULTLIB:libcmt.lib
     /NODEFAULTLIB:msvcrt.lib
     /NODEFAULTLIB:libcd.lib
     /NODEFAULTLIB:libcmtd.lib"
)
set(VS_MULTITHREADED_RELEASE_DLL_IGNORE_LIBRARY_FLAGS
    "/NODEFAULTLIB:libc.lib
     /NODEFAULTLIB:libcmt.lib
     /NODEFAULTLIB:libcd.lib
     /NODEFAULTLIB:libcmtd.lib
     /NODEFAULTLIB:msvcrtd.lib"
)
	
#--------------------------------------------------------------------
# Disable deprecation warnings for standard C functions
if(CMAKE_COMPILER_2005)
  add_definitions(-D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE)
endif(CMAKE_COMPILER_2005)

# Check if MAC
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  add_definitions(-DDARWIN)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

# Build shared libs or static libs
option(SHARED_LIBS "Build shared libs, else static libs" OFF)
mark_as_advanced(SHARED_LIBS)

if(SHARED_LIBS)
    set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libs, else static libs" FORCE)
    mark_as_advanced(BUILD_SHARED_LIBS)
else()
	IF(WIN32)
		IF(MSVC)
        	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
			set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
			set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
			set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zm1000")
		ENDIF(MSVC)
	ENDIF()
endif()

#--------------------------------------------------------------------
# Set preprocessor definition to indicate whether 
# to use the debug postfix

# Add debug postfix if WIN32
IF(WIN32)
    IF(MSVC)
        SET(CMAKE_DEBUG_POSTFIX "d")
       
        set(INVIWO_PROCESSOR_COUNT "0" CACHE STRING "Specify number of cores to use for parallel compilation when using MSVC IDE (default 0 = uses all available cores for compilation)")
        mark_as_advanced(INVIWO_PROCESSOR_COUNT)
        if( INVIWO_PROCESSOR_COUNT STREQUAL "0" )
             SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
             SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
        else()
             SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP${INVIWO_PROCESSOR_COUNT}")
             SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP${INVIWO_PROCESSOR_COUNT}")  	
         endif()	
    ENDIF(MSVC)
ENDIF()

if(DEBUG_POSTFIX)
	add_definitions(-D_DEBUG_POSTFIX)
endif(DEBUG_POSTFIX)

#--------------------------------------------------------------------
# Specify build-based defintions
if(BUILD_SHARED_LIBS)
  add_definitions(-DINVIWO_ALL_DYN_LINK)
else(BUILD_SHARED_LIBS)
  add_definitions(-DFREEGLUT_STATIC)
  add_definitions(-DGLEW_STATIC)
endif(BUILD_SHARED_LIBS)

