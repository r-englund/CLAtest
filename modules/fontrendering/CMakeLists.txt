if(IVW_MODULE_FONTRENDERING)
	add_subdirectory(ext/freetype)
endif(IVW_MODULE_FONTRENDERING)

#--------------------------------------------------------------------
# Inviwo Fontrendering Module
ivw_module(Fontrendering)

#--------------------------------------------------------------------
# Set include directories
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ext/freetype/include)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
	${IVW_MODULE_DIR}/fontrendering/fontrenderingmodule.h
	${IVW_MODULE_DIR}/fontrendering/fontrenderingmoduledefine.h
	${IVW_MODULE_DIR}/fontrendering/processors/imageoverlay.h
)
ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
	${IVW_MODULE_DIR}/fontrendering/fontrenderingmodule.cpp
	${IVW_MODULE_DIR}/fontrendering/processors/imageoverlay.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})

#--------------------------------------------------------------------
# Add shaders
set(SHADER_FILES
    ${IVW_MODULE_DIR}/fontrendering/glsl/fontrendering_freetype.frag
    ${IVW_MODULE_DIR}/fontrendering/glsl/fontrendering_freetype.vert
    ${IVW_MODULE_DIR}/fontrendering/glsl/fontrendering_passthrough.frag
)
ivw_group("Shader Files" ${SHADER_FILES})

#--------------------------------------------------------------------
# Create module
ivw_create_module(${SOURCE_FILES} ${HEADER_FILES})

#--------------------------------------------------------------------
# Add shader directory to pack
ivw_add_to_module_pack(${IVW_MODULE_DIR}/fontrendering/glsl)