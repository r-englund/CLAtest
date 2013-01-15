#--------------------------------------------------------------------
# Inviwo Qt Application
ivw_project(inviwo)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    inviwomainwindow.h
)
qt4_wrap_cpp(MOC_FILES ${HEADER_FILES})
ivw_group("Header Files" ${HEADER_FILES})
source_group("MOC Files" FILES ${MOC_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    inviwo.cpp
    inviwomainwindow.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})

#--------------------------------------------------------------------
# Add resource file
qt4_add_resources(QRC_FILE ${CMAKE_SOURCE_DIR}/resources/inviwo.qrc)
source_group("Resource Files" FILES ${QRC_FILE} ${CMAKE_SOURCE_DIR}/resources/inviwo.qrc)
        
#--------------------------------------------------------------------
# Define defintions
ivw_define_standard_definitions(inviwo)
add_definitions(-DQT_CORE_LIB
                      -DQT_GUI_LIB
                      -DQT_OPENGL_LIB
                      -DQT_DLL
                      -DQT_THREAD_SUPPORT)
                      
#--------------------------------------------------------------------
# Add general link directory
link_directories("${IVW_LIBRARY_DIR}")

#--------------------------------------------------------------------
# Create application
add_executable(inviwo WIN32 ${SOURCE_FILES} ${HEADER_FILES} ${MOC_FILES} ${QRC_FILE})

# Link towards modules
ivw_add_all_modules()

# Add dependencies to this list
ivw_add_dependencies(
    InviwoQtEditor
)

#--------------------------------------------------------------------
# Creates VS folder structure
ivw_vs_folder(inviwo application)

#--------------------------------------------------------------------
# Add console to debug build only
if(WIN32)
  if(MSVC)
	 set_target_properties(inviwo PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
     set_target_properties(inviwo PROPERTIES COMPILE_DEFINITIONS_DEBUG "_CONSOLE")
     set_target_properties(inviwo PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:CONSOLE")
     set_target_properties(inviwo PROPERTIES COMPILE_DEFINITIONS_RELWITHDEBINFO "_CONSOLE")
	 set_target_properties(inviwo PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS") 
	 set_target_properties(inviwo PROPERTIES MINSIZEREL "/SUBSYSTEM:WINDOWS")
    endif(MSVC)
endif(WIN32)
