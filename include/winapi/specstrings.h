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
#ifndef __specstrings
#define __specstrings 1

#include "__winstd.h"

#ifndef __nothrow
#ifdef __cplusplus
#define __nothrow __ATTR_NOTHROW
#else
#define __nothrow
#endif
#endif

#define SAL__deref_in
#define SAL__deref_in_ecount(size)
#define SAL__deref_in_bcount(size)

#define SAL__deref_in_opt
#define SAL__deref_in_ecount_opt(size)
#define SAL__deref_in_bcount_opt(size)

#define SAL__deref_opt_in
#define SAL__deref_opt_in_ecount(size)
#define SAL__deref_opt_in_bcount(size)

#define SAL__deref_opt_in_opt
#define SAL__deref_opt_in_ecount_opt(size)
#define SAL__deref_opt_in_bcount_opt(size)

#define SAL__out_awcount(expr,size)
#define SAL__in_awcount(expr,size)

/* Renamed __null to SAL__null for avoiding private keyword conflicts between
   gcc and MS world.  */
#define SAL__null
#define SAL__notnull
#define SAL__maybenull
#define SAL__readonly
#define SAL__notreadonly
#define SAL__maybereadonly
#define SAL__valid
#define SAL__notvalid
#define SAL__maybevalid
#define SAL__readableTo(extent)
#define SAL__elem_readableTo(size)
#define SAL__byte_readableTo(size)
#define SAL__writableTo(size)
#define SAL__elem_writableTo(size)
#define SAL__byte_writableTo(size)
#define SAL__deref
#define SAL__pre
#define SAL__post
#define SAL__precond(expr)
#define SAL__postcond(expr)
#define SAL__exceptthat
#define SAL__execeptthat
#define SAL__inner_success(expr)
#define SAL__inner_checkReturn
#define SAL__inner_typefix(ctype)
#define SAL__inner_override
#define SAL__inner_callback
#define SAL__inner_blocksOn(resource)
#define SAL__inner_fallthrough_dec
#define SAL__inner_fallthrough
#define __refparam
#define SAL__inner_control_entrypoint(category)
#define SAL__inner_data_entrypoint(category)

#define SAL__ecount(size)
#define SAL__bcount(size)

#define SAL__in
#define SAL__in_opt
#define SAL__in_nz
#define SAL__in_nz_opt
#define SAL__in_z
#define SAL__in_z_opt
#define SAL__in_ecount(size)
#define SAL__in_ecount_nz(size)
#define SAL__in_ecount_z(size)
#define SAL__in_bcount(size)
#define SAL__in_bcount_z(size)
#define SAL__in_bcount_nz(size)
#define SAL__in_ecount_opt(size)
#define SAL__in_bcount_opt(size)
#define SAL__in_ecount_z_opt(size)
#define SAL__in_bcount_z_opt(size)
#define SAL__in_ecount_nz_opt(size)
#define SAL__in_bcount_nz_opt(size)

#define SAL__out
#define SAL__out_ecount(size)
#define SAL__out_z
#define SAL__out_nz
#define SAL__out_nz_opt
#define SAL__out_z_opt
#define SAL__out_ecount_part(size,length)
#define SAL__out_ecount_full(size)
#define SAL__out_ecount_nz(size)
#define SAL__out_ecount_z(size)
#define SAL__out_ecount_part_z(size,length)
#define SAL__out_ecount_full_z(size)
#define SAL__out_bcount(size)
#define SAL__out_bcount_part(size,length)
#define SAL__out_bcount_full(size)
#define SAL__out_bcount_z(size)
#define SAL__out_bcount_part_z(size,length)
#define SAL__out_bcount_full_z(size)
#define SAL__out_bcount_nz(size)

#define SAL__inout
#define SAL__inout_ecount(size)
#define SAL__inout_bcount(size)
#define SAL__inout_ecount_part(size,length)
#define SAL__inout_bcount_part(size,length)
#define SAL__inout_ecount_full(size)
#define SAL__inout_bcount_full(size)
#define SAL__inout_z
#define SAL__inout_ecount_z(size)
#define SAL__inout_bcount_z(size)
#define SAL__inout_nz
#define SAL__inout_ecount_nz(size)
#define SAL__inout_bcount_nz(size)
#define SAL__ecount_opt(size)
#define SAL__bcount_opt(size)
#define SAL__out_opt
#define SAL__out_ecount_opt(size)
#define SAL__out_bcount_opt(size)
#define SAL__out_ecount_part_opt(size,length)
#define SAL__out_bcount_part_opt(size,length)
#define SAL__out_ecount_full_opt(size)
#define SAL__out_bcount_full_opt(size)
#define SAL__out_ecount_z_opt(size)
#define SAL__out_bcount_z_opt(size)
#define SAL__out_ecount_part_z_opt(size,length)
#define SAL__out_bcount_part_z_opt(size,length)
#define SAL__out_ecount_full_z_opt(size)
#define SAL__out_bcount_full_z_opt(size)
#define SAL__out_ecount_nz_opt(size)
#define SAL__out_bcount_nz_opt(size)
#define SAL__inout_opt
#define SAL__inout_ecount_opt(size)
#define SAL__inout_bcount_opt(size)
#define SAL__inout_ecount_part_opt(size,length)
#define SAL__inout_bcount_part_opt(size,length)
#define SAL__inout_ecount_full_opt(size)
#define SAL__inout_bcount_full_opt(size)
#define SAL__inout_z_opt
#define SAL__inout_ecount_z_opt(size)
#define SAL__inout_bcount_z_opt(size)
#define SAL__inout_nz_opt
#define SAL__inout_ecount_nz_opt(size)
#define SAL__inout_bcount_nz_opt(size)
#define SAL__deref_ecount(size)
#define SAL__deref_bcount(size)
#define SAL__deref_out
#define SAL__deref_out_ecount(size)
#define SAL__deref_out_bcount(size)
#define SAL__deref_out_ecount_part(size,length)
#define SAL__deref_out_bcount_part(size,length)
#define SAL__deref_out_ecount_full(size)
#define SAL__deref_out_bcount_full(size)
#define SAL__deref_out_z
#define SAL__deref_out_ecount_z(size)
#define SAL__deref_out_bcount_z(size)
#define SAL__deref_out_nz
#define SAL__deref_out_ecount_nz(size)
#define SAL__deref_out_bcount_nz(size)
#define SAL__deref_inout
#define SAL__deref_inout_ecount(size)
#define SAL__deref_inout_bcount(size)
#define SAL__deref_inout_ecount_part(size,length)
#define SAL__deref_inout_bcount_part(size,length)
#define SAL__deref_inout_ecount_full(size)
#define SAL__deref_inout_bcount_full(size)
#define SAL__deref_inout_z
#define SAL__deref_inout_ecount_z(size)
#define SAL__deref_inout_bcount_z(size)
#define SAL__deref_inout_nz
#define SAL__deref_inout_ecount_nz(size)
#define SAL__deref_inout_bcount_nz(size)
#define SAL__deref_ecount_opt(size)
#define SAL__deref_bcount_opt(size)
#define SAL__deref_out_opt
#define SAL__deref_out_ecount_opt(size)
#define SAL__deref_out_bcount_opt(size)
#define SAL__deref_out_ecount_part_opt(size,length)
#define SAL__deref_out_bcount_part_opt(size,length)
#define SAL__deref_out_ecount_full_opt(size)
#define SAL__deref_out_bcount_full_opt(size)
#define SAL__deref_out_z_opt
#define SAL__deref_out_ecount_z_opt(size)
#define SAL__deref_out_bcount_z_opt(size)
#define SAL__deref_out_nz_opt
#define SAL__deref_out_ecount_nz_opt(size)
#define SAL__deref_out_bcount_nz_opt(size)
#define SAL__deref_inout_opt
#define SAL__deref_inout_ecount_opt(size)
#define SAL__deref_inout_bcount_opt(size)
#define SAL__deref_inout_ecount_part_opt(size,length)
#define SAL__deref_inout_bcount_part_opt(size,length)
#define SAL__deref_inout_ecount_full_opt(size)
#define SAL__deref_inout_bcount_full_opt(size)
#define SAL__deref_inout_z_opt
#define SAL__deref_inout_ecount_z_opt(size)
#define SAL__deref_inout_bcount_z_opt(size)
#define SAL__deref_inout_nz_opt
#define SAL__deref_inout_ecount_nz_opt(size)
#define SAL__deref_inout_bcount_nz_opt(size)
#define SAL__deref_opt_ecount(size)
#define SAL__deref_opt_bcount(size)
#define SAL__deref_opt_out
#define SAL__deref_opt_out_z
#define SAL__deref_opt_out_ecount(size)
#define SAL__deref_opt_out_bcount(size)
#define SAL__deref_opt_out_ecount_part(size,length)
#define SAL__deref_opt_out_bcount_part(size,length)
#define SAL__deref_opt_out_ecount_full(size)
#define SAL__deref_opt_out_bcount_full(size)
#define SAL__deref_opt_inout
#define SAL__deref_opt_inout_ecount(size)
#define SAL__deref_opt_inout_bcount(size)
#define SAL__deref_opt_inout_ecount_part(size,length)
#define SAL__deref_opt_inout_bcount_part(size,length)
#define SAL__deref_opt_inout_ecount_full(size)
#define SAL__deref_opt_inout_bcount_full(size)
#define SAL__deref_opt_inout_z
#define SAL__deref_opt_inout_ecount_z(size)
#define SAL__deref_opt_inout_bcount_z(size)
#define SAL__deref_opt_inout_nz
#define SAL__deref_opt_inout_ecount_nz(size)
#define SAL__deref_opt_inout_bcount_nz(size)
#define SAL__deref_opt_ecount_opt(size)
#define SAL__deref_opt_bcount_opt(size)
#define SAL__deref_opt_out_opt
#define SAL__deref_opt_out_ecount_opt(size)
#define SAL__deref_opt_out_bcount_opt(size)
#define SAL__deref_opt_out_ecount_part_opt(size,length)
#define SAL__deref_opt_out_bcount_part_opt(size,length)
#define SAL__deref_opt_out_ecount_full_opt(size)
#define SAL__deref_opt_out_bcount_full_opt(size)
#define SAL__deref_opt_out_z_opt
#define SAL__deref_opt_out_ecount_z_opt(size)
#define SAL__deref_opt_out_bcount_z_opt(size)
#define SAL__deref_opt_out_nz_opt
#define SAL__deref_opt_out_ecount_nz_opt(size)
#define SAL__deref_opt_out_bcount_nz_opt(size)
#define SAL__deref_opt_inout_opt
#define SAL__deref_opt_inout_ecount_opt(size)
#define SAL__deref_opt_inout_bcount_opt(size)
#define SAL__deref_opt_inout_ecount_part_opt(size,length)
#define SAL__deref_opt_inout_bcount_part_opt(size,length)
#define SAL__deref_opt_inout_ecount_full_opt(size)
#define SAL__deref_opt_inout_bcount_full_opt(size)
#define SAL__deref_opt_inout_z_opt
#define SAL__deref_opt_inout_ecount_z_opt(size)
#define SAL__deref_opt_inout_bcount_z_opt(size)
#define SAL__deref_opt_inout_nz_opt
#define SAL__deref_opt_inout_ecount_nz_opt(size)
#define SAL__deref_opt_inout_bcount_nz_opt(size)

#define SAL__success(expr)
#define SAL__nullterminated
#define SAL__nullnullterminated
#define SAL__reserved
#define SAL__checkReturn
#define SAL__typefix(ctype)
#define SAL__override
#define SAL__callback
#define SAL__format_string
#define SAL__blocksOn(resource)
#define SAL__control_entrypoint(category)
#define SAL__data_entrypoint(category)

#ifndef __fallthrough
#define __fallthrough
#endif
#ifndef __analysis_assume
#define __analysis_assume(expr)
#endif
#ifndef __CLR_OR_THIS_CALL
#define __CLR_OR_THIS_CALL
#endif
#ifndef __CLRCALL_OR_CDECL
#define __CLRCALL_OR_CDECL __cdecl
#endif
#ifndef __STDC_WANT_SECURE_LIB__
#define __STDC_WANT_SECURE_LIB__ 0
#endif
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif

#ifndef RC_INVOKED
#ifndef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 0
#endif
#ifndef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT 0
#endif
#ifndef _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES
#define _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES 0
#endif
#endif
#ifndef DECLSPEC_ADDRSAFE
#if defined(_MSC_VER) && (_MSC_VER >= 1200) && (defined(_M_ALPHA) || defined(_M_AXP64))
#define DECLSPEC_ADDRSAFE  __declspec(address_safe)
#else
#define DECLSPEC_ADDRSAFE
#endif
#endif /* !DECLSPEC_ADDRSAFE */

#endif /* !__specstrings */
