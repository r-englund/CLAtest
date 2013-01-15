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
# Create module
ivw_create_module(${SOURCE_FILES} ${HEADER_FILES})

#--------------------------------------------------------------------
# Build FREEGLUT if it couldn't be found
if(BUILD_FREEGLUT)
	add_subdirectory(ext/freeglut)
endif(BUILD_FREEGLUT)