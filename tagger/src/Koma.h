// Koma.h: interface for the CKoma class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __KOMA__
#define __KOMA__

#include "KomaResult.h"
#include "KomaRsc.h"
#include "ph.h"
#include "nlp.def"

// Log 자료구조 정의 --------------------------------------------------------------------
#include "kulog.h"

#define Koma_LOG_ERR(slh, _fmt, _vaarg...) \
  KULOG_ERR(slh, "[Koma] " "%s,%d:: " _fmt, __FILE__ , __LINE__ , ##_vaarg)
#define Koma_LOG_SVC(slh, _fmt, _vaarg...) \
  KULOG_SVC(slh, "[Koma] " _fmt, ##_vaarg)
#define Koma_LOG_DBG(slh, _fmt, _vaarg...) \
  KULOG_DBG(slh, "[Koma] " "%s,%d:: " _fmt, __FILE__ , __LINE__ , ##_vaarg)
  
extern kulog_handler_t * kulog;

//---------------------------------------------------------------------------------

#define BOTTOM	    ((short)-1)
#define XL_WORD	    200	// swlee modified
#define XL_MSEQ	    30
#define XL_PSEQ	    50
#define XN_TW	    30
#define XN_TH	    100
#define XN_TM	    500
#define XN_TK	    100
#define XN_TU	    100
#define XN_TP	    200
#define XN_TA	    300
#define XN_TWA	    1000

/* table for korean morphological analysis */
typedef struct sTABLE 
{
  /* for recognizing hangul or symbol word */
  short topw;
  struct sWORD {
    char word[XL_WORD];
    short tag;
    short bi;		/* bottom index */
    short ti;		/* top index */
  } w[XN_TW];
  /* for analyzing head */
  short toph;
  short both;
  struct sHEAD {
    char head[XL_WORD];
    short pred;
    unsigned int predtags[2];
    unsigned int predfeat;
    short next;		/* next morpheme index */
  } h[XN_TH];
  /* for storing morphemes */
  short topm;
  struct sMORPH {
    char morph[XL_WORD];
    unsigned int tags[2];
    unsigned int feat;
    short next;		/* next morpheme index */
  } m[XN_TM];
  /* for storing known startable morphemes */
  short topk;
  struct sSTART {
    short mseq[XL_MSEQ];
  } k[XN_TK];
  /* for storing unknown morphemes */
  short topu;
  struct sSTART u[XN_TU];
  /* for storing morpheme-tag pair */
  short topp;
  struct sPAIR {
    char morph[XL_WORD];
    short tag;
    short feat;
  } p[XN_TP];
  /* for storing analysis result of part-word */
  short topa;
  struct sANAL {
    short pseq[XL_PSEQ];
    float prob;
  } a[XN_TA];
  /* for storing analysis result of word */
  short topwa;
  struct sANAL wa[XN_TWA];
  /* information for analyzing head */
  struct sINFO {
    char head [XL_WORD];	    /* head to analyze */
    short pred;		    /* predicted state of head */
    short next;		    /* morpheme index of head */
    unsigned int ptags[2];	    /* predicted tags of head */
    unsigned int pfeat;		    /* predicted feature of head */
    char real [XL_WORD];	    /* real part of head */
    unsigned int rtags[2];	    /* tags of real part */
    unsigned int rfeat;		    /* feature of real part */
    char form [XL_WORD];	    /* formal part of head */
    unsigned int ftags[2];	    /* tags of formal part */
    unsigned int ffeat;		    /* feature of formal part */
    short hlen;		    /* length of head */
    short rlen;		    /* length of real */
    short flen;		    /* length of formal */
    char morph [XL_WORD];	    /* recognized morpheme */
    unsigned int mtags[2];	    /* tags of morpheme */
    unsigned int mfeat;		    /* feature of morpheme */
    short wi;		    /* current word index */
  } i;
  /* control informations for analyzing word */
} tTABLE;

class CKoma  
{
public:
  CKoma();
  virtual ~CKoma();

  // 사전 및 정보 파일(모든 instance들이 공동으로 이용)
  static CKomaRsc m_Rsc;
  
  // 입력 code
  short m_HcodeType; /* 1: KS5601 code, 0: TRIGEM code */

  // 형태소 분석 결과
  bool AnalyzeWord(char *Word);
  bool AnalyzeWord(char *Word, char Result[MaxNumAmb][BuffSize]);
  int AnalyzeSentence(char *Sentence, KomaResultType *Result);

  // 문장 단위 형태소 분석 결과 관련
  void ResetResult(KomaResultType *Result);

  // Table 관련 
  tTABLE m_Table;
  tPH m_ppp, m_pp, m_cp, m_np, m_fp;
  void ResetSymbol();
  void ResetHangul();
  bool PushWord(char* word, short tag);
  bool PushHead(char* head, short pred, short next);
  bool PopHead();
  bool PushMorph(char* morph, short next);
  bool PushKnown(short next);
  bool PushUnknown(short next);
  bool BuildKnown();
  bool BuildUnknown();
  short SearchPair(char* morph, short tag, short feat);
  short PushPair(char* morph, short tag, short feat);
  bool PushAnal(short* pseq);
  bool PushWordAnal(short* pseq);
  void SwapWordAnal(short wai, short waj);

  // 분석 과정에서 사용하는 정보
  int m_isUnknown;
  int m_isHaOmit;

  // 형태소 분석 결과 접근용 함수
  short GetNumResult();
  short GetPair(short wai, short wcol);
  const char *GetMorph(short pi);
  const char *GetTag(short pi);

private:
};

#endif
