#--------------------------------------------------------------------
# Inviwo Base OpenCL Module
ivw_module(BaseCL)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${IVW_MODULE_DIR}/basecl/processors/entryexitpointscl.h
    ${IVW_MODULE_DIR}/basecl/processors/volumefirsthitcl.h
	${IVW_MODULE_DIR}/basecl/processors/volumemaxcl.h
)
ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    ${IVW_MODULE_DIR}/basecl/processors/entryexitpointscl.cpp
    ${IVW_MODULE_DIR}/basecl/processors/volumefirsthitcl.cpp
	${IVW_MODULE_DIR}/basecl/processors/volumemaxcl.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})

#--------------------------------------------------------------------
# Add OpenCL files
set(SHADER_FILES
    ${IVW_MODULE_DIR}/basecl/cl/entryexitpoints.cl
    ${IVW_MODULE_DIR}/basecl/cl/volumefirsthit.cl
	${IVW_MODULE_DIR}/basecl/cl/volumemax.cl
)
ivw_group("Shader Files" ${SHADER_FILES})

#--------------------------------------------------------------------
# Add definition
ivw_add_definition(__CL_ENABLE_EXCEPTIONS)

#--------------------------------------------------------------------
# Create module
ivw_create_module(${SOURCE_FILES} ${HEADER_FILES} ${SHADER_FILES})