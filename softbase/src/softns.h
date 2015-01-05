#ifndef SOFTNS_H_DEF
#define SOFTNS_H_DEF

#if !defined (SOFT_NAMESPACE)
# define SOFT_NAMESPACE soft
# define SOFT_BEGIN_NAMESPACE namespace SOFT_NAMESPACE {
# define SOFT_END_NAMESPACE }
#endif

#if !defined (ANONYMOUS_NAMESPACE)
# define ANONYMOUS_NAMESPACE
# define ANONYMOUS_BEGIN_NAMESPACE namespace {
# define ANONYMOUS_END_NAMESPACE }
#endif

#endif // SOFTNS_H_DEF
