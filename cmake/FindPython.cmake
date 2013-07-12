#
# Try to find PYTHON library and include path.
# Once done this will define
#
# PYTHON_FOUND
# PYTHON_INCLUDE_DIR
# PYTHON_LIBRARY_DIR
# 

if(WIN32)
	foreach(_VERSION 2.7 2.6 2.5 2.4)
		find_path(PYTHON_BASE_DIR 
			python.exe
			[HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\${_VERSION}\\InstallPath]
			)
		
		if(PYTHON_BASE_DIR)
			message(STATUS "Tada")
			set(PYTHON_VERSION ${_VERSION})
			STRING(REPLACE "." "" PYTHON_VERSION_NO_DOTS ${_VERSION})
			break()
		endif(PYTHON_BASE_DIR)
		
	endforeach(_VERSION)

	message(STATUS  ${PYTHON_VERSION_NO_DOTS})

	find_path(PYTHON_INCLUDE_DIR Python.h ${PYTHON_BASE_DIR}\\include)
	find_path(PYTHON_LIBRARY_DIR python${PYTHON_VERSION_NO_DOTS}.lib ${PYTHON_BASE_DIR}\\libs)

else(WIN32)
	message(FATAL_ERROR "Python not yet supported for non-windows platforms")
endif(WIN32)

if(PYTHON_INCLUDE_DIR AND PYTHON_LIBRARY_DIR)
	set(PYTHON_FOUND 1 CACHE STRING "Set to 1 if PYTHON is found, 0 otherwise")
else(PYTHON_INCLUDE_DIR AND PYTHON_LIBRARY_DIR)
	set(PYTHON_FOUND 0 CACHE STRING "Set to 1 if PYTHON is found, 0 otherwise")
endif(PYTHON_INCLUDE_DIR AND PYTHON_LIBRARY_DIR)

mark_as_advanced(PYTHON_FOUND)
mark_as_advanced(PYTHON_VERSION)
mark_as_advanced(PYTHON_INCLUDE_DIR)
mark_as_advanced(PYTHON_LIBRARY_DIR)

#=============================================================================
# InViWo Build

#--------------------------------------------------------------------
# Build PYTHON lib
if(NOT PYTHON_FOUND)
	set(PYTHON_BASE_DIR CACHE PATH "")
	message(FATAL_ERROR "No Python found")
endif(NOT PYTHON_FOUND)

