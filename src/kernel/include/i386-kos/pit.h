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
#ifndef GUARD_KERNEL_INCLUDE_I386_KOS_PIT_H
#define GUARD_KERNEL_INCLUDE_I386_KOS_PIT_H 1

#include <hybrid/compiler.h>
#include <kos/types.h>
#include <sys/io.h>
#include "interrupt.h"

DECL_BEGIN

#define PIT_DATA0                       0x40 /* I/O port: Channel #0 (read/write) */
#define PIT_DATA1                       0x41 /* I/O port: Channel #1 (read/write) */
#define PIT_DATA2                       0x42 /* I/O port: Channel #2 (read/write) */

/* PC_SPEAKER bits: http://www.manualsdir.com/manuals/128462/ibm-560.html?page=49 */
#define PIT_PCSPEAKER                   0x61 /* PC-speaker I/O port (connected to `PIT_DATA2'). */
#    define PIT_PCSPEAKER_FSYNCPIT      0x01 /* Synchronize the speaker with the state of `PIT_DATA2'. (read/write) */
#    define PIT_PCSPEAKER_FINOUT        0x02 /* Mask for the PC speaker position. (read/write) */
#    define PIT_PCSPEAKER_IN            0x00 /* Move the speaker to the in-position. */
#    define PIT_PCSPEAKER_OUT           0x02 /* Move the speaker to the out-position. */
#    define PIT_PCSPEAKER_FPARITYCHECK  0x04 /* Enable parity checks. */
#    define PIT_PCSPEAKER_FREFRESHTOGGL 0x10 /* Toggles during every refresh (what refresh???). (read) */
#    define PIT_PCSPEAKER_FPIT2OUT      0x20 /* Output of the `PIT_DATA2' channel (read) */
#    define PIT_PCSPEAKER_FCHANNELOK    0x40 /* Channel check was OK (read) */
#    define PIT_PCSPEAKER_FPARITYOK     0x80 /* Parity check was OK (read) */

#define PIT_COMMAND                           0x43 /* I/O port: PIT Command port (write) */
#    define PIT_COMMAND_FSELECT               0xc0 /* Mask for the selected channel. */
#        define PIT_COMMAND_SELEFT_F0         0x00 /* Channel #0. */
#        define PIT_COMMAND_SELEFT_F1         0x40 /* Channel #1. */
#        define PIT_COMMAND_SELEFT_F2         0x80 /* Channel #2. */
#    define PIT_COMMAND_FACCESS               0x30 /* Mask for the access mode. */
#        define PIT_COMMAND_ACCESS_FLATCH     0x00 /* Latch count command. */
#        define PIT_COMMAND_ACCESS_FLO        0x10 /* Low byte only. */
#        define PIT_COMMAND_ACCESS_FHI        0x20 /* High byte only. */
#        define PIT_COMMAND_ACCESS_FLOHI      0x30 /* Low byte, then high byte. */
#    define PIT_COMMAND_FMODE                 0x0e /* Mask for the operations mode. */
#        define PIT_COMMAND_MODE_FIRQONTERM   0x00 /* Operations mode #0 (Interrupt On Terminal Count). */
#        define PIT_COMMAND_MODE_FONESHOT     0x02 /* Operations mode #1 (Hardware re-triggerable one-shot). */
#        define PIT_COMMAND_MODE_FRATEGEN     0x04 /* Operations mode #2 (Rate generator). */
#        define PIT_COMMAND_MODE_FSQRWAVE     0x06 /* Operations mode #3 (Square wave generator). */
#        define PIT_COMMAND_MODE_FSOFTSTROBE  0x08 /* Operations mode #4 (Software triggered strobe). */
#        define PIT_COMMAND_MODE_FHARDSTROBE  0x0a /* Operations mode #5 (Hardware triggered strobe). */
#    define PIT_COMMAND_FBINARY               0x00 /* Data is accessed as 8/16-bit binary. */
#    define PIT_COMMAND_FBCD                  0x01 /* Data is accessed as four-digit BCD (no one needs this). */




//1.193182
#define PIT_HZ_X3          3579545
#define PIT_HZ            (PIT_HZ_X3/3)
#define PIT_HZ_DIV(x)    ((PIT_HZ_X3/(x))/3)
#define PIT_HZ_MUL(x)    ((PIT_HZ_X3*(x))/3)

#define PIT_RELOAD_DEFAULT 65536 /* Or `0' */





DECL_END

#endif /* !GUARD_KERNEL_INCLUDE_I386_KOS_PIT_H */
