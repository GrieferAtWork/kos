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
#ifndef GUARD_LIBS_LIBWM_WINDOW_C
#define GUARD_LIBS_LIBWM_WINDOW_C 1
#define _EXCEPT_SOURCE 1
#define _KOS_SOURCE 1

#include <hybrid/compiler.h>
#include <hybrid/atomic.h>
#include <hybrid/align.h>
#include <hybrid/xch.h>
#include <wm/api.h>
#include <wm/server.h>
#include <wm/window.h>
#include <unistd.h>
#include <except.h>
#include <sys/poll.h>
#include <sys/mman.h>
#include <malloc.h>

#include "libwm.h"

DECL_BEGIN


typedef struct {
    atomic_rwlock_t              wm_lock; /* Lock for this hash-map. */
    wms_window_id_t              wm_size; /* Number of windows in the window map. */
    wms_window_id_t              wm_mask; /* Hash-mask. */
    LIST_HEAD(struct wm_window) *wm_map;  /* [0..1][0..wm_mask+1][owned] Hash map. */
} WindowMap;

PRIVATE WindowMap libwm_window_map = {
    .wm_lock = ATOMIC_RWLOCK_INIT,
    .wm_size = 0,
    .wm_mask = 0,
    .wm_map  = NULL
};


PRIVATE void WMCALL
libwm_window_register(struct wm_window *__restrict win) {
 struct wm_window **bucket;
again:
 /* Insert into global window_id -> window hash table. */
 atomic_rwlock_write(&libwm_window_map.wm_lock);
 if ((libwm_window_map.wm_size/3)*2 >= libwm_window_map.wm_mask) {
  /* Rehash the hash-map. */
  size_t i,new_mask = libwm_window_map.wm_mask;
  struct wm_window **new_map,*iter,*next;
  while (new_mask <= libwm_window_map.wm_size)
         new_mask = (new_mask << 1) | 1;
  if (new_mask < 7) new_mask = 7;
  atomic_rwlock_endwrite(&libwm_window_map.wm_lock);
  /* Allocate a new map. */
  new_map = (struct wm_window **)Xcalloc(new_mask+1,sizeof(struct wm_window *));
  atomic_rwlock_write(&libwm_window_map.wm_lock);
  if unlikely(new_mask <= libwm_window_map.wm_mask) {
   atomic_rwlock_endwrite(&libwm_window_map.wm_lock);
   cfree(new_map);
   goto again;
  }
  if (libwm_window_map.wm_map) {
   for (i = 0; i <= libwm_window_map.wm_mask; ++i) {
    iter = libwm_window_map.wm_map[i];
    while (iter) {
     next = iter->w_chain.le_next;
     /* Re-insert the window into the new hash-map. */
     bucket = &new_map[iter->w_winid & new_mask];
     LIST_INSERT(*bucket,iter,w_chain);
     iter = next;
    }
   }
  }
  /* Save the new hash-map and mask. */
  new_map = XCH(libwm_window_map.wm_map,new_map);
  libwm_window_map.wm_mask = new_mask;
  atomic_rwlock_endwrite(&libwm_window_map.wm_lock);
  free(new_map);
  /* Start over now that the buffer has become larger. */
  goto again;
 }
 bucket = &libwm_window_map.wm_map[win->w_winid & libwm_window_map.wm_mask];
 LIST_INSERT(*bucket,win,w_chain);
 ++libwm_window_map.wm_size;
 atomic_rwlock_endwrite(&libwm_window_map.wm_lock);
}

PRIVATE ATTR_NOTHROW void WMCALL
libwm_window_delete(struct wm_window *__restrict win) {
 /* Remove from global window_id -> window hash table. */
 atomic_rwlock_write(&libwm_window_map.wm_lock);
 assert(libwm_window_map.wm_size >= 1);
 assert(win->w_chain.le_pself != NULL);
 LIST_REMOVE(win,w_chain);
 --libwm_window_map.wm_size;
 atomic_rwlock_endwrite(&libwm_window_map.wm_lock);
}

DEFINE_PUBLIC_ALIAS(wm_window_fromid,libwm_window_fromid);
INTERN ATTR_NOTHROW REF struct wm_window *WMCALL
libwm_window_fromid(wms_window_id_t id) {
 /* Search global window_id -> window hash table.
  * NOTE: Windows with a reference counter of ZERO(0) must be returned! */
 REF struct wm_window *result = NULL;
 atomic_rwlock_read(&libwm_window_map.wm_lock);
 if likely(libwm_window_map.wm_map) {
  result = libwm_window_map.wm_map[id & libwm_window_map.wm_mask];
  while (result && result->w_winid != id)
         result = result->w_chain.le_next;
  /* Try to add a reference for this window. */
  if (result && !ATOMIC_INCIFNONZERO(result->s_refcnt))
      result = NULL;
 }
 atomic_rwlock_endread(&libwm_window_map.wm_lock);
 return result;
}






DEFINE_PUBLIC_ALIAS(wm_window_move,libwm_window_move);
INTERN void WMCALL
libwm_window_move(struct wm_window *__restrict self,
                  int new_posx, int new_posy) {
 struct wms_request req;
 struct wms_response resp;
 wm_surface_lock(self);
 if (self->w_posx == new_posx &&
     self->w_posy == new_posy) {
  wm_surface_unlock(self);
  /* Nothing to do here! */
  return;
 }
 wm_surface_unlock(self);
 req.r_command        = WMS_COMMAND_MVWIN;
 req.r_flags          = WMS_COMMAND_FNORMAL;
 req.r_mvwin.mw_winid = self->w_winid;
 req.r_mvwin.mw_newx  = new_posx;
 req.r_mvwin.mw_newy  = new_posy;
 libwms_dorequest(&req,&resp);
}


DEFINE_PUBLIC_ALIAS(wm_window_bring_to_front,libwm_window_bring_to_front);
INTERN bool WMCALL
libwm_window_bring_to_front(struct wm_window *__restrict self) {
 struct wms_request req;
 struct wms_response resp;
 req.r_command = WMS_COMMAND_TOFRONT;
 req.r_flags   = WMS_COMMAND_FNORMAL;
 req.r_tofront.fw_winid = self->w_winid;
 libwms_dorequest(&req,&resp);
 return resp.r_answer == WMS_RESPONSE_TOFRONT_OK;
}

DEFINE_PUBLIC_ALIAS(wm_window_chstat,libwm_window_chstat);
INTERN u16 WMCALL
libwm_window_chstat(struct wm_window *__restrict self, u16 mask, u16 flag) {
 struct wms_request req;
 struct wms_response resp; u16 result;
 if (flag & ~(WM_WINDOW_STATE_FHIDDEN|WM_WINDOW_STATE_FFOCUSED))
     error_throw(E_INVALID_ARGUMENT);
 wm_surface_lock(self);
 result = self->w_state;
 if (((result & mask) | flag) == result) {
  /* Nothing changes */
  wm_surface_unlock(self);
  return result;
 }
 wm_surface_unlock(self);
 req.r_command = WMS_COMMAND_CHWIN;
 req.r_flags   = WMS_COMMAND_FNORMAL;
 req.r_chwin.cw_winid = self->w_winid;
 req.r_chwin.cw_mask  = mask;
 req.r_chwin.cw_flag  = flag;
 /* Window fields are actually updated by `WM_WINDOWEVENT_STATE_CHANGE' events. */
 libwms_dorequest(&req,&resp);
 if (resp.r_answer                 != WMS_RESPONSE_EVENT ||
     resp.r_event.e_type           != WM_EVENT_WINDOW ||
     resp.r_event.e_window.w_event != WM_WINDOWEVENT_STATE_CHANGE)
     error_throw(E_NOT_IMPLEMENTED);
 return resp.r_event.e_window.w_changed.s_oldstat;
}



STATIC_ASSERT(WM_WINDOW_DRAW_FASYNC == WMS_COMMAND_FNOACK);
STATIC_ASSERT(WM_WINDOW_DRAW_FVSYNC == WMS_COMMAND_DRAW_FVSYNC);
DEFINE_PUBLIC_ALIAS(wm_window_draw,libwm_window_draw);
INTERN void WMCALL
libwm_window_draw(struct wm_window *__restrict self,
                  unsigned int mode) {
 struct wms_request req;
 struct wms_response resp;
 if (mode & ~(WM_WINDOW_DRAW_FASYNC|WM_WINDOW_DRAW_FVSYNC))
     error_throw(E_INVALID_ARGUMENT);
 req.r_command          = WMS_COMMAND_DRAWALL;
 req.r_flags            = mode;
 req.r_drawall.dw_winid = self->w_winid;
 if (mode & WM_WINDOW_DRAW_FASYNC) {
  libwms_sendrequest(&req);
 } else {
  /* Also wait for an ACK. */
  libwms_dorequest(&req,&resp);
 }
}


DEFINE_PUBLIC_ALIAS(wm_window_draw_rect,libwm_window_draw_rect);
INTERN void WMCALL
libwm_window_draw_rect(struct wm_window *__restrict self,
                       int xmin, int ymin,
                       unsigned int xsiz, unsigned int ysiz,
                       unsigned int mode) {
 struct wms_request req;
 struct wms_response resp;
 if (mode & ~(WM_WINDOW_DRAW_FASYNC|WM_WINDOW_DRAW_FVSYNC))
     error_throw(E_INVALID_ARGUMENT);
 if (xmin < 0) {
  if (xsiz <= (unsigned int)-xmin) goto empty;
  xsiz -= -xmin;
  xmin = 0;
 }
 if (ymin < 0) {
  if (ysiz <= (unsigned int)-ymin) goto empty;
  ysiz -= -ymin;
  ymin = 0;
 }
 wm_surface_lock(self);
 TRY {
  if ((unsigned int)(xmin+xsiz) > self->s_sizex) {
   if ((unsigned int)xmin >= self->s_sizex) goto empty_unlock;
   xsiz = self->s_sizex-xmin;
  }
  if ((unsigned int)(ymin+ysiz) > self->s_sizey) {
   if ((unsigned int)ymin >= self->s_sizey) goto empty_unlock;
   ysiz = self->s_sizey-ymin;
  }
  /* Fill in the render request. */
  req.r_command          = WMS_COMMAND_DRAWONE;
  req.r_flags            = mode;
  req.r_drawone.dw_winid = self->w_winid;
  req.r_drawone.dw_xmin  = self->w_bordersz+(unsigned int)xmin;
  req.r_drawone.dw_ymin  = self->w_titlesz+(unsigned int)ymin;
  req.r_drawone.dw_xsiz  = xsiz;
  req.r_drawone.dw_ysiz  = ysiz;
 } FINALLY {
  wm_surface_unlock(self);
 }
 /* Send the request. */
 if (mode & WM_WINDOW_DRAW_FASYNC) {
  libwms_sendrequest(&req);
 } else {
  /* Wait for the ACK. */
  libwms_dorequest(&req,&resp);
 }
 return;
empty_unlock:
 wm_surface_unlock(self);
empty:
 if (mode & WM_WINDOW_DRAW_FASYNC) return;
 if (!(mode & WM_WINDOW_DRAW_FVSYNC)) return;
 /* Wait for a display sync. */
 req.r_command          = WMS_COMMAND_DRAWONE;
 req.r_flags            = WMS_COMMAND_DRAW_FVSYNC;
 req.r_drawone.dw_winid = self->w_winid;
 req.r_drawone.dw_xsiz  = 0;
 req.r_drawone.dw_ysiz  = 0;
 libwms_dorequest(&req,&resp);
}

DEFINE_PUBLIC_ALIAS(wm_window_draw_rects,libwm_window_draw_rects);
INTERN void WMCALL
libwm_window_draw_rects(struct wm_window *__restrict self,
                        size_t rectc, struct wm_rect const *__restrict rectv,
                        unsigned int mode) {
 size_t i;
 if (!rectc) return;
 for (i = 0; i < rectc-1; ++i) {
  /* Draw all rects leading up to the last in async mode. */
  libwm_window_draw_rect(self,
                         rectv[i].r_xmin,
                         rectv[i].r_ymin,
                         rectv[i].r_xsiz,
                         rectv[i].r_ysiz,
                         WM_WINDOW_DRAW_FASYNC);
 }
 /* Draw the last rectangle in the specified mode. */
 libwm_window_draw_rect(self,
                        rectv[i].r_xmin,
                        rectv[i].r_ymin,
                        rectv[i].r_xsiz,
                        rectv[i].r_ysiz,
                        mode);
}


DEFINE_PUBLIC_ALIAS(wm_window_close,libwm_window_close);
INTERN void WMCALL
libwm_window_close(struct wm_window *__restrict self) {
 union wm_event evt;
 u32 feat = ATOMIC_READ(self->w_features);
 if (!(feat & WM_WINDOW_FEAT_FNOAUTOHIDE))
       libwm_window_chstat(self,~0,WM_WINDOW_STATE_FHIDDEN);
 if (feat & WM_WINDOW_FEAT_FEXITONCLOSE)
     error_throw(E_EXIT_PROCESS);
 /* Trigger a window-close event. */
 evt.e_window.w_type   = WM_EVENT_WINDOW;
 evt.e_window.w_flag   = WM_EVENT_FNORMAL;
 evt.e_window.w_window = self;
 evt.e_window.w_event  = WM_WINDOWEVENT_CLOSE;
 libwm_window_incref(self);
 TRY {
  libwm_event_dealwith(&evt);
 } FINALLY {
  libwm_window_decref(self);
 }
}

DEFINE_PUBLIC_ALIAS(wm_window_getid,libwm_window_getid);
INTERN ATTR_NOTHROW ATTR_CONST wms_window_id_t WMCALL
libwm_window_getid(struct wm_window *__restrict self) {
 return self->w_winid;
}


DEFINE_PUBLIC_ALIAS(wm_window_create,libwm_window_create);
INTERN ATTR_RETNONNULL REF struct wm_window *WMCALL
libwm_window_create(int pos_x, int pos_y, unsigned int size_x, unsigned int size_y,
                    char const *title, u32 features, u16 state, u16 mode,
                    struct wm_winevent_ops *eventops, void *userdata) {
 REF struct wm_window *EXCEPT_VAR result;
 /* Check for invalid arguments. */
 if unlikely(!size_x || !size_y)
    error_throw(E_INVALID_ARGUMENT);
 if unlikely(size_x > WM_MAX_SURFACE_SIZE ||
             size_y > WM_MAX_SURFACE_SIZE)
    error_throw(E_INVALID_ARGUMENT);
 result = (REF struct wm_window *)Xmalloc(sizeof(struct wm_window));
 TRY {
  struct wms_request req;
  struct wms_response resp;
  unsigned int token;
  result->w_winfd    = -1;
  result->w_bordersz = 3;
  result->w_titlesz  = 18;
  result->w_features = features;
  result->w_mode     = mode;
  result->w_events   = eventops;
  result->w_userdata = userdata;
  atomic_rwlock_init(&result->s_lock);
  if (features & WM_WINDOW_FEAT_FNOBORDER)
      result->w_titlesz -= result->w_bordersz,
      result->w_bordersz = 0;
  if (features & WM_WINDOW_FEAT_FNOHEADER)
      result->w_titlesz = result->w_bordersz;

  COMPILER_WRITE_BARRIER();
  req.r_command        = WMS_COMMAND_MKWIN;
  req.r_flags          = WMS_COMMAND_FNORMAL;
  req.r_mkwin.mw_xmin  = pos_x;
  req.r_mkwin.mw_ymin  = pos_y;
  req.r_mkwin.mw_xsiz  = size_x + (2 * result->w_bordersz);
  req.r_mkwin.mw_ysiz  = size_y + (result->w_bordersz +
                                   result->w_titlesz);
  req.r_mkwin.mw_state = state;

  mutex_get(&libwms_lock);
  TRY {
   /* Send the request. */
   token = libwms_sendrequest(&req);
   /* Receive the response. */
   result->w_winfd = libwms_recvresponse_fd(token,&resp);
  } FINALLY {
   mutex_put(&libwms_lock);
  }

  if (resp.r_answer != WMS_RESPONSE_MKWIN_OK)
      error_throw(E_NOT_IMPLEMENTED);

  TRY {
   /* Map the display buffer into memory. */
   result->w_buffer = (byte_t *)mmap(NULL,
                                     resp.r_mkwin.w_sizey*resp.r_mkwin.w_stride,
                                     PROT_READ|PROT_WRITE|PROT_SHARED,
                                     MAP_SHARED|MAP_FILE,
                                     result->w_winfd,
                                     0);
   TRY {

    /* Extract data from the response. */
    result->w_winid    = resp.r_mkwin.w_id;
    result->w_state    = resp.r_mkwin.w_state;
    result->w_posx     = resp.r_mkwin.w_posx;
    result->w_posy     = resp.r_mkwin.w_posy;
    result->w_sizex    = resp.r_mkwin.w_sizex;
    result->w_sizey    = resp.r_mkwin.w_sizex;
    result->w_oldsizex = resp.r_mkwin.w_sizex;
    result->w_oldsizey = resp.r_mkwin.w_sizey;

    /* Adjust so the surface describes the window contents without the border. */
    result->s_flags   = WM_SURFACE_FWINDOW;
    result->s_sizex   = resp.r_mkwin.w_sizex - (2 * result->w_bordersz);
    result->s_sizey   = resp.r_mkwin.w_sizey - (result->w_bordersz + result->w_titlesz);
    result->s_stride  = resp.r_mkwin.w_stride;
    result->s_buffer  = result->w_buffer;
    result->s_buffer += result->w_titlesz * resp.r_mkwin.w_stride;
    result->s_buffer += result->w_bordersz;

    /* This currently assumes the palette that is implemented by the server. */
    result->s_format = libwm_format_create_pal(&libwm_palette_256,
                                                resp.r_mkwin.w_bpp);
    libwm_setup_surface_ops((struct wm_surface *)result);
    TRY {
     /* Map the window  */
     libwm_window_register(result);
    } EXCEPT (EXCEPT_EXECUTE_HANDLER) {
     libwm_format_decref(result->s_format);
     error_rethrow();
    }

   } EXCEPT (EXCEPT_EXECUTE_HANDLER) {
    munmap(result->w_buffer,
           resp.r_mkwin.w_sizey*
           resp.r_mkwin.w_stride);
    error_rethrow();
   }

  } EXCEPT (EXCEPT_EXECUTE_HANDLER) {
   /* Destroy the window again if something went wrong. */
   error_pushinfo();
   req.r_command        = WMS_COMMAND_RMWIN;
   req.r_flags          = WMS_COMMAND_FNOACK;
   req.r_rmwin.rw_winid = resp.r_mkwin.w_id;
   libwms_sendrequest(&req);
   error_popinfo();
   error_rethrow();
  }

 } EXCEPT (EXCEPT_EXECUTE_HANDLER) {
  if (result->w_winfd >= 0)
      close(result->w_winfd);
  free(result);
  error_rethrow();
 }
 return result;
}

INTERN void WMCALL
libwm_window_resize(struct wm_window *__restrict self,
                    unsigned int new_sizx,
                    unsigned int new_sizy) {
 /* TODO */
}



INTERN ATTR_NOTHROW void WMCALL
libwm_window_destroy(struct wm_window *__restrict self) {
 struct wm_window *EXCEPT_VAR xself = self;
 struct wms_request req;
 /* First off: Remove the window from the global id->window hash table. */
 assert(self->s_refcnt == 0);
 libwm_window_delete(self);
 /* Invoke a user-defined window-fini callback. */
 if (self->w_events &&
     self->w_events->wo_fini)
   (*self->w_events->wo_fini)(self);
 /* Unmap the window display buffer. */
 munmap(self->w_buffer,
        self->s_stride * self->w_sizey);
 /* Remove the window. */
 req.r_command        = WMS_COMMAND_RMWIN;
 req.r_flags          = WMS_COMMAND_FNOACK;
 req.r_rmwin.rw_winid = self->w_winid;
 TRY {
  libwms_sendrequest(&req);
 } EXCEPT (EXCEPT_EXECUTE_HANDLER) {
  error_printf("Failed to remove window %Iu\n",
                xself->w_winid);
  error_handled();
 }

 /* Close the window screen buffer file. */
 close(xself->w_winfd);

 /* Free all the remaining buffers and pointers. */
 libwm_format_decref(xself->s_format);
 free(xself->w_title);
 free(xself);
}




DEFINE_PUBLIC_ALIAS(wm_window_chfeat,libwm_window_chfeat);
INTERN u32 WMCALL
libwm_window_chfeat(struct wm_window *__restrict self, u32 mask, u32 flag) {
 /* TODO */
 return 0;
}

DEFINE_PUBLIC_ALIAS(wm_window_chmode,libwm_window_chmode);
INTERN u16 WMCALL
libwm_window_chmode(struct wm_window *__restrict self, u16 mask, u16 flag) {
 /* TODO */
 return 0;
}

DEFINE_PUBLIC_ALIAS(wm_window_settitle,libwm_window_settitle);
INTERN void WMCALL
libwm_window_settitle(struct wm_window *__restrict self,
                      char const *__restrict new_title) {
 /* TODO */
}


DEFINE_PUBLIC_ALIAS(wm_window_viewall,libwm_window_viewall);
INTERN ATTR_NOTHROW struct wm_surface_view *WMCALL
libwm_window_viewall(struct wm_surface_view *__restrict view,
                     struct wm_window const *__restrict self) {
 view->s_ops    = self->s_ops;
 view->s_format = self->s_format;
 view->s_flags  = WM_SURFACE_FNORMAL;
 atomic_rwlock_init(&view->s_lock);
 view->s_sizex  = self->w_sizex;
 view->s_sizey  = self->w_sizey;
 view->s_stride = self->s_stride;
 view->s_buffer = self->w_buffer;
 view->s_offx   = 0;
 view->s_offy   = 0;
 return view;
}

INTDEF struct wm_surface_ops empty_surface_ops;
DEFINE_PUBLIC_ALIAS(wm_window_viewtitle,libwm_window_viewtitle);
INTERN ATTR_NOTHROW struct wm_surface_view *WMCALL
libwm_window_viewtitle(struct wm_surface_view *__restrict view,
                       struct wm_window const *__restrict self) {
 view->s_ops    = self->s_ops;
 view->s_format = self->s_format;
 view->s_flags  = WM_SURFACE_FNORMAL;
 atomic_rwlock_init(&view->s_lock);
 view->s_sizex  = self->w_sizex;
 view->s_sizey  = self->w_titlesz;
 view->s_stride = self->s_stride;
 view->s_buffer = self->w_buffer;
 view->s_offx   = 0;
 view->s_offy   = 0;
 if (!view->s_sizey)
      view->s_ops = &empty_surface_ops;
 return view;
}


DECL_END

#endif /* !GUARD_LIBS_LIBWM_WINDOW_C */
