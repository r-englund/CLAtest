# Interactive Visualization Workshop (Inviwo)
if(DESIRED_QT_VERSION MATCHES 5)
cmake_minimum_required(VERSION 2.8.11)
cmake_policy(VERSION 2.8.11)
else()
cmake_minimum_required(VERSION 2.8.4)
cmake_policy(VERSION 2.8.4)
endif()
project(inviwo-projects)

# Build
option(SHARED_LIBS "Build shared libs, else static libs" ON)

include(cmake/globalconfig.cmake)

#--------------------------------------------------------------------
# Default setup of options for the build

option(IVW_SIGAR "Use sigar to detect system capabilities" ON)

# Modules
build_module(Base)
build_module(BaseGL)
build_module(OpenGL)
build_module(OpenGLQt)

# Applications/Qt
option(IVW_QT_APPLICATION "Build Inviwo Qt Application" ON)
option(IVW_QT_EDITOR "Build Inviwo Qt Editor" ON)
option(IVW_QT_WIDGETS "Build Inviwo Qt Widgets" ON)

option(IVW_TINY_GLFW_APPLICATION "Build Inviwo Tiny GLFW Application" OFF)
build_module_dependency(GLFW IVW_TINY_GLFW_APPLICATION)

option(IVW_TINY_GLUT_APPLICATION "Build Inviwo Tiny GLUT Application" OFF)
build_module_dependency(GLUT IVW_TINY_GLUT_APPLICATION)

option(IVW_UNIT_TEST_APPLICATION "Build Inviwo Unit tests Application" OFF)

#--------------------------------------------------------------------
# Add externals
add_subdirectory(ext/ticpp)
add_subdirectory(ext/tinythread) 
if(IVW_SIGAR)
add_subdirectory(ext/sigar)
endif()
add_subdirectory(ext/freeimage)
if(WIN32)
add_subdirectory(ext/stackwalker)
endif()

#--------------------------------------------------------------------
# Add Core
add_subdirectory(src/core)

#--------------------------------------------------------------------
# Add Qt
add_subdirectory(src/qt)

#--------------------------------------------------------------------
# Add modules
begin_add_modules()
add_internal_modules()
add_external_modules()
end_add_modules()

#--------------------------------------------------------------------
# Add applications
add_subdirectory(apps)

#--------------------------------------------------------------------
# Package creation
if(IVW_PACKAGE_PROJECT)
set(CPACK_PACKAGE_NAME "Inviwo")
set(CPACK_PACKAGE_VENDOR "Inviwo Organization")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Interactive Visualization Workshop")
set(CPACK_PACKAGE_VERSION_MAJOR "${IVW_MAJOR_VERSION}")
set(CPACK_PACKAGE_VERSION_MINOR "${IVW_MINOR_VERSION}")
if(WIN32)
set(CPACK_GENERATOR "ZIP")
else()
set(CPACK_GENERATOR "TGZ")
endif()
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}_v${IVW_VERSION}")
install(DIRECTORY ${IVW_ROOT_DIR}/data/images DESTINATION data COMPONENT data)
install(DIRECTORY ${IVW_ROOT_DIR}/data/scripts DESTINATION data COMPONENT data)
install(DIRECTORY ${IVW_ROOT_DIR}/data/settings DESTINATION data COMPONENT data)
install(FILES ${IVW_ROOT_DIR}/data/volumes/boron.dat DESTINATION data/volumes COMPONENT data)
install(FILES ${IVW_ROOT_DIR}/data/volumes/boron.raw DESTINATION data/volumes COMPONENT data)
install(FILES ${IVW_ROOT_DIR}/data/workspaces/boron.inv DESTINATION data/workspaces COMPONENT data)
set(CPACK_COMPONENTS_ALL applications data modules)
include(CPack)
endif()
