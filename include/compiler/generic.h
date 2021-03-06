/* Copyright (c) 2018 Griefer@Work                                            *
 *                                                                            *
 * This software is provided 'as-is', without any express or implied          *
 * warranty. In no event will the authors be held liable for any damages      *
 * arising from the use of this software.                                     *
 *                                                                            *
 * Permission is granted to anyone to use this software for any purpose,      *
 * including commercial applications, and to alter it and redistribute it     *
 * freely, subject to the following restrictions:                             *
 *                                                                            *
 * 1. The origin of this software must not be misrepresented; you must not    *
 *    claim that you wrote the original software. If you use this software    *
 *    in a product, an acknowledgement in the product documentation would be  *
 *    appreciated but is not required.                                        *
 * 2. Altered source versions must be plainly marked as such, and must not be *
 *    misrepresented as being the original software.                          *
 * 3. This notice may not be removed or altered from any source distribution. *
 */
#define __GCC_VERSION(a,b,c) 0
#ifndef __option
#define __option(x) 0
#endif
#ifndef __has_builtin
#define __has_builtin(x) 0
#endif
#ifndef __has_feature
#define __has_feature(x) 0
#endif
#ifndef __has_extension
#define __has_extension  __has_feature
#endif
#ifndef __has_attribute
#define __has_attribute(x) 0
#endif
#ifndef __has_declspec_attribute
#define __has_declspec_attribute(x) 0
#endif
#ifndef __has_cpp_attribute
#define __has_cpp_attribute(x) 0
#endif
#ifndef __has_include
#define __has_include(x) 0
#endif
#ifndef __has_include_next
#define __has_include_next(x) 0
#endif

#if __has_builtin(__builtin_expect) || \
   (defined(__INTEL_VERSION__) && __INTEL_VERSION__ >= 800)
#   define __likely(x)   (__builtin_expect(!!(x),1))
#   define __unlikely(x) (__builtin_expect(!!(x),0))
#else
#   define __builtin_expect(x,y) (x)
#   define __NO_builtin_expect    1
#   define __likely      /* Nothing */
#   define __unlikely    /* Nothing */
#endif
#if defined(__STDC_VERSION__) && __STDC_VERSION__+0 >= 201112L
#define __COMPILER_HAVE_C11_GENERIC 1
#endif
#if __has_builtin(__builtin_choose_expr)
/* Already a builtin. */
#elif defined(__COMPILER_HAVE_C11_GENERIC)
/* Use C11's _Generic keyword. */
#define __builtin_choose_expr(c,tt,ff) \
    _Generic((char(*)[1+!!(c)])0,char(*)[1]:(ff),default:(tt))
#else
#define __NO_builtin_choose_expr 1
#define __builtin_choose_expr(c,tt,ff) ((c)?(tt):(ff))
#endif
#if !__has_builtin(__builtin_types_compatible_p)
#define __NO_builtin_types_compatible_p   1
#define __builtin_types_compatible_p(...) 0
#endif

#ifdef __STDC__
#   define __P(x) x
#else
#   define __NO_PROTOTYPES 1
#   define __P(x) ()
#endif

/* TODO: Unify use of `__FUNCTION__'
#if __STDC_VERSION__ >= 199901L
#define __FUNCTION__ __func__
#endif
*/

#if !defined(__NO_LONG_LONG) && !defined(__DARWIN_NO_LONG_LONG) && \
   ((defined(__BORLANDC__) && __BORLANDC__ >= 0x561) || defined(__SUNPRO_CC) || \
     defined(__TINYC__) || defined(__DCC_VERSION__) || \
     defined(__CODEGEARC__) || defined(__DMC__) || \
    (defined(__HP_aCC) && __HP_aCC+0 >= 33900) || \
    (defined(__PGIC__) && __PGIC__+0 >= 10))
#   define __COMPILER_HAVE_LONGLONG 1
#endif
#define __COMPILER_HAVE_LONGDOUBLE 1
#define __COMPILER_HAVE_TRANSPARENT_STRUCT 1
#define __COMPILER_HAVE_TRANSPARENT_UNION 1
#if defined(__DCC_VERSION__) || defined(__TINYC__)
#define __COMPILER_HAVE_GCC_ASM 1
#define __COMPILER_HAVE_PRAGMA_PACK 1 /* XXX: So many more compilers support this... */
#elif defined(_PUSHPOP_SUPPORTED)
#define __COMPILER_HAVE_PRAGMA_PACK 1
#endif
#if __has_feature(__tpp_pragma_push_macro__) || \
   (defined(__TPP_VERSION__) && __TPP_VERSION__ == 103)
#define __COMPILER_HAVE_PRAGMA_PUSHMACRO 1
#endif
#if __has_feature(__tpp_pragma_deprecated__)
#define __COMPILER_HAVE_PRAGMA_DEPRECATED 1
#endif

#if defined(__DCC_VERSION__)
#   define __COMPILER_HAVE_AUTOTYPE 1
#elif __has_feature(cxx_auto_type)
#   define __auto_type              auto
#   define __COMPILER_HAVE_AUTOTYPE 1
#endif
#if defined(__DCC_VERSION__) || defined(__TINYC__)
#   define __COMPILER_HAVE_TYPEOF   1
#endif

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x599
#pragma defineonoption __CODEGEAR_0X_SUPPORT__ -Ax
#endif

#if __has_feature(cxx_static_assert) || \
   (defined(__IBMCPP_STATIC_ASSERT) && __IBMCPP_STATIC_ASSERT+0) || (defined(__cplusplus) && (\
   (defined(__BORLANDC__) && defined(__CODEGEAR_0X_SUPPORT__) && __BORLANDC__ >= 0x610) || \
   (defined(__CODEGEARC__) && __CODEGEARC__ > 0x620)))
#   define __STATIC_ASSERT(expr)         static_assert(expr,#expr)
#   define __STATIC_ASSERT_MSG(expr,msg) static_assert(expr,msg)
#elif defined(_Static_assert) || __has_feature(c_static_assert) || \
     (!defined(__cplusplus) && ( \
     (defined(__STDC_VERSION__) && __STDC_VERSION__+0 >= 201112L) || \
     (__GCC_VERSION(4,6,0) && !defined(__STRICT_ANSI__))))
#   define __STATIC_ASSERT(expr)         _Static_assert(expr,#expr)
#   define __STATIC_ASSERT_MSG(expr,msg) _Static_assert(expr,msg)
#elif defined(__TPP_COUNTER)
#   define __STATIC_ASSERT(expr)         typedef int __PP_CAT2(__static_assert_,__TPP_COUNTER(__static_assert))[(expr)?1:-1]
#   define __STATIC_ASSERT_MSG(expr,msg) typedef int __PP_CAT2(__static_assert_,__TPP_COUNTER(__static_assert))[(expr)?1:-1]
#elif defined(__COUNTER__)
#   define __STATIC_ASSERT(expr)         typedef int __PP_CAT2(__static_assert_,__COUNTER__)[(expr)?1:-1]
#   define __STATIC_ASSERT_MSG(expr,msg) typedef int __PP_CAT2(__static_assert_,__COUNTER__)[(expr)?1:-1]
#else
#   define __STATIC_ASSERT(expr)         typedef int __PP_CAT2(__static_assert_,__LINE__)[(expr)?1:-1]
#   define __STATIC_ASSERT_MSG(expr,msg) typedef int __PP_CAT2(__static_assert_,__LINE__)[(expr)?1:-1]
#endif

#if defined(__DCC_VERSION__) || defined(__TINYC__)
#   define __ASMNAME(x)   __asm__(x)
#else
#   define __NO_ASMNAME   1
#   define __ASMNAME(x)   /* Nothing */
#endif
#ifndef __DCC_VERSION__
#   define __extension__
#endif
#if __has_builtin(__builtin_va_list)
#   define __builtin_va_list      __builtin_va_list
#endif
#if !defined(__DCC_VERSION__) && !defined(__FUNCTION__)
#if defined(__TINYC__) || 1
#   define __FUNCTION__ __func__
#elif __has_builtin(__builtin_FUNCTION)
#   define __FUNCTION__ __builtin_FUNCTION()
#else
#   define __NO_FUNCTION__ 1
#   define __FUNCTION__ (char *)0
#endif
#endif
#if __has_attribute(__noinline__)
#   define __ATTR_NOINLINE       __attribute__((__noinline__))
#elif __has_declspec_attribute(noinline)
#   define __ATTR_NOINLINE       __declspec(noinline)
#else
#   define __NO_ATTR_NOINLINE    1
#   define __ATTR_NOINLINE       /* Nothing */
#endif
#if __has_attribute(__noreturn__) || defined(__TINYC__)
#   define __ATTR_NORETURN       __attribute__((__noreturn__))
#elif __has_declspec_attribute(noreturn)
#   define __ATTR_NORETURN       __declspec(noreturn)
#elif (defined(__SUNPRO_C) && (__SUNPRO_C >= 0x590))
#   define __ATTR_NORETURN       __attribute__((noreturn))
#elif defined(_Noreturn) || \
     (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112) /* __GCC_VERSION(4,7,0) */
#   define __ATTR_NORETURN       _Noreturn
#elif __has_cpp_attribute(noreturn) || \
     (defined(__cplusplus) && defined(_MSC_VER) && _MSC_VER >= 1900)
#   undef noreturn
#   define __ATTR_NORETURN       [[noreturn]]
#elif defined(noreturn)
#   define __ATTR_NORETURN       noreturn
#else
#   define __NO_ATTR_NORETURN    1
#   define __ATTR_NORETURN       /* Nothing */
#endif
#define __NO_ATTR_FALLTHROUGH    1
#define __ATTR_FALLTHROUGH       /* Nothing */
#define __NO_ATTR_W64            1
#define __ATTR_W64               /* Nothing */
#if __has_attribute(__fastcall__) || defined(__TINYC__)
#   define __ATTR_FASTCALL       __attribute__((__fastcall__))
#else
#   define __NO_ATTR_FASTCALL    1
#   define __ATTR_FASTCALL       /* Nothing */
#endif
#if __has_attribute(__stdcall__) || defined(__TINYC__)
#   define __ATTR_STDCALL        __attribute__((__stdcall__))
#else
#   define __NO_ATTR_STDCALL     1
#   define __ATTR_STDCALL        /* Nothing */
#endif
#if __has_attribute(__cdecl__) || defined(__TINYC__)
#   define __ATTR_CDECL          __attribute__((__cdecl__))
#else
#   define __NO_ATTR_CDECL       1
#   define __ATTR_CDECL          /* Nothing */
#endif
#if __has_attribute(__ms_abi__) || defined(__TINYC__)
#   define __ATTR_MSABI          __attribute__((__ms_abi__))
#else
#   define __NO_ATTR_MSABI       1
#   define __ATTR_MSABI          /* Nothing */
#endif
#if __has_attribute(__sysv_abi__) || defined(__TINYC__)
#   define __ATTR_SYSVABI        __attribute__((__sysv_abi__))
#else
#   define __NO_ATTR_SYSVABI     1
#   define __ATTR_SYSVABI        /* Nothing */
#endif
#if __has_attribute(__pure__)
#   define __ATTR_PURE           __attribute__((__pure__))
#elif __has_declspec_attribute(noalias)
#   define __ATTR_PURE           __declspec(noalias)
#else
#   define __NO_ATTR_PURE        1
#   define __ATTR_PURE           /* Nothing */
#endif
#if __has_attribute(__const__)
#   define __ATTR_CONST          __attribute__((__const__))
#elif !defined(__NO_ATTR_PURE)
#   define __ATTR_CONST          __ATTR_PURE
#else
#   define __NO_ATTR_CONST       1
#   define __ATTR_CONST          /* Nothing */
#endif
#if __has_attribute(__malloc__)
#   define __ATTR_MALLOC         __attribute__((__malloc__))
#else
#   define __NO_ATTR_MALLOC      1
#   define __ATTR_MALLOC         /* Nothing */
#endif
#if __has_attribute(__hot__)
#   define __ATTR_HOT            __attribute__((__hot__))
#else
#   define __NO_ATTR_HOT         1
#   define __ATTR_HOT            /* Nothing */
#endif
#if __has_attribute(__cold__) || \
   (defined(__ICC) && __ICC+0 > 1110)
#   define __ATTR_COLD           __attribute__((__cold__))
#else
#   define __NO_ATTR_COLD        1
#   define __ATTR_COLD           /* Nothing */
#endif
#if __has_attribute(__alloc_size__)
#   define __ATTR_ALLOC_SIZE(ppars) __attribute__((__alloc_size__ ppars))
#else
#   define __NO_ATTR_ALLOC_SIZE  1
#   define __ATTR_ALLOC_SIZE(ppars) /* Nothing */
#endif
#if __has_attribute(__alloc_align__)
#   define __ATTR_ALLOC_ALIGN(pari) __attribute__((__alloc_align__(pari)))
#else
#   define __NO_ATTR_ALLOC_ALIGN 1
#   define __ATTR_ALLOC_ALIGN(pari) /* Nothing */
#endif
#if __has_attribute(__assume_aligned__)
#   define __ATTR_ASSUME_ALIGNED(n) __attribute__((__assume_aligned__(n)))
#else
#   define __NO_ATTR_ASSUME_ALIGNED 1
#   define __ATTR_ASSUME_ALIGNED(n) /* Nothing */
#endif
#if __has_attribute(__noclone__)
#   define __ATTR_NOCLONE        __attribute__((__noclone__))
#else
#   define __NO_ATTR_NOCLONE     1
#   define __ATTR_NOCLONE        /* Nothing */
#endif
#if __has_attribute(__used__)
#   define __ATTR_USED           __attribute__((__used__))
#else
#   define __NO_ATTR_USED        1
#   define __ATTR_USED           /* Nothing */
#endif
#if __has_attribute(__unused__)
#   define __ATTR_UNUSED         __attribute__((__unused__))
#else
#   define __NO_ATTR_UNUSED      1
#   define __ATTR_UNUSED         /* Nothing */
#endif
#if __has_attribute(__sentinel__)
#   define __ATTR_SENTINEL       __attribute__((__sentinel__))
#ifdef __INTELLISENSE__
#   define __ATTR_SENTINEL_O(x)  __attribute__((__sentinel__))
#else
#   define __ATTR_SENTINEL_O(x)  __attribute__((__sentinel__(x)))
#endif
#else
#   define __NO_ATTR_SENTINEL    1
#   define __NO_ATTR_SENTINEL_O  1
#   define __ATTR_SENTINEL       /* Nothing */
#   define __ATTR_SENTINEL_O(x)  /* Nothing */
#endif
#if __has_feature(cxx_thread_local) || (defined(__cplusplus) && \
   (defined(__SUNPRO_CC) || defined(__IBMC__) || defined(__IBMCPP__)))
#   define __ATTR_THREAD         thread_local
#elif __has_feature(c_thread_local) || \
     (!defined(__cplusplus) && defined(__STDC_VERSION__) && __STDC_VERSION__ > 201000L)
#   define __ATTR_THREAD         _Thread_local
#elif __has_declspec_attribute(thread) || defined(__BORLANDC__) || defined(__DMC__)
#   define __ATTR_THREAD         __declspec(thread)
#elif (defined(__INTEL_COMPILER) || defined(__ICC) || defined(__ICL) || defined(__ECC))
#if defined(_WIN32) || defined(WIN32)
#   define __ATTR_THREAD         __thread
#else
#   define __ATTR_THREAD         __declspec(thread)
#endif
#else
#   define __NO_ATTR_THREAD      1
#   define __ATTR_THREAD         /* Nothing */
#endif
#if __has_attribute(__deprecated__)
#   define __ATTR_DEPRECATED_      __attribute__((__deprecated__))
#   define __ATTR_DEPRECATED(text) __attribute__((__deprecated__(text)))
#elif __has_declspec_attribute(deprecated)
#   define __ATTR_DEPRECATED_      __declspec(deprecated)
#   define __ATTR_DEPRECATED(text) __declspec(deprecated(text))
#elif __has_cpp_attribute(deprecated) >= 201309
#   define __ATTR_DEPRECATED_      [[deprecated]]
#   define __ATTR_DEPRECATED(text) [[deprecated(text)]]
#elif defined(__SUNPRO_C) && __SUNPRO_C >= 0x5130
#   define __ATTR_DEPRECATED_      __attribute__((deprecated))
#   define __ATTR_DEPRECATED(text) __attribute__((deprecated))
#else
#   define __NO_ATTR_DEPRECATED    1
#   define __ATTR_DEPRECATED_      /* Nothing */
#   define __ATTR_DEPRECATED(text) /* Nothing */
#endif
#if __has_attribute(__warning__)
#   define __ATTR_WARNING(text)  __attribute__((__warning__(text)))
#else
#   define __NO_ATTR_WARNING     1
#   define __ATTR_WARNING(text)  /* Nothing */
#endif
#if __has_attribute(__error__)
#   define __ATTR_ERROR(text)    __attribute__((__error__(text)))
#else
#   define __NO_ATTR_ERROR       1
#   define __ATTR_ERROR(text)    /* Nothing */
#endif
#if __has_attribute(__section__) || defined(__TINYC__)
#   define __ATTR_SECTION(name)  __attribute__((__section__(name)))
#else
#   define __NO_ATTR_SECTION     1
#   define __ATTR_SECTION(name)  /* Nothing */
#endif
#if __has_attribute(__nothrow__)
#   define __ATTR_NOTHROW        __attribute__((__nothrow__))
#else
#   define __NO_ATTR_NOTHROW     1
#   define __ATTR_NOTHROW        /* Nothing */
#endif
#if __has_attribute(__optimize__)
#   define __ATTR_OPTIMIZE(opt)  __attribute__((__optimize__(opt)))
#else
#   define __NO_ATTR_OPTIMIZE    1
#   define __ATTR_OPTIMIZE(opt)  /* Nothing */
#endif
#if __has_attribute(__returns_nonnull__)
#   define __ATTR_RETNONNULL     __attribute__((__returns_nonnull__))
#else
#   define __NO_ATTR_RETNONNULL  1
#   define __ATTR_RETNONNULL     /* Nothing */
#endif
#if __has_attribute(__packed__) || defined(__TINYC__)
#   define __ATTR_PACKED         __attribute__((__packed__))
#else
#   define __NO_ATTR_PACKED      1
#   define __ATTR_PACKED         /* Nothing */
#endif
#if __has_attribute(__alias__) || defined(__TINYC__)
#   define __ATTR_ALIAS(name)    __attribute__((__alias__(name)))
#else
#   define __NO_ATTR_ALIAS       1
#   define __ATTR_ALIAS(name)    /* Nothing */
#endif
#if __has_attribute(__aligned__) || defined(__TINYC__)
#   define __ATTR_ALIGNED(n)     __attribute__((__aligned__(n)))
#elif __has_declspec_attribute(align)
#   define __ATTR_ALIGNED(n)     __declspec(align(n))
#elif __has_feature(cxx_alignas) || __has_extension(cxx_alignas)
#   define __ATTR_ALIGNED(n)     alignas(n)
#else
#   define __NO_ATTR_ALIGNED     1
#   define __ATTR_ALIGNED(n)     /* Nothing */
#endif
#if __has_attribute(__selectany__)
#   define __ATTR_SELECTANY      __attribute__((__selectany__))
#elif __has_declspec_attribute(selectany)
#   define __ATTR_SELECTANY      __declspec(selectany)
#else
#   define __NO_ATTR_SELECTANY   1
#   define __ATTR_SELECTANY      /* Nothing */
#endif
#if __has_attribute(__weak__) || \
   (defined(__ELF__) || defined(__TINYC__))
#   define __ATTR_WEAK           __attribute__((__weak__))
#elif !defined(__NO_ATTR_SELECTANY)
#   define __ATTR_WEAK           __ATTR_SELECTANY
#   define __ATTR_WEAK_IS_SELECTANY 1
#else
#   define __NO_ATTR_WEAK        1
#   define __ATTR_WEAK           /* Nothing */
#endif
#if __has_attribute(__returns_twice__)
#   define __ATTR_RETURNS_TWICE  __attribute__((__returns_twice__))
#else
#   define __NO_ATTR_RETURNS_TWICE 1
#   define __ATTR_RETURNS_TWICE  /* Nothing */
#endif
#if __has_attribute(__externally_visible__)
#   define __ATTR_EXTERNALLY_VISIBLE __attribute__((__externally_visible__))
#else
#   define __NO_ATTR_EXTERNALLY_VISIBLE 1
#   define __ATTR_EXTERNALLY_VISIBLE /* Nothing */
#endif
#if __has_attribute(__visibility__) || \
   (defined(__ELF__) || defined(__TINYC__))
#   define __ATTR_VISIBILITY(vis) __attribute__((__visibility__(vis)))
#else
#   define __NO_ATTR_VISIBILITY  1
#   define __ATTR_VISIBILITY(vis) /* Nothing */
#endif
#if __has_attribute(__format__)
#   define __ATTR_FORMAT_PRINTF(fmt,args) __attribute__((__format__(__printf__,fmt,args)))
#if 0 /* TODO: Only `printf' is supported by everything implementing `__has_attribute(__format__)' */
#   define __ATTR_FORMAT_SCANF(fmt,args)    __attribute__((__format__(__scanf__,fmt,args)))
#   define __ATTR_FORMAT_STRFMON(fmt,args)  __attribute__((__format__(__strfmon__,fmt,args)))
#   define __ATTR_FORMAT_STRFTIME(fmt,args) __attribute__((__format__(__strftime__,fmt,args)))
#endif
#else
#   define __NO_ATTR_FORMAT_PRINTF          1
#   define __ATTR_FORMAT_PRINTF(fmt,args)   /* Nothing */
#endif
#ifndef __ATTR_FORMAT_SCANF
#   define __NO_ATTR_FORMAT_SCANF           1
#   define __ATTR_FORMAT_SCANF(fmt,args)    /* Nothing */
#endif /* !__ATTR_FORMAT_SCANF */
#ifndef __ATTR_FORMAT_STRFMON
#   define __NO_ATTR_FORMAT_STRFMON         1
#   define __ATTR_FORMAT_STRFMON(fmt,args)  /* Nothing */
#endif /* !__ATTR_FORMAT_STRFMON */
#ifndef __ATTR_FORMAT_STRFTIME
#   define __NO_ATTR_FORMAT_STRFTIME        1
#   define __ATTR_FORMAT_STRFTIME(fmt,args) /* Nothing */
#endif /* !__ATTR_FORMAT_STRFTIME */
#if __has_attribute(__dllimport__)
#   define __ATTR_DLLIMPORT      __attribute__((__dllimport__))
#   define __ATTR_DLLEXPORT      __attribute__((__dllexport__))
#elif defined(__TINYC__)
#   define __ATTR_DLLIMPORT      __attribute__((dllimport))
#   define __ATTR_DLLEXPORT      __attribute__((dllexport))
#elif __has_declspec_attribute(dllimport)
#   define __ATTR_DLLIMPORT      __declspec(dllimport)
#   define __ATTR_DLLEXPORT      __declspec(dllexport)
#elif defined(__PE__) || defined(_WIN32)
#   define __ATTR_DLLIMPORT      __declspec(dllimport)
#   define __ATTR_DLLEXPORT      __declspec(dllexport)
#else
#   define __NO_ATTR_DLLIMPORT   1
#   define __ATTR_DLLIMPORT      /* Nothing */
#   define __NO_ATTR_DLLEXPORT   1
#   define __ATTR_DLLEXPORT      /* Nothing */
#endif
#if __has_attribute(__nonnull__)
#   define __NONNULL(ppars)      __attribute__((__nonnull__ ppars))
#else
#   define __NO_NONNULL          1
#   define __NONNULL(ppars)      /* Nothing */
#endif
#if __has_attribute(__warn_unused_result__)
#   define __WUNUSED             __attribute__((__warn_unused_result__))
#else
#   define __NO_WUNUSED          1
#   define __WUNUSED             /* Nothing */
#endif
#if __has_attribute(__transparent_union__)
#   define __ATTR_TRANSPARENT_UNION __attribute__((__transparent_union__))
#else
#   define __NO_ATTR_TRANSPARENT_UNION 1
#   define __ATTR_TRANSPARENT_UNION    /* Nothing */
#endif
#if defined(__DCC_VERSION__) || defined(__TINYC__)
#   define __XBLOCK              __extension__
#   define __XRETURN             /* Nothing */
#else
#   define __NO_XBLOCK           1
#   define __XBLOCK(...)         do __VA_ARGS__ while(0)
#   define __XRETURN             /* Nothing */
#endif
#define __DEFINE_PRIVATE_ALIAS(new,old)      /* Nothing */
#define __DEFINE_PUBLIC_ALIAS(new,old)       /* Nothing */
#define __DEFINE_INTERN_ALIAS(new,old)       /* Nothing */
#define __DEFINE_PRIVATE_WEAK_ALIAS(new,old) /* Nothing */
#define __DEFINE_PUBLIC_WEAK_ALIAS(new,old)  /* Nothing */
#define __DEFINE_INTERN_WEAK_ALIAS(new,old)  /* Nothing */
#define __NO_DEFINE_ALIAS 1
#if defined(__INTELLISENSE__)
#elif defined(__TPP_VERSION__)
#   define __PRIVATE_PRAGMA(...) _Pragma(#__VA_ARGS__)
#   define __pragma(...) __PRIVATE_PRAGMA(__VA_ARGS__)
#else
#   define __NO_pragma   1
#   define __pragma(...) /* Nothing */
#endif
#if !__has_builtin(__builtin_assume)
#   define __NO_builtin_assume   1
#   define __builtin_assume(x)  (void)0
#endif
#if !__has_builtin(__builtin_unreachable) && !defined(__TINYC__)
#   define __NO_builtin_unreachable 1
#   define __builtin_unreachable() do;while(1)
#endif
#if !__has_builtin(__builtin_constant_p) && !defined(__TINYC__)
#   define __NO_builtin_constant_p 1
#   define __builtin_constant_p(x) 0
#endif
#if __has_feature(cxx_alignof) || (defined(__cplusplus) && \
   (defined(__CODEGEARC__)  || (defined(__BORLANDC__) && \
    defined(__CODEGEAR_0X_SUPPORT__) && __BORLANDC__ >= 0x610)))
#   define __COMPILER_ALIGNOF alignof
#elif (defined(__ghs__) && (__GHS_VERSION_NUMBER >= 600)) || \
      (defined(__SUNPRO_CC) && __SUNPRO_CC >= 0x5130) || \
       defined(__DCC_VERSION__) || defined(__TINYC__)
#   define __COMPILER_ALIGNOF __alignof__
#elif defined(__cplusplus)
namespace __intern { template<class T> struct __compiler_alignof { char __x; T __y; }; }
#   define __COMPILER_ALIGNOF(T) (sizeof(::__intern::__compiler_alignof< T >)-sizeof(T))
#else
#   define __COMPILER_ALIGNOF(T) ((__SIZE_TYPE__)&((struct{ char __x; T __y; } *)0)->__y)
#endif
#if !__has_builtin(__builtin_offsetof)
#   define __builtin_offsetof(s,m) ((__SIZE_TYPE__)&((s *)0)->m)
#endif
#if defined(inline) || defined(__cplusplus) || \
   (defined(__SUNPRO_C) && (__SUNPRO_C >= 0x550)) || \
   (defined(__STDC_VERSION__) && (__STDC_VERSION__-0 >= 199901L))
#   define __ATTR_INLINE inline
#elif defined(__BORLANDC__) || defined(__DMC__) || \
      defined(__SC__) || defined(__WATCOMC__) || \
      defined(__LCC__) || defined(__DECC)
#   define __ATTR_INLINE __inline
#elif __has_attribute(__always_inline__) || \
      defined(__DCC_VERSION__) || defined(__TINYC__)
#   define __ATTR_INLINE __inline__
#else
#   define __NO_ATTR_INLINE 1
#   define __ATTR_INLINE /* Nothing */
#endif
#if __has_attribute(__always_inline__)
#   define __ATTR_FORCEINLINE __ATTR_INLINE __attribute__((__always_inline__))
#else
#   define __NO_ATTR_FORCEINLINE 1
#   define __ATTR_FORCEINLINE __ATTR_INLINE /* Nothing */
#endif
#define __LOCAL      static __ATTR_INLINE
#define __FORCELOCAL static __ATTR_FORCEINLINE

#ifdef __COMPILER_HAVE_LONGLONG
#   define __LONGLONG   signed long long
#   define __ULONGLONG  unsigned long long
#else
/* XXX: What if the compiler doesn't support __int64 either? */
#   define __LONGLONG   signed __int64
#   define __ULONGLONG  unsigned __int64
#endif
#if !__has_builtin(__builtin_prefetch)
#   define __NO_builtin_prefetch    1
#   define __builtin_prefetch(...) (void)0
#endif
#ifndef __INTELLISENSE__
#ifndef __restrict
#if defined(restrict) || \
   (defined(__STDC_VERSION__) && __STDC_VERSION__+0 >= 199901L)
#define __restrict  restrict
#else
#define __restrict  /* Nothing */
#endif
#endif /* !__restrict */
#endif /* !__INTELLISENSE__ */

#if defined(__STDC_VERSION__) && __STDC_VERSION__+0 >= 199901L
#   define __restrict_arr restrict
#else
#   define __restrict_arr /* Not supported.  */
#endif
#define __empty_arr(T,x) T x[1]

#ifdef __cplusplus
#if __cplusplus >= 201703L /* C++17 */
#   define __STATIC_IF(x)   if constexpr(x)
#   define __STATIC_ELSE(x) if constexpr(!(x))
#else
#   define __STATIC_IF(x)   if(x)
#   define __STATIC_ELSE(x) if(!(x))
#endif
#   define __IF0     if(false)
#   define __IF1     if(true)
#   define __WHILE0  while(false)
#   define __WHILE1  while(true)
#else
#   define __STATIC_IF(x)   if(x)
#   define __STATIC_ELSE(x) if(!(x))
#   define __IF0     if(0)
#   define __IF1     if(1)
#   define __WHILE0  while(0)
#   define __WHILE1  while(1)
#endif
#define __COMPILER_BARRIERS_ALL_IDENTICAL 1
#define __COMPILER_BARRIER()       (void)0 /* ??? */
#define __COMPILER_READ_BARRIER()  (void)0 /* ??? */
#define __COMPILER_WRITE_BARRIER() (void)0 /* ??? */

#ifdef __cplusplus
#ifdef __INTELLISENSE__
#   define __NULLPTR    nullptr
#else
#   define __NULLPTR          0
#endif
#else
#   define __NULLPTR ((void *)0)
#endif

/* Mark the wchar_t type as already being defined when pre-defined by the compiler */
#ifdef __cplusplus
#if !defined(__MWERKS__) || __option(wchar_type)
#define __native_wchar_t_defined 1
#define __wchar_t_defined 1
#endif
#endif

#ifdef __TINYC__
/* TCC predefines its own redirect macro incompatible with our's.
 * That's no good, but there is one pretty neat thing we can take
 * out of this: It does so when targeting ELF, meaning that while
 * deleting its predefinition, we can easily detect when compiling
 * for ELF, and conversely when compiling for PE. */
#ifdef __REDIRECT
#   define __ELF__ 1
#   undef __REDIRECT
#   undef __REDIRECT_NTH
#else /* __REDIRECT */
#   define __PE__  1
#endif /* !__REDIRECT */
#endif /* __TINYC__ */

#ifdef c_plusplus
#if (c_plusplus+0) != 0
#   define __cplusplus  (c_plusplus)
#else
#   define __cplusplus   0
#endif
#endif


/* Define varargs macros expected by system headers. */
#if __has_builtin(__builtin_va_list) || \
    __has_builtin(__builtin_va_start)
#define __builtin_va_list                  __builtin_va_list
#elif defined(__TINYC__)
#ifdef __x86_64__
#ifndef _WIN64
#define __builtin_va_list                    void *
#define __builtin_va_start(ap,last) (void)((ap)=__va_start(__builtin_frame_address(0)))
#define __builtin_va_arg(ap,type)   (*(type *)__va_arg(ap,__builtin_va_arg_types(type),sizeof(type)))
#define __builtin_va_copy(dest,src) (void)((dest)=__va_copy(src))
#define __builtin_va_end(ap)         __va_end(ap)
extern __builtin_va_list (__va_start)(void *fp);
extern __builtin_va_list (__va_copy)(__builtin_va_list src);
extern void *(__va_arg)(__builtin_va_list ap, int arg_type, int size);
extern void (__va_end)(__builtin_va_list ap);
#else /* _WIN64 */
#define __builtin_va_list                    char *
#define __builtin_va_start(ap,last) (void)((ap)=((char *)&(last))+((sizeof(last)+7)&~7))
#define __builtin_va_arg(ap,type)   ((ap)+=(sizeof(type)+7)&~7,*(type *)((ap)-((sizeof(type)+7)&~7)))
#define __builtin_va_copy(dest,src) (void)((dest)=(src))
#define __builtin_va_end(ap)        (void)0
#endif /* !_WIN64 */
#else
#define __builtin_va_list                    char *
#define __builtin_va_start(ap,last) (void)((ap)=((char *)&(last))+((sizeof(last)+3)&~3))
#define __builtin_va_arg(ap,type)   ((ap)+=(sizeof(type)+3)&~3,*(type *)((ap)-((sizeof(type)+3)&~3)))
#define __builtin_va_copy(dest,src) (void)((dest)=(src))
#define __builtin_va_end(ap)        (void)0
#endif
#elif defined(__NO_KOS_SYSTEM_HEADERS__)
/* Without KOS's system headers around, we can
 * try to make use of someone else's work :) */
#include <stdarg.h>
#define __builtin_va_list            va_list
#define __builtin_va_start(ap,v)     va_start(ap,v)
#define __builtin_va_arg(ap,T)       va_arg(ap,T)
#define __builtin_va_end(ap)         va_end(ap)
#else
/* Just guess some generic implementation... */
#define __builtin_va_list                    char *
#define __VA_ADDROF(v)              &(v)
#define __VA_SIZEOF(n)              ((sizeof(n)+3)&~3)
#define __builtin_va_start(ap,v)    (ap = (__builtin_va_list)__VA_ADDROF(v)+__VA_SIZEOF(v))
#define __builtin_va_arg(ap,T)      (*(T *)((ap += __VA_SIZEOF(T))-__VA_SIZEOF(T)))
#define __builtin_va_end(ap)        (void)0
#endif


