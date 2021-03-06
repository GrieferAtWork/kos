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
#ifndef _FLOAT_H
#define _FLOAT_H 1

#include "__stdinc.h"
#include "features.h"
#include <hybrid/floatcore.h>
#include <hybrid/typecore.h>
#include <hybrid/host.h>

__SYSDECL_BEGIN

/* Because GCC rather stop-drop-and-burn instead of having
   `__has_include_next()' return 0 when no other include paths can
   be searched, we sadly can't look for another version of
   <float.h>, until one of the GCC guys realizes this stupidity.
#if __has_include_next(<float.h>)
#include_next <float.h>
#endif
*/

/* Mostly taken from GCC's fixinclude <float.h> header, but still
 * required due to several extensions DOS puts in this header.
 * The following disclaimer can be found in GCC's header. */

/* Copyright (C) 2002-2016 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

Under Section 7 of GPL version 3, you are granted additional
permissions described in the GCC Runtime Library Exception, version
3.1, as published by the Free Software Foundation.

You should have received a copy of the GNU General Public License and
a copy of the GCC Runtime Library Exception along with this program;
see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
<http://www.gnu.org/licenses/>.  */

#define FLT_RADIX        __FLT_RADIX__
#define FLT_MANT_DIG     __FLT_MANT_DIG__
#define DBL_MANT_DIG     __DBL_MANT_DIG__
#define LDBL_MANT_DIG    __LDBL_MANT_DIG__
#define FLT_DIG          __FLT_DIG__
#define DBL_DIG          __DBL_DIG__
#define LDBL_DIG         __LDBL_DIG__
#define FLT_MIN_EXP      __FLT_MIN_EXP__
#define DBL_MIN_EXP      __DBL_MIN_EXP__
#define LDBL_MIN_EXP     __LDBL_MIN_EXP__
#define FLT_MIN_10_EXP   __FLT_MIN_10_EXP__
#define DBL_MIN_10_EXP   __DBL_MIN_10_EXP__
#define LDBL_MIN_10_EXP  __LDBL_MIN_10_EXP__
#define FLT_MAX_EXP      __FLT_MAX_EXP__
#define DBL_MAX_EXP      __DBL_MAX_EXP__
#define LDBL_MAX_EXP     __LDBL_MAX_EXP__
#define FLT_MAX_10_EXP   __FLT_MAX_10_EXP__
#define DBL_MAX_10_EXP   __DBL_MAX_10_EXP__
#define LDBL_MAX_10_EXP  __LDBL_MAX_10_EXP__
#define FLT_MAX          __FLT_MAX__
#define DBL_MAX          __DBL_MAX__
#define LDBL_MAX         __LDBL_MAX__
#define FLT_EPSILON      __FLT_EPSILON__
#define DBL_EPSILON      __DBL_EPSILON__
#define LDBL_EPSILON     __LDBL_EPSILON__
#define FLT_MIN          __FLT_MIN__
#define DBL_MIN          __DBL_MIN__
#define LDBL_MIN         __LDBL_MIN__
#define FLT_ROUNDS       1

#ifdef __USE_ISOC99
#define FLT_EVAL_METHOD  __FLT_EVAL_METHOD__
#define DECIMAL_DIG      __DECIMAL_DIG__
#endif /* __USE_ISOC99 */

#ifdef __USE_ISOC11
#define FLT_DECIMAL_DIG  __FLT_DECIMAL_DIG__
#define DBL_DECIMAL_DIG  __DBL_DECIMAL_DIG__
#define LDBL_DECIMAL_DIG __DECIMAL_DIG__
#define FLT_HAS_SUBNORM  __FLT_HAS_DENORM__
#define DBL_HAS_SUBNORM  __DBL_HAS_DENORM__
#define LDBL_HAS_SUBNORM __LDBL_HAS_DENORM__
#define FLT_TRUE_MIN     __FLT_DENORM_MIN__
#define DBL_TRUE_MIN     __DBL_DENORM_MIN__
#define LDBL_TRUE_MIN    __LDBL_DENORM_MIN__
#endif /* __USE_ISOC11 */

#ifdef __STDC_WANT_DEC_FP__
#define DEC32_EPSILON        __DEC32_EPSILON__
#define DEC32_MANT_DIG       __DEC32_MANT_DIG__
#define DEC32_MAX            __DEC32_MAX__
#define DEC32_MAX_EXP        __DEC32_MAX_EXP__
#define DEC32_MIN            __DEC32_MIN__
#define DEC32_MIN_EXP        __DEC32_MIN_EXP__
#define DEC32_SUBNORMAL_MIN  __DEC32_SUBNORMAL_MIN__
#define DEC64_EPSILON        __DEC64_EPSILON__
#define DEC64_MANT_DIG       __DEC64_MANT_DIG__
#define DEC64_MAX            __DEC64_MAX__
#define DEC64_MAX_EXP        __DEC64_MAX_EXP__
#define DEC64_MIN            __DEC64_MIN__
#define DEC64_MIN_EXP        __DEC64_MIN_EXP__
#define DEC64_SUBNORMAL_MIN  __DEC64_SUBNORMAL_MIN__
#define DEC128_EPSILON       __DEC128_EPSILON__
#define DEC128_MANT_DIG      __DEC128_MANT_DIG__
#define DEC128_MAX           __DEC128_MAX__
#define DEC128_MAX_EXP       __DEC128_MAX_EXP__
#define DEC128_MIN           __DEC128_MIN__
#define DEC128_MIN_EXP       __DEC128_MIN_EXP__
#define DEC128_SUBNORMAL_MIN __DEC128_SUBNORMAL_MIN__
#define DEC_EVAL_METHOD      __DEC_EVAL_METHOD__
#endif /* __STDC_WANT_DEC_FP__ */


#ifdef __USE_DOS
__LIBC __UINT32_TYPE__ (__LIBCCALL _clearfp)(void);
__LIBC __UINT32_TYPE__ (__LIBCCALL _controlfp)(__UINT32_TYPE__ __newval, __UINT32_TYPE__ __mask);
__LIBC void (__LIBCCALL _set_controlfp)(__UINT32_TYPE__ __newval, __UINT32_TYPE__ __mask);
__LIBC errno_t (__LIBCCALL _controlfp_s)(__UINT32_TYPE__ *__pcurrent, __UINT32_TYPE__ __newval, __UINT32_TYPE__ __mask);
__LIBC __UINT32_TYPE__ (__LIBCCALL _statusfp)(void);
__LIBC void (__LIBCCALL _fpreset)(void);
__LIBC void (__LIBCCALL _statusfp2)(__UINT32_TYPE__ *__x86_stat, __UINT32_TYPE__ *__sse2_stat);
#define _clear87        _clearfp
#define _status87       _statusfp

#define _SW_INEXACT     0x00000001 /* Inexact precision. */
#define _SW_UNDERFLOW   0x00000002 /* Underflow. */
#define _SW_OVERFLOW    0x00000004 /* Overflow. */
#define _SW_ZERODIVIDE  0x00000008 /* Zero divide. */
#define _SW_INVALID     0x00000010 /* Invalid. */
#define _SW_DENORMAL    0x00080000 /* Denormal bit. */
#define _EM_AMBIGUIOUS  0x80000000
#define _EM_AMBIGUOUS   0x80000000

#define _MCW_EM         0x0008001f     /* Interrupt exception masks. */
#define _EM_INEXACT     _SW_INEXACT    /* Inexact precision. */
#define _EM_UNDERFLOW   _SW_UNDERFLOW  /* Underflow. */
#define _EM_OVERFLOW    _SW_OVERFLOW   /* Overflow. */
#define _EM_ZERODIVIDE  _SW_ZERODIVIDE /* Zero division. */
#define _EM_INVALID     _SW_INVALID    /* Invalid. */
#define _EM_DENORMAL    _SW_DENORMAL   /* Denormal exception bit.) */

#define _MCW_RC         0x00000300 /* Rounding mode mask. */
#define _RC_NEAR        0x00000000 /* Round to nearest. */
#define _RC_DOWN        0x00000100 /* Round down. */
#define _RC_UP          0x00000200 /* Round up. */
#define _RC_CHOP        0x00000300 /* Truncate result. */

#define _MCW_PC         0x00030000 /* Precision width mask. */
#define _PC_64          0x00000000 /* 64-bit precision. */
#define _PC_53          0x00010000 /* 53-bit precision. */
#define _PC_24          0x00020000 /* 24-bit precision. */

#define _MCW_IC         0x00040000 /* Infinity mode mask. */
#define _IC_AFFINE      0x00040000 /* Affine mode. */
#define _IC_PROJECTIVE  0x00000000 /* Projective mode. */

#define _MCW_DN         0x03000000 /* Denormal control mask. */
#define _DN_SAVE        0x00000000 /* Save. */
#define _DN_FLUSH       0x01000000 /* Flush to zero. */
#define _DN_FLUSH_OPERANDS_SAVE_RESULTS 0x02000000  /* Flush operands to + save result. */
#define _DN_SAVE_OPERANDS_FLUSH_RESULTS 0x03000000  /* Save + flush results to zero. */

#if defined(__x86_64__) || defined(__i386__)
#define _CW_DEFAULT (_RC_NEAR|_PC_53|_EM_INVALID|_EM_ZERODIVIDE|_EM_OVERFLOW|_EM_UNDERFLOW|_EM_INEXACT|_EM_DENORMAL)
#else /* defined (_M_X64) || defined (_M_ARM) */
#define _CW_DEFAULT (_RC_NEAR|_EM_INVALID|_EM_ZERODIVIDE|_EM_OVERFLOW|_EM_UNDERFLOW|_EM_INEXACT|_EM_DENORMAL)
#endif

__LIBC __UINT32_TYPE__ (__LIBCCALL _control87)(__UINT32_TYPE__ __newval, __UINT32_TYPE__ __mask);
#if defined(__x86_64__) || defined(__i386__)
__LIBC int (__LIBCCALL __control87_2)(__UINT32_TYPE__ __newval, __UINT32_TYPE__ __mask,
                                      __UINT32_TYPE__ *__x86_control_word,
                                      __UINT32_TYPE__ *__sse2_control_word);
#endif /* X64... */

__LIBC int *(__LIBCCALL __fpecode)(void);
#define _fpecode      (*__fpecode())

#define _SW_UNEMULATED          0x0040
#define _SW_SQRTNEG             0x0080
#define _SW_STACKOVERFLOW       0x0200
#define _SW_STACKUNDERFLOW      0x0400
#define _FPE_INVALID            0x81
#define _FPE_DENORMAL           0x82
#define _FPE_ZERODIVIDE         0x83
#define _FPE_OVERFLOW           0x84
#define _FPE_UNDERFLOW          0x85
#define _FPE_INEXACT            0x86
#define _FPE_UNEMULATED         0x87
#define _FPE_SQRTNEG            0x88
#define _FPE_STACKOVERFLOW      0x8a
#define _FPE_STACKUNDERFLOW     0x8b
#define _FPE_EXPLICITGEN        0x8c
#define _FPE_MULTIPLE_TRAPS     0x8d
#define _FPE_MULTIPLE_FAULTS    0x8e

__LIBC double (__LIBCCALL _copysign)(double __num, double __sign);
__LIBC double (__LIBCCALL _chgsign)(double __x);
__LIBC double (__LIBCCALL _scalb)(double __x, long __y);
__LIBC double (__LIBCCALL _logb)(double __x);
__LIBC double (__LIBCCALL _nextafter)(double __x, double __y);
__LIBC int    (__LIBCCALL _finite)(double __x);
__LIBC int    (__LIBCCALL _isnan)(double __x);
__LIBC int    (__LIBCCALL _fpclass)(double __x);
#if defined(__x86_64__) || defined(__i386__)
__LIBC float (__LIBCCALL _scalbf)(float __x, long __y);
#endif

#define _FPCLASS_SNAN   0x0001 /* signaling NaN. */
#define _FPCLASS_QNAN   0x0002 /* quiet NaN. */
#define _FPCLASS_NINF   0x0004 /* negative infinity. */
#define _FPCLASS_NN     0x0008 /* negative normal. */
#define _FPCLASS_ND     0x0010 /* negative denormal. */
#define _FPCLASS_NZ     0x0020 /* -0 */
#define _FPCLASS_PZ     0x0040 /* +0 */
#define _FPCLASS_PD     0x0080 /* positive denormal. */
#define _FPCLASS_PN     0x0100 /* positive normal. */
#define _FPCLASS_PINF   0x0200 /* positive infinity. */

__REDIRECT_VOID(__LIBC,,__LIBCCALL,fpreset,(void),_fpreset,())
#define clear87            _clear87
#define status87           _status87
#define control87          _control87
#define DBL_RADIX          _DBL_RADIX
#define DBL_ROUNDS         _DBL_ROUNDS
#define LDBL_RADIX         _LDBL_RADIX
#define LDBL_ROUNDS        _LDBL_ROUNDS
#define EM_AMBIGUIOUS      _EM_AMBIGUOUS
#define EM_AMBIGUOUS       _EM_AMBIGUOUS
#define MCW_EM             _MCW_EM
#define EM_INVALID         _EM_INVALID
#define EM_DENORMAL        _EM_DENORMAL
#define EM_ZERODIVIDE      _EM_ZERODIVIDE
#define EM_OVERFLOW        _EM_OVERFLOW
#define EM_UNDERFLOW       _EM_UNDERFLOW
#define EM_INEXACT         _EM_INEXACT
#define MCW_IC             _MCW_IC
#define IC_AFFINE          _IC_AFFINE
#define IC_PROJECTIVE      _IC_PROJECTIVE
#define MCW_RC             _MCW_RC
#define RC_CHOP            _RC_CHOP
#define RC_UP              _RC_UP
#define RC_DOWN            _RC_DOWN
#define RC_NEAR            _RC_NEAR
#define MCW_PC             _MCW_PC
#define PC_24              _PC_24
#define PC_53              _PC_53
#define PC_64              _PC_64
#define CW_DEFAULT         _CW_DEFAULT
#define SW_INVALID         _SW_INVALID
#define SW_DENORMAL        _SW_DENORMAL
#define SW_ZERODIVIDE      _SW_ZERODIVIDE
#define SW_OVERFLOW        _SW_OVERFLOW
#define SW_UNDERFLOW       _SW_UNDERFLOW
#define SW_INEXACT         _SW_INEXACT
#define SW_UNEMULATED      _SW_UNEMULATED
#define SW_SQRTNEG         _SW_SQRTNEG
#define SW_STACKOVERFLOW   _SW_STACKOVERFLOW
#define SW_STACKUNDERFLOW  _SW_STACKUNDERFLOW
#define FPE_INVALID        _FPE_INVALID
#define FPE_DENORMAL       _FPE_DENORMAL
#define FPE_ZERODIVIDE     _FPE_ZERODIVIDE
#define FPE_OVERFLOW       _FPE_OVERFLOW
#define FPE_UNDERFLOW      _FPE_UNDERFLOW
#define FPE_INEXACT        _FPE_INEXACT
#define FPE_UNEMULATED     _FPE_UNEMULATED
#define FPE_SQRTNEG        _FPE_SQRTNEG
#define FPE_STACKOVERFLOW  _FPE_STACKOVERFLOW
#define FPE_STACKUNDERFLOW _FPE_STACKUNDERFLOW
#define FPE_EXPLICITGEN    _FPE_EXPLICITGEN

#endif /* __USE_DOS */


__SYSDECL_END

#endif /* !_FLOAT_H */
