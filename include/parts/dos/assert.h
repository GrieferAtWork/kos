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
#ifndef _PARTS_DOS_ASSERT_H
#define _PARTS_DOS_ASSERT_H 1

#include "__stdinc.h"
#include <features.h>

__SYSDECL_BEGIN

__LIBC __ATTR_NORETURN __ATTR_COLD void (__LIBCCALL _assert)(char const *__assertion, char const *__file, int __line);
#if defined(_MSC_VER) && 1 /* Fix Visual studio's broken breakpoint system (Assuming that that IDE is being used). */
__NAMESPACE_INT_BEGIN
extern void __debugbreak(void);
__NAMESPACE_INT_END
#define __yes_assert(sexpr,expr)         (void)(__ASSERT_LIKELY(expr) || (__NAMESPACE_INT_SYM __debugbreak(),_assert(sexpr,__FILE__,__LINE__),0))
#define __yes_asserte(sexpr,expr)        (void)(__ASSERT_LIKELY(expr) || (__NAMESPACE_INT_SYM __debugbreak(),_assert(sexpr,__FILE__,__LINE__),0))
#define __yes_assertf(sexpr,expr,...)    (void)(__ASSERT_LIKELY(expr) || (__NAMESPACE_INT_SYM __debugbreak(),_assert(sexpr,__FILE__,__LINE__),0))
#define __yes_assertef(sexpr,expr,...)   (void)(__ASSERT_LIKELY(expr) || (__NAMESPACE_INT_SYM __debugbreak(),_assert(sexpr,__FILE__,__LINE__),0))
#define __IMPL2_yes_assert_d(sexpr,expr,file,line,func,...) (void)(__ASSERT_LIKELY(expr) || (__NAMESPACE_INT_SYM __debugbreak(),_assert(sexpr,file,line,func),0))
#else
#define __yes_assert(sexpr,expr)         (void)(__ASSERT_LIKELY(expr) || (_assert(sexpr,__FILE__,__LINE__),0))
#define __yes_asserte(sexpr,expr)        (void)(__ASSERT_LIKELY(expr) || (_assert(sexpr,__FILE__,__LINE__),0))
#define __yes_assertf(sexpr,expr,...)    (void)(__ASSERT_LIKELY(expr) || (_assert(sexpr,__FILE__,__LINE__),0))
#define __yes_assertef(sexpr,expr,...)   (void)(__ASSERT_LIKELY(expr) || (_assert(sexpr,__FILE__,__LINE__),0))
#define __IMPL2_yes_assert_d(sexpr,expr,file,line,func,...) (void)(__ASSERT_LIKELY(expr) || (_assert(sexpr,file,line,func),0))
#endif
#define __IMPL_yes_assert_d(args)        __IMPL2_yes_assert_d args
#define __yes_assert_d(sexpr,expr,...)   __IMPL_yes_assert_d((sexpr,expr,__VA_ARGS__))
#define __yes_assertf_d(sexpr,expr,...)  __IMPL_yes_assert_d((sexpr,expr,__VA_ARGS__))
#define __yes_asserte_d(sexpr,expr,...)  __IMPL_yes_assert_d((sexpr,expr,__VA_ARGS__))
#define __yes_assertef_d(sexpr,expr,...) __IMPL_yes_assert_d((sexpr,expr,__VA_ARGS__))

__SYSDECL_END

#endif /* !_PARTS_DOS_ASSERT_H */
