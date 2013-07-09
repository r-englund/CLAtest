#--------------------------------------------------------------------
# Dependencies for OpenglQt module
# Add qt dependencies to this list
if(DESIRED_QT_VERSION MATCHES 5)
    set(dependencies 
        InviwoOpenGLModule
        InviwoQtWidgets
        Qt5Core
    )
else()
    set(dependencies 
        InviwoOpenGLModule
        InviwoQtWidgets
        Qt
    )
endif()



