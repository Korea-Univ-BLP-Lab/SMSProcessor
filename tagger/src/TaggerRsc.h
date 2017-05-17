// TaggerRsc.h: interface for the CTaggerRsc class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __TAGGERRSC__
#define __TAGGERRSC__

#include "nlp.def"
#include "FST.h"

#define e 0.000001
#define MaxExistDataLen 128
#define MaxPosDataLen 128

class CTaggerRsc  
{
public:
  CTaggerRsc();
  virtual ~CTaggerRsc();
  bool IsValid();
  bool Load(const char *ProbEntryFilename, const char *ProbDataFilename,
			     const char *PosEntryFilename, const char *PosDataFilename,
	             const char *ExistEntryFilename, const char *ExistDataFilename);

  bool ReLoad(const char *ProbEntryFilename, const char *ProbDataFilename,
			     const char *PosEntryFilename, const char *PosDataFilename,
	             const char *ExistEntryFilename, const char *ExistDataFilename);

  bool UnLoad();
  double GetPrbWrd(char *Anl);
  double GetPrbTrn(char *PrevAnal, char *CurrAnal);
  double GetPrbMrp(char *Key);
  double GetPrbPos2gram(char *Key);

  char *FindExistEntry( char *entry );// 기분석 검색
  char *FindPosEntry( char *entry ); // 제품태그 검색
private:
  bool m_bValid;


  FST *m_Entry;		// 메인 사전
  int m_nEntry;
  double *m_Prob;

 FST *m_ExistEntry; // 기분석 사전용 FST
  int m_nExistEntry;
  char **m_ExistData;

   FST *m_PosEntry; // 제품태그 사전용 FST
  int m_nPosEntry;
  char **m_PosData;


};

#endif 
