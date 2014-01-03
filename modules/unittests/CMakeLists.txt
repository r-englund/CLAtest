#--------------------------------------------------------------------
# InviwoUnitTests Module
ivw_module(UnitTests)

add_definitions(-DGTEST_HAS_TR1_TUPLE=0)


add_subdirectory(ext/gtest-1.7.0)
ivw_vs_folder(gtest ext)
ivw_make_package(gtest gtest)


if(BUILD_SHARED_LIBS)
	add_definitions(-DGTEST_LINKED_AS_SHARED_LIBRARY=1)
endif()

include_directories(${IVW_MODULE_DIR}/unittests/ext/gtest-1.7.0/include)
include_directories(${CMAKE_BINARY_DIR}/modules/_generated)

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

if(IVW_MODULE_OPENCL)
	ivw_add_definition(__CL_ENABLE_EXCEPTIONS)
endif()


configure_file(${CMAKE_BINARY_DIR}/modules/_generated/unittests_temp.h ${CMAKE_BINARY_DIR}/modules/_generated/unittests.h)
file(REMOVE ${CMAKE_BINARY_DIR}/modules/_generated/unittests_temp.h)


add_dependency_libs_to_module(gtest)


#--------------------------------------------------------------------
# Create module
ivw_create_module(${SOURCE_FILES} ${HEADER_FILES})


