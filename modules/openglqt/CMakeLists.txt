if(DESIRED_QT_VERSION MATCHES 5)
        string(REGEX REPLACE "bin\\\\qmake([^ ]*)$" "lib\\\\cmake" QT5_PATH "${QT_QMAKE_EXECUTABLE}")
        set(CMAKE_PREFIX_PATH "${QT5_PATH}")
else()
        set(QT_USE_QTOPENGL TRUE)
endif()

#--------------------------------------------------------------------
# Inviwo OpenGLQt Module
ivw_module(OpenGLQt)

#--------------------------------------------------------------------
# Add MOC files
set(MOC_FILES
    ${IVW_MODULE_DIR}/openglqt/canvasqt.h
    ${IVW_MODULE_DIR}/openglqt/canvasprocessorwidgetqt.h
)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${IVW_MODULE_DIR}/openglqt/openglqtcapabilities.h
    ${IVW_MODULE_DIR}/openglqt/openglqtmodule.h
    ${IVW_MODULE_DIR}/openglqt/openglqtmoduledefine.h
    ${MOC_FILES}
)
ivw_group("Header Files" ${HEADER_FILES})

ivw_qt_wrap_cpp(MOC_FILES ${MOC_FILES})
source_group("MOC Files" FILES ${MOC_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    ${IVW_MODULE_DIR}/openglqt/canvasqt.cpp
    ${IVW_MODULE_DIR}/openglqt/openglqtcapabilities.cpp
    ${IVW_MODULE_DIR}/openglqt/openglqtmodule.cpp
    ${IVW_MODULE_DIR}/openglqt/canvasprocessorwidgetqt.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})

ivw_define_qt_definitions()
ivw_add_definition(QT_OPENGL_LIB)

# Create module
ivw_create_module(${SOURCE_FILES} ${MOC_FILES} ${HEADER_FILES})

# Add Qt OpenGL to package
if(IVW_PACKAGE_PROJECT AND BUILD_SHARED_LIBS)
    if(WIN32)
        if(DESIRED_QT_VERSION MATCHES 5)
            if(Qt5OpenGL_FOUND)
                set(QT_BINARY_DIR "${Qt5OpenGL_DIR}/../../../bin")
                install(FILES ${QT_BINARY_DIR}/Qt5OpenGL${CMAKE_DEBUG_POSTFIX}.dll DESTINATION bin COMPONENT qt_modules CONFIGURATIONS Debug)
                install(FILES ${QT_BINARY_DIR}/Qt5OpenGL.dll DESTINATION bin COMPONENT qt_modules CONFIGURATIONS Release)
            endif()
         else()
                install(FILES ${QT_BINARY_DIR}/QtOpenGL${CMAKE_DEBUG_POSTFIX}4.dll DESTINATION bin COMPONENT qt_modules CONFIGURATIONS Debug)
                install(FILES ${QT_BINARY_DIR}/QtOpenGL4.dll DESTINATION bin COMPONENT qt_modules CONFIGURATIONS Release)
        endif()
    endif()
endif()
