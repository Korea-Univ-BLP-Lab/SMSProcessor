#ifndef __KOMARSC__
#define __KOMARSC__

#include "FST.h"
#include "nlp.def"
#include "postype.h"

extern char TagName[NUM_TAG][40];
extern char TagOutName[NUM_TAG][40];

typedef enum { CTYPE_STRICT, CTYPE_LOOSE } ConnectionType;

class CKomaRsc  
{
public:
  CKomaRsc();
  virtual ~CKomaRsc();

  bool IsValid();
  bool Load(const char *MainEntryFilename, const char *MainDataFilename, 
				 const char *UserEntryFilename, const char *UserDataFilename, 
            const char *ConnectionTableFilename,
            const char *TagFilename, const char *TagOutFilename );

  bool ReLoad(const char *MainEntryFilename, const char *MainDataFilename,
			  const char *UserEntryFilename, const char *UserDataFilename,
              const char *ConnectionTableFilename,
              const char *TagFilename, const char *TagOutFilename );

  bool UnLoad();

  bool GetPos(const char *Key, unsigned int *Info);
  bool GetPos(const char *Key, unsigned int *tags, unsigned int *feat);

  bool IsConnectable(int PrevTag, int NextTag);


private:
  bool m_bValid;

  FST *m_MainEntry;	// 메인사전
  unsigned int *m_MainData;	 //메인 데이터

  FST *m_UserEntry; // 사용자 사전
  unsigned int *m_UserData; // 사용자 데이터

  int *m_ConnectionTable;
};

#endif
