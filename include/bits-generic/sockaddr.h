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
#ifndef _BITS_GENERIC_SOCKADDR_H
#define _BITS_GENERIC_SOCKADDR_H 1
#define _BITS_SOCKADDR_H 1

#include <__stdinc.h>
#include <hybrid/typecore.h>

__SYSDECL_BEGIN

#ifndef __sa_family_t_defined
#define __sa_family_t_defined 1
#ifdef __CC__
typedef __UINT16_TYPE__ sa_family_t;
#endif /* __CC__ */
#define __SIZEOF_SA_FAMILY_T__ 2
#endif /* !__sa_family_t_defined */

#define __SOCKADDR_COMMON(sa_prefix) sa_family_t sa_prefix##family
#define __SOCKADDR_COMMON_SIZE      (sizeof(sa_family_t))
#define _SS_SIZE                     128

__SYSDECL_END

#endif /* !_BITS_GENERIC_SOCKADDR_H */
