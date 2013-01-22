function(createTest NAME)
	set(SAMPLE_NAME test-${NAME})
	ivw_project(${SAMPLE_NAME})
	
	add_executable(${SAMPLE_NAME} MACOSX_BUNDLE WIN32 ${NAME}.cpp)
	#--------------------------------------------------------------------
	# Add dependencies
	ivw_add_dependencies(InviwoOpenCLModule)
	#--------------------------------------------------------------------
	# Set VS Executable properties
	ivw_vs_executable_setup(${SAMPLE_NAME})
	ivw_vs_folder(${SAMPLE_NAME} test)
	add_test( 
	    NAME ${SAMPLE_NAME}
	    COMMAND $<TARGET_FILE:${SAMPLE_NAME}> )
endfunction()



createTest(opencl)

