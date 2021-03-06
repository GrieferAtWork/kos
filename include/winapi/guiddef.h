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
#ifndef _GUIDDEF_H_
#define _GUIDDEF_H_ 1

#ifndef _WINDEF_
#include "windef.h"
#endif /* !_WINDEF_ */

__DECL_BEGIN

#ifndef GUID_DEFINED
#define GUID_DEFINED
typedef struct _GUID {
    __UINT32_TYPE__ Data1;
    __UINT16_TYPE__ Data2;
    __UINT16_TYPE__ Data3;
    __UINT8_TYPE__  Data4[8];
} GUID;
#endif

#undef DEFINE_GUID
#ifdef INITGUID
#ifdef __cplusplus
#define DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) EXTERN_C const GUID DECLSPEC_SELECTANY name = { l,w1,w2,{ b1,b2,b3,b4,b5,b6,b7,b8 } }
#else
#define DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) const GUID DECLSPEC_SELECTANY name = { l,w1,w2,{ b1,b2,b3,b4,b5,b6,b7,b8 } }
#endif
#else
#define DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) EXTERN_C const GUID name
#endif
#define DEFINE_OLEGUID(name,l,w1,w2)    DEFINE_GUID(name,l,w1,w2,0xC0,0,0,0,0,0,0,0x46)

#ifndef __LPGUID_DEFINED__
#define __LPGUID_DEFINED__
typedef GUID *LPGUID;
#endif
#ifndef __LPCGUID_DEFINED__
#define __LPCGUID_DEFINED__
typedef const GUID *LPCGUID;
#endif

#ifndef __IID_DEFINED__
#define __IID_DEFINED__
typedef GUID IID,*LPIID;
#define IID_NULL                       GUID_NULL
#define IsEqualIID(riid1,riid2)        IsEqualGUID(riid1,riid2)

typedef GUID CLSID,*LPCLSID;
#define CLSID_NULL GUID_NULL
#define IsEqualCLSID(rclsid1,rclsid2) IsEqualGUID(rclsid1,rclsid2)

typedef GUID FMTID,*LPFMTID;
#define FMTID_NULL GUID_NULL
#define IsEqualFMTID(rfmtid1,rfmtid2) IsEqualGUID(rfmtid1,rfmtid2)

#ifdef __midl_proxy
#define __MIDL_CONST
#else
#define __MIDL_CONST const
#endif

#ifndef _REFGUID_DEFINED
#define _REFGUID_DEFINED
#ifdef __cplusplus
#define REFGUID const GUID &
#else
#define REFGUID const GUID *__MIDL_CONST
#endif
#endif

#ifndef _REFIID_DEFINED
#define _REFIID_DEFINED
#ifdef __cplusplus
#define REFIID const IID &
#else
#define REFIID const IID *__MIDL_CONST
#endif
#endif

#ifndef _REFCLSID_DEFINED
#define _REFCLSID_DEFINED
#ifdef __cplusplus
#define REFCLSID const IID &
#else
#define REFCLSID const IID *__MIDL_CONST
#endif
#endif

#ifndef _REFFMTID_DEFINED
#define _REFFMTID_DEFINED
#ifdef __cplusplus
#define REFFMTID const IID &
#else
#define REFFMTID const IID *__MIDL_CONST
#endif
#endif
#endif

#ifndef _SYS_GUID_OPERATORS_
#define _SYS_GUID_OPERATORS_
#include <string.h>

#ifdef __cplusplus
__LOCAL int (InlineIsEqualGUID)(REFGUID rguid1, REFGUID rguid2) {
 return (((__UINT32_TYPE__ *)&rguid1)[0] == ((__UINT32_TYPE__ *)&rguid2)[0] &&
         ((__UINT32_TYPE__ *)&rguid1)[1] == ((__UINT32_TYPE__ *)&rguid2)[1] &&
         ((__UINT32_TYPE__ *)&rguid1)[2] == ((__UINT32_TYPE__ *)&rguid2)[2] &&
         ((__UINT32_TYPE__ *)&rguid1)[3] == ((__UINT32_TYPE__ *)&rguid2)[3]);
}
__LOCAL int (IsEqualGUID)(REFGUID rguid1, REFGUID rguid2) { return !memcmp(&rguid1,&rguid2,sizeof(GUID)); }
#else
#define InlineIsEqualGUID(rguid1,rguid2) \
         (((__UINT32_TYPE__ *)(rguid1))[0] == ((__UINT32_TYPE__ *)(rguid2))[0] && \
          ((__UINT32_TYPE__ *)(rguid1))[1] == ((__UINT32_TYPE__ *)(rguid2))[1] && \
          ((__UINT32_TYPE__ *)(rguid1))[2] == ((__UINT32_TYPE__ *)(rguid2))[2] && \
          ((__UINT32_TYPE__ *)(rguid1))[3] == ((__UINT32_TYPE__ *)(rguid2))[3])
#define IsEqualGUID(rguid1,rguid2)         (!memcmp(rguid1,rguid2,sizeof(GUID)))
#endif

#ifdef __INLINE_ISEQUAL_GUID
#undef IsEqualGUID
#define IsEqualGUID(rguid1,rguid2) InlineIsEqualGUID(rguid1,rguid2)
#endif

#define IsEqualIID(riid1,riid2)       IsEqualGUID(riid1,riid2)
#define IsEqualCLSID(rclsid1,rclsid2) IsEqualGUID(rclsid1,rclsid2)

#if !defined(_SYS_GUID_OPERATOR_EQ_) && \
    !defined(_NO_SYS_GUID_OPERATOR_EQ_)
#define _SYS_GUID_OPERATOR_EQ_
#ifdef __cplusplus
__LOCAL int operator == (REFGUID guidOne, REFGUID guidOther) { return IsEqualGUID(guidOne,guidOther); }
__LOCAL int operator != (REFGUID guidOne, REFGUID guidOther) { return !(guidOne==guidOther); }
#endif
#endif
#endif

__DECL_END

#endif /* !_GUIDDEF_H_ */
