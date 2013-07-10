#
# Try to find PYTHON library and include path.
# Once done this will define
#
# PYTHON_FOUND
# PYTHON_INCLUDE_DIR
# PYTHON_LIBRARY_DIR
# 

if(WIN32)
	find_path( PYTHON_INCLUDE_DIR Python.h
		C:/Python27/include
		${PYTHON_BASE_DIR}/include
		DOC "The directory where Python.h resides")
	find_path( PYTHON_LIBRARY_DIR python27.lib
		C:/Python27/libs
		${PYTHON_BASE_DIR}/libs
		DOC "The directory where Python27.lib resides")
else(WIN32)
	message(FATAL_ERROR "Python not yet supported for non-windows platforms")
endif(WIN32)

if(PYTHON_INCLUDE_DIR AND PYTHON_LIBRARY_DIR)
	set(PYTHON_FOUND 1 CACHE STRING "Set to 1 if PYTHON is found, 0 otherwise")
else(PYTHON_INCLUDE_DIR AND PYTHON_LIBRARY_DIR)
	set(PYTHON_FOUND 0 CACHE STRING "Set to 1 if PYTHON is found, 0 otherwise")
endif(PYTHON_INCLUDE_DIR AND PYTHON_LIBRARY_DIR)

mark_as_advanced(PYTHON_FOUND)
mark_as_advanced(PYTHON_INCLUDE_DIR)
mark_as_advanced(PYTHON_LIBRARY_DIR)

#=============================================================================
# InViWo Build

#--------------------------------------------------------------------
# Build PYTHON lib
if(NOT PYTHON_FOUND)
	set(PYTHON_BASE_DIR CACHE PATH "")
endif(NOT PYTHON_FOUND)

