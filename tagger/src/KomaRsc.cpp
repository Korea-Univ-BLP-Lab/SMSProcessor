// KomaRsc.cpp: implementation of the CKomaRsc class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "postype.h"
#include "KomaRsc.h"
#include "hcc98.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

char TagName[NUM_TAG][40];
char TagOutName[NUM_TAG][40];

CKomaRsc::CKomaRsc()
{
  m_bValid=false;
  m_MainEntry=NULL;
  m_MainData=NULL;
  m_UserEntry = NULL;
  m_UserData = NULL;
  m_ConnectionTable=NULL;
}

CKomaRsc::~CKomaRsc()
{
  UnLoad();
}

bool CKomaRsc::IsValid()
{
  return m_bValid;
}

bool CKomaRsc::Load(const char *MainEntryFilename, const char *MainDataFilename, 
					const char *UserEntryFilename, const char *UserDataFilename, 
                    const char *ConnectionTableFilename,
                    const char *TagFilename, const char *TagOutFilename )
{
  FILE *f;
  int i, n;

  if (m_bValid) return true;

  // 메인 사전 로드
  if ((m_MainEntry=LoadFST(MainEntryFilename))==NULL) return false;
  if ((f=fopen(MainDataFilename, "rb"))==NULL) return false;
  n=GetNumberOfEntry(m_MainEntry);
  if ((m_MainData=(unsigned int *)malloc(3*sizeof(unsigned int)*n))==NULL) 
    return false;
  for (i=0; i<n; i++) 
    fscanf(f, "%8x%8x%8x", m_MainData+i*3, m_MainData+i*3+1, m_MainData+i*3+2);
  fclose(f);

  if ((f=fopen(TagFilename, "rb"))==NULL) return false;
  for (i=0; i<NUM_TAG; i++)
    fscanf (f, "%s%*s", TagName[i]);
  fclose(f);

  if ((f=fopen(TagOutFilename, "rb"))==NULL) return false;
  for (i=0; i<NUM_TAG; i++)
    fscanf (f, "%s%*s", TagOutName[i]);
  fclose(f);

  if ((f=fopen(ConnectionTableFilename, "rb"))==NULL) return false;
  if ((m_ConnectionTable=(int *)malloc(NUM_TAG*NUM_TAG*sizeof(int)))==NULL) 
    return false;
/* 품사에 따라 정렬되어 있지 않거나 0인 항목들이 빠져있을 땐 아래 것을 이용하도록 */
  int pt, nt;
  for (pt=0; pt<NUM_TAG; pt++)
    for (nt=0; nt<NUM_TAG; nt++)
      fscanf(f, "%*s%*s%d", m_ConnectionTable+pt*NUM_TAG+nt);
/*  memset(m_ConnectionTable, 0, sizeof(int)*NUM_TAG*NUM_TAG);
  char PrevTag[40], NextTag[40];
  while (fscanf(f, "%s%s%d", PrevTag, NextTag, &n)!=EOF) {
    for (pt=0; pt<NUM_TAG; pt++)
      if (strcmp(TagName[pt], PrevTag)==0) break;
    if (pt==NUM_TAG) return false;
    for (nt=0; nt<NUM_TAG; nt++)
      if (strcmp(TagName[nt], NextTag)==0) break;
    if (nt==NUM_TAG) return false;
    m_ConnectionTable[pt*NUM_TAG+nt]=n;
  } */
  fclose(f); 

	// 사용자 사전 로드

  if ((m_UserEntry=LoadFST(UserEntryFilename))==NULL) return false;
  if ((f=fopen(UserDataFilename, "rb"))==NULL) return false;
  n=GetNumberOfEntry(m_UserEntry);
  if ((m_UserData=(unsigned int *)malloc(3*sizeof(unsigned int)*n))==NULL) 
    return false;
  for (i=0; i<n; i++) 
    fscanf(f, "%8x%8x%8x", m_UserData+i*3, m_UserData+i*3+1, m_UserData+i*3+2);
  fclose(f);

  if ((f=fopen(TagFilename, "rb"))==NULL) return false;
  for (i=0; i<NUM_TAG; i++)
    fscanf (f, "%s%*s", TagName[i]);
  fclose(f);

  if ((f=fopen(TagOutFilename, "rb"))==NULL) return false;
  for (i=0; i<NUM_TAG; i++)
    fscanf (f, "%s%*s", TagOutName[i]);
  fclose(f);

  if ((f=fopen(ConnectionTableFilename, "rb"))==NULL) return false;
  if ((m_ConnectionTable=(int *)malloc(NUM_TAG*NUM_TAG*sizeof(int)))==NULL) 
    return false;
/* 품사에 따라 정렬되어 있지 않거나 0인 항목들이 빠져있을 땐 아래 것을 이용하도록 */
  for (pt=0; pt<NUM_TAG; pt++)
    for (nt=0; nt<NUM_TAG; nt++)
      fscanf(f, "%*s%*s%d", m_ConnectionTable+pt*NUM_TAG+nt);
/*  memset(m_ConnectionTable, 0, sizeof(int)*NUM_TAG*NUM_TAG);
  char PrevTag[40], NextTag[40];
  while (fscanf(f, "%s%s%d", PrevTag, NextTag, &n)!=EOF) {
    for (pt=0; pt<NUM_TAG; pt++)
      if (strcmp(TagName[pt], PrevTag)==0) break;
    if (pt==NUM_TAG) return false;
    for (nt=0; nt<NUM_TAG; nt++)
      if (strcmp(TagName[nt], NextTag)==0) break;
    if (nt==NUM_TAG) return false;
    m_ConnectionTable[pt*NUM_TAG+nt]=n;
  } */
  fclose(f); 

  m_bValid=true;
  return true;
}

bool CKomaRsc::UnLoad()
{
  if (!m_bValid) return false;
  if (m_MainEntry) { free(m_MainEntry); m_MainEntry=NULL; }
  if (m_MainData) { free(m_MainData); m_MainData=NULL; }
  if (m_UserEntry) { free(m_UserEntry); m_UserEntry=NULL; }
  if (m_UserData) { free(m_UserData); m_UserData=NULL; }
  if (m_ConnectionTable) { free(m_ConnectionTable); m_ConnectionTable=NULL; }


  m_bValid=false;
  return true;
}

bool CKomaRsc::ReLoad(const char *MainEntryFilename, const char *MainDataFilename, 
						const char *UserEntryFilename, const char *UserDataFilename, 
                      const char *ConnectionTableFilename,
                      const char *TagFilename, const char *TagOutFilename )
{

  UnLoad();
  return Load(MainEntryFilename, MainDataFilename, UserEntryFilename, UserDataFilename, ConnectionTableFilename, TagFilename, TagOutFilename );
}

bool CKomaRsc::GetPos(const char *Key, unsigned int *Info)
{
  int Index;

  if (!m_bValid) return false;

  if ((Index=String2Hash(m_MainEntry, Key))==NULL_INDEX) 
    memset(Info, 0, sizeof(unsigned int)*3);
  else memcpy(Info, m_MainData+Index*3, sizeof(unsigned int)*3);
  return true;
}

bool CKomaRsc::GetPos(const char *Key, unsigned int *tags, unsigned int *feat)
{
  int Index;

  if (!m_bValid) return false;


  // 사용자 사전 검색
  if ((Index=String2Hash(m_UserEntry, Key))==NULL_INDEX) {
	    // 없으면 메인 사전에서 로드
	  if ((Index=String2Hash(m_MainEntry, Key))==NULL_INDEX) {
		tags[0]=0; tags[1]=0; *feat=0;
	  }
	  else {
		tags[0]=m_MainData[Index*3];
		tags[1]=m_MainData[Index*3+1];
		*feat=m_MainData[Index*3+2];
	  }
  }
  else	 {
	  // 있으면 사용자 사전에서 로드
  		tags[0]=m_UserData[Index*3];
		tags[1]=m_UserData[Index*3+1];
		*feat=m_UserData[Index*3+2];
  }

  return true;
}

bool CKomaRsc::IsConnectable(int PrevTag, int NextTag)
{
  if (!m_bValid) return false;

  return m_ConnectionTable[PrevTag*NUM_TAG+NextTag]!=0;
}
