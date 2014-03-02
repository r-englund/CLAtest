#--------------------------------------------------------------------
# Inviwo OpenCL Module
ivw_module(OpenCL)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${IVW_MODULE_DIR}/opencl/buffer/buffercl.h
    ${IVW_MODULE_DIR}/opencl/buffer/bufferclbase.h
    ${IVW_MODULE_DIR}/opencl/buffer/bufferclconverter.h
    ${IVW_MODULE_DIR}/opencl/buffer/bufferclgl.h
    ${IVW_MODULE_DIR}/opencl/buffer/bufferclglconverter.h
    ${IVW_MODULE_DIR}/opencl/image/imagecl.h
    ${IVW_MODULE_DIR}/opencl/image/imageclgl.h
    ${IVW_MODULE_DIR}/opencl/image/layercl.h
	${IVW_MODULE_DIR}/opencl/image/layerclbase.h
    ${IVW_MODULE_DIR}/opencl/image/layerclgl.h
    ${IVW_MODULE_DIR}/opencl/image/layerclconverter.h
	${IVW_MODULE_DIR}/opencl/image/layerclglconverter.h
    ${IVW_MODULE_DIR}/opencl/image/layerclresizer.h
    ${IVW_MODULE_DIR}/opencl/processors/grayscalecl.h
    ${IVW_MODULE_DIR}/opencl/processors/volumeraycastercl.h
    ${IVW_MODULE_DIR}/opencl/settings/openclsettings.h
    ${IVW_MODULE_DIR}/opencl/volume/volumecl.h
	${IVW_MODULE_DIR}/opencl/volume/volumeclbase.h
    ${IVW_MODULE_DIR}/opencl/volume/volumeclconverter.h
    ${IVW_MODULE_DIR}/opencl/volume/volumeclgl.h
    ${IVW_MODULE_DIR}/opencl/volume/volumeclglconverter.h
    ${IVW_MODULE_DIR}/opencl/cl.hpp
    ${IVW_MODULE_DIR}/opencl/glmcl.h
	${IVW_MODULE_DIR}/opencl/inviwoopencl.h
	${IVW_MODULE_DIR}/opencl/kernelmanager.h
	${IVW_MODULE_DIR}/opencl/kernelowner.h
	${IVW_MODULE_DIR}/opencl/openclcapabilities.h
    ${IVW_MODULE_DIR}/opencl/openclmodule.h
    ${IVW_MODULE_DIR}/opencl/openclsharing.h
    ${IVW_MODULE_DIR}/opencl/syncclgl.h
)
ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    ${IVW_MODULE_DIR}/opencl/buffer/buffercl.cpp
    ${IVW_MODULE_DIR}/opencl/buffer/bufferclbase.cpp
    ${IVW_MODULE_DIR}/opencl/buffer/bufferclconverter.cpp
    ${IVW_MODULE_DIR}/opencl/buffer/bufferclgl.cpp
    ${IVW_MODULE_DIR}/opencl/buffer/bufferclglconverter.cpp

    ${IVW_MODULE_DIR}/opencl/image/imagecl.cpp
    ${IVW_MODULE_DIR}/opencl/image/imageclgl.cpp
    ${IVW_MODULE_DIR}/opencl/image/layercl.cpp
	${IVW_MODULE_DIR}/opencl/image/layerclbase.cpp
    ${IVW_MODULE_DIR}/opencl/image/layerclgl.cpp
	${IVW_MODULE_DIR}/opencl/image/layerclconverter.cpp
	${IVW_MODULE_DIR}/opencl/image/layerclglconverter.cpp
	${IVW_MODULE_DIR}/opencl/image/layerclresizer.cpp
    ${IVW_MODULE_DIR}/opencl/processors/grayscalecl.cpp
    ${IVW_MODULE_DIR}/opencl/processors/volumeraycastercl.cpp
    ${IVW_MODULE_DIR}/opencl/settings/openclsettings.cpp
    ${IVW_MODULE_DIR}/opencl/volume/volumecl.cpp
	${IVW_MODULE_DIR}/opencl/volume/volumeclbase.cpp
	${IVW_MODULE_DIR}/opencl/volume/volumeclconverter.cpp
	${IVW_MODULE_DIR}/opencl/volume/volumeclglconverter.cpp
	${IVW_MODULE_DIR}/opencl/volume/volumeclgl.cpp
    ${IVW_MODULE_DIR}/opencl/cl.cpp
    ${IVW_MODULE_DIR}/opencl/glmcl.cpp
    ${IVW_MODULE_DIR}/opencl/inviwoopencl.cpp
	${IVW_MODULE_DIR}/opencl/kernelmanager.cpp
	${IVW_MODULE_DIR}/opencl/kernelowner.cpp
    ${IVW_MODULE_DIR}/opencl/openclmodule.cpp
    ${IVW_MODULE_DIR}/opencl/openclcapabilities.cpp
    ${IVW_MODULE_DIR}/opencl/openclsharing.cpp
    ${IVW_MODULE_DIR}/opencl/syncclgl.cpp

)
ivw_group("Source Files" ${SOURCE_FILES})

#--------------------------------------------------------------------
# Add OpenCL files
set(SHADER_FILES
    ${IVW_MODULE_DIR}/opencl/cl/datastructures/bbox.cl
    ${IVW_MODULE_DIR}/opencl/cl/datastructures/lightsource.cl
    ${IVW_MODULE_DIR}/opencl/cl/intersection/rayboxintersection.cl
    ${IVW_MODULE_DIR}/opencl/cl/intersection/raydiscintersection.cl
    ${IVW_MODULE_DIR}/opencl/cl/intersection/rayplaneintersection.cl
    ${IVW_MODULE_DIR}/opencl/cl/intersection/raysphereintersection.cl
    ${IVW_MODULE_DIR}/opencl/cl/light/light.cl
    ${IVW_MODULE_DIR}/opencl/cl/shading/abcbrdf.cl
    ${IVW_MODULE_DIR}/opencl/cl/shading/ashikiminbrdf.cl
    ${IVW_MODULE_DIR}/opencl/cl/shading/blinnphongbrdf.cl
    ${IVW_MODULE_DIR}/opencl/cl/shading/cooktorrancebrdf.cl
    ${IVW_MODULE_DIR}/opencl/cl/shading/microfacet.cl
    ${IVW_MODULE_DIR}/opencl/cl/shading/phasefunctions.cl
    ${IVW_MODULE_DIR}/opencl/cl/shading/shading.cl
    ${IVW_MODULE_DIR}/opencl/cl/shading/shadingmath.cl
    ${IVW_MODULE_DIR}/opencl/cl/shading/wardbrdf.cl
    ${IVW_MODULE_DIR}/opencl/cl/gradients.cl
    ${IVW_MODULE_DIR}/opencl/cl/grayscale.cl
    ${IVW_MODULE_DIR}/opencl/cl/img_resize.cl
    ${IVW_MODULE_DIR}/opencl/cl/samplers.cl
    ${IVW_MODULE_DIR}/opencl/cl/transformations.cl
    ${IVW_MODULE_DIR}/opencl/cl/volumeraycaster.cl
)
ivw_group("Shader Files" ${SHADER_FILES})

#--------------------------------------------------------------------
# Add definition
ivw_add_definition(__CL_ENABLE_EXCEPTIONS)


set(TEST_FILES
	#${CMAKE_CURRENT_SOURCE_DIR}/test/opencl-test.h
)
ivw_group("Unittest Files" ${TEST_FILES})
ivw_add_unittest(${TEST_FILES})

#--------------------------------------------------------------------
# Create module
ivw_create_module(${SOURCE_FILES} ${HEADER_FILES} ${SHADER_FILES} ${TEST_FILES})


