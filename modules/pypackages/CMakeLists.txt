#--------------------------------------------------------------------
# Inviwo PyPackages Module
ivw_module(PyPackages)

#--------------------------------------------------------------------
# Add MOC files
set(MOC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/pypackagemanagerwidget.h
)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/pypackagesmodule.h
    ${CMAKE_CURRENT_SOURCE_DIR}/pypackagesmoduledefine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/pyscriptrunner.h
	${CMAKE_CURRENT_SOURCE_DIR}/pypackagesformatsmacro.h
    ${CMAKE_CURRENT_SOURCE_DIR}/pypackagesinterface/pypackagesutil.h
	${CMAKE_CURRENT_SOURCE_DIR}/pypackagesinterface/pypackageparsers.h
	${CMAKE_CURRENT_SOURCE_DIR}/processors/pyprocessorbase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/pycuda/pycudaimageinvert.h
	${CMAKE_CURRENT_SOURCE_DIR}/processors/numpy/numpybuffertest.h
	${CMAKE_CURRENT_SOURCE_DIR}/processors/numpy/numpyimagecontour.h
	${CMAKE_CURRENT_SOURCE_DIR}/processors/numpy/numpyvolumehistogram.h
	#widgets
	${CMAKE_CURRENT_SOURCE_DIR}/widgets/pypackagemanagerwidget.h
	${CMAKE_CURRENT_SOURCE_DIR}/widgets/pypackagemenu.h
	${MOC_FILES}
)
ivw_group("Header Files" ${HEADER_FILES})

ivw_qt_wrap_cpp(MOC_FILES ${MOC_FILES})
source_group("MOC Files" FILES ${MOC_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES    
	${CMAKE_CURRENT_SOURCE_DIR}/pypackagesmodule.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pyscriptrunner.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pypackagesinterface/pypackagesutil.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/pypackagesinterface/pypackageparsers.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/processors/pyprocessorbase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/pycuda/pycudaimageinvert.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/processors/numpy/numpybuffertest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/processors/numpy/numpyimagecontour.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/processors/numpy/numpyvolumehistogram.cpp
	#widgets
	${CMAKE_CURRENT_SOURCE_DIR}/widgets/pypackagemanagerwidget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/widgets/pypackagemenu.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})


ivw_define_qt_definitions()
ivw_add_definition(IVW_PYPACKAGES)

#--------------------------------------------------------------------
# Create module
ivw_create_module(${SOURCE_FILES} ${MOC_FILES} ${HEADER_FILES})