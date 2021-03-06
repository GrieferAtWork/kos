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
#include <hybrid/compiler.h>
#include <hybrid/asm.h>
#include <hybrid/host.h>

#ifdef __x86_64__
#define PTR  .quad
#if __has_include("x86_64-kos-kernel-symbols.h")
#define SYMBOL_TABLE_FILENAME "x86_64-kos-kernel-symbols.h"
#endif
#else
#define PTR  .int
#if __has_include("i386-kos-kernel-symbols.h")
#define SYMBOL_TABLE_FILENAME "i386-kos-kernel-symbols.h"
#endif
#endif


#ifdef SYMBOL_TABLE_FILENAME
#include SYMBOL_TABLE_FILENAME
#else
.section .rodata.kernel_symtab
INTERN_ENTRY(kernel_symbol_table)
	PTR   0 /* kst_mask */
	PTR   0 /* kst_tab[0].kse_name */
	PTR   0 /* kst_tab[0].kse_base */
	PTR   0 /* kst_tab[0].kse_size */
	PTR   0 /* kst_tab[0].kse_hash */
SYMEND(kernel_symbol_table)
#endif

