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
#ifndef GUARD_KERNEL_INCLUDE_KERNEL_USER_H
#define GUARD_KERNEL_INCLUDE_KERNEL_USER_H 1

#include <hybrid/compiler.h>
#include <kos/types.h>
#include <hybrid/host.h>

#if defined(__i386__) || defined(__x86_64__)
#include <i386-kos/user.h>
#else
#error "Unsupported Architecture"
#endif

DECL_BEGIN

#ifdef __CC__
/* Validate user-pointers for being allowed to be used for the specified operations.
 * Since the kernel is allowed to access memory that is marked as `PROT_NOUSER',
 * user-pointers passed from user-space must be checked for the same restrictions.
 * In order words: If the user passes a pointer that is part of kernel-space,
 *                 these functions are used to deny the user access to such memory.
 * @throw E_SEGFAULT: User-space has not been granted access to the given address range. */
FUNDEF void KCALL validate_user(UNCHECKED USER void const *base, size_t num_bytes);
FUNDEF void KCALL validate_userm(UNCHECKED USER void const *base, size_t num_items, size_t item_size_in_bytes);
FUNDEF void KCALL validate_readable(UNCHECKED USER void const *base, size_t num_bytes);
FUNDEF void KCALL validate_readablem(UNCHECKED USER void const *base, size_t num_items, size_t item_size_in_bytes);
FUNDEF void KCALL validate_writable(UNCHECKED USER void *base, size_t num_bytes);
FUNDEF void KCALL validate_writablem(UNCHECKED USER void *base, size_t num_items, size_t item_size_in_bytes);
FUNDEF void KCALL validate_executable(UNCHECKED USER void const *base);
/* Same as the function above, but also accept `NULL' */
FUNDEF void KCALL validate_readable_opt(UNCHECKED USER void const *base, size_t num_bytes);
FUNDEF void KCALL validate_readablem_opt(UNCHECKED USER void const *base, size_t num_items, size_t item_size_in_bytes);
FUNDEF void KCALL validate_writable_opt(UNCHECKED USER void *base, size_t num_bytes);
FUNDEF void KCALL validate_writablem_opt(UNCHECKED USER void *base, size_t num_items, size_t item_size_in_bytes);
FUNDEF void KCALL validate_executable_opt(UNCHECKED USER void const *base);

/* Search for the end of the given user-space string.
 * Search stops when an unmapped page is encountered, or if
 * the string extends into memory that the user does not have
 * access permissions for.
 * Upon such an error, an `E_SEGFAULT' is thrown. Otherwise,
 * the length of the string (in bytes) is returned. */
FUNDEF size_t KCALL user_strlen(UNCHECKED USER char const *base);
#endif /* __CC__ */

DECL_END

#endif /* !GUARD_KERNEL_INCLUDE_KERNEL_USER_H */
