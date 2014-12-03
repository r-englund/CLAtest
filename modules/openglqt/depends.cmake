#--------------------------------------------------------------------
# Dependencies for OpenglQt module
set(dependencies 
    InviwoOpenGLModule
    InviwoQtWidgets
)

if(DESIRED_QT_VERSION MATCHES 5)
    list(APPEND dependencies Qt5OpenGL Qt5Help)
else()
    list(APPEND dependencies Qt)
endif()



