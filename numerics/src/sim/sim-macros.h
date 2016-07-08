#ifndef SIM_MACROS_H
#define SIM_MACROS_H

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

#ifdef __cplusplus
#  define SIM_BEGIN_DECLS extern "C" {
#  define SIM_END_DECLS }
#else
#  define SIM_BEGIN_DECLS
#  define SIM_END_DECLS
#endif

#define SIM_UNUSED(arg) (void)arg;

#endif 
