#--------------------------------------------------------------------
# Inviwo PyPackages Module
ivw_module(PyPackages)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${IVW_MODULE_DIR}/pypackages/pypackagesmodule.h
    ${IVW_MODULE_DIR}/pypackages/pypackagesmoduledefine.h
    ${IVW_MODULE_DIR}/pypackages/pyscriptrunner.h
    ${IVW_MODULE_DIR}/pypackages/pypackagesinterface/pypackagesutil.h
	${IVW_MODULE_DIR}/pypackages/pypackagesinterface/pypackageparsers.h
	${IVW_MODULE_DIR}/pypackages/processors/pyprocessorbase.h
    ${IVW_MODULE_DIR}/pypackages/processors/pycuda/pycudaimageinvert.h
	${IVW_MODULE_DIR}/pypackages/processors/numpy/numpybuffertest.h
)
ivw_group("Header Files" ${HEADER_FILES})

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
)
ivw_group("Source Files" ${SOURCE_FILES})

#--------------------------------------------------------------------
# Create module
ivw_create_module(${SOURCE_FILES} ${HEADER_FILES})