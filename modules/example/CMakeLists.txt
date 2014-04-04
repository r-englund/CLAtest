#--------------------------------------------------------------------
# Inviwo Example Module
ivw_module(Example)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${IVW_MODULE_DIR}/example/exampleprocessor.h
    ${IVW_MODULE_DIR}/example/exampleprogressbar.h
)
ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    ${IVW_MODULE_DIR}/example/exampleprocessor.cpp
    ${IVW_MODULE_DIR}/example/exampleprogressbar.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})

#--------------------------------------------------------------------
# Create module
ivw_create_module(${SOURCE_FILES} ${HEADER_FILES})