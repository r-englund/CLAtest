#--------------------------------------------------------------------
# Inviwo Python Module
ivw_module(Python)

#find_package(Python REQUIRED)

# if(BUILD_PYTHON)	
	# add_subdirectory(ext/python27)
# endif(BUILD_PYTHON)


#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${IVW_MODULE_DIR}/python/pythoninterface/pyinviwo.h
    ${IVW_MODULE_DIR}/python/pythoninterface/camera.py.h
    ${IVW_MODULE_DIR}/python/pythoninterface/canvas.py.h	
    ${IVW_MODULE_DIR}/python/pythoninterface/info.py.h
    ${IVW_MODULE_DIR}/python/pythoninterface/list.py.h
    ${IVW_MODULE_DIR}/python/pythoninterface/properties.py.h
    ${IVW_MODULE_DIR}/python/pythoninterface/snapshot.py.h
    ${IVW_MODULE_DIR}/python/pyvariant.h
    ${IVW_MODULE_DIR}/python/pythonmodule.h
    ${IVW_MODULE_DIR}/python/pythonscript.h
    ${IVW_MODULE_DIR}/python/pythonmoduledefine.h
)

if (IVW_MODULE_HUMANCOMPUTATIONENGINE)
LIST(APPEND HEADER_FILES
    ${IVW_MODULE_DIR}/python/pythoninterface/hcemodule.py.h
)
endif (IVW_MODULE_HUMANCOMPUTATIONENGINE)

ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add MOC files
set(MOC_FILES
    ${IVW_MODULE_DIR}/python/pythoneditorwidget.h
)
qt4_wrap_cpp(MOC_FILES ${MOC_FILES})
source_group("MOC Files" FILES ${MOC_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    ${IVW_MODULE_DIR}/python/pythoneditorwidget.cpp
    ${IVW_MODULE_DIR}/python/pythoninterface/pyinviwo.cpp
    ${IVW_MODULE_DIR}/python/pythonscript.cpp
    ${IVW_MODULE_DIR}/python/pythonmodule.cpp
    ${IVW_MODULE_DIR}/python/pyvariant.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})

#include_directories(${PYTHON_INCLUDE_DIR})
#link_directories(${PYTHON_LIBRARY_DIR})

#ivw_define_qt_definitions()
#--------------------------------------------------------------------
# Define defintions
#ivw_add_definition(IVW_PYTHON)
add_definitions(-DIVW_PYTHON)
ivw_define_qt_definitions()


# Create module
ivw_create_module(${SOURCE_FILES} ${HEADER_FILES} ${MOC_FILES})
