// Tagger.h: interface for the CTagger class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __TAGGER__
#define __TAGGER__

#include "KomaAPI.h"
#include "KomaResult.h"
#include "TaggerRsc.h"
#include "FST.h"

#ifndef __TaggingMethod__
#define __TaggingMethod__
typedef enum {
  NO_TAGGING,           // ���¼� �м���
  PATH_BASED_TAGGING,   // ��� ��� �±�
  STATE_BASED_TAGGING,   // ���� ��� �±�
  PATH_BASED_TAGGING_KU
} TaggingMethod;
#endif

class CTagger  
{
public:
	CTagger();
	virtual ~CTagger();

  static CTaggerRsc m_Rsc;
  void *m_koma;
  KomaResultType m_KomaResult;

public:
  int DoTagging(char *Sentence, char *Wrd[MaxNumWrd], char *Result[MaxNumWrd][MaxNumAmb], TaggingMethod m, int BestNum=1);
  int PathBasedTagging(char *Cnd[MaxNumWrd][MaxNumAmb], char *Tag[MaxNumWrd][MaxNumAmb], int numwin);
  int StateBasedTagging(char *Cnd[MaxNumWrd][MaxNumAmb], char *Tag[MaxNumWrd][MaxNumAmb], int numwin);
  bool CheckExist( char *src, char *result );
  bool CheckPos( char *src, char *result );

private:
};

#endif
