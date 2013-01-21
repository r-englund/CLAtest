# FindOpenCL - attempts to locate the OpenCL library.
#
# This module defines the following variables:
#   OPENCL_FOUND		- Module was found
#   OPENCL_INCLUDE_DIR - Directory where cl.h is located
#   OPENCL_LIBRARIES    - Libraries necessary to compile 
#
# Note: Untested with Linux and AMD combinations
#=============================================================================
FIND_PACKAGE( PackageHandleStandardArgs )

IF(WIN32) # Windows
	
	# NVIDIA uses environment variable CUDA_PATH
	# According to AMD specification, http://developer.amd.com/download/AMD_APP_SDK_Installation_Notes.pdf
	# Find out build target 
	IF(CMAKE_SIZEOF_VOID_P EQUAL 8) # 64-bit
		SET(OPENCL_LIB_DIR "$ENV{AMDAPPSDKROOT}/lib/x86_64") # AMD
		SET(OPENCL_LIB_DIR "$ENV{CUDA_PATH}/lib/x64") 		 # NVIDIA
		SET(OPENCL_LIB_DIR "$ENV{INTELOCLSDKROOT}/lib/x64") 	# INTEL
	ELSE(CMAKE_SIZEOF_VOID_P EQUAL 8) # 32-bit
		SET(OPENCL_LIB_DIR "$ENV{AMDAPPSDKROOT}/lib/x86") 	 # AMD
		SET(OPENCL_LIB_DIR "$ENV{CUDA_PATH}/lib/Win32") 	 # NVIDIA
		SET(OPENCL_LIB_DIR "$ENV{INTELOCLSDKROOT}/lib/x86") 	# INTEL
	ENDIF(CMAKE_SIZEOF_VOID_P EQUAL 8)
	
	
	# Optional search path
	GET_FILENAME_COMPONENT(OPENCL_INCLUDE_SEARCH_DIR ${OPENCL_LIB_DIR}/../../include ABSOLUTE)	
	
	FIND_LIBRARY(OPENCL_LIBRARIES OpenCL.lib ${OPENCL_LIB_DIR} DOC "OpenCL lib for Windows")
	FIND_PATH(OPENCL_INCLUDE_DIR CL/cl.h ${OPENCL_INCLUDE_SEARCH_DIR} DOC "Include directory for OpenCL on OSX")
	
	# Add quotes to paths to make sure it works on windows (at least Visual Studio complains)
	string(REPLACE " " "\\ " OPENCL_LIBRARIES ${OPENCL_LIBRARIES})
ELSE(WIN32)
	IF(APPLE)
		# Tested, at least works if OpenCL is distributed with OSX
		FIND_LIBRARY(OPENCL_LIBRARIES OpenCL DOC "OpenCL lib for OSX")
		FIND_PATH(OPENCL_INCLUDE_DIR OpenCL/cl.h DOC "Include directory for OpenCL on OSX")
	ELSE(APPLE) # Linux. Note, untested
		# Library needs to be in set in the LD_LIBRARY_PATH variable
		FIND_LIBRARY(OPENCL_LIBRARIES OpenCL 
					"$ENV{AMDAPPSDKROOT}/lib"
					ENV LD_LIBRARY_PATH
					DOC "OpenCL lib for Linux")
		# Take care of custom install locations
		GET_FILENAME_COMPONENT(OPENCL_LIB_DIR ${OPENCL_LIBRARIES} PATH)
		# Optional search path
		GET_FILENAME_COMPONENT(OPENCL_INCLUDE_SEARCH_DIR ${OPENCL_LIB_DIR}/../../include ABSOLUTE)		
		FIND_PATH(OPENCL_INCLUDE_DIR CL/cl.h 
			"/usr/local/cuda/include" # NVIDIA directory
			"/opt/AMDAPP/include" # AMD directory
			${OPENCL_INCLUDE_SEARCH_DIR} # Include directory of library location in LD_LIBRARY_PATH
			DOC "Include for OpenCL on Linux")
	ENDIF(APPLE)
ENDIF(WIN32)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS( OpenCL DEFAULT_MSG OPENCL_LIBRARIES OPENCL_INCLUDE_DIR )

MARK_AS_ADVANCED(
  OPENCL_INCLUDE_DIR
  OPENCL_LIBRARIES
)

