#--------------------------------------------------------------------
# CVIE3D Module
ivw_module(CVIE3D)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${IVW_MODULE_DIR}/cvie3d/cvie3dprocessor.h
)
ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    ${IVW_MODULE_DIR}/cvie3d/cvie3dprocessor.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})

#--------------------------------------------------------------------
# Create module
ivw_create_module(${SOURCE_FILES} ${HEADER_FILES})