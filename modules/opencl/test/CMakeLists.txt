ivw_project(opencl)

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    ${IVW_MODULE_DIR}/opencl/test/opencl.cpp
)

#--------------------------------------------------------------------
# Define libraries that should be linked
ivw_retrieve_all_modules(package_list)

#--------------------------------------------------------------------
# Need to add dependent directories before creating application
ivw_add_dependency_directories(${package_list})

#--------------------------------------------------------------------
# Create test
ivw_create_test(${SOURCE_FILES})

#--------------------------------------------------------------------
# Add dependencies
ivw_add_dependencies(${package_list})
