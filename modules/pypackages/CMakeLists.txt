#--------------------------------------------------------------------
# Inviwo PyPackages Module
ivw_module(PyPackages)

#--------------------------------------------------------------------
# Add MOC files
set(MOC_FILES
    ${IVW_MODULE_DIR}/pypackages/widgets/pypackagemanagerwidget.h
)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${IVW_MODULE_DIR}/pypackages/pypackagesmodule.h
    ${IVW_MODULE_DIR}/pypackages/pypackagesmoduledefine.h
    ${IVW_MODULE_DIR}/pypackages/pyscriptrunner.h
	${IVW_MODULE_DIR}/pypackages/pypackagesformatsmacro.h
    ${IVW_MODULE_DIR}/pypackages/pypackagesinterface/pypackagesutil.h
	${IVW_MODULE_DIR}/pypackages/pypackagesinterface/pypackageparsers.h
	${IVW_MODULE_DIR}/pypackages/processors/pyprocessorbase.h
    ${IVW_MODULE_DIR}/pypackages/processors/pycuda/pycudaimageinvert.h
	${IVW_MODULE_DIR}/pypackages/processors/numpy/numpybuffertest.h
	${IVW_MODULE_DIR}/pypackages/processors/numpy/numpyimagecontour.h
	${IVW_MODULE_DIR}/pypackages/processors/numpy/numpyvolumehistogram.h
	#widgets
	${IVW_MODULE_DIR}/pypackages/widgets/pypackagemanagerwidget.h
	${IVW_MODULE_DIR}/pypackages/widgets/pypackagemenu.h
	${MOC_FILES}
)
ivw_group("Header Files" ${HEADER_FILES})

ivw_qt_wrap_cpp(MOC_FILES ${MOC_FILES})
source_group("MOC Files" FILES ${MOC_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES    
	${IVW_MODULE_DIR}/pypackages/pypackagesmodule.cpp
    ${IVW_MODULE_DIR}/pypackages/pyscriptrunner.cpp
    ${IVW_MODULE_DIR}/pypackages/pypackagesinterface/pypackagesutil.cpp
	${IVW_MODULE_DIR}/pypackages/pypackagesinterface/pypackageparsers.cpp
	${IVW_MODULE_DIR}/pypackages/processors/pyprocessorbase.cpp
    ${IVW_MODULE_DIR}/pypackages/processors/pycuda/pycudaimageinvert.cpp
	${IVW_MODULE_DIR}/pypackages/processors/numpy/numpybuffertest.cpp
	${IVW_MODULE_DIR}/pypackages/processors/numpy/numpyimagecontour.cpp
	${IVW_MODULE_DIR}/pypackages/processors/numpy/numpyvolumehistogram.cpp
	#widgets
	${IVW_MODULE_DIR}/pypackages/widgets/pypackagemanagerwidget.cpp
	${IVW_MODULE_DIR}/pypackages/widgets/pypackagemenu.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})


ivw_define_qt_definitions()
ivw_add_definition(IVW_PYPACKAGES)

#--------------------------------------------------------------------
# Create module
ivw_create_module(${SOURCE_FILES} ${MOC_FILES} ${HEADER_FILES})