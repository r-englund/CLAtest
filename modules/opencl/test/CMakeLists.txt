ivw_project(opencl)

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    ${IVW_MODULE_DIR}/opencl/test/opencl.cpp
)

#--------------------------------------------------------------------
# Create test
ivw_create_test(${SOURCE_FILES})

#--------------------------------------------------------------------
# Add dependency
ivw_add_dependencies(InviwoOpenCLModule)
ivw_add_dependencies(InviwoBaseModule)
ivw_add_dependencies(InviwoOpenGLModule)
#ivw_add_dependencies(InviwoQtWidgets)

