cmake_minimum_required(VERSION 2.6)
project(freeglut)

# NOTE: On Windows and Cygwin, the dll's are placed in the
# CMAKE_RUNTIME_OUTPUT_DIRECTORY, while their corresponding import
# libraries end up in CMAKE_ARCHIVE_OUTPUT_DIRECTORY. On other
# platforms, such as Linux, the shared libraries are put in
# CMAKE_ARCHIVE_OUTPUT_DIRECTORY instead.
# Static libraries end up in CMAKE_ARCHIVE_OUTPUT_DIRECTORY on all
# platforms.
#SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
#SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

file(GLOB FREEGLUT_HEADERS ./include/GL/*.h)
file(GLOB FREEGLUT_SOURCES_H ./src/*.h)
file(GLOB FREEGLUT_SOURCES_C ./src/*.c)

SET(FREEGLUT_SRCS
    ${FREEGLUT_HEADERS}
	${FREEGLUT_SOURCES_H}
	${FREEGLUT_SOURCES_C}
)

FIND_PACKAGE(OpenGL QUIET REQUIRED)
LIST(APPEND LIBS ${OPENGL_gl_LIBRARY})
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

# lib m for math, not needed on windows
IF (NOT WIN32)
    # For compilation:
    LIST(APPEND LIBS m)
    # For CHECK_FUNCTION_EXISTS:
    LIST(APPEND CMAKE_REQUIRED_LIBRARIES m)
ENDIF()

IF(WIN32)
    # hide insecure CRT warnings, common practice
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS /wd4133)
    IF(MSVC)
        SET( CMAKE_DEBUG_POSTFIX "d" )
    ENDIF(MSVC)
    
ENDIF()

IF(CMAKE_COMPILER_IS_GNUCC)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

INCLUDE(CheckIncludeFiles)
IF(UNIX)
    FIND_PACKAGE(X11 REQUIRED)
    LIST(APPEND LIBS ${X11_LIBRARIES})
    IF(X11_Xrandr_FOUND)
        SET(HAVE_X11_EXTENSIONS_XRANDR_H TRUE)
        LIST(APPEND LIBS ${X11_Xrandr_LIB})
    ENDIF()
    IF(X11_xf86vmode_FOUND)
        SET(HAVE_X11_EXTENSIONS_XF86VMODE_H TRUE)
        # Work-around http://www.cmake.org/Bug/bug_view_page.php?bug_id=6976
        IF(NOT "${X11_Xxf86vm_LIB}")
            SET(X11_Xxf86vm_LIB "Xxf86vm")
        ENDIF()
        LIST(APPEND LIBS ${X11_Xxf86vm_LIB})
    ENDIF()
    IF(X11_Xinput_FOUND)
        # Needed for multi-touch:
        CHECK_INCLUDE_FILES("${X11_Xinput_INCLUDE_PATH}/X11/extensions/XInput2.h" HAVE_X11_EXTENSIONS_XINPUT2_H)
        LIST(APPEND LIBS ${X11_Xinput_LIB})
    ENDIF()
ENDIF()

#INCLUDE(CheckFunctionExists)
#INCLUDE(CheckTypeSize)
#CHECK_INCLUDE_FILES(sys/types.h HAVE_SYS_TYPES_H)
#CHECK_INCLUDE_FILES(unistd.h 	HAVE_UNISTD_H)
#CHECK_INCLUDE_FILES(sys/time.h 	HAVE_SYS_TIME_H)
#CHECK_INCLUDE_FILES(stdbool.h 	HAVE_STDBOOL_H)
#CHECK_INCLUDE_FILES(sys/param.h HAVE_SYS_PARAM_H)
#CHECK_INCLUDE_FILES(sys/ioctl.h HAVE_SYS_IOCTL_H)
#CHECK_INCLUDE_FILES(fcntl.h 	HAVE_FCNTL_H)
#CHECK_INCLUDE_FILES(errno.h 	HAVE_ERRNO_H)
#CHECK_INCLUDE_FILES(usbhid.h 	HAVE_USBHID_H)
#CHECK_FUNCTION_EXISTS(gettimeofday HAVE_GETTIMEOFDAY)
#CHECK_FUNCTION_EXISTS(vfprintf  HAVE_VFPRINTF)
#CHECK_FUNCTION_EXISTS(_doprnt   HAVE_DOPRNT)
#CHECK_FUNCTION_EXISTS(XParseGeometry   HAVE_XPARSEGEOMETRY)
# decide on suitable type for internal time keeping, 64-bit if possible
#CHECK_INCLUDE_FILES(stdint.h    HAVE_STDINT_H)
#CHECK_INCLUDE_FILES(inttypes.h  HAVE_INTTYPES_H)
#IF (NOT (HAVE_STDINT_H OR HAVE_INTTYPES_H))
#    IF (MSVC)
        # Some old Microsoft VC releases don't support unsigned long
        # long, but all we care about is support for unsigned __int64 on
        # MSVC, so test for presence of that type
        #CHECK_TYPE_SIZE("unsigned __int64" U__INT64 BUILTIN_TYPES_ONLY)
    #ELSEIF()
        #CHECK_TYPE_SIZE("unsigned long long" ULONG_LONG BUILTIN_TYPES_ONLY)
    #ENDIF()
#ENDIF()

# The generated config.h is placed in the project's build directory, just to
# ensure that all CMake-generated files are kept away from the main source tree.
# As a result, the build directory must to be added to the include path list.
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_BINARY_DIR}/config.h)
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/src)
ADD_DEFINITIONS(-DHAVE_CONFIG_H)
IF(WIN32)
    # we also have to generate freeglut.rc, which contains the version
    # number
    CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/freeglut.rc.in ${CMAKE_BINARY_DIR}/freeglut.rc)
    IF (MSVC AND NOT CMAKE_CL_64)
        # .def file only for 32bit Windows builds with Visual Studio
        CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/src/freeglutdll.def.in ${CMAKE_BINARY_DIR}/freeglutdll.def)
    ENDIF()
ENDIF()

IF(BUILD_SHARED_LIBS)
    ADD_LIBRARY(freeglut SHARED ${FREEGLUT_SRCS})
ELSE(BUILD_SHARED_LIBS)
    ADD_LIBRARY(freeglut STATIC ${FREEGLUT_SRCS})
ENDIF()


IF(WIN32)
    LIST(APPEND LIBS winmm)
    IF(BUILD_SHARED_LIBS)
        SET_TARGET_PROPERTIES(freeglut PROPERTIES COMPILE_FLAGS -DFREEGLUT_EXPORTS)
    ELSE(BUILD_SHARED_LIBS)
        SET_TARGET_PROPERTIES(freeglut PROPERTIES COMPILE_FLAGS -DFREEGLUT_STATIC)
        # need to set machine:x64 for linker, at least for VC10, and
        # doesn't hurt for older compilers:
        # http://public.kitware.com/Bug/view.php?id=11240#c22768
        IF (CMAKE_CL_64)
            SET_TARGET_PROPERTIES(freeglut PROPERTIES STATIC_LIBRARY_FLAGS "/machine:x64")
        ENDIF()
    ENDIF()
ELSE()
    # on UNIX we need to make sure:
    # - all shared libraries must have a soname/version, see :
    #   http://sourceware.org/autobook/autobook/autobook_91.html#SEC91
    #   http://www.gnu.org/software/libtool/manual/html_node/Updating-version-info.html
    #   Current: -version-info 12:0:9 -> 3.9.0
    #   Note: most platforms now prefer the latter major.minor.revision form
    #   (e.g. FreeBSD, cf. http://debbugs.gnu.org/cgi/bugreport.cgi?bug=8765),
    #   or special-cased FreeGLUT long ago (e.g. .so.4 on OpenBSD), so
    #   the lack of support for current:revision:age in CMake should
    #   not be a problem.
    # - the output library should be named glut so it'll be linkable with -lglut
    # - the shared library should link to the dependency libraries so that the user
    #   won't have to link them explicitly (they shouldn't have to know that we depend
    #   on Xrandr or Xxf86vm)
   
   
    SET(LIBNAME glut)

    SET_TARGET_PROPERTIES(freeglut PROPERTIES VERSION 3.9.0 SOVERSION 3 OUTPUT_NAME ${LIBNAME})
    SET_TARGET_PROPERTIES(freeglut PROPERTIES OUTPUT_NAME ${LIBNAME})
ENDIF()
IF(BUILD_SHARED_LIBS)
    TARGET_LINK_LIBRARIES(freeglut ${LIBS})
ELSE(BUILD_SHARED_LIBS)
    TARGET_LINK_LIBRARIES(freeglut ${LIBS})
ENDIF()

#IF(BUILD_SHARED_LIBS)
#    INSTALL(TARGETS freeglut DESTINATION lib)
#ELSE(BUILD_SHARED_LIBS)
#    INSTALL(TARGETS freeglut DESTINATION lib)
#ENDIF()
#INSTALL(FILES ${FREEGLUT_HEADERS} DESTINATION include/GL)

# InViWo Macros
ivw_define_standard_definitions(freeglut)
ivw_vs_folder(freeglut ext)

# pkg-config support, to install at $(libdir)/pkgconfig
# Define static build dependencies
IF(WIN32)
  SET(PC_LIBS_PRIVATE opengl32 winmm gdi32)
ELSE()
  SET(PC_LIBS_PRIVATE X11 Xxf86vm Xrandr GL)
ENDIF()
# Client applications need to define FreeGLUT GLES version to
# bootstrap headers inclusion in freeglut_std.h:
SET(PC_LIBNAME "glut")
SET(PC_FILENAME "freeglut.pc")
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/freeglut.pc.in ${CMAKE_BINARY_DIR}/freeglut.pc @ONLY)
#INSTALL(FILES ${CMAKE_BINARY_DIR}/freeglut.pc DESTINATION share/pkgconfig/ RENAME ${PC_FILENAME})
# TODO: change the library and .pc name when building for GLES,
# e.g. -lglut-GLES2

#--------------------------------------------------------------------
# Add to package
if(IVW_PACKAGE_PROJECT AND BUILD_SHARED_LIBS)  
   install(TARGETS freeglut
            RUNTIME
            DESTINATION bin
            COMPONENT glut_module)
endif()
