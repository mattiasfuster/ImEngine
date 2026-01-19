
#ifndef IMENGINE_CORE_API_H
#define IMENGINE_CORE_API_H

#ifdef IMENGINE_CORE_STATIC_DEFINE
#  define IMENGINE_CORE_API
#  define IMENGINE_CORE_NO_EXPORT
#else
#  ifndef IMENGINE_CORE_API
#    ifdef ImEngine_Core_EXPORTS
        /* We are building this library */
#      define IMENGINE_CORE_API __declspec(dllexport)
#    else
        /* We are using this library */
#      define IMENGINE_CORE_API __declspec(dllimport)
#    endif
#  endif

#  ifndef IMENGINE_CORE_NO_EXPORT
#    define IMENGINE_CORE_NO_EXPORT 
#  endif
#endif

#ifndef IMENGINE_CORE_DEPRECATED
#  define IMENGINE_CORE_DEPRECATED __declspec(deprecated)
#endif

#ifndef IMENGINE_CORE_DEPRECATED_EXPORT
#  define IMENGINE_CORE_DEPRECATED_EXPORT IMENGINE_CORE_API IMENGINE_CORE_DEPRECATED
#endif

#ifndef IMENGINE_CORE_DEPRECATED_NO_EXPORT
#  define IMENGINE_CORE_DEPRECATED_NO_EXPORT IMENGINE_CORE_NO_EXPORT IMENGINE_CORE_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef IMENGINE_CORE_NO_DEPRECATED
#    define IMENGINE_CORE_NO_DEPRECATED
#  endif
#endif

#endif /* IMENGINE_CORE_API_H */
