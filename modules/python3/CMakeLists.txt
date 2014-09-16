#--------------------------------------------------------------------
# Inviwo Python3 Module
ivw_module(Python3)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    #${IVW_MODULE_DIR}/python3/python3processor.h
    ${IVW_MODULE_DIR}/python3/defaultinterface/pycamera.h
    ${IVW_MODULE_DIR}/python3/defaultinterface/pycanvas.h    
    ${IVW_MODULE_DIR}/python3/defaultinterface/pylist.h
    ${IVW_MODULE_DIR}/python3/defaultinterface/pyproperties.h
    ${IVW_MODULE_DIR}/python3/defaultinterface/pyutil.h
    ${IVW_MODULE_DIR}/python3/defaultinterface/pyvolume.h
    ${IVW_MODULE_DIR}/python3/pyinviwo.h
    ${IVW_MODULE_DIR}/python3/pyinviwoobserver.h
    ${IVW_MODULE_DIR}/python3/pythonexecutionoutputobservable.h
    ${IVW_MODULE_DIR}/python3/pythonincluder.h
    ${IVW_MODULE_DIR}/python3/pythoninterface/pymethod.h
    ${IVW_MODULE_DIR}/python3/pythoninterface/pymodule.h
    ${IVW_MODULE_DIR}/python3/pythoninterface/pyvalueparser.h
    ${IVW_MODULE_DIR}/python3/pythonscript.h
)
ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    #${IVW_MODULE_DIR}/python3/python3processor.cpp
    ${IVW_MODULE_DIR}/python3/defaultinterface/pycamera.cpp
    ${IVW_MODULE_DIR}/python3/defaultinterface/pycanvas.cpp
    ${IVW_MODULE_DIR}/python3/defaultinterface/pylist.cpp
    ${IVW_MODULE_DIR}/python3/defaultinterface/pyproperties.cpp
    ${IVW_MODULE_DIR}/python3/defaultinterface/pyutil.cpp
    ${IVW_MODULE_DIR}/python3/defaultinterface/pyvolume.cpp
    ${IVW_MODULE_DIR}/python3/defaultinterface/pyinfo.cpp
    ${IVW_MODULE_DIR}/python3/pyinviwo.cpp
    ${IVW_MODULE_DIR}/python3/pyinviwoobserver.cpp
    ${IVW_MODULE_DIR}/python3/pythonexecutionoutputobservable.cpp
    ${IVW_MODULE_DIR}/python3/pythoninterface/pymethod.cpp
    ${IVW_MODULE_DIR}/python3/pythoninterface/pymodule.cpp
    ${IVW_MODULE_DIR}/python3/pythoninterface/pyvalueparser.cpp
    ${IVW_MODULE_DIR}/python3/pythonscript.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})


#--------------------------------------------------------------------
# Add shaders
#set(SHADER_FILES
    #${IVW_MODULE_DIR}/python3/glsl/python3processor.frag
#)
#ivw_group("Shader Files" ${SHADER_FILES})


#--------------------------------------------------------------------
# Add Unittests
#set(TEST_FILES
#	${IVW_MODULE_DIR}/python3/tests/python3-test.cpp
#)
#ivw_add_unittest(${TEST_FILES})

#--------------------------------------------------------------------
# Create module
ivw_create_module(${SOURCE_FILES} ${HEADER_FILES} ${SHADER_FILES})

#--------------------------------------------------------------------
# Add shader directory to pack
# ivw_add_to_module_pack(${IVW_MODULE_DIR}/python3/glsl)
