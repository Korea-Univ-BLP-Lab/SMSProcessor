// Koma.cpp: implementation of the CKoma class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Koma.h"
#include "core.h"
#include "hcc98.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKomaRsc CKoma::m_Rsc;

CKoma::CKoma()
{
  m_HcodeType=1;
  ResetSymbol();
  ResetHangul();
}

CKoma::~CKoma()
{
}

short CKoma::GetNumResult()
{
  return m_Table.topwa;
}

short CKoma::GetPair(short wai, short wcol)
{
  return m_Table.wa[wai].pseq[wcol];
}

const char *CKoma::GetMorph(short pi)
{
  return m_Table.p[pi].morph;
}

const char *CKoma::GetTag(short pi)
{
  return TagOutName[m_Table.p[pi].tag];
}

void CKoma::ResetResult(KomaResultType *Result)
{
  Result->NextToFill=0;
}

/* morphologically analyze a word */
bool CKoma::AnalyzeWord(char *Word)
{
  char tword[XL_WORD];
//  int i=0;

  // 초기화 
  ResetSymbol();

  if (m_HcodeType) {
    ks2tg((unsigned char *)Word, (unsigned char *)tword);
    if (!SplitHangulSymbol(this, tword)) return false;
  }
  else if (!SplitHangulSymbol(this, Word)) return false;

  // 미분석 부분에 대해 형태소 분석

  for (short wi=0; wi<m_Table.topw; wi++) {
    if (m_Table.w[wi].tag == T_UN) {
      if (!AnalyzeHangul(this, wi)) return false;
    } 
    else {
      if (!AnalyzeSymbol(this, wi)) return false;
    }
  }
  if (!ConcateHangulSymbol(this)) return false;

  // 형태소 분석 결과를 변환
  for (short pi=0; pi<m_Table.topp; pi++) {
    if (m_HcodeType) {
      tg2ks((unsigned char *)m_Table.p[pi].morph, (unsigned char *)tword);
      strcpy(m_Table.p[pi].morph, tword);
    }
  }
  
  return true;
}

bool CKoma::AnalyzeWord(char *Word, char Result[MaxNumAmb][BuffSize])
{
  short wtop, wai, wcol, pi, Length=0;

  if ((Word[0]==0)||(m_Rsc.IsValid()==false)) {
    Result[0][0]=0;
    return false;
  }

  if ((AnalyzeWord(Word)==false)||((wtop=GetNumResult())==0)) {
    strcpy(Result[0], "UNK");
    Result[1][0]=0;
  }
  else {
    if (wtop>=MaxNumAmb-1) wtop=MaxNumAmb-1;
    for (wai=0; wai<wtop; wai++) {
      Result[wai][0]=0;
      for (pi=GetPair(wai, wcol=0); pi!=(short)-1; pi=GetPair(wai, ++wcol)) {
        Length+=strlen(GetMorph(pi))+strlen(GetTag(pi))+2;
        if (Length>=BuffSize-1) break;
        sprintf(Result[wai]+strlen(Result[wai]), "%s%s%s%s", GetMorph(pi), MrpTagMark, GetTag(pi), MrpConMark);
      }
      Result[wai][strlen(Result[wai])-1]=0;
    }
    Result[wai][0]=0;
  }
  
  return true;
}

#define AddWordToBuffer(Name, Value) {\
  Length=strlen(Value)+1;\
  if (Length+Result->NextToFill>=MaxKomaBuffer) {\
    Result->Wrd[n]=NULL;\
    Result->Cnd[n][0]=NULL;\
    return n;\
  }\
  Name=Result->Buffer+Result->NextToFill;\
  strcpy(Name, Value);\
  Result->NextToFill+=Length;\
}


#define AddCndToBuffer(CndIndex, Lex, Pos) {\
  Length=strlen(Lex)+strlen(Pos)+2;\
  if (Length+Result->NextToFill>=MaxKomaBuffer) {\
    Result->Wrd[n]=NULL;\
    Result->Cnd[n][0]=NULL;\
    return n;\
  }\
  Result->Cnd[n][CndIndex]=Result->Buffer+Result->NextToFill;\
  sprintf(Result->Cnd[n][CndIndex], "%s%s%s", Lex, MrpTagMark, Pos);\
  Result->NextToFill+=Length;\
}

int CKoma::AnalyzeSentence(char *Sentence, KomaResultType *Result)
{
  char *From=Sentence, *To;
  short wtop, wai, wcol, pi;
  int n, Length;
  int tLen;

  ResetResult(Result);
  n=0;

  while (From&&*From) {
    // 단어 분리
    while (*From&&strchr(" \t\r\n", *From)) 
      From++;
    if (*From==0) break;
    To=From;
    do {
        To++;
    } while (*To&&(strchr(" \t\r\n", *To)==NULL));
    if (*To) *To=0;
    else To=NULL;
   
    AddWordToBuffer(Result->Wrd[n], From); 

/*
    //원호가 고침
    tLen = strlen(From);
    if (tLen >= 103)
    {
	if( From[100] < 0 )	{
		From[100] = '\0';
	}
	else	{
     	  	From[101] = '\0';
	}
        return 0;


    }
    //여기까지
*/

tLen = strlen( From );
if( tLen >= 103 )       {
        int i=0;
        for( ; i<100; i++ )     {
                if( From[i] < 0 )   {
                        i++;    // skip for 2byte char.
                }
        }
        From[i] = '\0';
	return 0;
  }


    // 형태소 분석
    if (AnalyzeWord(From)==false) {
      AddCndToBuffer(0, Result->Wrd[n], "UNK");
      Result->Cnd[n][1]=NULL;
    }
    else {
      wtop=GetNumResult();
      if (wtop==0) {
        AddCndToBuffer(0, Result->Wrd[n], "UNK");
        Result->Cnd[n][1]=NULL;
      }
      else {
        if (wtop>=MaxNumAmb-1) wtop=MaxNumAmb-1;
        for (wai=0; wai<wtop; wai++) {
          Result->Cnd[n][wai]=Result->Buffer+Result->NextToFill;
          /* 형태소 분석한 내용을 기록한다. */
          for (pi=GetPair(wai, wcol=0); pi!=(short)-1; pi=GetPair(wai, ++wcol)) {
            Length=strlen(GetMorph(pi))+strlen(GetTag(pi))+2;
            if (Length+Result->NextToFill>=MaxKomaBuffer) {
              Result->Wrd[n]=NULL;
              Result->Cnd[n][0]=NULL;
              return 0;
            }
            sprintf(Result->Buffer+Result->NextToFill, "%s%s%s%s", GetMorph(pi), MrpTagMark, GetTag(pi), MrpConMark);
            Result->NextToFill+=Length;
          }
          Result->Buffer[Result->NextToFill-1]=0;
        }
        Result->Cnd[n][wai]=NULL;
      }
    }
    n++;
    if (n==MaxNumWrd-1) break;
    if (To) From=To+1;
    else From=NULL;
  }

  Result->Wrd[n]=NULL;
  Result->Cnd[n][0]=NULL;
  return n;
}

void CKoma::ResetSymbol() 
{
  m_Table.topw = 0;
  m_Table.topp = 0;
  m_Table.topa = 0;
  m_Table.topwa = 0;
}

void CKoma::ResetHangul() 
{
  m_Table.toph = 0;
  m_Table.both = 0;
  m_Table.topm = 0;
  m_Table.topk = 0;
  m_Table.topu = 0;
}

bool CKoma::PushWord(char* word, short tag)
{
  /* check overflow */
  if (m_Table.topw>=XN_TW) return false;
  strcpy(m_Table.w[m_Table.topw].word, word);
  m_Table.w[m_Table.topw].tag = tag;
  (m_Table.topw)++;
  return true;
}

bool CKoma::PushHead(char* head, short pred, short next) 
{
  /* check overflow */
  if (((m_Table.toph+1)%XN_TH)==m_Table.both) return false;
  strcpy (m_Table.h[m_Table.toph].head, head);
  m_Table.h[m_Table.toph].pred=pred;
  m_Table.h[m_Table.toph].predtags[0]=m_Table.i.rtags[0];
  m_Table.h[m_Table.toph].predtags[1]=m_Table.i.rtags[1];
  m_Table.h[m_Table.toph].predfeat=m_Table.i.rfeat;
  m_Table.h[m_Table.toph].next=next;
  m_Table.toph=(m_Table.toph+1)%XN_TH;
  return true;
}

bool CKoma::PopHead() 
{
  /* check empty */
  if (m_Table.both==m_Table.toph) return false;
  strcpy(m_Table.i.head, m_Table.h[m_Table.both].head);
  m_Table.i.pred=m_Table.h[m_Table.both].pred;
  m_Table.i.ptags[0]=m_Table.h[m_Table.both].predtags[0];
  m_Table.i.ptags[1]=m_Table.h[m_Table.both].predtags[1];
  m_Table.i.pfeat=m_Table.h[m_Table.both].predfeat;
  m_Table.i.next=m_Table.h[m_Table.both].next;
  m_Table.both=(m_Table.both+1)%XN_TH;
  return true;
}

bool CKoma::PushMorph(char* morph, short next)
{
  /* check overflow */
  if (m_Table.topm>=XN_TM) return false;
  strcpy (m_Table.m[m_Table.topm].morph, morph);
  m_Table.m[m_Table.topm].tags[0]=m_Table.i.mtags[0];
  m_Table.m[m_Table.topm].tags[1]=m_Table.i.mtags[1];
  m_Table.m[m_Table.topm].feat=m_Table.i.mfeat;
  m_Table.m[m_Table.topm].next=next;
  (m_Table.topm)++;
  return true;
}

bool CKoma::PushKnown(short next) 
{
  /* check overflow */
  if (m_Table.topk>=XN_TK) return false;
  m_Table.k[m_Table.topk].mseq[0]=next;
  (m_Table.topk)++;
  return true;
}

bool CKoma::PushUnknown(short next) 
{
  /* check overflow */
  if (m_Table.topu>=XN_TU) return false;
  m_Table.u[m_Table.topu].mseq[0]=next;
  (m_Table.topu)++;
  return true;
}

/* build all sequence with known startable morpheme */
bool CKoma::BuildKnown() 
{
  short ki;		/* known index */
  short col;		/* column of mseq */
  short curr;		/* current morpheme index */
  short next;		/* next morpheme index */
  if (!m_Table.topk) return true;
  for (ki=0; ki<m_Table.topk; ki++) {
    col=0;
    do {
	    curr=m_Table.k[ki].mseq[col]; 
	    col++;
	    next=m_Table.m[curr].next;
	    m_Table.k[ki].mseq[col]=next;
	    if (col>=XL_MSEQ) return false;
    } while (next!=BOTTOM);
  }
  return true;
}

/* build all sequence with unknown startable morpheme */
bool CKoma::BuildUnknown() 
{
  short ui;		/* unknown index */
  short col;		/* column of mseq */
  short curr;		/* current morpheme index */
  short next;		/* next morpheme index */
  if (!m_Table.topu) return true;
  for (ui=0; ui<m_Table.topu; ui++) {
	  col=0;
	  do {
	    curr=m_Table.u[ui].mseq[col]; 
	    col++;
	    next=m_Table.m[curr].next;
	    m_Table.u[ui].mseq[col]=next;
	    if (col>=XL_MSEQ) return false;
	  } while (next!=BOTTOM);
  }
  return true;
}

short CKoma::SearchPair(char* morph, short tag, short feat) 
{
  short i;
  for (i=0; i < m_Table.topp; i++) {
    if (!strcmp (morph, m_Table.p[i].morph) &&
	      tag == m_Table.p[i].tag &&
	      feat == m_Table.p[i].feat) return i+1;
  }
  return 0;
}

short CKoma::PushPair(char* morph, short tag, short feat)
{
  short pi;
  /* search pair table */
  if (pi=SearchPair (morph, tag, feat)) return pi;
  /* check overflow */
  if (m_Table.topp>=XN_TP) return 0;
  strcpy (m_Table.p[m_Table.topp].morph, morph);
  m_Table.p[m_Table.topp].tag = tag;
  m_Table.p[m_Table.topp].feat = feat;
  (m_Table.topp)++;
  return m_Table.topp;
}

bool CKoma::PushAnal(short* pseq) 
{
  short col;		/* column index */
  if (m_Table.topa>=XN_TA) return false;
  col = 0;
  while (col<XL_PSEQ) {
    m_Table.a[m_Table.topa].pseq[col]=pseq[col];
    if (pseq[col++]!=BOTTOM) continue;
    (m_Table.topa)++;
    return true;
  }
  return false;
}

bool CKoma::PushWordAnal(short* pseq) 
{
  short col;		/* column index */
  if (m_Table.topwa>=XN_TWA) return false;
  col = 0;
  while (col<XL_PSEQ) {
    m_Table.wa[m_Table.topwa].pseq[col]=pseq[col];
    if (pseq[col++]!=BOTTOM) continue;
    (m_Table.topwa)++;
    return true;
  }
  return false;
}

void CKoma::SwapWordAnal (short wai, short waj) 
{
  short pseq[XL_PSEQ];
  float prob;
  short col;

  prob=m_Table.wa [wai].prob;
  m_Table.wa[wai].prob=m_Table.wa[waj].prob;
  m_Table.wa[waj].prob=prob;

  col=0;
  do { 
    pseq[col]=m_Table.wa [wai].pseq[col]; 
  } while (pseq[col++]!=BOTTOM);

  col=0;
  do { 
    m_Table.wa[wai].pseq [col]=m_Table.wa[waj].pseq[col];
  } while (m_Table.wa[waj].pseq[col++]!=BOTTOM);

  col=0;
  do { 
    m_Table.wa [waj].pseq[col]=pseq[col];
  } while (pseq[col++]!=BOTTOM);
}
