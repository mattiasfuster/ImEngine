
#ifndef IMENGINE_RUNTIME_API_H
#define IMENGINE_RUNTIME_API_H

#ifdef IMENGINE_RUNTIME_STATIC_DEFINE
#  define IMENGINE_RUNTIME_API
#  define IMENGINE_RUNTIME_NO_EXPORT
#else
#  ifndef IMENGINE_RUNTIME_API
#    ifdef ImEngine_Runtime_EXPORTS
        /* We are building this library */
#      define IMENGINE_RUNTIME_API __declspec(dllexport)
#    else
        /* We are using this library */
#      define IMENGINE_RUNTIME_API __declspec(dllimport)
#    endif
#  endif

#  ifndef IMENGINE_RUNTIME_NO_EXPORT
#    define IMENGINE_RUNTIME_NO_EXPORT 
#  endif
#endif

#ifndef IMENGINE_RUNTIME_DEPRECATED
#  define IMENGINE_RUNTIME_DEPRECATED __declspec(deprecated)
#endif

#ifndef IMENGINE_RUNTIME_DEPRECATED_EXPORT
#  define IMENGINE_RUNTIME_DEPRECATED_EXPORT IMENGINE_RUNTIME_API IMENGINE_RUNTIME_DEPRECATED
#endif

#ifndef IMENGINE_RUNTIME_DEPRECATED_NO_EXPORT
#  define IMENGINE_RUNTIME_DEPRECATED_NO_EXPORT IMENGINE_RUNTIME_NO_EXPORT IMENGINE_RUNTIME_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef IMENGINE_RUNTIME_NO_DEPRECATED
#    define IMENGINE_RUNTIME_NO_DEPRECATED
#  endif
#endif

#endif /* IMENGINE_RUNTIME_API_H */
