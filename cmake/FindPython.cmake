#
# Try to find PYTHON library and include path.
# Once done this will define
#
# For windows 
#
# PYTHON_FOUND
# PYTHON_INCLUDE_DIR
# PYTHON_LIBRARY_DIR
# PYTHON_VERSION
#
#
# FOR UNIX
#
# PYTHON_FOUND
# PYTHON_INCLUDE_DIR
# PYTHON_LIBRARY_DIR
# PYTHON_VERSION

INCLUDE(FindPackageHandleStandardArgs)

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
        find_library(PYTHON_LIBRARIES NAMES ${PYTHON_LIBS} PATHS ${PYTHON_LIBRARY_DIR})
        #get_filename_component(PYTHON_LIBRARIES ${PYTHON_LIBRARIES} NAME)
	endif(PYTHON_BASE_DIR)
    
	FIND_PACKAGE_HANDLE_STANDARD_ARGS( Python DEFAULT_MSG PYTHON_LIBRARY_DIR PYTHON_INCLUDE_DIR )

	mark_as_advanced(PYTHON_FOUND)
	mark_as_advanced(PYTHON_VERSION)
	mark_as_advanced(PYTHON_INCLUDE_DIR)
	mark_as_advanced(PYTHON_LIBRARY_DIR)
    mark_as_advanced(PYTHON_LIBRARY)
    mark_as_advanced(PYTHON_LIBRARIES)
elseif(APPLE )
	message(FATAL_ERROR "Python not yet supported on apple platforms")
elseif(UNIX)
	set(UNIX_INCLUDE_PATHS /usr/local/pyenv/versions/2.7.6/include/python2.7 /usr/include/python2.7 /usr/include/python2.6 /usr/include/python2.5)
	set(UNIX_LIB_PATHS /usr/local/pyenv/versions/2.7.6/lib/ /usr/lib/i386-linux-gnu)
    find_path(PYTHON_INCLUDE_DIR Python.h ${UNIX_INCLUDE_PATHS})
    if(BUILD_SHARED_LIBS)
        set(PYTHON_UNIX_LIBS libpython2.7.so libpython2.6.so libpython2.5.so libpython2.4.so)
    else()
        set(PYTHON_UNIX_LIBS libpython2.7.a libpython2.6.a libpython2.5.a libpython2.4.a)
    endif()
    find_library(PYTHON_LIBRARIES NAMES ${PYTHON_UNIX_LIBS} PATHS ${UNIX_LIB_PATHS})
	#find_library(PYTHON_FIND_LIB1 NAMES libpthread.a libdl.a  PATHS ${UNIX_LIB_PATHS})
	#find_library(PYTHON_FIND_LIB2 NAMES libpthread.so  PATHS ${UNIX_LIB_PATHS})
	#find_library(PYTHON_FIND_LIB3 NAMES libdl.so  PATHS ${UNIX_LIB_PATHS})
	#find_library(PYTHON_FIND_LIB4 NAMES libc.so  PATHS ${UNIX_LIB_PATHS})
	#find_library(PYTHON_FIND_LIB5 NAMES libutil.so  PATHS ${UNIX_LIB_PATHS})
	#set(PYTHON_LIBRARIES ${PYTHON_FIND_LIB1} ${PYTHON_FIND_LIB2} ${PYTHON_FIND_LIB3} ${PYTHON_FIND_LIB4} ${PYTHON_FIND_LIB5})
	
	FIND_PACKAGE_HANDLE_STANDARD_ARGS(Python DEFAULT_MSG PYTHON_LIBRARIES PYTHON_INCLUDE_DIR)
	
	mark_as_advanced(PYTHON_FOUND)
	mark_as_advanced(PYTHON_INCLUDE_DIR)
	mark_as_advanced(PYTHON_LIBRARY_DIR)
	mark_as_advanced(PYTHON_LIBRARIES)
endif()

