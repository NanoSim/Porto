#
# Settings for win64
#

# HAVE_IOEO_EXITCODE
#    indicates whether the executable would have been able to run on its
#    target platform. If so, set HAVE_IOEO_EXITCODE to
#    the exit code (in many cases 0 for success), otherwise enter "FAILED_TO_RUN".

set( HAVE_IOEO_EXITCODE
     0
     CACHE STRING "Result from TRY_RUN" FORCE)


# H5_PRINTF_LL_TEST_RUN
#    indicates whether the executable would have been able to run on its
#    target platform. If so, set H5_PRINTF_LL_TEST_RUN to
#    the exit code (in many cases 0 for success), otherwise enter "FAILED_TO_RUN".
# H5_PRINTF_LL_TEST_RUN__TRYRUN_OUTPUT
#    contains the text the executable would have printed on stdout and stderr.
#    If the executable would not have been able to run, set H5_PRINTF_LL_TEST_RUN__TRYRUN_OUTPUT empty.
#    Otherwise check if the output is evaluated by the calling CMake code. If so,

set( H5_PRINTF_LL_TEST_RUN
     0
     CACHE STRING "Result from TRY_RUN" FORCE)

set( H5_PRINTF_LL_TEST_RUN__TRYRUN_OUTPUT
     "PRINTF_LL_WIDTH=[I64]"
     CACHE STRING "Output from TRY_RUN" FORCE)


# H5_LDOUBLE_TO_LONG_SPECIAL_RUN
#    indicates whether the executable would have been able to run on its
#    target platform. If so, set H5_LDOUBLE_TO_LONG_SPECIAL_RUN to
#    the exit code (in many cases 0 for success), otherwise enter "FAILED_TO_RUN".
# H5_LDOUBLE_TO_LONG_SPECIAL_RUN__TRYRUN_OUTPUT
#    contains the text the executable would have printed on stdout and stderr.
#    If the executable would not have been able to run, set H5_LDOUBLE_TO_LONG_SPECIAL_RUN__TRYRUN_OUTPUT empty.


set( H5_LDOUBLE_TO_LONG_SPECIAL_RUN
     1
     CACHE STRING "Result from TRY_RUN" FORCE)

set( H5_LDOUBLE_TO_LONG_SPECIAL_RUN__TRYRUN_OUTPUT
     ""
     CACHE STRING "Output from TRY_RUN" FORCE)


# H5_LONG_TO_LDOUBLE_SPECIAL_RUN
#    indicates whether the executable would have been able to run on its
#    target platform. If so, set H5_LONG_TO_LDOUBLE_SPECIAL_RUN to
#    the exit code (in many cases 0 for success), otherwise enter "FAILED_TO_RUN".
# H5_LONG_TO_LDOUBLE_SPECIAL_RUN__TRYRUN_OUTPUT
#    contains the text the executable would have printed on stdout and stderr.
#    If the executable would not have been able to run, set H5_LONG_TO_LDOUBLE_SPECIAL_RUN__TRYRUN_OUTPUT empty.

set( H5_LONG_TO_LDOUBLE_SPECIAL_RUN
     1
     CACHE STRING "Result from TRY_RUN" FORCE)

set( H5_LONG_TO_LDOUBLE_SPECIAL_RUN__TRYRUN_OUTPUT
     ""
     CACHE STRING "Output from TRY_RUN" FORCE)


# H5_LDOUBLE_TO_LLONG_ACCURATE_RUN
#    indicates whether the executable would have been able to run on its
#    target platform. If so, set H5_LDOUBLE_TO_LLONG_ACCURATE_RUN to
#    the exit code (in many cases 0 for success), otherwise enter "FAILED_TO_RUN".
# H5_LDOUBLE_TO_LLONG_ACCURATE_RUN__TRYRUN_OUTPUT
#    contains the text the executable would have printed on stdout and stderr.
#    If the executable would not have been able to run, set H5_LDOUBLE_TO_LLONG_ACCURATE_RUN__TRYRUN_OUTPUT empty.

set( H5_LDOUBLE_TO_LLONG_ACCURATE_RUN
     0
     CACHE STRING "Result from TRY_RUN" FORCE)

set( H5_LDOUBLE_TO_LLONG_ACCURATE_RUN__TRYRUN_OUTPUT
     ""
     CACHE STRING "Output from TRY_RUN" FORCE)


# H5_LLONG_TO_LDOUBLE_CORRECT_RUN
#    indicates whether the executable would have been able to run on its
#    target platform. If so, set H5_LLONG_TO_LDOUBLE_CORRECT_RUN to
#    the exit code (in many cases 0 for success), otherwise enter "FAILED_TO_RUN".
# H5_LLONG_TO_LDOUBLE_CORRECT_RUN__TRYRUN_OUTPUT
#    contains the text the executable would have printed on stdout and stderr.
#    If the executable would not have been able to run, set H5_LLONG_TO_LDOUBLE_CORRECT_RUN__TRYRUN_OUTPUT empty.

set( H5_LLONG_TO_LDOUBLE_CORRECT_RUN
     0
     CACHE STRING "Result from TRY_RUN" FORCE)

set( H5_LLONG_TO_LDOUBLE_CORRECT_RUN__TRYRUN_OUTPUT
     ""
     CACHE STRING "Output from TRY_RUN" FORCE)


# H5_NO_ALIGNMENT_RESTRICTIONS_RUN
#    indicates whether the executable would have been able to run on its
#    target platform. If so, set H5_NO_ALIGNMENT_RESTRICTIONS_RUN to
#    the exit code (in many cases 0 for success), otherwise enter "FAILED_TO_RUN".
# H5_NO_ALIGNMENT_RESTRICTIONS_RUN__TRYRUN_OUTPUT
#    contains the text the executable would have printed on stdout and stderr.
#    If the executable would not have been able to run, set H5_NO_ALIGNMENT_RESTRICTIONS_RUN__TRYRUN_OUTPUT empty.

set( H5_NO_ALIGNMENT_RESTRICTIONS_RUN
     0
     CACHE STRING "Result from TRY_RUN" FORCE)

set( H5_NO_ALIGNMENT_RESTRICTIONS_RUN__TRYRUN_OUTPUT
     ""
     CACHE STRING "Output from TRY_RUN" FORCE)


# RUN_RESULT_VAR
#    indicates whether the executable would have been able to run on its
#    target platform. If so, set RUN_RESULT_VAR to
#    the exit code (in many cases 0 for success), otherwise enter FAILED_TO_RUN.
# RUN_RESULT_VAR__TRYRUN_OUTPUT
#    contains the text the executable would have printed on stdout and stderr.
#    If the executable would not have been able to run, set RUN_RESULT_VAR__TRY_RUN_OUTPUT empty.

#set( RUN_RESULT_VAR
#     53
#     CACHE STRING "Result from TRY_RUN" FORCE)
#
#set( RUN_RESULT_VAR__TRYRUN_OUTPUT
#     ""
#     CACHE STRING "Output from TRY_RUN" FORCE)


# H5_Fortran_REAL_AND_INTEGER_KIND_RUN
set( H5_Fortran_REAL_AND_INTEGER_KIND_RUN
     0
     CACHE STRING "Result from TRY_RUN" FORCE)

set( H5_Fortran_REAL_AND_INTEGER_KIND_RUN__TRYRUN_OUTPUT
     "1,2,4,8,16
4,8,10,16
33
5
4
"
     CACHE STRING "Result from TRY_RUN" FORCE)


# H5_Fortran_SIZEOF_NATIVE_KIND_RUN
set( H5_Fortran_SIZEOF_NATIVE_KIND_RUN
     0
     CACHE STRING "Result from TRY_RUN" FORCE)

set( H5_Fortran_SIZEOF_NATIVE_KIND_RUN__TRYRUN_OUTPUT
     "                    4
           4
                    4
           4
                    8
           8
"
     CACHE STRING "Output from TRY_RUN" FORCE)


# H5_C_MAXIMAL_DECIMAL_PRECISION_RUN
set( H5_C_MAXIMAL_DECIMAL_PRECISION_RUN
     1
     CACHE STRING "Result from TRY_RUN" FORCE)

set( H5_C_MAXIMAL_DECIMAL_PRECISION_RUN__TRYRUN_OUTPUT
     "21
33"
     CACHE STRING "Output from TRY_RUN" FORCE)
