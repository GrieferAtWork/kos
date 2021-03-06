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
#ifndef _BITS_GENERIC_STDIO_LIM_H
#define _BITS_GENERIC_STDIO_LIM_H 1
#define _BITS_STDIO_LIM_H 1

#include <features.h>

#ifndef L_tmpnam
#ifdef __USE_DOS
#   define L_tmpnam     14
#else
#   define L_tmpnam     20
#endif
#endif /* !L_tmpnam */

#ifndef FILENAME_MAX
#ifdef __USE_DOS
#   define FILENAME_MAX 260
#else
#   define FILENAME_MAX 4096
#endif
#endif /* !FILENAME_MAX */

#ifndef TMP_MAX
#ifdef __USE_DOS
#   define TMP_MAX      32767
#else
#   define TMP_MAX      238328
#endif
#endif /* !TMP_MAX */

#ifdef __USE_POSIX
#ifndef L_ctermid
#   define L_ctermid    9
#endif /* !L_ctermid */
#if !defined(__USE_XOPEN2K) || defined(__USE_GNU)
#ifndef L_cuserid
#   define L_cuserid    9
#endif /* !L_cuserid */
#endif /* !__USE_XOPEN2K || __USE_GNU */
#endif /* __USE_POSIX */

#ifndef FOPEN_MAX
#ifdef __USE_DOS
#   define FOPEN_MAX    20
#else
#   define FOPEN_MAX    16
#endif
#endif /* !FOPEN_MAX */

#ifndef IOV_MAX
#define IOV_MAX         1024
#endif /* !IOV_MAX */

#endif /* !_BITS_GENERIC_STDIO_LIM_H */
