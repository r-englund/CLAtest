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
ivw_link_directories(${LIBRARY_OUTPUT_PATH})


ivw_retrieve_all_modules(package_list)

foreach(module ${package_list})
    string(TOUPPER ${module} u_module)
    if(u_module MATCHES "QT+")
        list(REMOVE_ITEM package_list ${module})
    elseif(u_module MATCHES "GL+")
        list(REMOVE_ITEM package_list ${module})
    elseif(u_module MATCHES "VRN+")
        list(REMOVE_ITEM package_list ${module})
    else()
        ivw_mod_name_to_dir(libraryname ${module})
		add_dependency_libs_to_module(inviwo-module-${libraryname})
    endif()
endforeach()
list(APPEND package_list InviwoCore)

#--------------------------------------------------------------------
# Register the use of modules
ivw_register_use_of_modules(${package_list})

#--------------------------------------------------------------------
# Need to add dependent directories before creating application
ivw_add_dependency_directories(${package_list})


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


