#--------------------------------------------------------------------
# Inviwo Base Module
ivw_module(Base)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${IVW_MODULE_DIR}/base/processors/background.h
    ${IVW_MODULE_DIR}/base/processors/cubeproxygeometry.h
    ${IVW_MODULE_DIR}/base/processors/drawlinesprocessor.h
    ${IVW_MODULE_DIR}/base/processors/entryexitpoints.h
    ${IVW_MODULE_DIR}/base/processors/findedges.h
    ${IVW_MODULE_DIR}/base/processors/firstivwprocessor.h
    ${IVW_MODULE_DIR}/base/processors/imagegrayscale.h
    ${IVW_MODULE_DIR}/base/processors/imagemixer.h
    ${IVW_MODULE_DIR}/base/processors/imagesource.h
    ${IVW_MODULE_DIR}/base/processors/imagesourceseries.h
    ${IVW_MODULE_DIR}/base/processors/meshclipping.h
    ${IVW_MODULE_DIR}/base/processors/meshcreator.h
    ${IVW_MODULE_DIR}/base/processors/parallelepipedproxygeometry.h
    ${IVW_MODULE_DIR}/base/processors/positionwidgetprocessor.h
    ${IVW_MODULE_DIR}/base/processors/redgreenprocessor.h
    ${IVW_MODULE_DIR}/base/processors/simpleraycaster.h
    ${IVW_MODULE_DIR}/base/processors/testprocessor.h
    ${IVW_MODULE_DIR}/base/processors/transferfunctiontestprocessor.h
	${IVW_MODULE_DIR}/base/processors/volumebasistransformer.h
    ${IVW_MODULE_DIR}/base/processors/volumesplit.h
    ${IVW_MODULE_DIR}/base/processors/volumesplitcompositor.h
    ${IVW_MODULE_DIR}/base/processors/volumesplitimagecompositor.h
    ${IVW_MODULE_DIR}/base/processors/volumesubset.h
)
ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    ${IVW_MODULE_DIR}/base/processors/background.cpp
    ${IVW_MODULE_DIR}/base/processors/cubeproxygeometry.cpp
    ${IVW_MODULE_DIR}/base/processors/drawlinesprocessor.cpp
    ${IVW_MODULE_DIR}/base/processors/entryexitpoints.cpp
    ${IVW_MODULE_DIR}/base/processors/findedges.cpp
    ${IVW_MODULE_DIR}/base/processors/firstivwprocessor.cpp
    ${IVW_MODULE_DIR}/base/processors/imagegrayscale.cpp
    ${IVW_MODULE_DIR}/base/processors/imagemixer.cpp
    ${IVW_MODULE_DIR}/base/processors/imagesource.cpp
    ${IVW_MODULE_DIR}/base/processors/imagesourceseries.cpp
    ${IVW_MODULE_DIR}/base/processors/meshclipping.cpp
    ${IVW_MODULE_DIR}/base/processors/meshcreator.cpp
    ${IVW_MODULE_DIR}/base/processors/parallelepipedproxygeometry.cpp
    ${IVW_MODULE_DIR}/base/processors/positionwidgetprocessor.cpp
    ${IVW_MODULE_DIR}/base/processors/redgreenprocessor.cpp
    ${IVW_MODULE_DIR}/base/processors/simpleraycaster.cpp
    ${IVW_MODULE_DIR}/base/processors/testprocessor.cpp
    ${IVW_MODULE_DIR}/base/processors/transferfunctiontestprocessor.cpp
	${IVW_MODULE_DIR}/base/processors/volumebasistransformer.cpp
    ${IVW_MODULE_DIR}/base/processors/volumesplit.cpp
    ${IVW_MODULE_DIR}/base/processors/volumesplitcompositor.cpp
    ${IVW_MODULE_DIR}/base/processors/volumesplitimagecompositor.cpp
    ${IVW_MODULE_DIR}/base/processors/volumesubset.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})

#--------------------------------------------------------------------
# Create module
ivw_create_module(${SOURCE_FILES} ${HEADER_FILES})