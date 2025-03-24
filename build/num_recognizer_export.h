
#ifndef NUM_RECOGNIZER_EXPORT_H
#define NUM_RECOGNIZER_EXPORT_H

#ifdef NUM_RECOGNIZER_STATIC_DEFINE
#  define NUM_RECOGNIZER_EXPORT
#  define NUM_RECOGNIZER_NO_EXPORT
#else
#  ifndef NUM_RECOGNIZER_EXPORT
#    ifdef num_recognizer_EXPORTS
        /* We are building this library */
#      define NUM_RECOGNIZER_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define NUM_RECOGNIZER_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef NUM_RECOGNIZER_NO_EXPORT
#    define NUM_RECOGNIZER_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef NUM_RECOGNIZER_DEPRECATED
#  define NUM_RECOGNIZER_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef NUM_RECOGNIZER_DEPRECATED_EXPORT
#  define NUM_RECOGNIZER_DEPRECATED_EXPORT NUM_RECOGNIZER_EXPORT NUM_RECOGNIZER_DEPRECATED
#endif

#ifndef NUM_RECOGNIZER_DEPRECATED_NO_EXPORT
#  define NUM_RECOGNIZER_DEPRECATED_NO_EXPORT NUM_RECOGNIZER_NO_EXPORT NUM_RECOGNIZER_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef NUM_RECOGNIZER_NO_DEPRECATED
#    define NUM_RECOGNIZER_NO_DEPRECATED
#  endif
#endif

#endif /* NUM_RECOGNIZER_EXPORT_H */
