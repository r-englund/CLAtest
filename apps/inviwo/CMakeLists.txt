#--------------------------------------------------------------------
# Inviwo Qt Application
ivw_project(inviwo)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    inviwomainwindow.h
)
qt4_wrap_cpp(MOC_FILES ${HEADER_FILES})
ivw_group("Header Files" ${HEADER_FILES})
source_group("MOC Files" FILES ${MOC_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    inviwo.cpp
    inviwomainwindow.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})

#--------------------------------------------------------------------
# Add resource file
qt4_add_resources(QRC_FILE ${CMAKE_SOURCE_DIR}/resources/inviwo.qrc)
source_group("Resource Files" FILES ${CMAKE_SOURCE_DIR}/resources/inviwo.qrc)
        
#--------------------------------------------------------------------
# Define defintions
ivw_define_standard_definitions(inviwo)

#--------------------------------------------------------------------
# Define libraries that should be linked
ivw_retrieve_all_modules(package_list)
list(APPEND package_list InviwoQtEditor)

#--------------------------------------------------------------------
# Need to add dependent directories before creating application
ivw_add_dependency_directories(${package_list})

#--------------------------------------------------------------------
# Create application
add_executable(inviwo MACOSX_BUNDLE WIN32 ${SOURCE_FILES} ${HEADER_FILES} ${MOC_FILES} ${QRC_FILE})

#--------------------------------------------------------------------
# Add dependencies
ivw_add_dependencies(${package_list})

#--------------------------------------------------------------------
# Set VS Executable properties
ivw_vs_executable_setup(inviwo)
