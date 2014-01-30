if(DESIRED_QT_VERSION MATCHES 5)
        string(REGEX REPLACE "bin\\\\qmake([^ ]*)$" "lib\\\\cmake" QT5_PATH "${QT_QMAKE_EXECUTABLE}")
        set(CMAKE_PREFIX_PATH "${QT5_PATH}")
endif()

#--------------------------------------------------------------------
# Inviwo PythonQt Module
ivw_module(PythonQt)

#--------------------------------------------------------------------
# Add MOC files
set(MOC_FILES
    ${IVW_MODULE_DIR}/pythonqt/pythoneditorwidget.h
)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${IVW_MODULE_DIR}/pythonqt/pythoninfowidget.h
    ${IVW_MODULE_DIR}/pythonqt/pythonmenu.h
    ${IVW_MODULE_DIR}/pythonqt/pythonqtmethods/pythonqtmethods.h
    ${IVW_MODULE_DIR}/pythonqt/pythonqtmodule.h
    ${IVW_MODULE_DIR}/pythonqt/pythonqtmoduledefine.h
    ${MOC_FILES}
)
ivw_group("Header Files" ${HEADER_FILES})

ivw_qt_wrap_cpp(MOC_FILES ${MOC_FILES})
source_group("MOC Files" FILES ${MOC_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    ${IVW_MODULE_DIR}/pythonqt/pythoneditorwidget.cpp
    ${IVW_MODULE_DIR}/pythonqt/pythoninfowidget.cpp
    ${IVW_MODULE_DIR}/pythonqt/pythonmenu.cpp
    ${IVW_MODULE_DIR}/pythonqt/pythonqtmethods/pythonqtmethods.cpp
    ${IVW_MODULE_DIR}/pythonqt/pythonqtmodule.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})

ivw_define_qt_definitions()
ivw_add_definition(IVW_PYTHON_QT)

# Create module
ivw_create_module(${SOURCE_FILES} ${MOC_FILES} ${HEADER_FILES})