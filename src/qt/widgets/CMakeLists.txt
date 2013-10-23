#--------------------------------------------------------------------
# Inviwo Qt Widgets
ivw_project(inviwo-qtwidgets)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/colorwheel.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/customsliderwidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/editablelabelqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/eventconverterqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/floatsliderwidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/intsliderwidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/inviwoapplicationqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/inviwoqtwidgetsdefine.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/mappingpopup.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/rangesliderqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/spinboxrangesliderwidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/processors/processorwidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/boolpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/buttonpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/camerapropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/collapsivegroupboxwidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/colorpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/compositepropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/eventpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/filepropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/floatmat2propertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/floatmat3propertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/floatmat4propertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/floatminmaxpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/floatpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/floatvec2propertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/floatvec3propertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/floatvec4propertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/htmleditorwidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/htmllistwidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/imageeditorwidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/intminmaxpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/intpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/intvec2propertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/intvec3propertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/intvec4propertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/optionpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/propertysettingswidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/propertywidgetfactoryqt.h
    #${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/shadewidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/stringpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/texteditorwidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/transferfunctionpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/transferfunctionpropertydialog.h
)

if(IVW_MODULE_PYTHON)
	list(APPEND HEADER_FILES  ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/pythoneditorwidget.h)
endif()

ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add MOC files
set(MOC_FILES
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/colorwheel.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/customsliderwidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/editablelabelqt.h 
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/floatsliderwidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/intsliderwidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/inviwoapplicationqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/mappingpopup.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/rangesliderqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/spinboxrangesliderwidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/processors/processorwidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/boolpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/buttonpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/collapsivegroupboxwidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/colorpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/camerapropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/compositepropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/eventpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/filepropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/floatmat2propertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/floatmat3propertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/floatmat4propertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/floatminmaxpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/floatpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/floatvec2propertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/floatvec3propertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/floatvec4propertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/htmleditorwidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/htmllistwidgetqt.h    
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/imageeditorwidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/intminmaxpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/intpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/intvec2propertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/intvec3propertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/intvec4propertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/optionpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/propertysettingswidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/propertywidgetqt.h
    #${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/shadewidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/stringpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/texteditorwidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/transferfunctionpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/transferfunctionpropertydialog.h
)


if(IVW_MODULE_PYTHON)
	list(APPEND MOC_FILES  ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/pythoneditorwidget.h)
endif()

ivw_qt_wrap_cpp(MOC_FILES ${MOC_FILES})
source_group("MOC Files" FILES ${MOC_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    colorwheel.cpp
    customsliderwidgetqt.cpp
    editablelabelqt.cpp
    eventconverterqt.cpp
    floatsliderwidgetqt.cpp
    intsliderwidgetqt.cpp
    inviwoapplicationqt.cpp
    mappingpopup.cpp
    rangesliderqt.cpp
    spinboxrangesliderwidgetqt.cpp
    processors/processorwidgetqt.cpp
    properties/boolpropertywidgetqt.cpp
    properties/buttonpropertywidgetqt.cpp  
    properties/collapsivegroupboxwidgetqt.cpp
    properties/colorpropertywidgetqt.cpp
    properties/camerapropertywidgetqt.cpp
    properties/compositepropertywidgetqt.cpp
    properties/eventpropertywidgetqt.cpp
    properties/filepropertywidgetqt.cpp
    properties/floatmat2propertywidgetqt.cpp
    properties/floatmat3propertywidgetqt.cpp
    properties/floatmat4propertywidgetqt.cpp
    properties/floatminmaxpropertywidgetqt.cpp
    properties/floatpropertywidgetqt.cpp
    properties/floatvec2propertywidgetqt.cpp
    properties/floatvec3propertywidgetqt.cpp
    properties/floatvec4propertywidgetqt.cpp
    properties/htmleditorwidgetqt.cpp
    properties/htmllistwidgetqt.cpp
    properties/imageeditorwidgetqt.cpp
    properties/intminmaxpropertywidgetqt.cpp
    properties/intpropertywidgetqt.cpp
    properties/intvec2propertywidgetqt.cpp
    properties/intvec3propertywidgetqt.cpp
    properties/intvec4propertywidgetqt.cpp    
    properties/optionpropertywidgetqt.cpp
    properties/propertysettingswidgetqt.cpp
    properties/propertywidgetfactoryqt.cpp
    properties/propertywidgetqt.cpp
    #properties/shadewidgetqt.cpp
    properties/stringpropertywidgetqt.cpp
    properties/texteditorwidgetqt.cpp
    properties/transferfunctionpropertydialog.cpp
    properties/transferfunctionpropertywidgetqt.cpp 
)

if(IVW_MODULE_PYTHON)
	list(APPEND SOURCE_FILES  pythoneditorwidget.cpp)
endif()

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
    ivw_add_dependencies(Qt5Core Qt5Gui Qt5Widgets Qt5Multimedia)    
else()
    ivw_add_dependencies(Qt)
endif()

#--------------------------------------------------------------------
# Optimize compilation with pre-compilied headers based on inviwo-core
ivw_compile_optimize_inviwo_core()

#--------------------------------------------------------------------
# Make package (for other projects to find)
ivw_make_package(InviwoQtWidgets inviwo-qtwidgets)
