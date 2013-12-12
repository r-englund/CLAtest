#--------------------------------------------------------------------
# Inviwo Unittest Application
ivw_project(unittests)

add_definitions(-DGTEST_HAS_TR1_TUPLE=0)
add_subdirectory(gtest-1.7.0)
ivw_vs_folder(gtest ext)
ivw_make_package(gtest gtest)


#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/apps/unittests/inviwounittest.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})


set(TEST_FILES
	${CMAKE_SOURCE_DIR}/apps/unittests/defaulttests/inviwoapplication.test.h
)
ivw_group("Unittest Files" ${TEST_FILES})
ivw_add_unittest(${TEST_FILES})
 


#--------------------------------------------------------------------
# Define defintions
ivw_define_standard_definitions(unittests)

#--------------------------------------------------------------------
# Define libraries that should be linked
ivw_retrieve_all_modules(package_list)
list(APPEND package_list InviwoCore)

foreach(module ${package_list})
    string(TOUPPER ${module} u_module)
    if(u_module MATCHES "QT+")
        list(REMOVE_ITEM package_list ${module})
    endif()
    if(u_module MATCHES "GL+")
        list(REMOVE_ITEM package_list ${module})
    endif()
    if(u_module MATCHES "VRN+")
        list(REMOVE_ITEM package_list ${module})
    endif()
endforeach()

#--------------------------------------------------------------------
# Register the use of modules
ivw_register_use_of_modules(${package_list})

#--------------------------------------------------------------------
# Need to add dependent directories before creating application
ivw_add_dependency_directories(${package_list})

#--------------------------------------------------------------------
# Include generation directory to register modules etc
include_directories(gtest-1.7.0/include)
include_directories(${CMAKE_BINARY_DIR}/modules/_generated)
include_directories(${CMAKE_BINARY_DIR}/apps/unittests/_generated)
ivw_link_directories(${LIBRARY_OUTPUT_PATH})


#--------------------------------------------------------------------
# Create application
add_executable(unittests MACOSX_BUNDLE WIN32 ${SOURCE_FILES} ${HEADER_FILES} ${TEST_FILES})
target_link_libraries(unittests gtest)
#--------------------------------------------------------------------
# Add dependencies
ivw_add_dependencies(${package_list})

configure_file(${CMAKE_BINARY_DIR}/apps/unittests/_generated/unittests_temp.h ${CMAKE_BINARY_DIR}/apps/unittests/_generated/unittests.h)
file(REMOVE ${CMAKE_BINARY_DIR}/apps/unittests/_generated/unittests_temp.h)
#--------------------------------------------------------------------
# Optimize compilation with pre-compilied headers based on inviwo-core
ivw_compile_optimize_inviwo_core()
