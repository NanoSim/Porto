
#ifndef SOFTC_MACROS_H
#define SOFTC_MACROS_H

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

#ifdef __cplusplus
#  define SOFTC_BEGIN_DECLS extern "C" {
#  define SOFTC_END_DECLS }
#else
#  define SOFTC_BEGIN_DECLS
#  define SOFTC_END_DECLS
#endif

#define SOFT_UNUSED(arg) (void)arg;

#endif 
