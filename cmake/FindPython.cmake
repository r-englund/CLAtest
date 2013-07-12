#
# Try to find PYTHON library and include path.
# Once done this will define
#
# PYTHON_FOUND
# PYTHON_INCLUDE_DIR
# PYTHON_LIBRARY_DIR
# PYTHON_VERSION
#

if(WIN32)
	set(MS_PATHS   
		[HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\2.7\\InstallPath]
		[HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\2.6\\InstallPath]
		[HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\2.5\\InstallPath]
		[HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\2.4\\InstallPath] )
		  
	find_path(PYTHON_BASE_DIR python.exe ${PYTHON_BASE_DIR} ${MS_PATHS})

	set(PYTHON_LIBS python27.lib python26.lib python25.lib python24.lib python.lib)

	if(PYTHON_BASE_DIR)
		find_path(PYTHON_INCLUDE_DIR Python.h ${PYTHON_BASE_DIR}\\include)
		find_path(PYTHON_LIBRARY_DIR NAMES ${PYTHON_LIBS}  PATHS ${PYTHON_BASE_DIR}\\libs)
		string(REPLACE " " "\\ " PYTHON_LIBRARY_DIR ${PYTHON_LIBRARY_DIR})
	endif(PYTHON_BASE_DIR)

else(WIN32)
	message(FATAL_ERROR "Python not yet supported for non-windows platforms")
endif(WIN32)


unset(PYTHON_FOUND CACHE)
unset(PYTHON_FOUND)
if(PYTHON_INCLUDE_DIR AND PYTHON_LIBRARY_DIR)
	set(PYTHON_FOUND 1 CACHE STRING "Set to 1 if PYTHON is found, 0 otherwise")
else(PYTHON_INCLUDE_DIR AND PYTHON_LIBRARY_DIR)
	set(PYTHON_FOUND 0 CACHE STRING "Set to 1 if PYTHON is found, 0 otherwise")
endif(PYTHON_INCLUDE_DIR  AND PYTHON_LIBRARY_DIR)

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
	unset(PYTHON_INCLUDE_DIR)
	unset(PYTHON_LIBRARY_DIR)
	unset(PYTHON_VERSION)
	message(FATAL_ERROR "No Python found")
endif(NOT PYTHON_FOUND)

