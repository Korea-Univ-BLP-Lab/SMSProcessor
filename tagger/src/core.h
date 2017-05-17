#ifndef CORE_H
#define CORE_H

#include "Koma.h"

bool AnalyzeHangulSymbol (CKoma *koma, char* word);

bool SplitHangulSymbol (CKoma *koma, char* hword);

bool AnalyzeSymbol (CKoma *koma, short wi);

bool AnalyzeHangul (CKoma *koma, short wi);

bool ConcateHangulSymbol (CKoma *koma);

#endif
