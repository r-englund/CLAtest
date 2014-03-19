#--------------------------------------------------------------------
# Inviwo Qt Widgets
ivw_project(inviwo-qtwidgets)

#--------------------------------------------------------------------
# Add MOC files
set(MOC_FILES
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/colorwheel.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/customdoublespinboxqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/editablelabelqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/eventpropertymanagerwidget.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/sliderwidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/inviwoapplicationqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/inviwodockwidget.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/lightpositionwidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/mappingpopup.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/ordinaleditorwidget.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/propertylistwidget.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/rangesliderqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/rawdatareaderdialogqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/spinboxrangesliderwidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/timerqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/processors/processorwidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/boolpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/buttonpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/collapsiblegroupboxwidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/colorpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/camerapropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/compositepropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/directorypropertywidgetqt.h       
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/eventpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/filepropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/floatmat2propertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/floatmat3propertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/floatmat4propertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/floatminmaxpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/floatpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/htmleditorwidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/htmllistwidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/intminmaxpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/imageeditorwidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/lightpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/optionpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/propertysettingswidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/propertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/sphericalfloatvec3propertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/stringpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/syntaxhighlighter.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/texteditorwidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/transferfunctioneditor.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/transferfunctioneditorview.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/transferfunctionpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/transferfunctionpropertydialog.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/propertyselectiontreewidget.h
)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/eventconverterqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/eventpropertymanager.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/inviwoqtwidgetsdefine.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/labelgraphicsitem.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/transferfunctioneditorcontrolpoint.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/qtwidgetmodule.h
    ${MOC_FILES}
)
ivw_group("Header Files" ${HEADER_FILES})

ivw_qt_wrap_cpp(MOC_FILES ${MOC_FILES})
source_group("MOC Files" FILES ${MOC_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    colorwheel.cpp
    customdoublespinboxqt.cpp
    editablelabelqt.cpp
    eventconverterqt.cpp
    eventpropertymanager.cpp
    eventpropertymanagerwidget.cpp
    inviwoapplicationqt.cpp
    inviwodockwidget.cpp
    labelgraphicsitem.cpp
    lightpositionwidgetqt.cpp
    mappingpopup.cpp
    processors/processorwidgetqt.cpp
    properties/boolpropertywidgetqt.cpp
    properties/buttonpropertywidgetqt.cpp  
    properties/camerapropertywidgetqt.cpp
    properties/collapsiblegroupboxwidgetqt.cpp
    properties/colorpropertywidgetqt.cpp
    properties/compositepropertywidgetqt.cpp
    properties/directorypropertywidgetqt.cpp    
    properties/eventpropertywidgetqt.cpp
    properties/filepropertywidgetqt.cpp
    properties/floatmat2propertywidgetqt.cpp
    properties/floatmat3propertywidgetqt.cpp
    properties/floatmat4propertywidgetqt.cpp
    properties/floatminmaxpropertywidgetqt.cpp
    properties/floatpropertywidgetqt.cpp
    properties/glslsyntaxhighlighter.cpp
    properties/htmleditorwidgetqt.cpp
    properties/htmllistwidgetqt.cpp
    properties/imageeditorwidgetqt.cpp
    properties/intminmaxpropertywidgetqt.cpp
    properties/lightpropertywidgetqt.cpp  
    properties/optionpropertywidgetqt.cpp
    properties/propertysettingswidgetqt.cpp
    properties/propertywidgetqt.cpp
    properties/pythonsyntaxhighlighter.cpp
    properties/sphericalfloatvec3propertywidgetqt.cpp
    properties/stringpropertywidgetqt.cpp
    properties/syntaxhighlighter.cpp
    properties/texteditorwidgetqt.cpp
    properties/transferfunctioneditor.cpp
    properties/transferfunctioneditorcontrolpoint.cpp
    properties/transferfunctioneditorview.cpp
    properties/transferfunctionpropertydialog.cpp
    properties/transferfunctionpropertywidgetqt.cpp 
    propertylistwidget.cpp
    propertyselectiontreewidget.cpp
    qtwidgetmodule.cpp
    rangesliderqt.cpp
    rawdatareaderdialogqt.cpp
    sliderwidgetqt.cpp
    spinboxrangesliderwidgetqt.cpp
    ordinaleditorwidget.cpp
)

ivw_group("Source Files" ${SOURCE_FILES})

#--------------------------------------------------------------------
# Define defintions
ivw_define_standard_definitions(IVW_QTWIDGETS)
ivw_define_qt_definitions()

#--------------------------------------------------------------------
# Create library
add_library(inviwo-qtwidgets ${SOURCE_FILES} ${MOC_FILES} ${HEADER_FILES})

# Add core dependencies to this list
ivw_add_dependencies(InviwoCore)

# Add qt dependencies to this list
if(DESIRED_QT_VERSION MATCHES 5)
    ivw_add_dependencies(Qt5Core Qt5Gui Qt5Widgets)    
else()
    ivw_add_dependencies(Qt)
endif()

#--------------------------------------------------------------------
# Optimize compilation with pre-compilied headers based on inviwo-core
ivw_compile_optimize_inviwo_core()

#--------------------------------------------------------------------
# Make package (for other projects to find)
ivw_make_package(InviwoQtWidgets inviwo-qtwidgets)
