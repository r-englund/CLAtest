#--------------------------------------------------------------------
# InviwoUnitTests Module
ivw_module(UnitTests)

add_definitions(-DGTEST_HAS_TR1_TUPLE=0)
add_subdirectory(gtest-1.7.0)
ivw_vs_folder(gtest ext)
ivw_make_package(gtest gtest)


include_directories(${IVW_MODULE_DIR}/unittests/gtest-1.7.0/include)
include_directories(${CMAKE_BINARY_DIR}/modules/_generated)
include_directories(${CMAKE_BINARY_DIR}/apps/unittests/_generated)

#--------------------------------------------------------------------
# Add header files

set(HEADER_FILES
	${IVW_MODULE_DIR}/unittests/unittestsmoduledefine.h
	${IVW_MODULE_DIR}/unittests/unittestsmodule.h
)

ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add source files

set(SOURCE_FILES
	${IVW_MODULE_DIR}/unittests/unittestsmodule.cpp
)

ivw_group("Source Files" ${SOURCE_FILES})

#--------------------------------------------------------------------
# Create module
ivw_create_module(${SOURCE_FILES} ${HEADER_FILES})


