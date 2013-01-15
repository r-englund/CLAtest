#Generate module options
generate_unset_mod_options_and_depend_sort(IVW_SORTED_MODULES)

#Resolve dependencies for selected modules
resolve_module_dependencies(${IVW_SORTED_MODULES})

#Add modules based on user config file and dependcy resolve
add_modules(${IVW_SORTED_MODULES})