#--------------------------------------------------------------------
# Inviwo GLUT Application
ivw_project(glutminimum)

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    glutminimum.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})
        
#--------------------------------------------------------------------
# Define defintions
ivw_define_standard_definitions(glutminimum)

#--------------------------------------------------------------------
# Define libraries that should be linked
ivw_retrieve_all_modules(package_list)
list(APPEND package_list InviwoCore)

#--------------------------------------------------------------------
# Remove Qt stuff from list
foreach(module ${package_list})
    string(TOUPPER ${module} u_module)
    if(u_module MATCHES "QT+")
        list(REMOVE_ITEM package_list ${module})
    endif()
endforeach()
# Remove GLFW stuff from list
list(REMOVE_ITEM package_list InviwoGLFWModule)

#--------------------------------------------------------------------
# Register the use of modules
ivw_register_use_of_modules(${package_list})

#--------------------------------------------------------------------
# Need to add dependent directories before creating application
ivw_add_dependency_directories(${package_list})

#--------------------------------------------------------------------
# Include generation directory to register modules etc
include_directories(${CMAKE_BINARY_DIR}/modules/_generated)

#--------------------------------------------------------------------
# Create application
add_executable(glutminimum MACOSX_BUNDLE WIN32 ${SOURCE_FILES})

#--------------------------------------------------------------------
# Add dependencies
ivw_add_dependencies(${package_list})

#--------------------------------------------------------------------
# Optimize compilation with pre-compilied headers based on inviwo-core
ivw_compile_optimize_inviwo_core()

#--------------------------------------------------------------------
# Set VS Executable properties
#ivw_vs_executable_setup(glutminimum)
