#--------------------------------------------------------------------
# Inviwo OpenGL Module
ivw_module(OpenGL)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${IVW_MODULE_DIR}/opengl/canvasgl.h
    ${IVW_MODULE_DIR}/opengl/canvasprocessorgl.h
    ${IVW_MODULE_DIR}/opengl/clockgl.h
    ${IVW_MODULE_DIR}/opengl/glformats.h
    ${IVW_MODULE_DIR}/opengl/inviwoopengl.h
    ${IVW_MODULE_DIR}/opengl/openglcapabilities.h
    ${IVW_MODULE_DIR}/opengl/openglmodule.h
    ${IVW_MODULE_DIR}/opengl/portgroup.h
    ${IVW_MODULE_DIR}/opengl/processorgl.h
    ${IVW_MODULE_DIR}/opengl/textureobserver.h
    ${IVW_MODULE_DIR}/opengl/buffer/bufferglconverter.h
    ${IVW_MODULE_DIR}/opengl/buffer/elementbuffergl.h
    ${IVW_MODULE_DIR}/opengl/buffer/elementbufferglconverter.h
    ${IVW_MODULE_DIR}/opengl/buffer/buffergl.h
    ${IVW_MODULE_DIR}/opengl/geometry/geometrygl.h
    ${IVW_MODULE_DIR}/opengl/geometry/geometryrenderprocessorgl.h
    ${IVW_MODULE_DIR}/opengl/geometry/meshgl.h
    ${IVW_MODULE_DIR}/opengl/glwrap/bufferobject.h
    ${IVW_MODULE_DIR}/opengl/glwrap/bufferobjectarray.h
    ${IVW_MODULE_DIR}/opengl/glwrap/elementbufferobject.h
    ${IVW_MODULE_DIR}/opengl/glwrap/framebufferobject.h
    ${IVW_MODULE_DIR}/opengl/glwrap/renderbufferobject.h
    ${IVW_MODULE_DIR}/opengl/glwrap/shader.h
    ${IVW_MODULE_DIR}/opengl/glwrap/shadermanager.h
    ${IVW_MODULE_DIR}/opengl/glwrap/shaderobject.h
    ${IVW_MODULE_DIR}/opengl/glwrap/texture.h
    ${IVW_MODULE_DIR}/opengl/glwrap/texturecallback.h
    ${IVW_MODULE_DIR}/opengl/glwrap/texture2d.h
    ${IVW_MODULE_DIR}/opengl/glwrap/texture2darray.h
    ${IVW_MODULE_DIR}/opengl/glwrap/texture3d.h
    ${IVW_MODULE_DIR}/opengl/glwrap/textureunit.h
    ${IVW_MODULE_DIR}/opengl/image/imagegl.h
    ${IVW_MODULE_DIR}/opengl/image/layergl.h
    ${IVW_MODULE_DIR}/opengl/image/layerglconverter.h
    ${IVW_MODULE_DIR}/opengl/image/imageprocessorgl.h
    ${IVW_MODULE_DIR}/opengl/image/compositeprocessorgl.h
    ${IVW_MODULE_DIR}/opengl/rendering/meshrenderer.h
    ${IVW_MODULE_DIR}/opengl/volume/volumegl.h
    ${IVW_MODULE_DIR}/opengl/volume/volumeglconverter.h
    ${IVW_MODULE_DIR}/opengl/volume/volumeraycastergl.h
)
ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    ${IVW_MODULE_DIR}/opengl/clockgl.cpp
    ${IVW_MODULE_DIR}/opengl/canvasgl.cpp
    ${IVW_MODULE_DIR}/opengl/canvasprocessorgl.cpp
    ${IVW_MODULE_DIR}/opengl/inviwoopengl.cpp
    ${IVW_MODULE_DIR}/opengl/openglcapabilities.cpp
    ${IVW_MODULE_DIR}/opengl/openglmodule.cpp
    ${IVW_MODULE_DIR}/opengl/portgroup.cpp
    ${IVW_MODULE_DIR}/opengl/processorgl.cpp
    ${IVW_MODULE_DIR}/opengl/buffer/bufferglconverter.cpp
    ${IVW_MODULE_DIR}/opengl/buffer/elementbuffergl.cpp
    ${IVW_MODULE_DIR}/opengl/buffer/elementbufferglconverter.cpp
    ${IVW_MODULE_DIR}/opengl/buffer/buffergl.cpp
    ${IVW_MODULE_DIR}/opengl/geometry/geometrygl.cpp
    ${IVW_MODULE_DIR}/opengl/geometry/geometryrenderprocessorgl.cpp
    ${IVW_MODULE_DIR}/opengl/geometry/meshgl.cpp
    ${IVW_MODULE_DIR}/opengl/glwrap/bufferobject.cpp
    ${IVW_MODULE_DIR}/opengl/glwrap/bufferobjectarray.cpp
    ${IVW_MODULE_DIR}/opengl/glwrap/elementbufferobject.cpp
    ${IVW_MODULE_DIR}/opengl/glwrap/framebufferobject.cpp
    ${IVW_MODULE_DIR}/opengl/glwrap/renderbufferobject.cpp
    ${IVW_MODULE_DIR}/opengl/glwrap/shader.cpp
    ${IVW_MODULE_DIR}/opengl/glwrap/shadermanager.cpp
    ${IVW_MODULE_DIR}/opengl/glwrap/shaderobject.cpp
    ${IVW_MODULE_DIR}/opengl/glwrap/texture.cpp
    ${IVW_MODULE_DIR}/opengl/glwrap/texture2d.cpp
    ${IVW_MODULE_DIR}/opengl/glwrap/texture2darray.cpp
    ${IVW_MODULE_DIR}/opengl/glwrap/texture3d.cpp
    ${IVW_MODULE_DIR}/opengl/glwrap/textureunit.cpp
    ${IVW_MODULE_DIR}/opengl/image/imagegl.cpp
    ${IVW_MODULE_DIR}/opengl/image/layergl.cpp
    ${IVW_MODULE_DIR}/opengl/image/layerglconverter.cpp
    ${IVW_MODULE_DIR}/opengl/image/imageprocessorgl.cpp
    ${IVW_MODULE_DIR}/opengl/image/compositeprocessorgl.cpp
    ${IVW_MODULE_DIR}/opengl/rendering/meshrenderer.cpp
    ${IVW_MODULE_DIR}/opengl/volume/volumegl.cpp
    ${IVW_MODULE_DIR}/opengl/volume/volumeglconverter.cpp
    ${IVW_MODULE_DIR}/opengl/volume/volumeraycastergl.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})

#--------------------------------------------------------------------
# Add shaders
set(SHADER_FILES
    ${IVW_MODULE_DIR}/opengl/glsl/background.frag
    ${IVW_MODULE_DIR}/opengl/glsl/capnearclipping.frag
    ${IVW_MODULE_DIR}/opengl/glsl/composite.frag
    ${IVW_MODULE_DIR}/opengl/glsl/fontrendering_freetype.frag
    ${IVW_MODULE_DIR}/opengl/glsl/fontrendering_freetype.vert
    ${IVW_MODULE_DIR}/opengl/glsl/fontrendering_passthrough.frag
    ${IVW_MODULE_DIR}/opengl/glsl/geometryrendering.frag
    ${IVW_MODULE_DIR}/opengl/glsl/geometryrendering.vert
    ${IVW_MODULE_DIR}/opengl/glsl/img_classify.frag
    ${IVW_MODULE_DIR}/opengl/glsl/img_color.frag
    ${IVW_MODULE_DIR}/opengl/glsl/img_copy.frag
    ${IVW_MODULE_DIR}/opengl/glsl/img_findedges.frag
    ${IVW_MODULE_DIR}/opengl/glsl/img_graysc.frag
    ${IVW_MODULE_DIR}/opengl/glsl/img_identity.vert
    ${IVW_MODULE_DIR}/opengl/glsl/img_mix.frag
    ${IVW_MODULE_DIR}/opengl/glsl/img_noise.frag
    ${IVW_MODULE_DIR}/opengl/glsl/img_texturequad.frag
    ${IVW_MODULE_DIR}/opengl/glsl/img_texturequad.vert
    ${IVW_MODULE_DIR}/opengl/glsl/minimal.vert
    ${IVW_MODULE_DIR}/opengl/glsl/multiplanar.frag
    ${IVW_MODULE_DIR}/opengl/glsl/picking.frag
    ${IVW_MODULE_DIR}/opengl/glsl/raycasting.frag
    ${IVW_MODULE_DIR}/opengl/glsl/standard.frag
    ${IVW_MODULE_DIR}/opengl/glsl/standard.vert
    ${IVW_MODULE_DIR}/opengl/glsl/volumeslice.frag
    ${IVW_MODULE_DIR}/opengl/glsl/include/inc_classification.frag
    ${IVW_MODULE_DIR}/opengl/glsl/include/inc_compositing.frag
    ${IVW_MODULE_DIR}/opengl/glsl/include/inc_depth.frag
    ${IVW_MODULE_DIR}/opengl/glsl/include/inc_gradients.frag
    ${IVW_MODULE_DIR}/opengl/glsl/include/inc_raycasting.frag
    ${IVW_MODULE_DIR}/opengl/glsl/include/inc_sampler2d.frag
    ${IVW_MODULE_DIR}/opengl/glsl/include/inc_sampler3d.frag
    ${IVW_MODULE_DIR}/opengl/glsl/include/inc_shading.frag
    ${IVW_MODULE_DIR}/opengl/glsl/lighting/lightingraycasting.frag
	${IVW_MODULE_DIR}/opengl/glsl/lighting/lightpropagation.frag
    ${IVW_MODULE_DIR}/opengl/glsl/lighting/lightpropagation.geom
    ${IVW_MODULE_DIR}/opengl/glsl/lighting/lightpropagation.vert
    ${IVW_MODULE_DIR}/opengl/glsl/lighting/lightvolumeblend.frag
    ${IVW_MODULE_DIR}/opengl/glsl/lighting/lightvolumeblend.geom
    ${IVW_MODULE_DIR}/opengl/glsl/lighting/lightvolumeblend.vert
)
ivw_group("Shader Files" ${SHADER_FILES})

#--------------------------------------------------------------------
# Create module
ivw_create_module(${SOURCE_FILES} ${HEADER_FILES} ${SHADER_FILES})

#--------------------------------------------------------------------
# Add shader directory to pack
ivw_add_to_module_pack(${IVW_MODULE_DIR}/opengl/glsl)

#--------------------------------------------------------------------
# Build GLEW if it couldn't be found
if(BUILD_GLEW)
	add_subdirectory(ext/glew)
endif(BUILD_GLEW)