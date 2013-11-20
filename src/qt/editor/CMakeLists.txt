#--------------------------------------------------------------------
# Inviwo Qt Editor
ivw_project(inviwo-qteditor)

#--------------------------------------------------------------------
# Add MOC files
set(MOC_FILES
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/consolewidget.h
	${IVW_INCLUDE_DIR}/inviwo/qt/editor/eventpropertymanagerwidget.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/linkdialog.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/mappingwidget.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/networkeditor.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/processorgraphicsitem.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/processorlistwidget.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/propertylistwidget.h
	${IVW_INCLUDE_DIR}/inviwo/qt/editor/resourcemanagerwidget.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/settingswidget.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/transferfunctioneditor.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/transferfunctioneditorcontrolpoint.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/transferfunctioneditorlineitem.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/transferfunctioneditorview.h
)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${MOC_FILES}
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/connectiongraphicsitem.h
	${IVW_INCLUDE_DIR}/inviwo/qt/editor/eventpropertymanager.h
	${IVW_INCLUDE_DIR}/inviwo/qt/editor/eventpropertymanagerwidget.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/linkdialog.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/linkgraphicsitem.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/inviwodockwidget.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/inviwoqteditordefine.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/labelgraphicsitem.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/mappingwidget.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/networkeditorview.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/processorlistwidget.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/processorpropertywidget.h
	${IVW_INCLUDE_DIR}/inviwo/qt/editor/resourcemanagerwidget.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/settingswidget.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/transferfunctioneditor.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/transferfunctioneditorcontrolpoint.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/transferfunctioneditorlineitem.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/editor/transferfunctioneditorview.h
)
ivw_group("Header Files" ${HEADER_FILES})

ivw_qt_wrap_cpp(MOC_FILES ${MOC_FILES})
source_group("MOC Files" FILES ${MOC_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    connectiongraphicsitem.cpp
    consolewidget.cpp
	eventpropertymanager.cpp
	eventpropertymanagerwidget.cpp
    inviwodockwidget.cpp
    labelgraphicsitem.cpp
    linkdialog.cpp
    linkgraphicsitem.cpp
    mappingwidget.cpp
    networkeditor.cpp
    networkeditorview.cpp
    processorlistwidget.cpp
    processorgraphicsitem.cpp
    processorpropertywidget.cpp
    propertylistwidget.cpp
	resourcemanagerwidget.cpp
    settingswidget.cpp
    transferfunctioneditor.cpp
    transferfunctioneditorcontrolpoint.cpp
    transferfunctioneditorlineitem.cpp
    transferfunctioneditorview.cpp
    
)
ivw_group("Source Files" ${SOURCE_FILES})

#--------------------------------------------------------------------
# Define defintions
ivw_define_standard_definitions(IVW_QTEDITOR)
ivw_define_qt_definitions()

#--------------------------------------------------------------------
# Create library
add_library(inviwo-qteditor ${SOURCE_FILES} ${MOC_FILES} ${HEADER_FILES})

# Add core dependencies to this list
ivw_add_dependencies(
    InviwoCore
    InviwoQtWidgets
)

#--------------------------------------------------------------------
# Optimize compilation with pre-compilied headers based on inviwo-core
ivw_compile_optimize_inviwo_core()

#--------------------------------------------------------------------
# Make package (for other projects to find)
ivw_make_package(InviwoQtEditor inviwo-qteditor)