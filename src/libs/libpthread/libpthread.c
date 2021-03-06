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
#ifndef GUARD_LIBS_LIBPTHREAD_LIBPTHREAD_C
#define GUARD_LIBS_LIBPTHREAD_LIBPTHREAD_C 1

#include "libpthread.h"

DECL_BEGIN

__asm__(
".pushsection .data\n\t"
".global __stack_chk_fail_local\n\t"
".hidden __stack_chk_fail_local\n\t"
"__stack_chk_fail_local:\n\t"
"	jmp  __stack_chk_guard\n\t"
".size __stack_chk_fail_local, . - __stack_chk_fail_local\n\t"
".popsection"
);


DECL_END

#endif /* !GUARD_LIBS_LIBPTHREAD_LIBPTHREAD_C */
