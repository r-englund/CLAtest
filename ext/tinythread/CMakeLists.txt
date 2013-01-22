#--------------------------------------------------------------------
# TinyThread++ Package
ivw_project(tinythread)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${IVW_EXTENSIONS_DIR}/tinythread/fast_mutex.h
    ${IVW_EXTENSIONS_DIR}/tinythread/tinythread.h
)
ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    ${IVW_EXTENSIONS_DIR}/tinythread/tinythread.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})

#--------------------------------------------------------------------
# Define defintions
ivw_define_standard_definitions(tinythread)

#--------------------------------------------------------------------
# Create library
add_library(tinythread ${SOURCE_FILES} ${HEADER_FILES})

#--------------------------------------------------------------------
# Creates VS folder structure
ivw_vs_folder(tinythread ext)

#--------------------------------------------------------------------
# Make package (for other projects to find)
ivw_make_package(TinyThread tinythread)