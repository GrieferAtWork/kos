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
#ifndef GUARD_KERNEL_SRC_DEV_PS2_MOUSE_C
#define GUARD_KERNEL_SRC_DEV_PS2_MOUSE_C 1
#define _KOS_SOURCE 1

#include <hybrid/compiler.h>
#include <dev/ps2.h>
#include <dev/ps2-mouse.h>
#include <dev/ps2-program.h>
#include <hybrid/section.h>

#ifdef CONFIG_HAVE_DEV_PS2
DECL_BEGIN


INTERN ATTR_FREETEXT void KCALL
ps2_register_mouse(u8 port, u8 type) {
 /* TODO */
 /* It's a mouse! (remember that) */
 ps2_port_device[port] = PS2_PORT_DEVICE_FMOUSE;
}



DECL_END
#endif /* CONFIG_HAVE_DEV_PS2 */

#endif /* !GUARD_KERNEL_SRC_DEV_PS2_MOUSE_C */