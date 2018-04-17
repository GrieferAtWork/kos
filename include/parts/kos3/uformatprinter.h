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
#ifndef _PARTS_KOS3_UFORMATPRINTER_H
#define _PARTS_KOS3_UFORMATPRINTER_H 1

#include "__stdinc.h"
#include <features.h>
#include <bits/types.h>
#include <hybrid/typecore.h>
#include <parts/kos2/uformatprinter.h>
#include <xlocale.h>

#if __KOS_VERSION__ >= 300 && defined(__CRT_KOS) && !defined(__KERNEL__)

__SYSDECL_BEGIN

__REDIRECT_W16(__LIBC,,__ssize_t,__LIBCCALL,format_w16repeat,(pw16formatprinter __printer, void *__closure, char16_t __ch, __size_t __num_repetitions),format_wrepeat,(__printer,__closure,__ch,__num_repetitions))
__REDIRECT_W32(__LIBC,,__ssize_t,__LIBCCALL,format_w32repeat,(pw32formatprinter __printer, void *__closure, char32_t __ch, __size_t __num_repetitions),format_wrepeat,(__printer,__closure,__ch,__num_repetitions))

__NAMESPACE_STD_BEGIN
struct tm;
__NAMESPACE_STD_END


#if defined(__BUILDING_LIBC) || defined(__KERNEL__)
#define __DEFINE_STRINGPRINTER(T) \
     T   *sp_bufpos; /* [1..1][>= sp_buffer][<= sp_bufend] . */ \
     T   *sp_buffer; /* [1..1] Allocate buffer base pointer. */ \
     T   *sp_bufend; /* [1..1] Buffer end (Pointer to currently allocated `'\0'�-character). */
#else /* __BUILDING_LIBC || __KERNEL__ */
#define __DEFINE_STRINGPRINTER(T) \
     T *__sp_bufpos; /* [1..1][>= __sp_buffer][<= __sp_bufend] . */ \
     T *__sp_buffer; /* [1..1] Allocate buffer base pointer. */ \
     T *__sp_bufend; /* [1..1] Buffer end (Pointer to currently allocated `'\0'�-character). */
#endif /* !__BUILDING_LIBC && !__KERNEL__ */
#ifndef __string16printer_defined
#define __string16printer_defined 1
struct string16printer { __DEFINE_STRINGPRINTER(char16_t) };
struct string32printer { __DEFINE_STRINGPRINTER(char32_t) };
#endif /* !__string16printer_defined */
#undef __DEFINE_STRINGPRINTER


/* Helper functions for using any pformatprinter-style
 * function to print into a dynamically allocated string.
 * >> struct stringprinter printer; char *text;
 * >> if (stringprinter_init(&printer,0)) return handle_error();
 * >> if (format_printf(&stringprinter_print,&printer,"Hello %s","dynamic world")) {
 * >>   stringprinter_fini(&printer);
 * >>   return handle_error();
 * >> } else {
 * >>   text = stringprinter_pack(&printer,NULL);
 * >>   //stringprinter_fini(&printer); // No-op after pack has been called
 * >> }
 * >> ...
 * >> free(text);
 * @param: HINT: A hint as to how big the initial buffer should
 *               be allocated as (Pass ZERO if unknown).
 * @return:  0: Successfully printed to/initialized the given string printer.
 * @return: -1: Failed to initialize/print the given text (`errno' is set to ENOMEM) */
__REDIRECT_EXCEPT_W16_XVOID(__LIBC,__PORT_KOSONLY __NONNULL((1)),int,__LIBCCALL,string16printer_init,(struct string16printer *__restrict __self, __size_t __hint),wstringprinter_init,(__self,__hint))
__REDIRECT_EXCEPT_W32_XVOID(__LIBC,__PORT_KOSONLY __NONNULL((1)),int,__LIBCCALL,string32printer_init,(struct string32printer *__restrict __self, __size_t __hint),wstringprinter_init,(__self,__hint))
__REDIRECT_W16_VOID(__LIBC,__PORT_KOSONLY __NONNULL((1)),__LIBCCALL,string16printer_fini,(struct string16printer *__restrict __self),wstringprinter_fini,(__self))
__REDIRECT_W32_VOID(__LIBC,__PORT_KOSONLY __NONNULL((1)),__LIBCCALL,string32printer_fini,(struct string32printer *__restrict __self),wstringprinter_fini,(__self))
__REDIRECT_W16(__LIBC,__PORT_KOSONLY __ATTR_RETNONNULL __NONNULL((1)),char16_t *,__LIBCCALL,string16printer_pack,(struct string16printer *__restrict __self, __size_t *__length),wstringprinter_pack,(__self,__length))
__REDIRECT_W32(__LIBC,__PORT_KOSONLY __ATTR_RETNONNULL __NONNULL((1)),char32_t *,__LIBCCALL,string32printer_pack,(struct string32printer *__restrict __self, __size_t *__length),wstringprinter_pack,(__self,__length))
__REDIRECT_EXCEPT_W16(__LIBC,__PORT_KOSONLY,__ssize_t,__LIBCCALL,string16printer_print,(char16_t const *__restrict __data, __size_t __datalen, void *__closure),wstringprinter_print,(__data,__datalen,__closure))
__REDIRECT_EXCEPT_W32(__LIBC,__PORT_KOSONLY,__ssize_t,__LIBCCALL,string32printer_print,(char32_t const *__restrict __data, __size_t __datalen, void *__closure),wstringprinter_print,(__data,__datalen,__closure))
#ifdef __USE_EXCEPT
__REDIRECT_W16_VOID(__LIBC,__PORT_KOSONLY __NONNULL((1)),__LIBCCALL,Xstring16printer_init,(struct string16printer *__restrict __self, __size_t __hint),Xwstringprinter_init,(__self,__hint))
__REDIRECT_W32_VOID(__LIBC,__PORT_KOSONLY __NONNULL((1)),__LIBCCALL,Xstring32printer_init,(struct string32printer *__restrict __self, __size_t __hint),Xwstringprinter_init,(__self,__hint))
__REDIRECT_W16(__LIBC,__PORT_KOSONLY,__ssize_t,__LIBCCALL,Xstring16printer_print,(char16_t const *__restrict __data, __size_t __datalen, void *__closure),Xwstringprinter_print,(__data,__datalen,__closure))
__REDIRECT_W32(__LIBC,__PORT_KOSONLY,__ssize_t,__LIBCCALL,Xstring32printer_print,(char32_t const *__restrict __data, __size_t __datalen, void *__closure),Xwstringprinter_print,(__data,__datalen,__closure))
#endif /* __USE_EXCEPT */


/* Unicode format printers. */
__VREDIRECT_W16(__LIBC,__PORT_KOSONLY __ATTR_LIBC_W16PRINTF(3,4) __NONNULL((1,3)),__ssize_t,__ATTR_CDECL,format_w16printf,(pw16formatprinter __printer, void *__closure, char16_t const *__restrict __format, ...),format_wprintf,format_vwprintf,(__printer,__closure,__format),__format)
__VREDIRECT_W32(__LIBC,__PORT_KOSONLY __ATTR_LIBC_W32PRINTF(3,4) __NONNULL((1,3)),__ssize_t,__ATTR_CDECL,format_w32printf,(pw32formatprinter __printer, void *__closure, char32_t const *__restrict __format, ...),format_wprintf,format_vwprintf,(__printer,__closure,__format),__format)
__VREDIRECT_W16(__LIBC,__PORT_KOSONLY __ATTR_LIBC_W16PRINTF(3,5) __NONNULL((1,3)),__ssize_t,__ATTR_CDECL,format_w16printf_l,(pw16formatprinter __printer, void *__closure, char16_t const *__restrict __format, __locale_t __locale, ...),format_wprintf_l,format_vwprintf_l,(__printer,__closure,__format,__locale),__locale)
__VREDIRECT_W32(__LIBC,__PORT_KOSONLY __ATTR_LIBC_W32PRINTF(3,5) __NONNULL((1,3)),__ssize_t,__ATTR_CDECL,format_w32printf_l,(pw32formatprinter __printer, void *__closure, char32_t const *__restrict __format, __locale_t __locale, ...),format_wprintf_l,format_vwprintf_l,(__printer,__closure,__format,__locale),__locale)
__VREDIRECT_W16(__LIBC,__PORT_KOSONLY __ATTR_LIBC_W16PRINTF(3,4) __NONNULL((1,3)),__ssize_t,__ATTR_CDECL,format_w16printf_p,(pw16formatprinter __printer, void *__closure, char16_t const *__restrict __format, ...),format_wprintf_p,format_vwprintf_p,(__printer,__closure,__format),__format)
__VREDIRECT_W32(__LIBC,__PORT_KOSONLY __ATTR_LIBC_W32PRINTF(3,4) __NONNULL((1,3)),__ssize_t,__ATTR_CDECL,format_w32printf_p,(pw32formatprinter __printer, void *__closure, char32_t const *__restrict __format, ...),format_wprintf_p,format_vwprintf_p,(__printer,__closure,__format),__format)
__VREDIRECT_W16(__LIBC,__PORT_KOSONLY __ATTR_LIBC_W16PRINTF(3,5) __NONNULL((1,3)),__ssize_t,__ATTR_CDECL,format_w16printf_p_l,(pw16formatprinter __printer, void *__closure, char16_t const *__restrict __format, __locale_t __locale, ...),format_wprintf_p_l,format_vwprintf_p_l,(__printer,__closure,__format,__locale),__locale)
__VREDIRECT_W32(__LIBC,__PORT_KOSONLY __ATTR_LIBC_W32PRINTF(3,5) __NONNULL((1,3)),__ssize_t,__ATTR_CDECL,format_w32printf_p_l,(pw32formatprinter __printer, void *__closure, char32_t const *__restrict __format, __locale_t __locale, ...),format_wprintf_p_l,format_vwprintf_p_l,(__printer,__closure,__format,__locale),__locale)
__REDIRECT_W16(__LIBC,__PORT_KOSONLY __ATTR_LIBC_W16PRINTF(3,0) __NONNULL((1,3,4)),__ssize_t,__LIBCCALL,format_vw16printf,(pw16formatprinter __printer, void *__closure, char16_t const *__restrict __format, __builtin_va_list __args),format_vwprintf,(__printer,__closure,__format,__args))
__REDIRECT_W32(__LIBC,__PORT_KOSONLY __ATTR_LIBC_W32PRINTF(3,0) __NONNULL((1,3,4)),__ssize_t,__LIBCCALL,format_vw32printf,(pw32formatprinter __printer, void *__closure, char32_t const *__restrict __format, __builtin_va_list __args),format_vwprintf,(__printer,__closure,__format,__args))
__REDIRECT_W16(__LIBC,__PORT_KOSONLY __ATTR_LIBC_W16PRINTF(3,0) __NONNULL((1,3,5)),__ssize_t,__LIBCCALL,format_vw16printf_l,(pw16formatprinter __printer, void *__closure, char16_t const *__restrict __format, __locale_t __locale, __builtin_va_list __args),format_vwprintf_l,(__printer,__closure,__format,__locale,__args))
__REDIRECT_W32(__LIBC,__PORT_KOSONLY __ATTR_LIBC_W32PRINTF(3,0) __NONNULL((1,3,5)),__ssize_t,__LIBCCALL,format_vw32printf_l,(pw32formatprinter __printer, void *__closure, char32_t const *__restrict __format, __locale_t __locale, __builtin_va_list __args),format_vwprintf_l,(__printer,__closure,__format,__locale,__args))
__REDIRECT_W16(__LIBC,__PORT_KOSONLY __ATTR_LIBC_W16PRINTF(3,0) __NONNULL((1,3,4)),__ssize_t,__LIBCCALL,format_vw16printf_p,(pw16formatprinter __printer, void *__closure, char16_t const *__restrict __format, __builtin_va_list __args),format_vwprintf_p,(__printer,__closure,__format,__args))
__REDIRECT_W32(__LIBC,__PORT_KOSONLY __ATTR_LIBC_W32PRINTF(3,0) __NONNULL((1,3,4)),__ssize_t,__LIBCCALL,format_vw32printf_p,(pw32formatprinter __printer, void *__closure, char32_t const *__restrict __format, __builtin_va_list __args),format_vwprintf_p,(__printer,__closure,__format,__args))
__REDIRECT_W16(__LIBC,__PORT_KOSONLY __ATTR_LIBC_W16PRINTF(3,0) __NONNULL((1,3,5)),__ssize_t,__LIBCCALL,format_vw16printf_p_l,(pw16formatprinter __printer, void *__closure, char16_t const *__restrict __format, __locale_t __locale, __builtin_va_list __args),format_vwprintf_p_l,(__printer,__closure,__format,__locale,__args))
__REDIRECT_W32(__LIBC,__PORT_KOSONLY __ATTR_LIBC_W32PRINTF(3,0) __NONNULL((1,3,5)),__ssize_t,__LIBCCALL,format_vw32printf_p_l,(pw32formatprinter __printer, void *__closure, char32_t const *__restrict __format, __locale_t __locale, __builtin_va_list __args),format_vwprintf_p_l,(__printer,__closure,__format,__locale,__args))

/* Unicode format helpers. */
__REDIRECT_W16(__LIBC,__PORT_KOSONLY __NONNULL((1,3)),__ssize_t,__LIBCCALL,format_w16quote,(pw16formatprinter __printer, void *__closure, char16_t const *__restrict __text, __size_t __textlen, __UINT32_TYPE__ __flags),format_wquote,(__printer,__closure,__text,__textlen,__flags))
__REDIRECT_W32(__LIBC,__PORT_KOSONLY __NONNULL((1,3)),__ssize_t,__LIBCCALL,format_w32quote,(pw32formatprinter __printer, void *__closure, char32_t const *__restrict __text, __size_t __textlen, __UINT32_TYPE__ __flags),format_wquote,(__printer,__closure,__text,__textlen,__flags))
__REDIRECT_W16(__LIBC,__PORT_KOSONLY __NONNULL((1,3)),__ssize_t,__LIBCCALL,format_w16quote_l,(pw16formatprinter __printer, void *__closure, char16_t const *__restrict __text, __size_t __textlen, __UINT32_TYPE__ __flags, __locale_t __locale),format_wquote_l,(__printer,__closure,__text,__textlen,__flags,__locale))
__REDIRECT_W32(__LIBC,__PORT_KOSONLY __NONNULL((1,3)),__ssize_t,__LIBCCALL,format_w32quote_l,(pw32formatprinter __printer, void *__closure, char32_t const *__restrict __text, __size_t __textlen, __UINT32_TYPE__ __flags, __locale_t __locale),format_wquote_l,(__printer,__closure,__text,__textlen,__flags,__locale))
__REDIRECT_W16(__LIBC,__PORT_KOSONLY __NONNULL((1,3)),__ssize_t,__LIBCCALL,format_w16hexdump,(pw16formatprinter __printer, void *__closure, void const *__restrict __data, __size_t __size, __size_t __linesize, __UINT32_TYPE__ __flags),format_whexdump,(__printer,__closure,__data,__size,__linesize,__flags))
__REDIRECT_W32(__LIBC,__PORT_KOSONLY __NONNULL((1,3)),__ssize_t,__LIBCCALL,format_w32hexdump,(pw32formatprinter __printer, void *__closure, void const *__restrict __data, __size_t __size, __size_t __linesize, __UINT32_TYPE__ __flags),format_whexdump,(__printer,__closure,__data,__size,__linesize,__flags))
__REDIRECT_W16(__LIBC,__PORT_KOSONLY __NONNULL((1,3)),__ssize_t,__LIBCCALL,format_w16hexdump_l,(pw16formatprinter __printer, void *__closure, void const *__restrict __data, __size_t __size, __size_t __linesize, __UINT32_TYPE__ __flags, __locale_t __locale),format_whexdump_l,(__printer,__closure,__data,__size,__linesize,__flags,__locale))
__REDIRECT_W32(__LIBC,__PORT_KOSONLY __NONNULL((1,3)),__ssize_t,__LIBCCALL,format_w32hexdump_l,(pw32formatprinter __printer, void *__closure, void const *__restrict __data, __size_t __size, __size_t __linesize, __UINT32_TYPE__ __flags, __locale_t __locale),format_whexdump_l,(__printer,__closure,__data,__size,__linesize,__flags,__locale))

/* Unicode time formatting. */
__REDIRECT_W16(__LIBC,__PORT_KOSONLY __ATTR_LIBC_W16FTIME(3,4) __NONNULL((1,4)),__ssize_t,__LIBCCALL,format_w16ftime,(pw16formatprinter __printer, void *__closure, char16_t const *__restrict __format, struct __NAMESPACE_STD_SYM tm const *__restrict __tm),format_wcsftime,(__printer,__closure,__format,__tm))
__REDIRECT_W32(__LIBC,__PORT_KOSONLY __ATTR_LIBC_W32FTIME(3,4) __NONNULL((1,4)),__ssize_t,__LIBCCALL,format_w32ftime,(pw32formatprinter __printer, void *__closure, char32_t const *__restrict __format, struct __NAMESPACE_STD_SYM tm const *__restrict __tm),format_wcsftime,(__printer,__closure,__format,__tm))
__REDIRECT_W16(__LIBC,__PORT_KOSONLY __ATTR_LIBC_W16FTIME(3,4) __NONNULL((1,4)),__ssize_t,__LIBCCALL,format_w16ftime_l,(pw16formatprinter __printer, void *__closure, char16_t const *__restrict __format, struct __NAMESPACE_STD_SYM tm const *__restrict __tm, __locale_t __locale),format_wcsftime_l,(__printer,__closure,__format,__tm,__locale))
__REDIRECT_W32(__LIBC,__PORT_KOSONLY __ATTR_LIBC_W32FTIME(3,4) __NONNULL((1,4)),__ssize_t,__LIBCCALL,format_w32ftime_l,(pw32formatprinter __printer, void *__closure, char32_t const *__restrict __format, struct __NAMESPACE_STD_SYM tm const *__restrict __tm, __locale_t __locale),format_wcsftime_l,(__printer,__closure,__format,__tm,__locale))

#ifndef __pw16formatgetc_defined
#define __pw16formatgetc_defined 1
typedef __ssize_t (__LIBCCALL *pw16formatgetc)(char16_t *__pch, void *__closure);
typedef __ssize_t (__LIBCCALL *pw16formatungetc)(char16_t __ch, void *__closure);
typedef __ssize_t (__LIBCCALL *pw32formatgetc)(char32_t *__pch, void *__closure);
typedef __ssize_t (__LIBCCALL *pw32formatungetc)(char32_t __ch, void *__closure);
#endif /* !__pw16formatgetc_defined */

/* Unicode format scanners. */
__VREDIRECT_W16(__LIBC,__PORT_KOSONLY __ATTR_LIBC_WSCANF(4,5) __NONNULL((1,2,4)),__ssize_t,__ATTR_CDECL,format_w16scanf,(pw16formatgetc __pgetc, pw16formatungetc __pungetc, void *__closure, char16_t const *__restrict __format, ...),format_wscanf,format_vwscanf,(__pgetc,__pungetc,__closure,__format),__format)
__VREDIRECT_W32(__LIBC,__PORT_KOSONLY __ATTR_LIBC_WSCANF(4,5) __NONNULL((1,2,4)),__ssize_t,__ATTR_CDECL,format_w32scanf,(pw32formatgetc __pgetc, pw32formatungetc __pungetc, void *__closure, char32_t const *__restrict __format, ...),format_wscanf,format_vwscanf,(__pgetc,__pungetc,__closure,__format),__format)
__VREDIRECT_W16(__LIBC,__PORT_KOSONLY __ATTR_LIBC_WSCANF(4,6) __NONNULL((1,2,4)),__ssize_t,__ATTR_CDECL,format_w16scanf_l,(pw16formatgetc __pgetc, pw16formatungetc __pungetc, void *__closure, char16_t const *__restrict __format, __locale_t __locale, ...),format_wscanf_l,format_vwscanf_l,(__pgetc,__pungetc,__closure,__format,__locale),__locale)
__VREDIRECT_W32(__LIBC,__PORT_KOSONLY __ATTR_LIBC_WSCANF(4,6) __NONNULL((1,2,4)),__ssize_t,__ATTR_CDECL,format_w32scanf_l,(pw32formatgetc __pgetc, pw32formatungetc __pungetc, void *__closure, char32_t const *__restrict __format, __locale_t __locale, ...),format_wscanf_l,format_vwscanf_l,(__pgetc,__pungetc,__closure,__format,__locale),__locale)
__REDIRECT_W16(__LIBC,__PORT_KOSONLY __ATTR_LIBC_WSCANF(4,0) __NONNULL((1,2,4)),__ssize_t,__LIBCCALL,format_vw16scanf,(pw16formatgetc __pgetc, pw16formatungetc __pungetc, void *__closure, char16_t const *__restrict __format, __builtin_va_list __args),format_vwscanf,(__pgetc,__pungetc,__closure,__format,__args))
__REDIRECT_W32(__LIBC,__PORT_KOSONLY __ATTR_LIBC_WSCANF(4,0) __NONNULL((1,2,4)),__ssize_t,__LIBCCALL,format_vw32scanf,(pw32formatgetc __pgetc, pw32formatungetc __pungetc, void *__closure, char32_t const *__restrict __format, __builtin_va_list __args),format_vwscanf,(__pgetc,__pungetc,__closure,__format,__args))
__REDIRECT_W16(__LIBC,__PORT_KOSONLY __ATTR_LIBC_WSCANF(4,0) __NONNULL((1,2,4)),__ssize_t,__LIBCCALL,format_vw16scanf_l,(pw16formatgetc __pgetc, pw16formatungetc __pungetc, void *__closure, char16_t const *__restrict __format, __locale_t __locale, __builtin_va_list __args),format_vwscanf_l,(__pgetc,__pungetc,__closure,__format,__locale,__args))
__REDIRECT_W32(__LIBC,__PORT_KOSONLY __ATTR_LIBC_WSCANF(4,0) __NONNULL((1,2,4)),__ssize_t,__LIBCCALL,format_vw32scanf_l,(pw32formatgetc __pgetc, pw32formatungetc __pungetc, void *__closure, char32_t const *__restrict __format, __locale_t __locale, __builtin_va_list __args),format_vwscanf_l,(__pgetc,__pungetc,__closure,__format,__locale,__args))


#ifndef __OMIT_BUFFER_DECLARATIONS
/* Buffered format printing.
 * >> Since format printing is used quite often thoughout the user-space and the kernel,
 *    many less-than optimized print generators are often chained together with fairly
 *    slow print receivers.
 *    To speed up performance by bunching together a whole lot of data, a buffer
 *    can be used to automatically collect data until it is flushed, or deleted:
 * HINT: If the buffer fails to allocate memory, it will try to flush itself and
 *       attempt to allocate memory again. If this still fails, print commands
 *       are passed through directly, meaning that the buffer is still going to
 *       generated the desired output, be it with less efficient throughput.
 *
 * >> struct buffer *log_buffer;
 * >> log_buffer = buffer_new(&syslog_printer,SYSLOG_PRINTER_CLOSURE(LOG_WARNING));
 * >> // `format_printf' is unbuffered, meaning that normally each component would call
 * >> // `syslog_printer()', resulting in a total to 7 calls: "a" "foo" ",b" "bar" ",c" "foobar" "\n"
 * >> // Using a buffer, this function returning.
 * >> format_printf(&buffer_print,log_buffer,"a%s,b%s,c%s\n","foo","bar","foobar");
 * 
 * WARNING: Buffers are themself not thread-safe. They are intended for local
 *          use, or require the caller to perform their own synchronization.
 */
#ifndef __w16buffer_defined
#define __w16buffer_defined 1
#if defined(__BUILDING_LIBC) || defined(__USE_KOS)
#define __DEFINE_BUFFER(Tprinter,T) \
    Tprinter          b_printer; /* [1..1] The underlying printer. */ \
    void             *b_closure; /* [?..?] The closure argument passed to `b_printer' */ \
    union{ \
       __uintptr_t  __b_align0;  /* ... */ \
       __ssize_t      b_state;   /* The current printer state (< 0: Last error code returned by `b_printer'; >= 0: Sum of `b_printer' callbacks). */ \
    }; \
    T                *b_buffer;  /* [0..1][owned] Base-pointer of the allocated buffer. */ \
    T                *b_bufpos;  /* [0..1][>= b_buffer && <= b_bufend] The current buffer position (Pointer to the buffer byte written to next). */ \
    T                *b_bufend;  /* [0..1] End of the allocated buffer (first byte no longer apart of the buffer). */ \
    void           *__padding;   /* ... (Forward-compatibility & align to `8*sizeof(void *)', which is quite the pretty number) */
#else
#define __DEFINE_BUFFER(Tprinter,T) \
    Tprinter          __b_printer; /* [1..1] The underlying printer. */ \
    void             *__b_closure; /* [?..?] The closure argument passed to `b_printer' */ \
    union{ \
       __uintptr_t    __b_align0;  /* ... */ \
       __ssize_t      __b_state;   /* The current printer state (< 0: Last error code returned by `b_printer'; >= 0: Sum of `b_printer' callbacks). */ \
    }; \
    T                *__b_buffer;  /* [0..1][owned] Base-pointer of the allocated buffer. */ \
    T                *__b_bufpos;  /* [0..1][>= b_buffer && <= b_bufend] The current buffer position (Pointer to the buffer byte written to next). */ \
    T                *__b_bufend;  /* [0..1] End of the allocated buffer (first byte no longer apart of the buffer). */ \
    void             *__padding;   /* ... (Forward-compatibility & align to `8*sizeof(void *)', which is quite the pretty number) */
#endif
struct w16buffer { __DEFINE_BUFFER(pw16formatprinter,char16_t) };
struct w32buffer { __DEFINE_BUFFER(pw32formatprinter,char32_t) };
#undef __DEFINE_BUFFER
#endif /* !__w16buffer_defined */

#ifndef W16BUFFER_INIT
#define W16BUFFER_INIT(printer,closure)  {printer,closure,{0},__NULLPTR,__NULLPTR,__NULLPTR,__NULLPTR}
#define W32BUFFER_INIT(printer,closure)  {printer,closure,{0},__NULLPTR,__NULLPTR,__NULLPTR,__NULLPTR}
#endif

__REDIRECT_W16_VOID(__LIBC,__PORT_KOSONLY,__LIBCCALL,w16buffer_init,(struct w16buffer *__restrict self, pw16formatprinter __printer, void *__closure),wbuffer_init,(self,__printer,__closure))
__REDIRECT_W16(__LIBC,__PORT_KOSONLY,__ssize_t,__LIBCCALL,w16buffer_fini,(struct w16buffer *__restrict __buf),wbuffer_fini,(__buf))
__REDIRECT_W16(__LIBC,__PORT_KOSONLY,__ssize_t,__LIBCCALL,w16buffer_flush,(struct w16buffer *__restrict __buf),wbuffer_flush,(__buf))
__REDIRECT_W16(__LIBC,__PORT_KOSONLY,__ssize_t,__LIBCCALL,w16buffer_print,(char16_t const *__restrict __data, __size_t __datalen, void *__closure),wbuffer_print,(__data,__datalen,__closure))
__REDIRECT_W32_VOID(__LIBC,__PORT_KOSONLY,__LIBCCALL,w32buffer_init,(struct w32buffer *__restrict self, pw32formatprinter __printer, void *__closure),wbuffer_init,(self,__printer,__closure))
__REDIRECT_W32(__LIBC,__PORT_KOSONLY,__ssize_t,__LIBCCALL,w32buffer_fini,(struct w32buffer *__restrict __buf),wbuffer_fini,(__buf))
__REDIRECT_W32(__LIBC,__PORT_KOSONLY,__ssize_t,__LIBCCALL,w32buffer_flush,(struct w32buffer *__restrict __buf),wbuffer_flush,(__buf))
__REDIRECT_W32(__LIBC,__PORT_KOSONLY,__ssize_t,__LIBCCALL,w32buffer_print,(char32_t const *__restrict __data, __size_t __datalen, void *__closure),wbuffer_print,(__data,__datalen,__closure))

#endif /* !__OMIT_BUFFER_DECLARATIONS */

__VREDIRECT_W16(__LIBC,__PORT_KOSONLY __ATTR_LIBC_W16PRINTF(3,4) __NONNULL((1,3)),__ssize_t,__ATTR_CDECL,format_w16bprintf,(pw16formatprinter __bprinter, void *__closure, char16_t const *__restrict __format, ...),format_wbprintf,format_vwbprintf,(__bprinter,__closure,__format),__format)
__VREDIRECT_W32(__LIBC,__PORT_KOSONLY __ATTR_LIBC_W32PRINTF(3,4) __NONNULL((1,3)),__ssize_t,__ATTR_CDECL,format_w32bprintf,(pw32formatprinter __bprinter, void *__closure, char32_t const *__restrict __format, ...),format_wbprintf,format_vwbprintf,(__bprinter,__closure,__format),__format)
__VREDIRECT_W16(__LIBC,__PORT_KOSONLY __ATTR_LIBC_W16PRINTF(3,5) __NONNULL((1,3)),__ssize_t,__ATTR_CDECL,format_w16bprintf_l,(pw16formatprinter __bprinter, void *__closure, char16_t const *__restrict __format, __locale_t __locale, ...),format_wbprintf_l,format_vwbprintf_l,(__bprinter,__closure,__format,__locale),__locale)
__VREDIRECT_W32(__LIBC,__PORT_KOSONLY __ATTR_LIBC_W32PRINTF(3,5) __NONNULL((1,3)),__ssize_t,__ATTR_CDECL,format_w32bprintf_l,(pw32formatprinter __bprinter, void *__closure, char32_t const *__restrict __format, __locale_t __locale, ...),format_wbprintf_l,format_vwbprintf_l,(__bprinter,__closure,__format,__locale),__locale)
__VREDIRECT_W16(__LIBC,__PORT_KOSONLY __ATTR_LIBC_W16PRINTF_P(3,4) __NONNULL((1,3)),__ssize_t,__ATTR_CDECL,format_w16bprintf_p,(pw16formatprinter __bprinter, void *__closure, char16_t const *__restrict __format, ...),format_wbprintf_p,format_vwbprintf_p,(__bprinter,__closure,__format),__format)
__VREDIRECT_W32(__LIBC,__PORT_KOSONLY __ATTR_LIBC_W32PRINTF_P(3,4) __NONNULL((1,3)),__ssize_t,__ATTR_CDECL,format_w32bprintf_p,(pw32formatprinter __bprinter, void *__closure, char32_t const *__restrict __format, ...),format_wbprintf_p,format_vwbprintf_p,(__bprinter,__closure,__format),__format)
__VREDIRECT_W16(__LIBC,__PORT_KOSONLY __ATTR_LIBC_W16PRINTF_P(3,5) __NONNULL((1,3)),__ssize_t,__ATTR_CDECL,format_w16bprintf_p_l,(pw16formatprinter __bprinter, void *__closure, char16_t const *__restrict __format, __locale_t __locale, ...),format_wbprintf_p_l,format_vwbprintf_p_l,(__bprinter,__closure,__format,__locale),__locale)
__VREDIRECT_W32(__LIBC,__PORT_KOSONLY __ATTR_LIBC_W32PRINTF_P(3,5) __NONNULL((1,3)),__ssize_t,__ATTR_CDECL,format_w32bprintf_p_l,(pw32formatprinter __bprinter, void *__closure, char32_t const *__restrict __format, __locale_t __locale, ...),format_wbprintf_p_l,format_vwbprintf_p_l,(__bprinter,__closure,__format,__locale),__locale)
__REDIRECT_W16(__LIBC,__PORT_KOSONLY __ATTR_LIBC_W16PRINTF(3,0) __NONNULL((1,3,4)),__ssize_t,__LIBCCALL,format_vw16bprintf,(pw16formatprinter __bprinter, void *__closure, char16_t const *__restrict __format, __builtin_va_list __args),format_vwbprintf,(__bprinter,__closure,__format,__args))
__REDIRECT_W32(__LIBC,__PORT_KOSONLY __ATTR_LIBC_W32PRINTF(3,0) __NONNULL((1,3,4)),__ssize_t,__LIBCCALL,format_vw32bprintf,(pw32formatprinter __bprinter, void *__closure, char32_t const *__restrict __format, __builtin_va_list __args),format_vwbprintf,(__bprinter,__closure,__format,__args))
__REDIRECT_W16(__LIBC,__PORT_KOSONLY __ATTR_LIBC_W16PRINTF(3,0) __NONNULL((1,3,5)),__ssize_t,__LIBCCALL,format_vw16bprintf_l,(pw16formatprinter __bprinter, void *__closure, char16_t const *__restrict __format, __locale_t __locale, __builtin_va_list __args),format_vwbprintf_l,(__bprinter,__closure,__format,__locale,__args))
__REDIRECT_W32(__LIBC,__PORT_KOSONLY __ATTR_LIBC_W32PRINTF(3,0) __NONNULL((1,3,5)),__ssize_t,__LIBCCALL,format_vw32bprintf_l,(pw32formatprinter __bprinter, void *__closure, char32_t const *__restrict __format, __locale_t __locale, __builtin_va_list __args),format_vwbprintf_l,(__bprinter,__closure,__format,__locale,__args))
__REDIRECT_W16(__LIBC,__PORT_KOSONLY __ATTR_LIBC_W16PRINTF_P(3,0) __NONNULL((1,3,4)),__ssize_t,__LIBCCALL,format_vw16bprintf_p,(pw16formatprinter __bprinter, void *__closure, char16_t const *__restrict __format, __builtin_va_list __args),format_vwbprintf_p,(__bprinter,__closure,__format,__args))
__REDIRECT_W32(__LIBC,__PORT_KOSONLY __ATTR_LIBC_W32PRINTF_P(3,0) __NONNULL((1,3,4)),__ssize_t,__LIBCCALL,format_vw32bprintf_p,(pw32formatprinter __bprinter, void *__closure, char32_t const *__restrict __format, __builtin_va_list __args),format_vwbprintf_p,(__bprinter,__closure,__format,__args))
__REDIRECT_W16(__LIBC,__PORT_KOSONLY __ATTR_LIBC_W16PRINTF_P(3,0) __NONNULL((1,3,5)),__ssize_t,__LIBCCALL,format_vw16bprintf_p_l,(pw16formatprinter __bprinter, void *__closure, char16_t const *__restrict __format, __locale_t __locale, __builtin_va_list __args),format_vwbprintf_p_l,(__bprinter,__closure,__format,__locale,__args))
__REDIRECT_W32(__LIBC,__PORT_KOSONLY __ATTR_LIBC_W32PRINTF_P(3,0) __NONNULL((1,3,5)),__ssize_t,__LIBCCALL,format_vw32bprintf_p_l,(pw32formatprinter __bprinter, void *__closure, char32_t const *__restrict __format, __locale_t __locale, __builtin_va_list __args),format_vwbprintf_p_l,(__bprinter,__closure,__format,__locale,__args))

__SYSDECL_END

#endif /* __KOS_VERSION__ >= 300 && __CRT_KOS && !__KERNEL__ */

#endif /* !_PARTS_KOS3_UFORMATPRINTER_H */
