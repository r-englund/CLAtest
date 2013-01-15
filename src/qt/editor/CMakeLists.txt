#--------------------------------------------------------------------
# Inviwo Qt Editor
ivw_project(inviwo-qteditor)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/connectiongraphicsitem.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/consolewidget.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/linkdialog.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/linkgraphicsitem.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/inviwodockwidget.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/labelgraphicsitem.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/networkeditor.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/networkeditorview.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/processorlistwidget.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/processorgraphicsitem.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/processorpropertywidget.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/propertylistwidget.h
)
qt4_wrap_cpp(MOC_FILES ${HEADER_FILES})
ivw_group("Header Files" ${HEADER_FILES})
source_group("MOC Files" FILES ${MOC_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    connectiongraphicsitem.cpp
    consolewidget.cpp
    inviwodockwidget.cpp
    labelgraphicsitem.cpp
    linkdialog.cpp
    linkgraphicsitem.cpp
    networkeditor.cpp
    networkeditorview.cpp
    processorlistwidget.cpp
    processorgraphicsitem.cpp
    processorpropertywidget.cpp
    propertylistwidget.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})

#--------------------------------------------------------------------
# Define defintions
ivw_define_standard_definitions(IVW_QTEDITOR)
add_definitions(-DQT_CORE_LIB
                      -DQT_GUI_LIB
                      -DQT_OPENGL_LIB
                      -DQT_DLL
                      -DQT_THREAD_SUPPORT)

#--------------------------------------------------------------------
# Create library
add_library(inviwo-qteditor ${SOURCE_FILES} ${HEADER_FILES} ${MOC_FILES})

# Add core dependencies to this list
ivw_add_dependencies(
    InviwoCore
    Qt
    InviwoQtWidgets
)

#--------------------------------------------------------------------
# Creates VS folder structure
ivw_vs_folder(inviwo-qteditor qt)

#--------------------------------------------------------------------
# Make package (for other projects to find)
ivw_make_package(InviwoQtEditor inviwo-qteditor)
