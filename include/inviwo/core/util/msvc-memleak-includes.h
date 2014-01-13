#ifndef IVW_MSVC_MEMLEAK_INCLUDES_H
#define IVW_MSVC_MEMLEAK_INCLUDES_H

//#if !defined(INVIWO_EXPORTS)



#ifdef _MSC_VER
    #ifdef _DEBUG
        #ifdef IVW_ENABLE_MSVC_MEM_LEAK_TEST
            #ifdef _CRTDBG_MAP_ALLOC
                #error _CRTDBG_MAP_ALLOC already defined
            #endif
/* #ifdef _INC_STDLIB
#error stdlib.h already included
#endif*/
            //#ifdef _INC_CRTDBG
            //    #error crtdbg.h already included
            //#endif
            #ifdef QT_CORE_LIB
                //#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
                //#define new DEBUG_NEW
            #else
                #define _CRTDBG_MAP_ALLOC
                #include <stdlib.h>
                #include <crtdbg.h>
                #define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
                #define new DEBUG_NEW
            #endif
        #endif
//    #endif

#endif




#endif
#endif