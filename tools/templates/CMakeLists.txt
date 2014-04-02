#--------------------------------------------------------------------
# Inviwo <name> Module
ivw_module(<name>)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    #${IVW_MODULE_DIR}/<lname>/<lname>processor.h
)
ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    #${IVW_MODULE_DIR}/<lname>/<lname>processor.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})

#--------------------------------------------------------------------
# Create module
ivw_create_module(${SOURCE_FILES} ${HEADER_FILES})