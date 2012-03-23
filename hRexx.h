/* HREXX.H      (c)Copyright Enrico Sorichetti, 2012                 */
/*              REXX Interpreter Support                             */
/*                                                                   */
/*  Released under "The Q Public License Version 1"                  */
/*  (http://www.hercules-390.org/herclic.html) as modifications to   */
/*  Hercules.                                                        */

/*  inspired by the previous Rexx implementation by Jan Jaeger       */

// $Id:$

#ifndef _HREXX_H_
#define _HREXX_H_

#ifdef  _HREXX_C_
#define _HREXX_EXTERN
#else
#define _HREXX_EXTERN extern
#endif

#define REGINA_PACKAGE "Regina"
#define OOREXX_PACKAGE "ooRexx"

#define hSubcom  "HERCULES"
#define hSIOExit "HERCSIOE"

#if     defined( _MSVC_ )
#define EXTNDELIM             ";"
#define EXTENSIONS            ".REXX;.rexx;.REX;.rex;.CMD;.cmd;.RX;.rx"
#define PATHDELIM             ";"
#define PATHFORMAT            "%s\\%s%s"
#define REGINA_LIBRARY        "regina.dll"
#define OOREXX_LIBRARY        "rexx.dll"
#define OOREXX_API_LIBRARY    "rexxapi.dll"

#elif   defined ( __APPLE__ )
#define EXTNDELIM             ":"
#define EXTENSIONS            ".REXX:.rexx:.REX:.rex;.CMD:.cmd:.RX:.rx"
#define PATHDELIM             ":"
#define PATHFORMAT            "%s/%s%s"
#define REGINA_LIBRARY        "libregina.dylib"
#define OOREXX_LIBRARY        "librexx.dylib"
#define OOREXX_API_LIBRARY    "librexxapi.dylib"

#else
#define EXTNDELIM             ":"
#define EXTENSIONS            ".REXX:.rexx:.REX:.rex;.CMD:.cmd:.RX:.rx"
#define PATHDELIM             ":"
#define PATHFORMAT            "%s/%s%s"
#define REGINA_LIBRARY        "libregina.so"
#define OOREXX_LIBRARY        "librexx.so"
#define OOREXX_API_LIBRARY    "librexxapi.so"
#endif

#define REXX_START             "RexxStart"
#define REXX_REGISTER_SUBCOM   "RexxRegisterSubcomExe"
#define REXX_DEREGISTER_SUBCOM "RexxDeregisterSubcom"
#define REXX_REGISTER_EXIT     "RexxRegisterExitExe"
#define REXX_DEREGISTER_EXIT   "RexxDeregisterExit"
#define REXX_ALLOCATE_MEMORY   "RexxAllocateMemory"
#define REXX_FREE_MEMORY       "RexxFreeMemory"

#define REXX_VARIABLE_POOL     "RexxVariablePool"

#if defined ( _MSVC_ )
#define HDLOPEN( _LIBHND_, _LIBNAM_ , _LIBPAR_ ) do {\
    _LIBHND_  = LoadLibrary( ( _LIBNAM_ ) );\
    if (! ( _LIBHND_ )  ) { \
        WRMSG( HHC17531, "E", RexxPackage, ( _LIBNAM_ ) ) ; \
        return -1; \
    } \
} while (0)
#define HDLCLOSE( _LIBHND_) do {\
    if ( FreeLibrary( ( _LIBHND_ ) ) == 0 ) { \
        WRMSG( HHC17532, "E", RexxPackage, ( _LIBHND_ ) ) ; \
        return -1; \
    } \
    ( _LIBHND_ ) = NULL; \
} while (0)
#define HDLSYM(_SYMHND_, _LIBHND_, _SYMNAM_ ) do {\
    ( _SYMHND_ ) = (void *) GetProcAddress( ( _LIBHND_ ), ( _SYMNAM_ ) ) ; \
    if (! ( _SYMHND_ )  ) { \
        WRMSG( HHC17533, "E", RexxPackage, ( _SYMNAM_ ), "") ; \
        return -1; \
    } \
} while (0)

#else
#define HDLOPEN( _LIBHND_, _LIBNAM_, _LIBPAR_ ) do {\
    ( _LIBHND_ )  = dlopen( ( _LIBNAM_ ), ( _LIBPAR_ ) );\
    if (! ( _LIBHND_ )  ) { \
        WRMSG( HHC17530, "E", RexxPackage, dlerror() ) ; \
        return -1; \
    } \
} while (0)
#define HDLCLOSE( _LIBHND_) do {\
    if ( dlclose( ( _LIBHND_ ) ) != 0 ) { \
        WRMSG( HHC17530, "E", RexxPackage, dlerror() ) ; \
        return -1; \
    } \
    ( _LIBHND_ ) = NULL; \
} while (0)
#define HDLSYM(_SYMHND_, _LIBNAM_, _SYMNAM_ ) do {\
    ( _SYMHND_ )  = dlsym( ( _LIBNAM_ ), ( _SYMNAM_ ) );\
    if (! ( _SYMHND_ ) ) { \
        WRMSG( HHC17530, "E", RexxPackage, dlerror() ) ; \
        return -1; \
    } \
} while (0)

#endif


#ifndef _HREXX_TRIM_C
#define _HREXX_TRIM_C

#ifdef  _HREXX_C_
char *trim(char *str)
{
    char *cp1 ;
    char *cp2 ;

    // skip leading spaces
    for (cp1=str ; isspace(*cp1) ; cp1++ ) ;
    cp2 = str ;

    // shift left remaining chars, via cp2 and terminate string
    for ( ; *cp1 ; *cp2++=*cp1++) ;
    *cp2-- = 0 ;

    // replace trailing spaces with '\0'
    while ( cp2 > str && isspace(*cp2) )
        *cp2-- = 0 ;
    return str ;
}

#else
char *trim(char *str) ;

#endif /* #ifdef  _HREXX_C_  */

#endif /* #ifndef _HREXX_TRIM_C */

#ifndef _HREXX_PARSE_COMMAND_C
#define _HREXX_PARSE_COMMAND_C

#ifdef  _HREXX_C_
int parse_command(char *p, int argm, char **argv, int *argc)
{
    *argc = 0;

    // extract argv[0]
    while ( *p && isspace(*p) ) p++;
    if  ( !*p ) return (*argc) ;

    *argv = p ;
    (*argc)++ ;
    while (*p && *p != '(' ) p++;
    if  ( !*p ) return (*argc) ;

    *p++ = 0 ;
    while (*p && *argc < argm )
    {
        argv++ ;
        while ( *p && isspace(*p) ) p++;
        if  ( ! *p ) return (*argc) ;

        *argv = p;
        (*argc)++ ;
        while ( *p && !isspace(*p) ) p++;
        if (!*p) return (*argc);

        *p++ = 0;
    }
    return (*argc);
}

#else
int parse_command(char *p, int argm, char **argv, int *argc) ;

#endif /* #ifdef  _HREXX_C_  */

#endif /* #ifndef _HREXX_PARSE_COMMAND_C */

#endif /* #ifndef _HREXX_H_  */
