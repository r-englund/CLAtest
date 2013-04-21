#--------------------------------------------------------------------
# Inviwo Qt Widgets
ivw_project(inviwo-qtwidgets)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/canvasqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/eventconverterqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/floatsliderwidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/intsliderwidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/inviwoapplicationqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/inviwoqtwidgetsdefine.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/mappingpopup.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/processors/canvasprocessorwidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/processors/processorwidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/processors/processorwidgetfactoryqt.h
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
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/floatpropertywidgetqt.h
	${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/floatvec2propertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/floatvec3propertywidgetqt.h
	${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/floatvec4propertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/intpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/intvec2propertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/intvec3propertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/intvec4propertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/optionpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/propertysettingswidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/propertywidgetfactoryqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/propertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/snapshotpropertywidgetqt.h
    #${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/shadewidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/stringpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/transferfunctionpropertywidgetqt.h
)
ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add MOC files
set(MOC_FILES
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/floatsliderwidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/intsliderwidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/inviwoapplicationqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/mappingpopup.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/processors/canvasprocessorwidgetqt.h
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
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/floatpropertywidgetqt.h
	${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/floatvec2propertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/floatvec3propertywidgetqt.h
	${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/floatvec4propertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/intpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/intvec2propertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/intvec3propertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/intvec4propertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/optionpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/propertysettingswidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/snapshotpropertywidgetqt.h
    #${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/shadewidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/stringpropertywidgetqt.h
    ${IVW_INCLUDE_DIR}/inviwo/qt/widgets/properties/transferfunctionpropertywidgetqt.h
)
qt4_wrap_cpp(MOC_FILES ${MOC_FILES})
source_group("MOC Files" FILES ${MOC_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    canvasqt.cpp
    eventconverterqt.cpp
    floatsliderwidgetqt.cpp
    intsliderwidgetqt.cpp
    inviwoapplicationqt.cpp
    mappingpopup.cpp
    processors/canvasprocessorwidgetqt.cpp
    processors/processorwidgetqt.cpp
    processors/processorwidgetfactoryqt.cpp
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
    properties/floatpropertywidgetqt.cpp
	properties/floatvec2propertywidgetqt.cpp
    properties/floatvec3propertywidgetqt.cpp
	properties/floatvec4propertywidgetqt.cpp
    properties/intpropertywidgetqt.cpp
    properties/intvec2propertywidgetqt.cpp
    properties/intvec3propertywidgetqt.cpp
    properties/intvec4propertywidgetqt.cpp
    properties/optionpropertywidgetqt.cpp
    properties/propertysettingswidgetqt.cpp
    properties/propertywidgetfactoryqt.cpp
    properties/propertywidgetqt.cpp
    properties/snapshotpropertywidgetqt.cpp
    #properties/shadewidgetqt.cpp
    properties/stringpropertywidgetqt.cpp
    properties/transferfunctionpropertywidgetqt.cpp
    
)
ivw_group("Source Files" ${SOURCE_FILES})

#--------------------------------------------------------------------
# Define defintions
ivw_define_standard_definitions(IVW_QTWIDGETS)
add_definitions(-DQT_CORE_LIB
                      -DQT_GUI_LIB
                      -DQT_OPENGL_LIB
                      -DQT_DLL
                      -DQT_THREAD_SUPPORT)

#--------------------------------------------------------------------
# Create library
add_library(inviwo-qtwidgets ${SOURCE_FILES} ${MOC_FILES} ${HEADER_FILES})

# Add core dependencies to this list
ivw_add_dependencies(
    InviwoCore
    Qt
)

#--------------------------------------------------------------------
# Optimize compilation with pre-compilied headers based on inviwo-core
ivw_compile_optimize_inviwo_core()

#--------------------------------------------------------------------
# Make package (for other projects to find)
ivw_make_package(InviwoQtWidgets inviwo-qtwidgets)
