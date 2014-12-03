#--------------------------------------------------------------------
# Dependencies for current module
# List modules on the format "Inviwo<ModuleName>Module"
set(dependencies
    InviwoPython3Module
    InviwoQtWidgets
    InviwoQtEditor
)

if(DESIRED_QT_VERSION MATCHES 5)
    list(APPEND dependencies Qt5Widgets Qt5Help)
else()
    list(APPEND dependencies Qt)
endif()
