
ivw_project(ilmthread)

#--------------------------------------------------------------------
# Supress warnings
if(WIN32)
add_definitions( "/W0 /D_CRT_SECURE_NO_WARNINGS /wd4005 /wd4996 /nologo /IGNORE:4006" )
endif()

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    IlmThread.h
    IlmThreadMutex.h
    IlmThreadPool.h
    IlmThreadSemaphore.h
)
ivw_group("Iex Header Files" ${HEADER_FILES} )

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    IlmThread.cpp
    IlmThreadMutex.cpp
    IlmThreadMutexPosix.cpp
    IlmThreadPool.cpp
    IlmThreadPosix.cpp
    IlmThreadPool.cpp
    IlmThreadSemaphore.cpp
    IlmThreadSemaphorePosix.cpp
    IlmThreadSemaphorePosixCompat.cpp
)
if(WIN32)
    set(WIN32_FILES
        IlmThreadMutexWin32.cpp
        IlmThreadSemaphoreWin32.cpp
        IlmThreadWin32.cpp
    )
    list(APPEND SOURCE_FILES ${WIN32_FILES})
endif()

ivw_group("Iex Source Files" ${SOURCE_FILES} )

#--------------------------------------------------------------------
# Define defintions
ivw_define_standard_definitions(ilmthread)

#--------------------------------------------------------------------
# Create library
add_library(ilmthread ${SOURCE_FILES} ${HEADER_FILES})

#--------------------------------------------------------------------
# Creates VS folder structure
ivw_vs_folder(ilmthread ext/freeimagelib/openexrlib)

ivw_add_dependencies(Iex)
ivw_include_directories(${IVW_EXTENSIONS_DIR}/freeimage/openexr/)
ivw_include_directories(${IVW_EXTENSIONS_DIR}/freeimage/openexr/iex)

#--------------------------------------------------------------------
# Make package (for other projects to find)
ivw_make_package(IlmThread ilmthread)