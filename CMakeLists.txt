# Interactive Visualization Workshop (Inviwo)
cmake_minimum_required(VERSION 2.8.4)
project(inviwo-projects)
include(cmake/globalconfig.cmake)

#--------------------------------------------------------------------
# Default setup of options for the build

# Modules
build_module(Base)
#build_module(GLUT)
build_module(OpenGL)

# Applications/Qt
option(BUILD_IVW_QT_APPLICATION "Build Inviwo Qt Application" ON)
option(BUILD_IVW_QT_EDITOR "Build Inviwo Qt Editor" ON)
option(BUILD_IVW_QT_WIDGETS "Build Inviwo Qt Widgets" ON)
option(BUILD_IVW_TINY_GLUT_APPLICATION "Build Inviwo Tiny GLUT Application" OFF)

#--------------------------------------------------------------------
# Add subdirectories

add_subdirectory(ext)
add_subdirectory(src)
add_subdirectory(modules)
add_subdirectory(apps)