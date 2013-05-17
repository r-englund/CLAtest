#--------------------------------------------------------------------
# Inviwo OpenCL Module
ivw_module(OpenCL)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${IVW_MODULE_DIR}/opencl/cl.hpp
    ${IVW_MODULE_DIR}/opencl/glmcl.h
    ${IVW_MODULE_DIR}/opencl/imagecl.h
    ${IVW_MODULE_DIR}/opencl/imageclconverter.h
    ${IVW_MODULE_DIR}/opencl/imageclresizer.h
	${IVW_MODULE_DIR}/opencl/inviwoopencl.h
	${IVW_MODULE_DIR}/opencl/kernelmanager.h
	${IVW_MODULE_DIR}/opencl/openclinfo.h
    ${IVW_MODULE_DIR}/opencl/openclmodule.h
    ${IVW_MODULE_DIR}/opencl/processors/volumeraycastercl.h
    ${IVW_MODULE_DIR}/opencl/volumecl.h
    ${IVW_MODULE_DIR}/opencl/volumeclconverter.h
	${IVW_MODULE_DIR}/opencl/cl/img_resize.cl
	${IVW_MODULE_DIR}/opencl/cl/volumeraycaster.cl
)
ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    ${IVW_MODULE_DIR}/opencl/cl.cpp
    ${IVW_MODULE_DIR}/opencl/glmcl.cpp
    ${IVW_MODULE_DIR}/opencl/imagecl.cpp
	${IVW_MODULE_DIR}/opencl/imageclconverter.cpp
	${IVW_MODULE_DIR}/opencl/imageclresizer.cpp
    ${IVW_MODULE_DIR}/opencl/inviwoopencl.cpp
	${IVW_MODULE_DIR}/opencl/kernelmanager.cpp
    ${IVW_MODULE_DIR}/opencl/openclmodule.cpp
    ${IVW_MODULE_DIR}/opencl/openclinfo.cpp
    ${IVW_MODULE_DIR}/opencl/processors/volumeraycastercl.cpp
    ${IVW_MODULE_DIR}/opencl/volumecl.cpp
	${IVW_MODULE_DIR}/opencl/volumeclconverter.cpp

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

