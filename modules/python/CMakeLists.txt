#--------------------------------------------------------------------
# Inviwo Python Module
ivw_module(Python)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/defaultinterface/pycamera.h
    ${CMAKE_CURRENT_SOURCE_DIR}/defaultinterface/pycanvas.h    
    ${CMAKE_CURRENT_SOURCE_DIR}/defaultinterface/pylist.h
    ${CMAKE_CURRENT_SOURCE_DIR}/defaultinterface/pyproperties.h
    ${CMAKE_CURRENT_SOURCE_DIR}/defaultinterface/pyutil.h
    ${CMAKE_CURRENT_SOURCE_DIR}/defaultinterface/pyvolume.h
    ${CMAKE_CURRENT_SOURCE_DIR}/pyinviwo.h
    ${CMAKE_CURRENT_SOURCE_DIR}/pyinviwoobserver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/pythonexecutionoutputobeserver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/pythoninterface/pymethod.h
    ${CMAKE_CURRENT_SOURCE_DIR}/pythoninterface/pymodule.h
    ${CMAKE_CURRENT_SOURCE_DIR}/pythoninterface/pyvalueparser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/pythonmodule.h
    ${CMAKE_CURRENT_SOURCE_DIR}/pythonmoduledefine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/pythonscript.h
    ${CMAKE_CURRENT_SOURCE_DIR}/pythonscriptrecorderutil.h
)

ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/defaultinterface/pycamera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/defaultinterface/pycanvas.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/defaultinterface/pylist.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/defaultinterface/pyproperties.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/defaultinterface/pyutil.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/defaultinterface/pyvolume.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pyinviwo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pyinviwoobserver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pythonexecutionoutputobeserver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pythoninterface/pymethod.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pythoninterface/pymodule.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pythoninterface/pyvalueparser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pythonmodule.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pythonscript.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pythonscriptrecorderutil.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})


#--------------------------------------------------------------------
# Add script files
set(SCRIPT_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/scripts/outputredirector.py
)
ivw_group("Script Files" ${SCRIPT_FILES})


 set(TEST_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/tests/python-test.cpp
)
ivw_add_unittest(${TEST_FILES})

#--------------------------------------------------------------------
# Define defintions
add_definitions(-DIVW_PYTHON)

# Create module
ivw_create_module(${SOURCE_FILES} ${HEADER_FILES} ${SHADER_FILES} ${SCRIPT_FILES})

#--------------------------------------------------------------------
# Add Python to package
if(IVW_PACKAGE_PROJECT AND BUILD_SHARED_LIBS)
   get_filename_component(FILENAME "${PYTHON_LIBRARIES}" NAME_WE)
   if(WIN32)
        find_file(PYTHON_DLL ${FILENAME}.dll)
        mark_as_advanced(PYTHON_DLL)
        install(FILES ${PYTHON_DLL} DESTINATION bin COMPONENT modules)
   endif()
endif()
