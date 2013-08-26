#--------------------------------------------------------------------
# Inviwo Python Module
ivw_module(Python)


#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${IVW_MODULE_DIR}/python/pyinviwo.h
    ${IVW_MODULE_DIR}/python/pythoninterface/pymethod.h
    ${IVW_MODULE_DIR}/python/pythoninterface/pymodule.h
    ${IVW_MODULE_DIR}/python/defaultinterface/camera.h
    ${IVW_MODULE_DIR}/python/defaultinterface/canvas.h	
    ${IVW_MODULE_DIR}/python/defaultinterface/list.h
    ${IVW_MODULE_DIR}/python/defaultinterface/properties.h
    ${IVW_MODULE_DIR}/python/defaultinterface/snapshot.h
    ${IVW_MODULE_DIR}/python/pyvariant.h
    ${IVW_MODULE_DIR}/python/pythonmodule.h
    ${IVW_MODULE_DIR}/python/pythonscript.h
    ${IVW_MODULE_DIR}/python/pythonmoduledefine.h
)

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
    ${IVW_MODULE_DIR}/python/pyinviwo.cpp
    ${IVW_MODULE_DIR}/python/pythoninterface/pymethod.cpp
    ${IVW_MODULE_DIR}/python/pythoninterface/pymodule.cpp
    ${IVW_MODULE_DIR}/python/defaultinterface/camera.cpp
    ${IVW_MODULE_DIR}/python/defaultinterface/canvas.cpp
    ${IVW_MODULE_DIR}/python/defaultinterface/list.cpp
    ${IVW_MODULE_DIR}/python/defaultinterface/properties.cpp
    ${IVW_MODULE_DIR}/python/defaultinterface/snapshot.cpp
    ${IVW_MODULE_DIR}/python/pythonscript.cpp
    ${IVW_MODULE_DIR}/python/pythonmodule.cpp
    ${IVW_MODULE_DIR}/python/pyvariant.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})


#--------------------------------------------------------------------
# Define defintions
add_definitions(-DIVW_PYTHON)
ivw_define_qt_definitions()


# Create module
ivw_create_module(${SOURCE_FILES} ${HEADER_FILES} ${MOC_FILES})
