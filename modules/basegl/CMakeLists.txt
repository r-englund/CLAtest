#--------------------------------------------------------------------
# Inviwo Base OpenGL Module
ivw_module(BaseGL)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${IVW_MODULE_DIR}/basegl/processors/background.h
    ${IVW_MODULE_DIR}/basegl/processors/drawlines.h
    ${IVW_MODULE_DIR}/basegl/processors/drawfreehand.h
    ${IVW_MODULE_DIR}/basegl/processors/entryexitpoints.h
    ${IVW_MODULE_DIR}/basegl/processors/findedges.h
    ${IVW_MODULE_DIR}/basegl/processors/firstivwprocessor.h
    ${IVW_MODULE_DIR}/basegl/processors/imageclassify.h
    ${IVW_MODULE_DIR}/basegl/processors/imagegrayscale.h
    ${IVW_MODULE_DIR}/basegl/processors/imagelayoutgl.h
    ${IVW_MODULE_DIR}/basegl/processors/imagemixer.h
    ${IVW_MODULE_DIR}/basegl/processors/lightingraycaster.h
    ${IVW_MODULE_DIR}/basegl/processors/lightvolumegl.h
    ${IVW_MODULE_DIR}/basegl/processors/multiplanar.h
    ${IVW_MODULE_DIR}/basegl/processors/positionwidgetprocessor.h
    ${IVW_MODULE_DIR}/basegl/processors/redgreenprocessor.h
    ${IVW_MODULE_DIR}/basegl/processors/simpleraycaster.h
    ${IVW_MODULE_DIR}/basegl/processors/volumeslicegl.h
)
ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    ${IVW_MODULE_DIR}/basegl/processors/background.cpp
    ${IVW_MODULE_DIR}/basegl/processors/drawlines.cpp
    ${IVW_MODULE_DIR}/basegl/processors/drawfreehand.cpp
    ${IVW_MODULE_DIR}/basegl/processors/entryexitpoints.cpp
    ${IVW_MODULE_DIR}/basegl/processors/findedges.cpp
    ${IVW_MODULE_DIR}/basegl/processors/firstivwprocessor.cpp
    ${IVW_MODULE_DIR}/basegl/processors/imageclassify.cpp
    ${IVW_MODULE_DIR}/basegl/processors/imagegrayscale.cpp
    ${IVW_MODULE_DIR}/basegl/processors/imagelayoutgl.cpp
    ${IVW_MODULE_DIR}/basegl/processors/imagemixer.cpp
    ${IVW_MODULE_DIR}/basegl/processors/lightingraycaster.cpp
    ${IVW_MODULE_DIR}/basegl/processors/lightvolumegl.cpp
    ${IVW_MODULE_DIR}/basegl/processors/multiplanar.cpp
    ${IVW_MODULE_DIR}/basegl/processors/positionwidgetprocessor.cpp
    ${IVW_MODULE_DIR}/basegl/processors/redgreenprocessor.cpp
    ${IVW_MODULE_DIR}/basegl/processors/simpleraycaster.cpp
    ${IVW_MODULE_DIR}/basegl/processors/volumeslicegl.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})

#--------------------------------------------------------------------
# Add shaders
set(SHADER_FILES
    ${IVW_MODULE_DIR}/basegl/glsl/background.frag
    ${IVW_MODULE_DIR}/basegl/glsl/capnearclipping.frag
    ${IVW_MODULE_DIR}/basegl/glsl/img_classify.frag
    ${IVW_MODULE_DIR}/basegl/glsl/img_findedges.frag
    ${IVW_MODULE_DIR}/basegl/glsl/img_graysc.frag
    ${IVW_MODULE_DIR}/basegl/glsl/img_mix.frag
    ${IVW_MODULE_DIR}/basegl/glsl/multiplanar.frag
    ${IVW_MODULE_DIR}/basegl/glsl/volumeslice.frag
    ${IVW_MODULE_DIR}/basegl/glsl/lighting/lightingraycasting.frag
	${IVW_MODULE_DIR}/basegl/glsl/lighting/lightpropagation.frag
    ${IVW_MODULE_DIR}/basegl/glsl/lighting/lightpropagation.geom
    ${IVW_MODULE_DIR}/basegl/glsl/lighting/lightpropagation.vert
    ${IVW_MODULE_DIR}/basegl/glsl/lighting/lightvolumeblend.frag
    ${IVW_MODULE_DIR}/basegl/glsl/lighting/lightvolumeblend.geom
    ${IVW_MODULE_DIR}/basegl/glsl/lighting/lightvolumeblend.vert
)
ivw_group("Shader Files" ${SHADER_FILES})

#--------------------------------------------------------------------
# Create module
ivw_create_module(${SOURCE_FILES} ${HEADER_FILES})

#--------------------------------------------------------------------
# Add shader directory to pack
ivw_add_to_module_pack(${IVW_MODULE_DIR}/basegl/glsl)