#--------------------------------------------------------------------
# Inviwo Qt Application
ivw_project(inviwo)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${CMAKE_SOURCE_DIR}/apps/inviwo/inviwomainwindow.h
    ${CMAKE_SOURCE_DIR}/apps/inviwo/inviwosplashscreen.h
)
ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add header files
set(MOC_FILES
     ${CMAKE_SOURCE_DIR}/apps/inviwo/inviwomainwindow.h
     ${CMAKE_SOURCE_DIR}/apps/inviwo/inviwosplashscreen.h
)

if(DESIRED_QT_VERSION MATCHES 5)
    qt5_wrap_cpp(MOC_FILES ${MOC_FILES})    
else()
    qt4_wrap_cpp(MOC_FILES ${MOC_FILES})
endif()

source_group("MOC Files" FILES ${MOC_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/apps/inviwo/inviwo.cpp
    ${CMAKE_SOURCE_DIR}/apps/inviwo/inviwomainwindow.cpp
    ${CMAKE_SOURCE_DIR}/apps/inviwo/inviwosplashscreen.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})

source_group("Resource Files" FILES ${CMAKE_SOURCE_DIR}/resources/inviwo.qrc)

#--------------------------------------------------------------------
# Add resource file
if(DESIRED_QT_VERSION MATCHES 5)
    qt5_add_resources(QRC_FILE "${CMAKE_SOURCE_DIR}/resources/inviwo.qrc")
else()
    qt4_add_resources(QRC_FILE "${CMAKE_SOURCE_DIR}/resources/inviwo.qrc")
endif()
        
#--------------------------------------------------------------------
# Define defintions
ivw_define_standard_definitions(inviwo)

#--------------------------------------------------------------------
# Define libraries that should be linked
ivw_retrieve_all_modules(package_list)
list(APPEND package_list InviwoQtEditor)
list(APPEND package_list InviwoCore)

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
add_executable(inviwo MACOSX_BUNDLE WIN32 ${SOURCE_FILES} ${HEADER_FILES} ${MOC_FILES} ${QRC_FILE})

#--------------------------------------------------------------------
# Add dependencies
ivw_add_dependencies(${package_list})

#--------------------------------------------------------------------
# Optimize compilation with pre-compilied headers based on inviwo-core
ivw_compile_optimize_inviwo_core()

if(DESIRED_QT_VERSION MATCHES 5)
    qt5_use_modules(inviwo Widgets Gui OpenGL Multimedia Network Script)    
else()
    #qt4_use_modules(inviwo Core Gui Network Script)    
endif()