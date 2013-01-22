# Interactive Visualization Workshop (Inviwo)
cmake_minimum_required(VERSION 2.8.4)
project(inviwo-projects)
include(cmake/globalconfig.cmake)

#--------------------------------------------------------------------
# Default setup of options for the build

# Modules
build_module(Base)
build_module(OpenGL)

hide_module(GLUT)

# Applications/Qt
option(IVW_QT_APPLICATION "Build Inviwo Qt Application" ON)
option(IVW_QT_EDITOR "Build Inviwo Qt Editor" ON)
option(IVW_QT_WIDGETS "Build Inviwo Qt Widgets" ON)
option(IVW_TINY_GLUT_APPLICATION "Build Inviwo Tiny GLUT Application" OFF)
mark_as_advanced(FORCE  IVW_TINY_GLUT_APPLICATION)

#--------------------------------------------------------------------
# Add subdirectories

add_subdirectory(ext/ticpp)
add_subdirectory(ext/tinythread)
add_subdirectory(src/core)
add_subdirectory(modules)
add_subdirectory(src/qt)
add_subdirectory(apps)