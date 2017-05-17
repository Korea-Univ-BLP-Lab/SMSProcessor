// Koma.h: interface for the CKoma class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __KOMARESULT__
#define __KOMARESULT__

#include "nlp.def"

#define MaxKomaBuffer 100000

typedef struct {
  int NextToFill;
  char Buffer[MaxKomaBuffer];
  char *Wrd[MaxNumWrd];
  char *Cnd[MaxNumWrd][MaxNumAmb];
  char CndString[MaxNumWrd][MaxNumAmb][512];  
} KomaResultType;

#endif
