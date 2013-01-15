project(glew)

set(GLEW_MAJOR_VERSION 1)
set(GLEW_MINOR_VERSION 5)
set(GLEW_PATCH_VERSION 8)
set(GLEW_VERSION ${GLEW_MAJOR_VERSION}.${GLEW_MINOR_VERSION}.${GLEW_PATCH_VERSION})

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${IVW_MODULE_DIR}/opengl/ext/glew/include/GL/glew.h
    ${IVW_MODULE_DIR}/opengl/ext/glew/include/GL/glxew.h
    ${IVW_MODULE_DIR}/opengl/ext/glew/include/GL/wglew.h
)

source_group("Header Files" FILES ${HEADER_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    ${IVW_MODULE_DIR}/opengl/ext/glew/src/glew.c
)

source_group("Source Files" FILES ${SOURCE_FILES})

#--------------------------------------------------------------------
# Locate OpenGL
find_package(OpenGL QUIET REQUIRED)

#--------------------------------------------------------------------
# Set includes
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include 
    ${OPENGL_INCLUDE_DIR}
)

#--------------------------------------------------------------------
# Set the compiler flags
if(WIN32)
  add_definitions(-DGLEW_BUILD)
else(WIN32)
  add_definitions(-DHAVE_CONFIG_H)
endif(WIN32)

add_definitions(-DUNICODE)
source_group("CMake Files" FILES ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt)

#--------------------------------------------------------------------
# Create a target for the library
add_library(glew ${SOURCE_FILES} ${HEADER_FILES})

# InViWo Macros
ivw_vs_folder(glew ext)

target_link_libraries(glew
    ${OPENGL_LIBRARIES}
)