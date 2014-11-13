#--------------------------------------------------------------------
# Inviwo Base OpenGL Module
ivw_module(BaseGL)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/background.h
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/drawlines.h
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/drawfreehand.h
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/entryexitpoints.h
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/findedges.h
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/firstivwprocessor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/imagegpuprocessor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/imageclassify.h
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/imagegrayscale.h
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/imagelayoutgl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/imagemixer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/lightingraycaster.h
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/lightvolumegl.h
	${CMAKE_CURRENT_SOURCE_DIR}/processors/multichannelraycaster.h
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/multiplanar.h
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/positionwidgetprocessor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/redgreenprocessor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/simpleraycaster.h
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/isoraycaster.h
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/volumeslicegl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/imageprocessing/imageinvert.h
)
ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/background.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/drawlines.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/drawfreehand.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/entryexitpoints.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/findedges.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/firstivwprocessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/imagegpuprocessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/imageclassify.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/imagegrayscale.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/imagelayoutgl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/imagemixer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/lightingraycaster.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/lightvolumegl.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/processors/multichannelraycaster.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/multiplanar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/positionwidgetprocessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/redgreenprocessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/simpleraycaster.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/isoraycaster.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/volumeslicegl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/imageprocessing/imageinvert.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})

#--------------------------------------------------------------------
# Add shaders
set(SHADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/glsl/background.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/glsl/capnearclipping.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/glsl/img_classify.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/glsl/img_findedges.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/glsl/img_graysc.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/glsl/img_mix.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/glsl/img_invert.frag
	${CMAKE_CURRENT_SOURCE_DIR}/glsl/multichannelraycaster.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/glsl/multiplanar.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/glsl/volumeslice.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/glsl/lighting/lightingraycasting.frag
	${CMAKE_CURRENT_SOURCE_DIR}/glsl/lighting/lightpropagation.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/glsl/lighting/lightpropagation.geom
    ${CMAKE_CURRENT_SOURCE_DIR}/glsl/lighting/lightpropagation.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/glsl/lighting/lightvolumeblend.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/glsl/lighting/lightvolumeblend.geom
    ${CMAKE_CURRENT_SOURCE_DIR}/glsl/lighting/lightvolumeblend.vert
)
ivw_group("Shader Files" ${SHADER_FILES})

#--------------------------------------------------------------------
# Create module
ivw_create_module(${SOURCE_FILES} ${HEADER_FILES} ${SHADER_FILES})

#--------------------------------------------------------------------
# Add shader directory to pack
ivw_add_to_module_pack(${CMAKE_CURRENT_SOURCE_DIR}/glsl)