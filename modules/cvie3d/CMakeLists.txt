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
# Add CVIE3D directories
ivw_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ext/CVIE3DSDK/include)
ivw_link_directories(${CMAKE_CURRENT_SOURCE_DIR}/ext/CVIE3DSDK/lib)

#--------------------------------------------------------------------
# Create module
ivw_create_module(${SOURCE_FILES} ${HEADER_FILES})

#--------------------------------------------------------------------
# Link CVIE3D lib
target_link_libraries(inviwo-module-cvie3d CVIE3D)