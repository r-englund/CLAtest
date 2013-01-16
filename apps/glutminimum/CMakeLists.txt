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
# Need to add dependent directories before creating application
ivw_add_dependency_directories(${package_list})

#--------------------------------------------------------------------
# Create application
add_executable(glutminimum WIN32 ${SOURCE_FILES})

#--------------------------------------------------------------------
# Add dependencies
ivw_add_dependencies(${package_list})

#--------------------------------------------------------------------
# Set VS Executable properties
ivw_vs_executable_setup(glutminimum)
