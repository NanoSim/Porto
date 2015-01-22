#ifndef SOFTC_MACROS_H
#define SOFTC_MACROS_H

#ifdef __cplusplus
#    define SOFT_BEGIN_DECLS extern "C" {
#    define SOFT_END_DECLS }
#else /* __cplusplus not defined */
#    define SOFT_BEGIN_DECLS
#    define SOFT_END_DECLS
#endif /* __cplusplus */

#endif // SOFTC_MACROS_H
