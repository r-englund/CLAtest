#--------------------------------------------------------------------
# CVIE Module
ivw_module(CVIE)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${IVW_MODULE_DIR}/cvie/cvie2dimageprocessor.h
    ${IVW_MODULE_DIR}/cvie/cvie2dvolumeprocessor.h
)
ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    ${IVW_MODULE_DIR}/cvie/cvie2dimageprocessor.cpp
    ${IVW_MODULE_DIR}/cvie/cvie2dvolumeprocessor.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})

#--------------------------------------------------------------------
# Add CVIE directories
ivw_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ext/CVIESDK/include)
ivw_link_directories(${CMAKE_CURRENT_SOURCE_DIR}/ext/CVIESDK/lib)

#--------------------------------------------------------------------
# Create module
ivw_create_module(${SOURCE_FILES} ${HEADER_FILES})

#--------------------------------------------------------------------
# Link CVIE lib
if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8") 
target_link_libraries(inviwo-module-cvie cvie64)
else()
target_link_libraries(inviwo-module-cvie cvie32)
endif()