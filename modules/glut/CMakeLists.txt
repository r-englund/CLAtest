#--------------------------------------------------------------------
# Build FREEGLUT if it couldn't be found
find_package(FreeGLUT)

if(BUILD_FREEGLUT)
	add_subdirectory(ext/freeglut)
endif(BUILD_FREEGLUT)

#--------------------------------------------------------------------
# Inviwo GLUT Module
ivw_module(GLUT)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${IVW_MODULE_DIR}/glut/canvasglut.h
    ${IVW_MODULE_DIR}/glut/glutmodule.h
)
ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    ${IVW_MODULE_DIR}/glut/canvasglut.cpp
    ${IVW_MODULE_DIR}/glut/glutmodule.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})

#--------------------------------------------------------------------
# Add additonal dependency libs
if(BUILD_FREEGLUT AND NOT BUILD_SHARED_LIBS)
    include(${CMAKE_BINARY_DIR}/freeglut.pc)
    add_dependency_libs_to_module(${GLUT_LIBS_PRIVATE})
endif()

#--------------------------------------------------------------------
# Create module
ivw_set_cpack_name(glut_modules)
ivw_create_module(${SOURCE_FILES} ${HEADER_FILES})