#--------------------------------------------------------------------
# Dependencies for OpenGL module
IF (WIN32)
	set(dependencies 
		glew
		OpenGL
	)
ELSE (WIN32)
	set(dependencies 
		GLEW
		OpenGL
	)
ENDIF (WIN32)

