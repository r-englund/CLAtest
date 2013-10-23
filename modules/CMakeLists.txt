#Generate module options
generate_unset_mod_options_and_depend_sort(IVW_SORTED_MODULES)

#Resolve dependencies for selected modules
resolve_module_dependencies(${IVW_SORTED_MODULES})

if(IVW_MODULE_HUMANCOMPUTATIONENGINE OR IVW_MODULE_OPENGLQT)
    # Find the QtWidgets library
    if(DESIRED_QT_VERSION MATCHES 5)
        find_package(Qt5Widgets QUIET REQUIRED)     
    else()
        find_package(Qt QUIET REQUIRED)
    endif()
endif()

if(IVW_MODULE_OPENGLQT)
    set(QT_USE_QTOPENGL TRUE)    
endif()

if(IVW_MODULE_HUMANCOMPUTATIONENGINE)
    set(QT_USE_QTNETWORK TRUE)
    set(QT_USE_QTSCRIPT TRUE)
endif()	

#Add modules based on user config file and dependcy resolve
add_modules(${IVW_SORTED_MODULES})