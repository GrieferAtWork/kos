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
#ifndef _KOS_KEYBOARD_H
#define _KOS_KEYBOARD_H 1

#include <__stdinc.h>

#define KEY_FPRESSED     0x0000 /* FLAG: The key was pressed. */
#define KEY_FREPEAT      0x4000 /* FLAG: The key is being repeated periodically,
                                 *       because the user is holding down the button. */
#define KEY_FRELEASED    0x8000 /* FLAG: The key was released. */
#define KEY_FMASK        0xc000 /* MASK: Key state flags. */
#define KEY_KMASK        0x3fff /* MASK: Key code. */
#define KEY_ISUP(k)     ((k)&KEY_FRELEASED)
#define KEY_ISDOWN(k) (!((k)&KEY_FRELEASED))
#define KEYCODE(k)      ((k)&KEY_KMASK)
#define KEYUP(k)        ((k)|KEY_FRELEASED)
#define KEYDOWN(k)       (k)

#define KEYBOARD_COL_BITS 5
#define KEYBOARD_ROW_BITS 3
#define KEY(r,c)       (((r) << KEYBOARD_COL_BITS) | c)

/* Unknown key press. */
#define KEY_UNKNOWN     0x0000

/* Unused key codes (These must never never generated by drivers) */
#define KEY_UNUSED_MIN  0x0001
#define KEY_UNUSED_MAX  0x000f

/* Arrow keys */
#define KEY_UP          KEY(0,16)
#define KEY_LEFT        KEY(0,17)
#define KEY_RIGHT       KEY(0,18)
#define KEY_DOWN        KEY(0,19)

/* Home-row keys (NOTE: These count downwards) */
#define KEY_SCROLLLOCK  KEY(0,22)
#define KEY_PAUSE       KEY(0,23)
#define KEY_BREAK       KEY(0,24)
#define KEY_PRINTSCREEN KEY(0,25)
#define KEY_INSERT      KEY(0,26)
#define KEY_DELETE      KEY(0,27)
#define KEY_HOME        KEY(0,28)
#define KEY_END         KEY(0,29)
#define KEY_PGUP        KEY(0,30)
#define KEY_PGDOWN      KEY(0,31)

#define KEY_BACKTICK    KEY(1,0) /* ` (back tick) */
#define KEY_1           KEY(1,1)
#define KEY_2           KEY(1,2)
#define KEY_3           KEY(1,3)
#define KEY_4           KEY(1,4)
#define KEY_5           KEY(1,5)
#define KEY_6           KEY(1,6)
#define KEY_7           KEY(1,7)
#define KEY_8           KEY(1,8)
#define KEY_9           KEY(1,9)
#define KEY_0           KEY(1,10)
#define KEY_MINUS       KEY(1,11)
#define KEY_EQUALS      KEY(1,12)
#define KEY_BACKSLASH   KEY(1,13) /* \ (backslash) */
#define KEY_BACKSPACE   KEY(1,14)
#define KEY_GRAVE_ACCENT KEY_BACKTICK

#define KEY_TAB         KEY(2,0)
#define KEY_Q           KEY(2,1)
#define KEY_W           KEY(2,2)
#define KEY_E           KEY(2,3)
#define KEY_R           KEY(2,4)
#define KEY_T           KEY(2,5)
#define KEY_Y           KEY(2,6)
#define KEY_U           KEY(2,7)
#define KEY_I           KEY(2,8)
#define KEY_O           KEY(2,9)
#define KEY_P           KEY(2,10)
#define KEY_LBRACKET    KEY(2,11)
#define KEY_RBRACKET    KEY(2,12)
#define KEY_ENTER       KEY(2,13)

#define KEY_CAPSLOCK    KEY(3,0)
#define KEY_A           KEY(3,1)
#define KEY_S           KEY(3,2)
#define KEY_D           KEY(3,3)
#define KEY_F           KEY(3,4)
#define KEY_G           KEY(3,5)
#define KEY_H           KEY(3,6)
#define KEY_J           KEY(3,7)
#define KEY_K           KEY(3,8)
#define KEY_L           KEY(3,9)
#define KEY_SEMICOLON   KEY(3,10)
#define KEY_SINGLEQUOTE KEY(3,11) /* ' (single quote) */
#define KEY_NUMBERSIGN  KEY_BACKSLASH /* # (European keyboards have this located here...) */
#define KEY_APOSTROPHE  KEY_SINGLEQUOTE

#define KEY_LSHIFT      KEY(4,0)
#define KEY_LESS        KEY(4,1)  /* < */
#define KEY_Z           KEY(4,2)
#define KEY_X           KEY(4,3)
#define KEY_C           KEY(4,4)
#define KEY_V           KEY(4,5)
#define KEY_B           KEY(4,6)
#define KEY_N           KEY(4,7)
#define KEY_M           KEY(4,8)
#define KEY_COMMA       KEY(4,9)  /* , (comma) */
#define KEY_DOT         KEY(4,10) /* . (dot) */
#define KEY_SLASH       KEY(4,11) /* / (slash) */
#define KEY_RSHIFT      KEY(4,12)

#define KEY_LCTRL       KEY(5,0)
#define KEY_LGUI        KEY(5,1)
#define KEY_LALT        KEY(5,2)
#define KEY_SPACE       KEY(5,3)
#define KEY_RALT        KEY(5,4)
#define KEY_RGUI        KEY(5,5)
#define KEY_APPS        KEY(5,6)
#define KEY_RCTRL       KEY(5,7)
#define KEY_MENU        KEY_APPS
#define KEY_ALTGR       KEY_RALT
#define KEY_LMETA       KEY_LGUI
#define KEY_RMETA       KEY_RGUI

/* Keypad keys. */
#define KEY_KP_NUMLOCK  KEY(6,0)
#define KEY_KP_SLASH    KEY(6,1)
#define KEY_KP_STAR     KEY(6,2)
#define KEY_KP_MINUS    KEY(6,3)
#define KEY_KP_7        KEY(6,4)
#define KEY_KP_8        KEY(6,5)
#define KEY_KP_9        KEY(6,6)
#define KEY_KP_PLUS     KEY(6,7)
#define KEY_KP_4        KEY(6,8)
#define KEY_KP_5        KEY(6,9)
#define KEY_KP_6        KEY(6,10)
#define KEY_KP_1        KEY(6,11)
#define KEY_KP_2        KEY(6,12)
#define KEY_KP_3        KEY(6,13)
#define KEY_KP_ENTER    KEY(6,14)
#define KEY_KP_0        KEY(6,15)
#define KEY_KP_DOT      KEY(6,16)
#define KEY_KP_DIVIDE   KEY_KP_SLASH
#define KEY_KP_MULTIPLY KEY_KP_STAR
#define KEY_KP_SUBTRACT KEY_KP_MINUS
#define KEY_KP_ADD      KEY_KP_PLUS
#define KEY_KP_DECIMAL  KEY_KP_DOT
#define KEY_KP_COMMA    KEY_KP_DOT
#define KEY_NUMLOCK     KEY_KP_NUMLOCK

/* Function keys. */
#define KEY_FKEY(n)     KEY(7,n)
#define KEY_ESC         KEY_FKEY(0)
#define KEY_F1          KEY_FKEY(1)
#define KEY_F2          KEY_FKEY(2)
#define KEY_F3          KEY_FKEY(3)
#define KEY_F4          KEY_FKEY(4)
#define KEY_F5          KEY_FKEY(5)
#define KEY_F6          KEY_FKEY(6)
#define KEY_F7          KEY_FKEY(7)
#define KEY_F8          KEY_FKEY(8)
#define KEY_F9          KEY_FKEY(9)
#define KEY_F10         KEY_FKEY(10)
#define KEY_F11         KEY_FKEY(11)
#define KEY_F12         KEY_FKEY(12)
#define KEY_F13         KEY_FKEY(13)
#define KEY_F14         KEY_FKEY(14)
#define KEY_F15         KEY_FKEY(15)
#define KEY_F16         KEY_FKEY(16)
#define KEY_F17         KEY_FKEY(17)
#define KEY_F18         KEY_FKEY(18)
#define KEY_F19         KEY_FKEY(19)
#define KEY_F20         KEY_FKEY(20)
#define KEY_F21         KEY_FKEY(21)
#define KEY_F22         KEY_FKEY(22)
#define KEY_F23         KEY_FKEY(23)
#define KEY_F24         KEY_FKEY(24)
#define KEY_F25         KEY_FKEY(25)
#define KEY_F26         KEY_FKEY(26)
#define KEY_F27         KEY_FKEY(27)
#define KEY_F28         KEY_FKEY(28)
#define KEY_F29         KEY_FKEY(29)
#define KEY_F30         KEY_FKEY(30)
#define KEY_F31         KEY_FKEY(31)
#define KEY_F32         KEY_FKEY(32)


/* Special multi-media keys */
#define KEY_VOLUME_DOWN       0x1000
#define KEY_VOLUME_UP         0x1001	
#define KEY_VOLUME_MUTE       0x1002
#define KEY_MM_PREVIOUS_TRACK 0x1010
#define KEY_MM_NEXT_TRACK     0x1011
#define KEY_MM_PLAY_PAUSE     0x1012
#define KEY_MM_STOP           0x1013
#define KEY_MM_SELECT         0x1014

#define KEY_MY_COMPUTER       0x1040
#define KEY_MEDIA_PLAYER      0x1041
#define KEY_EMAIL_CLIENT      0x1042
#define KEY_CALCULATOR        0x1043
#define KEY_WWW_SEARCH        0x1080
#define KEY_WWW_FAVORITES     0x1081
#define KEY_WWW_REFRESH       0x1082
#define KEY_WWW_STOP          0x1083
#define KEY_WWW_FORWARD       0x1084
#define KEY_WWW_BACK          0x1085
#define KEY_WWW_HOME          0x1086

/* Special APIC (power-control) keys */
#define KEY_APIC_POWER  0x1100
#define KEY_APIC_SLEEP  0x1101
#define KEY_APIC_WAKE   0x1102 		

#endif /* !_KOS_KEYBOARD_H */
