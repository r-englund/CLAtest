#--------------------------------------------------------------------
# Dependencies for Python module
set(dependencies 
    InviwoPythonModule
    InviwoQtWidgets
)

if(DESIRED_QT_VERSION MATCHES 5)
    list(APPEND dependencies Qt5OpenGL)
else()
    list(APPEND dependencies Qt)
endif()