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
#ifndef __GUARD_HYBRID_ATREE_H
#define __GUARD_HYBRID_ATREE_H 1

#include <hybrid/compiler.h>
#include <kos/types.h>
#include <assert.h>

DECL_BEGIN

/* An key tree is a binary-tree based container designed
 * for extremely fast lookup to a mapping for any key.
 * Worst case lookup speed is O(31) on 32-bit and O(63) on 64-bit,
 * though available ram will severely reduce this number again,
 * with lookup speed also being affected by the amount of mapped
 * leafs/branches, as well as how they are mapped, as well as the
 * index of the least significant bit set in a given key.
 *
 * Also noteworthy is the fact that instead of mapping individual
 * addresses to unique leafs, an key tree maps regions of
 * addresses (so called key ranges/regions).
 * With that in mind, mapping an key range more than once
 * isn't allowed, causing either an error, or undefined behavior.
 *
 * An key tree follows strict ordering rules that enforce
 * a min-max mapping for every leaf that can be determined by
 * its associated addrsemi and addrlevel value.
 * 
 * addrsemi:
 *   - The center point used as hint to plot the
 *     correct path for any mapped key.
 *   - Each time a branch is reached, the given pointer
 *     is checked to be located inside of its mapping.
 *     Only when it wasn't found, continue search as described below.
 *   Assuming 32-bit addresses, a path might be plotted like this:
 *     ADDR: 0x73400000
 *     LEVEL 31; SEMI 0x80000000 >= 0x73400000 -> MIN (unset bit 31; set bit 30)
 *     LEVEL 30; SEMI 0x40000000 <  0x73400000 -> MAX (              set bit 29)
 *     LEVEL 29; SEMI 0x60000000 <  0x73400000 -> MAX (              set bit 28)
 *     LEVEL 28; SEMI 0x70000000 <  0x73400000 -> MAX (              set bit 27)
 *     LEVEL 27; SEMI 0x78000000 >= 0x73400000 -> MIN (unset bit 27; set bit 26)
 *     LEVEL 26; SEMI 0x74000000 >= 0x73400000 -> MIN (unset bit 26; set bit 25)
 *     LEVEL 25; SEMI 0x72000000 <  0x73400000 -> MAX (              set bit 24)
 *     LEVEL 24; SEMI 0x73000000 <  0x73400000 -> MAX (              set bit 23)
 *     LEVEL 23; SEMI 0x73800000 >= 0x73400000 -> MIN (unset bit 23; set bit 22)
 *     LEVEL 22; SEMI 0x73400000 == 0x73400000 -> STOP SEARCH
 *   >> The key `0x73400000' can be mapped to 10 different leafes,
 *      with the worst case (aka. failure) lookup time for this key
 *      always being O(10).
 *      Note though that for the lookup time to actually be 10, other mapping
 *      must be available that cover _all_ of the SEMI-values during all levels.
 *     
 * addrlevel:
 *   - The index of the bit that was set for the current iteration.
 *   - Technically not required, as it can be calculated from an addrsemi,
 *     carrying this around still allows the addrtree to run faster.
 *
 *
 * The 'addrtree_head' is designed as an inline
 * structure to be included in other, larger objects:
 *
 * >> struct my_mapping {
 * >>   struct addrtree_head head;
 * >>   int                  value;
 * >> };
 * >> 
 * >> static struct my_mapping *tree = NULL;
 * >> 
 * >> static int lookup(uintptr_t addr) {
 * >>   struct my_mapping *m = (struct my_mapping *)ATREE_LOCATE(tree,addr);
 * >>   return m ? m->value : -1;
 * >> }
 * >> static void map(uintptr_t min, uintptr_t max, int val) {
 * >>   struct my_mapping *m = omalloc(struct my_mapping);
 * >>   addrtree_init_for_insert(&m->head,min,max);
 * >>   m->value = val;
 * >>   ATREE_INSERT(&tree,m);
 * >> }
 * >> static void unmapat(uintptr_t addr) {
 * >>   // NOTE: Fast ranged unmapping is also possible, but not covered here.
 * >>   free(ATREE_REMOVE(&tree,addr));
 * >> }
 */


#define ATREE_HEAD(T)  T *
#define ATREE_NODE(T,Tkey) \
 struct { T    *a_min;  /* [0..1] Lower node. */ \
          T    *a_max;  /* [0..1] Upper node. */ \
          Tkey  a_vmin; /* Lower bound. */ \
          Tkey  a_vmax; /* Upper bound. */ \
 }

#define ATREE_NODE_SINGLE(T,Tkey) \
 struct { T    *a_min;   /* [0..1] Lower node. */ \
          T    *a_max;   /* [0..1] Upper node. */ \
          Tkey  a_vaddr; /* Node address. */ \
 }

#define ATREE_XNODE(T) \
 struct { T    *a_min;  /* [0..1] Lower node. */ \
          T    *a_max;  /* [0..1] Upper node. */ \
 }

#define ATREE_LEVEL_T                     unsigned int
#define ATREE_SEMI_T(Tkey)                Tkey
#define ATREE_SEMI0(Tkey)                ((((ATREE_SEMI_T(Tkey))-1)/2)+1)
#define ATREE_LEVEL0(Tkey)               ((sizeof(Tkey)*8)-1)

#define ATREE_SEMI0_BITS(Tkey,key_bits)  (((((ATREE_SEMI_T(Tkey))1 << (key_bits))-1)/2)+1)
#define ATREE_LEVEL0_BITS(Tkey,key_bits)    ((key_bits)-1)

#define ATREE_MAPMIN(Tkey,semi,level)  (Tkey)((semi)&~(((ATREE_SEMI_T(Tkey))1 << (level))))
#define ATREE_MAPMAX(Tkey,semi,level)  (Tkey)((semi)| (((ATREE_SEMI_T(Tkey))1 << (level))-1))
#define ATREE_WALKMIN(Tkey,semi,level) ((semi)  = (((semi)&~((ATREE_SEMI_T(Tkey))1 << (level)))| \
                                                   ((ATREE_SEMI_T(Tkey))1 << ((level)-1))), \
                                        --(level)) /* unset level`th bit; set level`th-1 bit. */
#define ATREE_WALKMAX(Tkey,semi,level) (--(level),(semi) |= ((ATREE_SEMI_T(Tkey))1 << (level))) /* set level`th-1 bit. */
#define ATREE_NEXTMIN(Tkey,semi,level) (((semi)&~((ATREE_SEMI_T(Tkey))1 << (level)))| \
                                       ((ATREE_SEMI_T(Tkey))1 << ((level)-1))) /* unset level`th bit; set level`th-1 bit. */
#define ATREE_NEXTMAX(Tkey,semi,level) ((semi)|((ATREE_SEMI_T(Tkey))1 << ((level)-1))) /* set level`th-1 bit. */
#define ATREE_NEXTLEVEL(level)         ((level)-1)
#define ATREE_SEMILEVEL(semi)          (ATREE_LEVEL_T)(ffs(semi)-1) /* Get the current level associated with a given semi-key. */

DECL_END

#endif /* !__GUARD_HYBRID_ATREE_H */
