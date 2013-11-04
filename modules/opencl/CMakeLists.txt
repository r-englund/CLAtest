#--------------------------------------------------------------------
# Inviwo OpenCL Module
ivw_module(OpenCL)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${IVW_MODULE_DIR}/opencl/buffer/buffercl.h
    ${IVW_MODULE_DIR}/opencl/buffer/bufferclconverter.h
    ${IVW_MODULE_DIR}/opencl/buffer/bufferclgl.h
    ${IVW_MODULE_DIR}/opencl/buffer/bufferclglconverter.h
    ${IVW_MODULE_DIR}/opencl/cl.hpp
    ${IVW_MODULE_DIR}/opencl/glmcl.h
    ${IVW_MODULE_DIR}/opencl/image/imagecl.h
    ${IVW_MODULE_DIR}/opencl/image/imageclconverter.h
    ${IVW_MODULE_DIR}/opencl/image/imageclgl.h
	${IVW_MODULE_DIR}/opencl/image/imageclglconverter.h
    ${IVW_MODULE_DIR}/opencl/image/imageclresizer.h
	${IVW_MODULE_DIR}/opencl/inviwoopencl.h
	${IVW_MODULE_DIR}/opencl/kernelmanager.h
	${IVW_MODULE_DIR}/opencl/openclcapabilities.h
    ${IVW_MODULE_DIR}/opencl/openclmodule.h
	${IVW_MODULE_DIR}/opencl/processors/grayscalecl.h
    ${IVW_MODULE_DIR}/opencl/processors/volumeraycastercl.h
    ${IVW_MODULE_DIR}/opencl/volume/volumecl.h
    ${IVW_MODULE_DIR}/opencl/volume/volumeclconverter.h
    ${IVW_MODULE_DIR}/opencl/volume/volumeclgl.h
    ${IVW_MODULE_DIR}/opencl/volume/volumeclglconverter.h
    ${IVW_MODULE_DIR}/opencl/syncclgl.h
	${IVW_MODULE_DIR}/opencl/cl/grayscale.cl
	${IVW_MODULE_DIR}/opencl/cl/img_resize.cl
	${IVW_MODULE_DIR}/opencl/cl/volumeraycaster.cl
)
ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    ${IVW_MODULE_DIR}/opencl/buffer/buffercl.cpp
    ${IVW_MODULE_DIR}/opencl/buffer/bufferclconverter.cpp
    ${IVW_MODULE_DIR}/opencl/buffer/bufferclgl.cpp
    ${IVW_MODULE_DIR}/opencl/buffer/bufferclglconverter.cpp
    ${IVW_MODULE_DIR}/opencl/cl.cpp
    ${IVW_MODULE_DIR}/opencl/glmcl.cpp
    ${IVW_MODULE_DIR}/opencl/image/imagecl.cpp
    ${IVW_MODULE_DIR}/opencl/image/imageclgl.cpp
	${IVW_MODULE_DIR}/opencl/image/imageclconverter.cpp
	${IVW_MODULE_DIR}/opencl/image/imageclglconverter.cpp
	${IVW_MODULE_DIR}/opencl/image/imageclresizer.cpp
    ${IVW_MODULE_DIR}/opencl/inviwoopencl.cpp
	${IVW_MODULE_DIR}/opencl/kernelmanager.cpp
    ${IVW_MODULE_DIR}/opencl/openclmodule.cpp
    ${IVW_MODULE_DIR}/opencl/openclcapabilities.cpp
	${IVW_MODULE_DIR}/opencl/processors/grayscalecl.cpp
    ${IVW_MODULE_DIR}/opencl/processors/volumeraycastercl.cpp
    ${IVW_MODULE_DIR}/opencl/volume/volumecl.cpp
	${IVW_MODULE_DIR}/opencl/volume/volumeclconverter.cpp
	${IVW_MODULE_DIR}/opencl/volume/volumeclglconverter.cpp
	${IVW_MODULE_DIR}/opencl/volume/volumeclgl.cpp

)
ivw_group("Source Files" ${SOURCE_FILES})

#--------------------------------------------------------------------
# Add definition
ivw_add_definition(__CL_ENABLE_EXCEPTIONS)

#--------------------------------------------------------------------
# Create module
ivw_create_module(${SOURCE_FILES} ${HEADER_FILES} ${SHADER_FILES})

#--------------------------------------------------------------------
# Add tests
if(TEST_APPLICATIONS)
    add_subdirectory(test)
endif()

