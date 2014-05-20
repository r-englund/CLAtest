#--------------------------------------------------------------------
# Inviwo Python Module
ivw_module(Python)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${IVW_MODULE_DIR}/python/defaultinterface/pycamera.h
    ${IVW_MODULE_DIR}/python/defaultinterface/pycanvas.h    
    ${IVW_MODULE_DIR}/python/defaultinterface/pylist.h
    ${IVW_MODULE_DIR}/python/defaultinterface/pyproperties.h
    ${IVW_MODULE_DIR}/python/defaultinterface/pyutil.h
    ${IVW_MODULE_DIR}/python/defaultinterface/pyvolume.h
    ${IVW_MODULE_DIR}/python/pyinviwo.h
    ${IVW_MODULE_DIR}/python/pyinviwoobserver.h
    ${IVW_MODULE_DIR}/python/pythonexecutionoutputobeserver.h
    ${IVW_MODULE_DIR}/python/pythoninterface/pymethod.h
    ${IVW_MODULE_DIR}/python/pythoninterface/pymodule.h
    ${IVW_MODULE_DIR}/python/pythoninterface/pyvalueparser.h
    ${IVW_MODULE_DIR}/python/pythonmodule.h
    ${IVW_MODULE_DIR}/python/pythonmoduledefine.h
    ${IVW_MODULE_DIR}/python/pythonscript.h
    ${IVW_MODULE_DIR}/python/pythonscriptrecorderutil.h
)

ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    ${IVW_MODULE_DIR}/python/defaultinterface/pycamera.cpp
    ${IVW_MODULE_DIR}/python/defaultinterface/pycanvas.cpp
    ${IVW_MODULE_DIR}/python/defaultinterface/pylist.cpp
    ${IVW_MODULE_DIR}/python/defaultinterface/pyproperties.cpp
    ${IVW_MODULE_DIR}/python/defaultinterface/pyutil.cpp
    ${IVW_MODULE_DIR}/python/defaultinterface/pyvolume.cpp
    ${IVW_MODULE_DIR}/python/pyinviwo.cpp
    ${IVW_MODULE_DIR}/python/pyinviwoobserver.cpp
    ${IVW_MODULE_DIR}/python/pythonexecutionoutputobeserver.cpp
    ${IVW_MODULE_DIR}/python/pythoninterface/pymethod.cpp
    ${IVW_MODULE_DIR}/python/pythoninterface/pymodule.cpp
    ${IVW_MODULE_DIR}/python/pythoninterface/pyvalueparser.cpp
    ${IVW_MODULE_DIR}/python/pythonmodule.cpp
    ${IVW_MODULE_DIR}/python/pythonscript.cpp
    ${IVW_MODULE_DIR}/python/pythonscriptrecorderutil.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})

 set(TEST_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/tests/python-test.h
)
ivw_group("Unittest Files" ${TEST_FILES})
ivw_add_unittest(${TEST_FILES})

#--------------------------------------------------------------------
# Define defintions
add_definitions(-DIVW_PYTHON)

# Create module
ivw_create_module(${SOURCE_FILES} ${HEADER_FILES} ${TEST_FILES})

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