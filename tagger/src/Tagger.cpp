// Tagger.cpp: implementation of the CTagger class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Tagger.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTaggerRsc CTagger::m_Rsc;

CTagger::CTagger()
{
  m_koma=CreateKoma(1);
}

CTagger::~CTagger()
{
  if (m_koma) FreeKoma(m_koma);
}

int CTagger::DoTagging(char *Sentence, char *Wrd[MaxNumWrd], char *Result[MaxNumWrd][MaxNumAmb], TaggingMethod m, int BestNum)
{
  int ReturnValue;
	
  if (m_koma==NULL) return 0;

  if ((ReturnValue=DoKomaOneSentence(m_koma, Sentence, &m_KomaResult))==0) {
    Result[0][0]=NULL;
    return 0;
  }
  memcpy(Wrd, m_KomaResult.Wrd, sizeof(char*)*ReturnValue);
  Wrd[ReturnValue]=NULL;
  switch (m) {
    case NO_TAGGING: 
      if (ReturnValue>0) {
        memcpy(Result, m_KomaResult.Cnd, sizeof(char *)*ReturnValue*MaxNumAmb);
        Result[ReturnValue][0]=NULL;
        return ReturnValue;
      }
      else {
        Result[0][0]=NULL;
        return 0;
      }
    case PATH_BASED_TAGGING:
      return PathBasedTagging(m_KomaResult.Cnd, Result, BestNum);
    case PATH_BASED_TAGGING_KU:
      return PathBasedTagging(m_KomaResult.Cnd, Result, BestNum);
    case STATE_BASED_TAGGING:
      return StateBasedTagging(m_KomaResult.Cnd, Result, BestNum);
  }

  Result[0][0]=NULL;
  return 0;
}

int CTagger::PathBasedTagging(char *Cnd[MaxNumWrd][MaxNumAmb], char *Tag[MaxNumWrd][MaxNumAmb], int numwin)
{
  int     i, j, k;     /* Loop Control Variable */
  int     NumWrd;      /* 단어의 개수 */
  double  SeqPrb[MaxNumWrd][MaxNumAmb] = {0,};
  int     BackPtr[MaxNumWrd][MaxNumAmb] = {-1,};
  int     BestPath[MaxNumWrd];
  double  PrbWrd, PrbTrn;
  double   CurrPrb;

  if (Cnd[0][0]==NULL) {
    Tag[0][0]=NULL;
    return 0;
  }

  /*---------------------------------------------------*/
  /* 문장 시작 처리 */
  /*---------------------------------------------------*/
  for (j=0; Cnd[0][j]!=NULL; j++) {
	  PrbWrd=m_Rsc.GetPrbWrd(Cnd[0][j]);
	  PrbTrn=m_Rsc.GetPrbTrn(IniCnd, Cnd[0][j]);
	  BackPtr[0][j]=0;
	  SeqPrb[0][j]=PrbTrn*PrbWrd;
  } /* for (j) */
  /*---------------------------------------------------*/

  /* Iteration Step */
  for (i=1; Cnd[i][0]!=NULL; i++) {
    for (j=0; Cnd[i][j]!=NULL; j++) {
	    PrbWrd=m_Rsc.GetPrbWrd(Cnd[i][j]);
  	  /* 이전 Wrd의 첫번째 후보에 대한 처리 */
	    PrbTrn=m_Rsc.GetPrbTrn(Cnd[i-1][0], Cnd[i][j]);
	    CurrPrb=SeqPrb[i-1][0]*PrbTrn*PrbWrd;
  	  /* 초기화 */
	    BackPtr[i][j]=0;
	    SeqPrb[i][j]=CurrPrb;
  	  /* 이전 Wrd의 나머지 후보에 대한 처리 */
	    for (k=1; Cnd[i-1][k]!=NULL; k++) {
        PrbTrn=m_Rsc.GetPrbTrn(Cnd[i-1][k], Cnd[i][j]);
        CurrPrb=SeqPrb[i-1][k]*PrbTrn*PrbWrd;
        if (CurrPrb>SeqPrb[i][j]) {
		      BackPtr[i][j]=k;
		      SeqPrb[i][j]=CurrPrb;
        } /* if */
  	  } /* for (k) */
    } /* for (j) */
  } /* for (i) */

  /*---------------------------------------------------*/
  /* 문장 끝 처리 */
  /*---------------------------------------------------*/
  PrbWrd=m_Rsc.GetPrbWrd(EndCnd);
  /* 이전 Wrd의 첫번째 후보에 대한 처리 */
	PrbTrn=m_Rsc.GetPrbTrn(Cnd[i-1][0], EndCnd);
	CurrPrb=SeqPrb[i-1][0]*PrbTrn*PrbWrd;
	/* 초기화 */
  BackPtr[i][0]=0;
  SeqPrb[i][0]=CurrPrb;
 	/* 이전 Wrd의 나머지 후보에 대한 처리 */
  for (k=1; Cnd[i-1][k]!=NULL; k++) {
    PrbTrn=m_Rsc.GetPrbTrn(Cnd[i-1][k], EndCnd);
    CurrPrb=SeqPrb[i-1][k]*PrbTrn*PrbWrd;
    if (CurrPrb>SeqPrb[i][0]) {
      BackPtr[i][0]=k;
      SeqPrb[i][0]=CurrPrb;
    } /* if */
 	} /* for (k) */
  /*---------------------------------------------------*/

  NumWrd=i;

  /* Sequence Identification Step */
  BestPath[NumWrd]=0;
  for (i=NumWrd-1; i>=0; i--) {
    BestPath[i]=BackPtr[i+1][BestPath[i+1]];
  } /* for (i) */

  /* Make Winner */
  for (i=0; i<NumWrd; i++) {
    Tag[i][0]=Cnd[i][BestPath[i]];
    Tag[i][1]=NULL;
  } /* for (i) */

  Tag[i][0]=NULL;

  return NumWrd;
}

#define delta 0.1

int CTagger::StateBasedTagging(char *Cnd[MaxNumWrd][MaxNumAmb], char *Tag[MaxNumWrd][MaxNumAmb], int numwin)
{
  int    i, j;
  int    NumWrd, NumAnl;
  double PrbSen;
  double PrbAWrd[MaxNumWrd][MaxNumAmb];
  double Alpha[MaxNumWrd][MaxNumAmb];
  double Beta[MaxNumWrd][MaxNumAmb];
  int    MaxIndex;
  double MaxPrb;
  double TempPrb;
  char   *TempTag;

  if (Cnd[0][0]==NULL) {
    Tag[0][0]=NULL;
    return 0;
  }

  /* Alpha를 계산한다. */
  /*---------------------------------------------------*/
  /* 문장 시작 처리 */
  /*---------------------------------------------------*/
  for (NumAnl=0; Cnd[0][NumAnl]!=NULL; NumAnl++) 
    Alpha[0][NumAnl]=m_Rsc.GetPrbTrn(IniCnd,Cnd[0][NumAnl])
                     *m_Rsc.GetPrbWrd(Cnd[0][NumAnl]);
  /*---------------------------------------------------*/

  /* 문장 중간 처리 */
  for (NumWrd=1; Cnd[NumWrd][0]!=NULL; NumWrd++) {	
    for (NumAnl=0; Cnd[NumWrd][NumAnl]!=NULL; NumAnl++) {
      Alpha[NumWrd][NumAnl]=0;
      for (j=0; Cnd[NumWrd-1][j]!=NULL; j++) {
        Alpha[NumWrd][NumAnl]+=Alpha[NumWrd-1][j]
	                             *m_Rsc.GetPrbTrn(Cnd[NumWrd-1][j],Cnd[NumWrd][NumAnl])
	                             *m_Rsc.GetPrbWrd(Cnd[NumWrd][NumAnl]);
      } /* for (j) */
    } /* for (NumAnl) */
  } /* for (NumWrd) */

  /*---------------------------------------------------*/
  /* 문장 끝 처리 */
  /*---------------------------------------------------*/
  Alpha[NumWrd][0]=0;
  for (j=0; Cnd[NumWrd-1][j]!=NULL; j++) 
    Alpha[NumWrd][0]+=Alpha[NumWrd-1][j]
                      *m_Rsc.GetPrbTrn(Cnd[NumWrd-1][j],EndCnd)
	                    *m_Rsc.GetPrbWrd(IniCnd);
  /*---------------------------------------------------*/

  /* 문장 발생 확률을 계산한다. */
  PrbSen=Alpha[NumWrd][0];

  /* Beta를 계산한다. */
  Beta[NumWrd][0]=0;
  for (NumWrd-=1; NumWrd>=0; NumWrd--) {
    for (NumAnl=0; Cnd[NumWrd][NumAnl]!=NULL; NumAnl++) {
      Beta[NumWrd][NumAnl]=0;
      for (j=0; Cnd[NumWrd+1][j]!=NULL; j++) {
        Beta[NumWrd][NumAnl]+=Beta[NumWrd+1][j]
	                            *(double)m_Rsc.GetPrbTrn(Cnd[NumWrd][NumAnl],Cnd[NumWrd+1][j])
	                            *(double)m_Rsc.GetPrbWrd(Cnd[NumWrd+1][j]);
      } /* for (j) */
    } /* for (NumAnl) */
  } /* for (NumWrd) */


  /* 각 단어마다 probability 값을 계산한다. */
  for (NumWrd=0; Cnd[NumWrd][0]!=NULL; NumWrd++) {
    for (NumAnl=0; Cnd[NumWrd][NumAnl]!=NULL; NumAnl++) {
      PrbAWrd[NumWrd][NumAnl]=(Alpha[NumWrd][NumAnl]*Beta[NumWrd][NumAnl])/PrbSen;
    } /* for (NumAnl) */
  } /* for (NumWrd) */

  /* 분석 후보들을 정렬한다. */
  for (NumWrd=0; Cnd[NumWrd][0]!=NULL; NumWrd++) {
    for (i=0; Cnd[NumWrd][i]!=NULL; i++) {
      MaxIndex=i;
      MaxPrb=PrbAWrd[NumWrd][i];
      for (j=i+1; Cnd[NumWrd][j]!=NULL; j++) {
        if (PrbAWrd[NumWrd][j]>MaxPrb) {
	        MaxIndex=j;
	        MaxPrb=PrbAWrd[NumWrd][j];
        } /* if */
      } /* for (j) */

      TempTag=Cnd[NumWrd][i];
      TempPrb=PrbAWrd[NumWrd][i];

      Cnd[NumWrd][i]=Cnd[NumWrd][MaxIndex];
      PrbAWrd[NumWrd][i]=PrbAWrd[NumWrd][MaxIndex];

      Cnd[NumWrd][MaxIndex]=TempTag;
      PrbAWrd[NumWrd][MaxIndex]=TempPrb;
    } /* for (i) */
  } /* for (NumWrd) */

  if (numwin<1) numwin=1;
  for (i=0; i<NumWrd; i++) {
    for (j=0; (j<numwin)&&Cnd[i][j]; j++)
      /* if ((PrbAWrd[NumWrd][0]-PrbAWrd[NumWrd][j])>=delta) break;*/
      Tag[i][j]=Cnd[i][j];
    Tag[i][j]=NULL;
  } /* for (i) */
  Tag[i][0]=NULL;

  return NumWrd;
}



bool CTagger::CheckExist( char *src, char *result )
{
	char *find_str = m_Rsc.FindExistEntry( src );
	if( find_str == NULL )	return false;

	strcpy( result, find_str );
	return true;
}


bool CTagger::CheckPos( char *src, char *result )
{
	char *find_str = m_Rsc.FindPosEntry( src );
	if( find_str == NULL )	return false;

	strcpy( result, find_str );
	return true;
}
