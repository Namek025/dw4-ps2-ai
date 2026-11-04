/*
** $Id: lmem.c,v 1.39 2000/10/30 16:29:59 roberto Exp $
** Interface to Memory Manager
** See Copyright Notice in lua.h
*/


#include <stdlib.h>

#include "lua.h"

#include "ldo.h"
#include "lmem.h"
#include "lobject.h"
#include "lstate.h"




#ifdef LUA_DEBUG
/*
** {======================================================================
** Controlled version for realloc.
** =======================================================================
*/


#include <assert.h>
#include <limits.h>
#include <string.h>

#define realloc(b, s)	debug_realloc(b, s)
#define malloc(b)	debug_realloc(NULL, b)
#define free(b)		debug_realloc(b, 0)


/* ensures maximum alignment for HEADER */
#define HEADER	(sizeof(union L_Umaxalign))

#define MARKSIZE	16
#define MARK		0x55  /* 01010101 (a nice pattern) */


#define blocksize(b)	((unsigned long *)((char *)(b) - HEADER))

unsigned long memdebug_numblocks = 0;
unsigned long memdebug_total = 0;
unsigned long memdebug_maxmem = 0;
unsigned long memdebug_memlimit = LONG_MAX;


static void *checkblock (void *block) {
  unsigned long *b = blocksize(block);
  unsigned long size = *b;
  int i;
  for (i=0;i<MARKSIZE;i++)
    assert(*(((char *)b)+HEADER+size+i) == MARK+i);  /* corrupted block? */
  memdebug_numblocks--;
  memdebug_total -= size;
  return b;
}


static void freeblock (void *block) {
  if (block) {
    size_t size = *blocksize(block);
    block = checkblock(block);
    memset(block, -1, size+HEADER+MARKSIZE);  /* erase block */
    (free)(block);  /* free original block */
  }
}


static void *debug_realloc (void *block, size_t size) {
  if (size == 0) {
    freeblock(block);
    return NULL;
  }
  else if (memdebug_total+size > memdebug_memlimit)
    return NULL;  /* to test memory allocation errors */
  else {
    size_t realsize = HEADER+size+MARKSIZE;
    char *newblock = (char *)(malloc)(realsize);  /* alloc a new block */
    int i;
    if (realsize < size) return NULL;  /* overflow! */
    if (newblock == NULL) return NULL;
    if (block) {
      size_t oldsize = *blocksize(block);
      if (oldsize > size) oldsize = size;
      memcpy(newblock+HEADER, block, oldsize);
      freeblock(block);  /* erase (and check) old copy */
    }
    memdebug_total += size;
    if (memdebug_total > memdebug_maxmem) memdebug_maxmem = memdebug_total;
    memdebug_numblocks++;
    *(unsigned long *)newblock = size;
    for (i=0;i<MARKSIZE;i++)
      *(newblock+HEADER+size+i) = (char)(MARK+i);
    return newblock+HEADER;
  }
}


/* }====================================================================== */
#endif



/*
** Real ISO (ANSI) systems do not need these tests;
** but some systems (Sun OS) are not that ISO...
*/
#ifdef OLD_ANSI
#define realloc(b,s)	((b) == NULL ? malloc(s) : (realloc)(b, s))
#define free(b)		if (b) (free)(b)
#endif


void *luaM_growaux (lua_State *L, void *block, size_t nelems,
               int inc, size_t size, const char *errormsg, size_t limit) {
  size_t newn = nelems+inc;
  if (nelems >= limit-inc) lua_error(L, errormsg);
  if ((newn ^ nelems) <= nelems ||  /* still the same power-of-2 limit? */
       (nelems > 0 && newn < MINPOWER2))  /* or block already is MINPOWER2? */
      return block;  /* do not need to reallocate */
  else  /* it crossed a power-of-2 boundary; grow to next power */
    return luaM_realloc(L, block, luaO_power2(newn)*size);
}


/*
** generic allocation routine.
*/
/* port: luaM_realloc is the game's own. It uses the allocator object passed to
 * lua_open (L->unk70) if there is one, else the global heap functions.
 * Only the allocator slots Lua uses are known. MWCC vtables have an 8-byte
 * header, so slot n is at 8 + 4 * n. */
class GameAllocator {
public:
  virtual void unk8();
  virtual void unkC();
  virtual void unk10();
  virtual void unk14();
  virtual void unk18();
  virtual void unk1C();
  virtual void unk20();
  virtual void unk24();
  virtual void unk28();
  virtual void unk2C();
  virtual void unk30();
  virtual void unk34();
  virtual void unk38();
  virtual void unk3C();
  virtual void unk40();
  virtual void unk44();
  virtual void unk48();
  virtual void unk4C();
  virtual void unk50();
  virtual void unk54();
  virtual void unk58();
  virtual void unk5C();
  virtual void unk60();
  virtual void unk64();
  virtual void unk68();
  virtual void unk6C();
  virtual void unk70();
  virtual void unk74();
  virtual void unk78();
  virtual void unk7C();
  virtual void unk80();
  virtual void unk84();
  virtual void unk88();
  virtual void unk8C();
  virtual void unk90();
  virtual void unk94();
  virtual void unk98();
  virtual void unk9C();
  virtual void unkA0();
  virtual void unkA4();
  virtual void unkA8();
  virtual void unkAC();
  virtual void unkB0();
  virtual void unkB4();
  virtual void unkB8();
  virtual void unkBC();
  virtual void unkC0();
  virtual void unkC4();
  virtual void unkC8();
  virtual void unkCC();
  virtual void unkD0();
  virtual void unkD4();
  virtual void *Alloc(lint32 size);                 /* 0xD8 */
  virtual void unkDC();
  virtual void unkE0();
  virtual void unkE4();
  virtual void *Realloc(void *block, lint32 size);  /* 0xE8 */
  virtual void unkEC();
  virtual void unkF0();
  virtual void unkF4();
  virtual void unkF8();
  virtual void unkFC();
  virtual void unk100();
  virtual void unk104();
  virtual void Free(void *block);                   /* 0x108 */
};
extern void func_002ED4B0(void *block);
extern void *func_002ED440(void *block, lint32 size);
extern void *func_002ED200(lint32 size);

void *luaM_realloc (lua_State *L, void *block, lint32 size) {
  GameAllocator *a;
  if (size >= MAX_SIZET) {
    lua_error(L, "memory allocation error: block too big");
    return NULL;
  }
  if (L != NULL && (a = (GameAllocator *)L->unk70) != NULL && block != L) {
    if (size == 0) {
      a->Free(block);
      return NULL;
    }
    if (block != NULL)
      block = a->Realloc(block, size);
    else
      block = a->Alloc(size);
    if (block == NULL)
      luaD_breakrun(L, LUA_ERRMEM);
    return block;
  }
  if (size == 0) {
    func_002ED4B0(block);
    return NULL;
  }
  if (block != NULL)
    return func_002ED440(block, size);
  return func_002ED200(size);
}


