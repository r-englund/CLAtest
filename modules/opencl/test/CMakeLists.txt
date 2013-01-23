ivw_project(opencl)

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    ${IVW_MODULE_DIR}/opencl/test/opencl.cpp

)
ivw_group("Source Files" ${SOURCE_FILES})

#--------------------------------------------------------------------
# Create test
ivw_create_test(${SOURCE_FILES})

#--------------------------------------------------------------------
# Add dependency
ivw_add_dependencies(InviwoOpenCLModule)

