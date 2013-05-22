#--------------------------------------------------------------------
# Inviwo OpenglQt Module
ivw_module(OpenglQt)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${IVW_MODULE_DIR}/openglqt/openglqtinfo.h
    ${IVW_MODULE_DIR}/openglqt/openglqtmodule.h
	${IVW_MODULE_DIR}/openglqt/openglqtmoduledefine.h
)
ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    ${IVW_MODULE_DIR}/openglqt/openglqtinfo.cpp
    ${IVW_MODULE_DIR}/openglqt/openglqtmodule.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})

# Create module
ivw_create_module(${SOURCE_FILES} ${HEADER_FILES} ${SHADER_FILES})
