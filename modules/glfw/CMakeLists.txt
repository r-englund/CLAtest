#--------------------------------------------------------------------
# Build GLFW
add_subdirectory(ext/glfw)

#--------------------------------------------------------------------
# Inviwo GLFW Module
ivw_module(GLFW)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${IVW_MODULE_DIR}/glfw/canvasglfw.h
    ${IVW_MODULE_DIR}/glfw/glfwmodule.h
)
ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    ${IVW_MODULE_DIR}/glfw/canvasglfw.cpp
    ${IVW_MODULE_DIR}/glfw/glfwmodule.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})

#--------------------------------------------------------------------
# Create module
ivw_create_module(${SOURCE_FILES} ${HEADER_FILES})