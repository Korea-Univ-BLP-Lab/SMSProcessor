// TaggerRsc.cpp: implementation of the CTaggerRsc class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TaggerRsc.h"
#include "nlp.def"
#include "Taganal.h"

#define MaxCurrMrp 2000

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//#define ProbDataSize 15

CTaggerRsc::CTaggerRsc()
{
  m_bValid=false;
  m_Entry=NULL;
  m_Prob=NULL;

  m_ExistEntry=NULL;
  m_ExistData=NULL;
}

CTaggerRsc::~CTaggerRsc()
{
  UnLoad();
}

bool CTaggerRsc::IsValid()
{
  return m_bValid;
}

  bool CTaggerRsc::Load(const char *ProbEntryFilename, const char *ProbDataFilename,
			     const char *PosEntryFilename, const char *PosDataFilename,
	             const char *ExistEntryFilename, const char *ExistDataFilename)
{
  FILE *f;
  int i;

  if (m_bValid) return true;

  if ((m_Entry=LoadFST(ProbEntryFilename))==NULL) return false;
  if ((f=fopen(ProbDataFilename, "rb"))==NULL) return false;
  m_nEntry=GetNumberOfEntry(m_Entry);
  if ((m_Prob=(double *)malloc(sizeof(double)*m_nEntry))==NULL) 
    return false;
  for (i=0; i<m_nEntry; i++) 
    fscanf(f, "%le", m_Prob+i);
  fclose(f);

// ����� �ε�
  if ((m_ExistEntry=LoadFST(ExistEntryFilename))==NULL ) return false;
   m_nExistEntry=GetNumberOfEntry(m_ExistEntry);

	//fprintf( stderr, "%d %s / %s\n", m_nExistEntry, ExistEntryFilename,  ExistDataFilename );
   m_ExistData=(char**)malloc(sizeof(char*)*m_nExistEntry);

// ���� ���� �ε�
  if ((f=fopen(ExistDataFilename, "rb"))==NULL) return false;
  char tmp_data[MaxExistDataLen];
  i=0;
  while( fgets( tmp_data, MaxExistDataLen, f ) )	{
	    size_t len = strlen( tmp_data );
		if( len >= MaxExistDataLen -3 )	{
			break;
		}

		tmp_data[len-1] ='\0';
		len--;

		m_ExistData[i] = (char *)malloc( sizeof(char)*len +1 );		
		strcpy( m_ExistData[i], tmp_data );
		i++;
  }
  fclose(f);

/*
for( i=0; i<m_nExistEntry; i++ )	{
  fprintf( stderr, "[%d %s]\n", i, m_ExistData[i] );
}
*/

	// ��ǰ�±� ���� �ε�
  if ((m_PosEntry=LoadFST(PosEntryFilename))==NULL ) return false;
   m_nPosEntry=GetNumberOfEntry(m_PosEntry);

	//fprintf( stderr, "%d %s / %s\n", m_nPosEntry, PosEntryFilename,  PosDataFilename );
   m_PosData=(char**)malloc(sizeof(char*)*m_nPosEntry);

// ���� ���� �ε�
  if ((f=fopen(PosDataFilename, "rb"))==NULL) return false;
  i=0;
  while( fgets( tmp_data, MaxPosDataLen, f ) )	{
	    size_t len = strlen( tmp_data );
		if( len >= MaxPosDataLen -3 )	{
			break;
		}

		tmp_data[len-1] ='\0';
		len--;

		m_PosData[i] = (char *)malloc( sizeof(char)*len +1 );		
		strcpy( m_PosData[i], tmp_data );
		i++;
  }
  fclose(f);

/*
for( i=0; i<m_nExistEntry; i++ )	{
  fprintf( stderr, "[%d %s]\n", i, m_ExistData[i] );
}
*/



  m_bValid=true;
  return true;

}

bool CTaggerRsc::UnLoad()
{
  if (!m_bValid) return false;

  // ���� ���� ����
  if (m_Entry) free(m_Entry);
  if (m_Prob) free(m_Prob);

  // ��ǰ�±׻��� ����
  if( m_PosEntry ) 
  { 
  	free(m_PosEntry); 
  	m_PosEntry=NULL; 
  }
  if ( m_PosData ) { 
	  for( int i=0; i<m_nPosEntry; i++ )	{
		  free( m_PosData[i] );
	  }
	  free(m_PosData); 

	  m_PosData=NULL; 
  }

  
  // ��м� ���� ����
  if( m_ExistEntry ) 
  { 
  	free(m_ExistEntry); 
  	m_ExistEntry=NULL; 
  }
  if ( m_ExistData ) { 
	  for( int i=0; i<m_nExistEntry; i++ )	{
		  free( m_ExistData[i] );
	  }
	  free(m_ExistData); 

	  m_ExistData=NULL; 
  }



  m_bValid=false;
  return true;
}

bool CTaggerRsc::ReLoad(const char *ProbEntryFilename, const char *ProbDataFilename,
			     const char *PosEntryFilename, const char *PosDataFilename,
	             const char *ExistEntryFilename, const char *ExistDataFilename)
{

  UnLoad();
  return Load(ProbEntryFilename, ProbDataFilename,
	               PosEntryFilename, PosDataFilename,
	               ExistEntryFilename, ExistDataFilename );
}

double CTaggerRsc::GetPrbMrp(char *Key) 
{
  int Index=String2Hash(m_Entry, Key);

  if (Index==NULL_INDEX) 
    return e;
  else return m_Prob[Index];
} /* GetPrbMrp */

double CTaggerRsc::GetPrbPos2gram(char *Key) 
{
  int Index=String2Hash(m_Entry, Key);
  if (Index==NULL_INDEX) 
    return e;
  else return m_Prob[Index];
} /* GetPrbPos2gram */

/*=====================================================*\
  Wrd�� �߻� Ȯ���� ���Ѵ�.
\*=====================================================*/

double CTaggerRsc::GetPrbWrd (char *Anl) 
{
  int NumMrp=0;
  double PrbWrd=0;
  char *Last=NULL;
  char *Res=NULL;
  char Buffer[BuffSize];
  char UnkTag[5];
  int LenStr=0;
  int i=0;

  char CurrMrp[MaxCurrMrp], CurrPos[MaxLenPos], PrevPos[MaxLenPos];
  char MrpPos[MaxCurrMrp+MaxLenPos+1], Pos2gram[MaxLenPos*2+1];

  /* �м� ����� ���� ��� */
  if (Anl==NULL) 
    return e;

  LenStr=strlen(Anl);
  for(i=0;i<4;i++)
    UnkTag[i] = Anl[LenStr-4+i];
  UnkTag[i]=NULL;

  if (strcmp(UnkTag,"/UNK")==0)
  {
      for(i=0;i<4;i++)
          UnkTag[i]=UnkTag[i+1];      
      for(i=0;i<LenStr-4;i++)
          CurrMrp[i] = Anl[i];
      CurrMrp[i] = NULL;      
      
      strcpy(CurrPos, UnkTag);

      sprintf(MrpPos, "%s%s%s", CurrMrp, MrpTagMark, CurrPos);
      if (NumMrp==0) 
	    PrbWrd=GetPrbMrp(MrpPos);
      else {
	    /* ���� ����Ȯ���� ���ϱ� ���� Ű�� �����. */
        sprintf(Pos2gram, "%s%s%s", PrevPos, MrpConMark, CurrPos);
	    PrbWrd*=GetPrbPos2gram(Pos2gram)*GetPrbMrp(MrpPos);
      } /* else ... if */
      strcpy(PrevPos, CurrPos);

      return PrbWrd;
  }
  
  strcpy(Buffer, Anl);
  /* PrbWrd�� ��� */
  for (Res=anltok_r(Buffer, &Last), NumMrp=0; Res!=NULL; Res=anltok_r(NULL, &Last), NumMrp++) {
    /* ������ ���¼Ҹ� �о�´�. */
    if (strlen(Res)>MaxLenMrp-1) return e;
    strcpy(CurrMrp, Res);
    /* ������ ǰ�縦 �о�´�. */
    if ((Res=anltok_r(NULL, &Last))==NULL) return e;
    if (strlen(Res)>MaxLenPos-1) return e;
    strcpy(CurrPos, Res);

    /* ���¼� �߻�Ȯ���� ���ϱ� ���� Ű�� �����. */
    sprintf(MrpPos, "%s%s%s", CurrMrp, MrpTagMark, CurrPos);
    if (NumMrp==0) 
	    PrbWrd=GetPrbMrp(MrpPos);
    else {
	    /* ���� ����Ȯ���� ���ϱ� ���� Ű�� �����. */
      sprintf(Pos2gram, "%s%s%s", PrevPos, MrpConMark, CurrPos);
	    PrbWrd*=GetPrbPos2gram(Pos2gram)*GetPrbMrp(MrpPos);
    } /* else ... if */
    strcpy(PrevPos, CurrPos);
  } /* for (CurrPair) */

  return PrbWrd;
} /* GetPrbWrd */

/*=====================================================*\
  Wrd�� ǰ�翭 ���� Ȯ���� ���Ѵ�.
\*=====================================================*/
double CTaggerRsc::GetPrbTrn (char *PrevAnal, char *CurrAnal) 
{
  char Pos2gram[MaxLenPos*2+1];
  char PrevTail[MaxLenPos];
  char CurrHead[MaxLenPos];

  /* �ܺ� ����Ȯ���� ���ϱ� ���� Ű�� �����. */
  GetPosTail(PrevAnal, PrevTail);
  GetPosHead(CurrAnal, CurrHead);

  sprintf(Pos2gram, "%s%s%s", PrevTail, WrdConMark, CurrHead);

  return GetPrbPos2gram(Pos2gram);
} /* GetPrbTrn */

// ��м������ �ִ��� �������� �˻��Ͽ� ���� ����
// ���ٸ� NULL
char *CTaggerRsc::FindExistEntry( char *entry )
{
	int IndexList[FSTMaxMatch];

	int hash = String2Hash( m_ExistEntry, entry, IndexList );
	if( hash >= 0 )	{
		return m_ExistData[hash];
	}
	else	{
		return NULL;
	}
}

// ��ǰ �±װ� �ִ��� �������� �˻��Ͽ� ���� ����
// ���ٸ� NULL
char *CTaggerRsc::FindPosEntry( char *entry )
{
	int IndexList[FSTMaxMatch];
	int hash = String2Hash( m_PosEntry, entry, IndexList );

	if( hash >= 0 )	{
		return m_PosData[hash];
	}
	else	{
		return NULL;
	}
}

