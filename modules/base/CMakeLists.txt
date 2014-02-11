#--------------------------------------------------------------------
# Inviwo Base Module
ivw_module(Base)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${IVW_MODULE_DIR}/base/processors/cubeproxygeometry.h
    ${IVW_MODULE_DIR}/base/processors/diffuselightsourceprocessor.h
    ${IVW_MODULE_DIR}/base/processors/directionallightsourceprocessor.h
    ${IVW_MODULE_DIR}/base/processors/imagesource.h
    ${IVW_MODULE_DIR}/base/processors/imagesourceseries.h
    ${IVW_MODULE_DIR}/base/processors/meshclipping.h
    ${IVW_MODULE_DIR}/base/processors/meshcreator.h
    ${IVW_MODULE_DIR}/base/processors/pointlightsourceprocessor.h
    ${IVW_MODULE_DIR}/base/processors/volumesource.h
    ${IVW_MODULE_DIR}/base/processors/volumeexport.h
    ${IVW_MODULE_DIR}/base/processors/volumebasistransformer.h
    ${IVW_MODULE_DIR}/base/processors/volumeslice.h
    ${IVW_MODULE_DIR}/base/processors/volumesubset.h
)
ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    ${IVW_MODULE_DIR}/base/processors/cubeproxygeometry.cpp
    ${IVW_MODULE_DIR}/base/processors/diffuselightsourceprocessor.cpp
    ${IVW_MODULE_DIR}/base/processors/directionallightsourceprocessor.cpp
    ${IVW_MODULE_DIR}/base/processors/imagesource.cpp
    ${IVW_MODULE_DIR}/base/processors/imagesourceseries.cpp
    ${IVW_MODULE_DIR}/base/processors/meshclipping.cpp
    ${IVW_MODULE_DIR}/base/processors/meshcreator.cpp
    ${IVW_MODULE_DIR}/base/processors/pointlightsourceprocessor.cpp
    ${IVW_MODULE_DIR}/base/processors/volumesource.cpp
    ${IVW_MODULE_DIR}/base/processors/volumeexport.cpp
    ${IVW_MODULE_DIR}/base/processors/volumebasistransformer.cpp
    ${IVW_MODULE_DIR}/base/processors/volumeslice.cpp
    ${IVW_MODULE_DIR}/base/processors/volumesubset.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})

#--------------------------------------------------------------------
# Create module
ivw_create_module(${SOURCE_FILES} ${HEADER_FILES})