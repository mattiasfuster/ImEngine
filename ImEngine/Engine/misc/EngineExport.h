
#ifndef IMENGINE_API_H
#define IMENGINE_API_H

#ifdef IMENGINE_ENGINE_STATIC_DEFINE
#  define IMENGINE_API
#  define IMENGINE_ENGINE_NO_EXPORT
#else
#  ifndef IMENGINE_API
#    ifdef ImEngine_Engine_EXPORTS
        /* We are building this library */
#      define IMENGINE_API __declspec(dllexport)
#    else
        /* We are using this library */
#      define IMENGINE_API __declspec(dllimport)
#    endif
#  endif

#  ifndef IMENGINE_ENGINE_NO_EXPORT
#    define IMENGINE_ENGINE_NO_EXPORT 
#  endif
#endif

#ifndef IMENGINE_ENGINE_DEPRECATED
#  define IMENGINE_ENGINE_DEPRECATED __declspec(deprecated)
#endif

#ifndef IMENGINE_ENGINE_DEPRECATED_EXPORT
#  define IMENGINE_ENGINE_DEPRECATED_EXPORT IMENGINE_API IMENGINE_ENGINE_DEPRECATED
#endif

#ifndef IMENGINE_ENGINE_DEPRECATED_NO_EXPORT
#  define IMENGINE_ENGINE_DEPRECATED_NO_EXPORT IMENGINE_ENGINE_NO_EXPORT IMENGINE_ENGINE_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef IMENGINE_ENGINE_NO_DEPRECATED
#    define IMENGINE_ENGINE_NO_DEPRECATED
#  endif
#endif

#endif /* IMENGINE_API_H */
