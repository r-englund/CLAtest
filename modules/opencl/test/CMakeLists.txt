function(createTest NAME)
	set(SAMPLE_NAME test-${NAME})
	ivw_project(${SAMPLE_NAME})

	add_executable(${SAMPLE_NAME} MACOSX_BUNDLE WIN32 ${NAME}.cpp)
	#--------------------------------------------------------------------
	# Add dependencies
	ivw_add_dependencies(InviwoOpenCLModule)

	add_test( 
	    NAME ${SAMPLE_NAME}
	    COMMAND $<TARGET_FILE:${SAMPLE_NAME}> )
endfunction()



createTest(opencl)

