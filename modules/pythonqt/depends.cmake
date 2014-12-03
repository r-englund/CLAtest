#--------------------------------------------------------------------
# Dependencies for Python module
set(dependencies 
    InviwoPythonModule
    InviwoQtWidgets
    InviwoQtEditor
)

if(DESIRED_QT_VERSION MATCHES 5)
    list(APPEND dependencies Qt5Help)
else()
    list(APPEND dependencies Qt)
endif()