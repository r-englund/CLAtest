#--------------------------------------------------------------------
# CVSC Module
ivw_module(CVSC)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${IVW_MODULE_DIR}/cvsc/cvscprocessor.h
)
ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    ${IVW_MODULE_DIR}/cvsc/cvscprocessor.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})

#--------------------------------------------------------------------
# Add CVSC directories
ivw_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ext/CVSCSDK/include)
ivw_link_directories(${CMAKE_CURRENT_SOURCE_DIR}/ext/CVSCSDK/lib)

#--------------------------------------------------------------------
# Create module
ivw_create_module(${SOURCE_FILES} ${HEADER_FILES})

#--------------------------------------------------------------------
# Link CVSC lib
target_link_libraries(inviwo-module-cvsc cvsc64)