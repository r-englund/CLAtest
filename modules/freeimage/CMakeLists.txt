#--------------------------------------------------------------------
# FreeImage Module
ivw_module(FreeImage)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${IVW_MODULE_DIR}/freeimage/freeimagereader.h
    ${IVW_MODULE_DIR}/freeimage/freeimagewriter.h
)
ivw_group("Header Files" ${HEADER_FILES})


#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    ${IVW_MODULE_DIR}/freeimage/freeimagereader.cpp
    ${IVW_MODULE_DIR}/freeimage/freeimagewriter.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})

#--------------------------------------------------------------------
# Create module
ivw_create_module(${SOURCE_FILES} ${MOC_FILES} ${HEADER_FILES})