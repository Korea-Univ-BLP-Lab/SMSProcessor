#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Tagger.h"
#include "HanTagAPI.h"

//---------------------------------------------------------------
#include "kulog_HanTag.h"

//------------------------------------------------------------------------
// 2007.05.17 - (START) - �ڼҿ�����
#define IS_2BYTE(str0, str1) ( (str0 >= 0xA1) && (str0 <= 0xAC) && (str1 >= 0xA1) && (str1 <= 0xFE) )
#define IS_HANGULE(str0, str1) ( (str0 >= 0xB0) && (str0 <= 0xC8) && (str1 >= 0xA1) && (str1 <= 0xFE) )




kulog_handler_t * kulog_tagger;

int HanTag_init( HanTag_config_t * pStHanTagConf, kulog_handler_t * HanTag_log, void ** HanTag )
{
	kulog_tagger = HanTag_log;

	if(!pStHanTagConf->tagger)
		return 0;
		
	if(pStHanTagConf->tagger_path == NULL || *(pStHanTagConf->tagger_path) =='\0')
		strcpy(pStHanTagConf->tagger_path , DEFAULT_TAGGER_PATH );

	char temp1[256];
	char temp2[256];
	char temp3[256];
	char temp4[256];
	char temp5[256];
	char temp6[256];
	char temp7[256];
	char temp8[256];
	char temp9[256];
	char temp10[256];
	char temp11[256];
	char temp13[256];
	char temp14[256];
	
	sprintf(temp1,"%s/main.FST",pStHanTagConf->tagger_path);
	sprintf(temp2,"%s/main.dat",pStHanTagConf->tagger_path);
	sprintf(temp13,"%s/userdic.FST",pStHanTagConf->tagger_path);
	sprintf(temp14,"%s/userdic.dat",pStHanTagConf->tagger_path);
	sprintf(temp3,"%s/connection.txt",pStHanTagConf->tagger_path);
	sprintf(temp4,"%s/tag.nam",pStHanTagConf->tagger_path);
	sprintf(temp5,"%s/tagout.nam",pStHanTagConf->tagger_path);
	sprintf(temp8,"%s/exist.FST",pStHanTagConf->tagger_path);
	sprintf(temp9,"%s/exist.dat",pStHanTagConf->tagger_path);
	sprintf(temp10,"%s/usertag.FST",pStHanTagConf->tagger_path);
	sprintf(temp11,"%s/usertag.dat",pStHanTagConf->tagger_path);

  if (LoadKomaEngine(temp1, temp2, temp13, temp14, temp3, temp4, temp5 )==false) {
    HanTag_LOG_ERR(HanTag_log, "ERROR :: cannot load KOMA engine\n");
    return -1;
  }

	sprintf(temp1,"%s/prob.FST",pStHanTagConf->tagger_path);
	sprintf(temp2,"%s/prob.dat",pStHanTagConf->tagger_path);

if (LoadHanTagEngine(temp1, temp2, temp10, temp11, temp8, temp9 )==false) {
    HanTag_LOG_ERR(HanTag_log, "ERROR :: cannot load HanTag engine\n");
    return -1;
  }

  if ((*HanTag=CreateHanTag())==NULL) {
    HanTag_LOG_ERR(HanTag_log, "ERROR :: cannot make HanTag instance\n");
    return -1;
  }  
  
	return 0;
}

int HanTag_run( HanTag_config_t * pStHanTagConf, void *HanTag, char *Sentence, char *Wrd[MaxNumWrd], char *Result[MaxNumWrd][MaxNumAmb] )
{
	if(!pStHanTagConf->tagger)
		return 0;
	else	
		return DoKomaAndHanTag(HanTag, PATH_BASED_TAGGING, Sentence, Wrd, Result, 'F'); 
}

bool Convert2Usertag ( void *HanTag,  char Result[MaxNumAmb] )
{
	  CTagger *x=(CTagger *)HanTag;
	char result_str[1024];
	result_str[0] = '\0';
	char	buf[1024];
	strcpy( buf, Result );

	for( char *temp, *temp2=strtok_r(buf,"+",&temp); temp2!=NULL; temp2=strtok_r(NULL,"+",&temp) )
	{
		char tmpstr[1024];
		if( x->CheckPos( temp2, tmpstr) == true )	 {
			strcat(result_str, tmpstr );
			strcat(result_str, "+" );
		}
		else	{
			strcat(result_str, temp2);
			strcat(result_str, "+" );
		}
	}
	size_t len = strlen( result_str );
	if( len >= MaxNumAmb )	 {
		return false;
	}
	else	{
		result_str[ strlen(result_str)-1 ] = '\0'; // '+' ����
		strcpy( Result, result_str );
		return true;
	}

}

int HanTag_finish(void * HanTag)
{
	
  FreeHanTag(HanTag);

  EndHanTagEngine();
  EndKomaEngine();

	return 0;
}

//------------------------------------------------------------------------
// 2006.07.11 - (START) - ���� �߰�
int HanTag_ReverseFind(char *pStr, char target)
{
	int i;
	int nLen = strlen(pStr);
	
	for (i = nLen-1; i >= 0; i--)
	{
		if (pStr[i] == target) return i;
	}
	return -1;
}

int HanTag_Find(char *pStr, char target)
{
	int i;
	int nLen = strlen(pStr);
	
	for (i = 0; i < nLen; i++)
	{
		if (pStr[i] == target) return i;
	}
	return -1;
}

// ��� ǰ����� ����� ���� ǰ������� ��ȯ�� �Ѵ�.
int ConvertTagSetFromKU2SEJONG(char *pStr)
{
	int i, nSlashPos, nLen;
	char *pBuf;
	char szLex[MaxLenMrp];
	char szPos[MaxLenPos];
	char szPos_New[MaxLenPos];
	char szSen[BuffSize];
	char szSen_New[BuffSize];
	int nCur = 0;
	
	sprintf(szSen, "%s", pStr);
	
	for (i = 0; 1; i++)
	{
		//if (i == 0) pBuf = strtok(szSen, "+");
		  //else pBuf = strtok(NULL, "+");
		  char *tmp_buff;
		  if (i == 0) pBuf = strtok_r(szSen, "+", &tmp_buff);
		  else pBuf = strtok_r(NULL, "+", &tmp_buff);
		if (pBuf == NULL) break; 

		nLen = strlen(pBuf);
		nSlashPos = HanTag_Find(pBuf, '/');

		if(nSlashPos==-1)
		{
			if (i > 0 || szSen[0]=='+' )
				sprintf(szSen_New+nCur, "+%s", pBuf);
			else
				sprintf(szSen_New+nCur, "%s", pBuf);
			nCur = strlen(szSen_New);
			continue;
		}
		
		memcpy(szLex, pBuf, nSlashPos);		
		szLex[nSlashPos] = '\0';
		memcpy(szPos, pBuf+nSlashPos+1, nLen-nSlashPos-1);
		szPos[nLen-nSlashPos-1] = '\0';

		szPos_New[0] = '\0';
		if ( !strcmp(szPos,"NNCG"))	 		 strcpy(szPos_New,"NNG");
		else if ( !strcmp(szPos,"NNCV")) strcpy(szPos_New,"NNG");
		else if ( !strcmp(szPos,"NNCJ")) strcpy(szPos_New,"XR");
		else if ( !strcmp(szPos,"NNBU")) strcpy(szPos_New,"NNB");
		else if ( !strcmp(szPos,"NPP"))	 strcpy(szPos_New,"NP");
		else if ( !strcmp(szPos,"NPI"))	 strcpy(szPos_New,"NP");
		else if ( !strcmp(szPos,"NU"))	 strcpy(szPos_New,"NR");
		
		else if ( !strcmp(szPos,"XSNN")) strcpy(szPos_New,"XSN");
		else if ( !strcmp(szPos,"XSNN")) strcpy(szPos_New,"XSN");
		else if ( !strcmp(szPos,"XSNP")) strcpy(szPos_New,"XSN");
		else if ( !strcmp(szPos,"XSNU")) strcpy(szPos_New,"XSN");
		else if ( !strcmp(szPos,"XPNN")) strcpy(szPos_New,"XPN");
		else if ( !strcmp(szPos,"XPNU")) strcpy(szPos_New,"XPN");
		
		else if ( !strcmp(szPos,"PS"))	 strcpy(szPos_New,"JKS");
		else if ( !strcmp(szPos,"PC"))	 strcpy(szPos_New,"JKC");
		else if ( !strcmp(szPos,"PO"))	 strcpy(szPos_New,"JKO");
		else if ( !strcmp(szPos,"PD"))	 strcpy(szPos_New,"JKG");
		else if ( !strcmp(szPos,"PA"))	 strcpy(szPos_New,"JKB");
		else if ( !strcmp(szPos,"PV"))	 strcpy(szPos_New,"JKV");
		else if ( !strcmp(szPos,"PN"))	 strcpy(szPos_New,"JC");
		else if ( !strcmp(szPos,"PX"))	 strcpy(szPos_New,"JX");

		else if ( !strcmp(szPos,"DA"))	 strcpy(szPos_New,"MM");
		else if ( !strcmp(szPos,"DI"))	 strcpy(szPos_New,"MM");
		else if ( !strcmp(szPos,"DU"))	 strcpy(szPos_New,"MM");

		else if ( !strcmp(szPos,"XSD"))	 strcpy(szPos_New,"XSN");

		else if ( !strcmp(szPos,"AA"))	 strcpy(szPos_New,"MAG");
		else if ( !strcmp(szPos,"AP"))	 strcpy(szPos_New,"MAG");
		else if ( !strcmp(szPos,"AI"))	 strcpy(szPos_New,"MAG");

		else if ( !strcmp(szPos,"AC"))	 strcpy(szPos_New,"MAJ");
		else if ( !strcmp(szPos,"AV"))	 strcpy(szPos_New,"MAG");
		else if ( !strcmp(szPos,"AJ"))	 strcpy(szPos_New,"MAG");
		else if ( !strcmp(szPos,"XSA"))	 strcpy(szPos_New,"MAG");
		else if ( !strcmp(szPos,"C"))	 	 strcpy(szPos_New,"IC");
		else if ( !strcmp(szPos,"I"))    strcpy(szPos_New,"VCP");
			
		else if ( !strcmp(szPos,"VV"))	 strcpy(szPos_New,"VV");
		else if ( !strcmp(szPos,"VJ"))	 strcpy(szPos_New,"VA");

		else if ( !strcmp(szPos,"XSVV")) strcpy(szPos_New,"XSV");
		else if ( !strcmp(szPos,"XSVJ")) strcpy(szPos_New,"XSA");

		else if ( !strcmp(szPos,"EFF"))	 strcpy(szPos_New,"EF");
		else if ( !strcmp(szPos,"EFC"))	 strcpy(szPos_New,"EC");
		else if ( !strcmp(szPos,"EFN"))	 strcpy(szPos_New,"ETN");
		else if ( !strcmp(szPos,"EFD"))	 strcpy(szPos_New,"ETM");
		else if ( !strcmp(szPos,"EFA"))	 strcpy(szPos_New,"EC");

		else if ( !strcmp(szPos,"NN?"))	 strcpy(szPos_New,"NF");
		else if ( !strcmp(szPos,"V?"))	 strcpy(szPos_New,"NV");

		else if ( !strcmp(szPos,"SS."))	 strcpy(szPos_New,"SF");
		else if ( !strcmp(szPos,"SS?"))	 strcpy(szPos_New,"SF");
		else if ( !strcmp(szPos,"SS!"))	 strcpy(szPos_New,"SF");
		else if ( !strcmp(szPos,"SS,"))	 strcpy(szPos_New,"SP");
		else if ( !strcmp(szPos,"SS/"))	 strcpy(szPos_New,"SP");
		else if ( !strcmp(szPos,"/SS/"))	 strcpy(szPos_New,"SP");
		else if ( !strcmp(szPos,"SS:"))	 strcpy(szPos_New,"SP");
		else if ( !strcmp(szPos,"SS;"))	 strcpy(szPos_New,"SP");
		else if ( !strcmp(szPos,"SS`"))	 strcpy(szPos_New,"SS");
		else if ( !strcmp(szPos,"SS'"))	 strcpy(szPos_New,"SS");
		else if ( !strcmp(szPos,"SS("))	 strcpy(szPos_New,"SS");
		else if ( !strcmp(szPos,"SS)"))	 strcpy(szPos_New,"SS");
		else if ( !strcmp(szPos,"SS-"))	 strcpy(szPos_New,"SS");
		else if ( !strcmp(szPos,"SSA"))	 strcpy(szPos_New,"SE");
		else if ( !strcmp(szPos,"SCF"))	 strcpy(szPos_New,"SL");
		else if ( !strcmp(szPos,"SCH"))	 strcpy(szPos_New,"SH");
		else if ( !strcmp(szPos,"SCD"))	 strcpy(szPos_New,"SN");
		else if ( !strcmp(szPos,"SSX"))	 strcpy(szPos_New,"SW");	
		if (strlen(szPos_New) == 0) sprintf(szPos_New, "%s", szPos);
			
		if (strlen(szLex) == 0 && szPos[0] == '/') sprintf(szLex, "/");
		if (strlen(szLex) == 0 && szPos[0] != '/') sprintf(szLex, "+");
		if (i == 0) sprintf(szSen_New+nCur, "%s/%s", szLex, szPos_New);
			else sprintf(szSen_New+nCur, "+%s/%s", szLex, szPos_New);
		nCur = strlen(szSen_New);
	}	
	sprintf(pStr, "%s", szSen_New);
	return 0;
}
// 2006.07.11 - ( END ) - ���� �߰�

int GetCharFromStream(FILE *f, char *c)
{
  if ((c[0]=fgetc(f))==EOF) {
    c[0]=0;
    return false;
  }
  if ((c[0]<0)&&((c[1]=fgetc(f))!=EOF)) 
    c[2]=0;
  else c[1]=0;
  return true;
}

int GetCharFromString(char **s, char *c)
{
  
  if ((s==NULL)||(*s==NULL)||((*s)[0]==0)) return false;
  if (((*s)[0]>0)||((*s)[1]==0)) {
    c[0]=(*s)[0];
    c[1]=0;
    (*s)++;
  }
  else {
    c[0]=(*s)[0];
    c[1]=(*s)[1];
    c[2]=0;
    (*s)+=2;
  }
  return true;
}

#define SpaceChar " \t\r\n"
#define EOSChar ". ? ! �� �� �� ��"
#define QuoteChar "\" ` ' �� �� �� �� �� �� �� �� ��"
#define NeutralSingleQuoteChar "' �� ��"
#define OpenSingleQuoteChar "` �� ��"
#define CloseSingleQuoteChar "��"
#define NeutralDoubleQuoteChar "\" �� ��"
#define OpenDoubleQuoteChar "��"
#define CloseDoubleQuoteChar "��"

typedef enum { 
  LOAD_INIT, LOAD_CONTENT, LOAD_SPACE, LOAD_EOS, LOAD_QUOTE
} LoadStateType;

typedef enum {
  RSC_FILE, RSC_PSTR
} RSCType;

int LoadSentence(RSCType t, void *RSC, char *Sentence, int MaxLength)
{
//if (time(NULL) > 1172674799) { HanTag_LOG_ERR(kulog_tagger, "Error : Expired, http://nlp.korea.ac.kr\n"); return 0; }
  int i=0;
  int bSingleOpen=true, bDoubleOpen=true;
  char c[3]={0,};
  LoadStateType State=LOAD_INIT, LastState=LOAD_INIT;

  Sentence[0]=0;
  while (((t==RSC_FILE)?GetCharFromStream((FILE *)RSC, c):GetCharFromString((char **)RSC, c))) {
    if (strstr(SpaceChar, c)) {
      c[0]=' '; c[1]=0;
      if ((State==LOAD_INIT)||(State==LOAD_SPACE)) continue;
      if ((i>2)&&(State==LOAD_EOS)) break;
      if ((i>3)&&(State==LOAD_QUOTE)&&(LastState==LOAD_EOS)) {
        if (strcmp(Sentence+i-2, "��")==0)
          strcpy(Sentence+i-2, "��");
        else if (strcmp(Sentence+i-2, "��")==0)
          strcpy(Sentence+i-2, "��");
        break;
      }
      LastState=State;
      State=LOAD_SPACE;
    }
    else if (strstr(EOSChar, c)) {
      LastState=State;
      State=LOAD_EOS;
    }
    else if (strstr(NeutralSingleQuoteChar, c)) {
      LastState=State;
      State=LOAD_QUOTE;
      if (bSingleOpen)
        strcpy(c, "'");
      else strcpy(c, "'");
      bSingleOpen=!bSingleOpen;
    }
    /*else if (strstr(OpenSingleQuoteChar, c)) {
      LastState=State;
      State=LOAD_QUOTE;
      strcpy(c, "��");
      bSingleOpen=false;
    }
    else if (strstr(CloseSingleQuoteChar, c)) {
      LastState=State;
      State=LOAD_QUOTE;
      strcpy(c, "��");
      bSingleOpen=true;
    }*/
    else if (strstr(NeutralDoubleQuoteChar, c)) {
      LastState=State;
      State=LOAD_QUOTE;
      if (bDoubleOpen)
        strcpy(c, "\"");
      else strcpy(c, "\"");
      bDoubleOpen=!bDoubleOpen;
    }
    else if (strstr(OpenDoubleQuoteChar, c)) {
      LastState=State;
      State=LOAD_QUOTE;
      strcpy(c, "\"");
      bDoubleOpen=false;
    }
    else if (strstr(CloseDoubleQuoteChar, c)) {
      LastState=State;
      State=LOAD_QUOTE;
      strcpy(c, "\"");
      bDoubleOpen=true;
    }
    else {
      LastState=State;
      State=LOAD_CONTENT;
    }

    strcpy(Sentence+i, c);
    i+=strlen(c);
    if (i+3>=MaxLength) break;
  }

  return i>0;  
}

int LoadSentenceFromStream(FILE *f, char *Sentence, int MaxLength)
{
//if (time(NULL) > 1172674799) { HanTag_LOG_ERR(kulog_tagger, "Error : Expired, http://nlp.korea.ac.kr\n"); return 0; }
  return LoadSentence(RSC_FILE, f, Sentence, MaxLength);
}

int LoadSentenceFromString(char **pString, char *Sentence, int MaxLength)
{

//if (time(NULL) > 1172674799) { HanTag_LOG_ERR(kulog_tagger, "Error : Expired, http://nlp.korea.ac.kr\n"); return 0; }
  return LoadSentence(RSC_PSTR, pString, Sentence, MaxLength);
}

// HanTag ������ ���� ���� �� ���� �б�
int LoadHanTagEngine(const char *ProbEntryFilename, const char *ProbDataFilename, 
								const char *PosEntryFilename, const char *PosDataFilename, 
								const char *ExistEntryFilename, const char *ExistDataFilename)
{
  
//if (time(NULL) > 1172674799) { HanTag_LOG_ERR(kulog_tagger, "Error : Expired, http://nlp.korea.ac.kr\n"); return 0; }
  if (!CTagger::m_Rsc.Load(ProbEntryFilename, ProbDataFilename, PosEntryFilename, PosDataFilename, ExistEntryFilename, ExistDataFilename ))
    return 0;
  return 1;
}

// ���� �� ������ ���� �޸� ����
void EndHanTagEngine(void)
{
//if (time(NULL) > 1172674799) { HanTag_LOG_ERR(kulog_tagger, "Error : Expired, http://nlp.korea.ac.kr\n"); return; }
  CTagger::m_Rsc.UnLoad();
}

// HanTag class�� instance�� �Ҵ� �ޱ�
void *CreateHanTag(void)
{
//if (time(NULL) > 1172674799) { HanTag_LOG_ERR(kulog_tagger, "Error : Expired, http://nlp.korea.ac.kr\n"); return 0; }
  CTagger *x=new CTagger;
  return x;
}

// HanTag class�� instance�� ����
void FreeHanTag(void *HanTag)
{
//if (time(NULL) > 1172674799) { HanTag_LOG_ERR(kulog_tagger, "Error : Expired, http://nlp.korea.ac.kr\n"); return; }
  delete ((CTagger *)HanTag);
}

// ������ : �߰�
// Result[nIndex][] ���Ұ� ������ ���κ������� �Ǵ�
// ������ �� �Ǵ���, �迭�� ���̰ų�, ������ ���������� ��ȣ�� ������ ��쿡�� ���� ������ �ν��Ѵ�.
// return val : -1 : ����, 0 : ���峡�� �ƴ�, 1: ���峡��
static int isEndofSentense(char *Result[MaxNumWrd][MaxNumAmb], int nResult, int nIndex){
	if(nIndex>=nResult)
		return -1;
	if(Result[nIndex]==NULL)
		return -1;
	
	if(nIndex==nResult-1)
		return 1;
	
	char *temp;

	if( (nIndex>=0) && (nIndex<nResult-1) ){
		for(int j=nIndex+1; j<nResult; j++){
			if((temp=strstr(Result[j][0],"/S"))==NULL)
				return 0;
		}
	}
	return 1;
}
// �̹� �Ҵ� ���� HanTag�� instance�� �̿��Ͽ� ���� ���¼� �м� + �±��� ����
int DoKomaAndHanTag(void *HanTag, TaggingMethod Method, char *Sentence, char *Wrd[MaxNumWrd], char *Result[MaxNumWrd][MaxNumAmb], char bSEJONG/*='T'*/)
{
	int i, j, nResult;
//if (time(NULL) > 1172674799) { HanTag_LOG_ERR(kulog_tagger, "Error : Expired, http://nlp.korea.ac.kr\n"); return 0; }
  CTagger *x=(CTagger *)HanTag;
  char *temp=NULL, buf[10000]; memset(buf,'\0',10000);

  if (x==NULL) {
    Wrd[0]=NULL;
    Result[0][0]=NULL;
    return 0;
	}

  HanTag_LOG_SVC(kulog_tagger, "Sentence : %s",Sentence);
  if ( Sentence[strlen(Sentence)-1]!='\n' ) { strcat(Sentence,"\n"); }

  // 2006.09.18 - �ڼҿ� �߰�
  // (START) - ��ħǥ ������ ���峡�� ��ħǥ�� �߰�
  if ( (Method==PATH_BASED_TAGGING)&&(Sentence[strlen(Sentence)-1]=='\n')&&(Sentence[strlen(Sentence)-2]!='.')&&(Sentence[strlen(Sentence)-2]!=' ')&&(Sentence[strlen(Sentence)-2]!='+') ) 
  {
    HanTag_LOG_DBG(kulog_tagger, "end 1st character:%d\t2nd character:%d\n",Sentence[strlen(Sentence)-1],Sentence[strlen(Sentence)-2] );

    strncpy(buf,Sentence,strlen(Sentence)-1);
    strcat(buf,".\n");
    

    nResult = x->DoTagging(buf, Wrd, Result, Method, 1); 
    if ( nResult == 0 )
    {
       HanTag_LOG_SVC( kulog_tagger, "fail to analyze the sentence %s", Sentence );
       return -1;
    }

    for ( i=0, j=0; (i<MaxNumWrd)&&(Wrd[i]!=NULL)&&(Result[i][0]!=NULL); i++ ); i--;
    Wrd[i][strlen(Wrd[i])-1]='\0';
	j=strlen(Result[i][0]);
	if ( (j>6)&&!strcmp(&Result[i][0][j-6],"+./SS.") )
	{
	  if ( (j>9)&&!strcmp(&Result[i][0][j-9],"EFC+./SS.") ) { Result[i][0][j-7]='F'; }
      Result[i][0][j-6]='\0';
      Result[i][0][j-5]='\0';
      Result[i][0][j-4]='\0';
      Result[i][0][j-3]='\0';
      Result[i][0][j-2]='\0';
      Result[i][0][j-1]='\0';
	}
	else if ( (j>5)&&!strcmp(&Result[i][0][j-5],"./SS.") )
	{
      Result[i][0][j-5]='\0';
      Result[i][0][j-4]='\0';
      Result[i][0][j-3]='\0';
      Result[i][0][j-2]='\0';
      Result[i][0][j-1]='\0';
	}
  }
  else 
  { 
    nResult = x->DoTagging(Sentence, Wrd, Result, Method, 1); 
    if ( nResult == 0 )
    {
       HanTag_LOG_SVC( kulog_tagger, "fail to analyze the sentence (%s)", buf );
       return -1;
    }
  }
  // (END) - ��ħǥ ������ ���峡�� ��ħǥ�� �߰�

////////////////////////////////////////////////////////
////////////////////�±��� ��ó�� ���//////////////////
////////////////////////////////////////////////////////
  // 2006.09.18 - �ڼҿ� �߰�
  // (START) - ���� �� EFC => EFF & ��=>��/NP+��/JX
  for ( i=0; (i<nResult)&&(Wrd[i]!=NULL); i++ )
  {
	//  if ( !strcmp(Wrd[i],"��") ) { strcpy(Result[i][0],"��/NPP+��/PX"); }
	//  else if ( !strcmp(Wrd[i],"��") ) { strcpy(Result[i][0],"��/NPP"); }

	  //��������� ���� ����
	  if( x->CheckExist( Wrd[i], Result[i][0] ) )	
	  {
		  // Result�� �̹� �����
		 // fprintf( stderr, "\nres2:%s\n", Result[i][0] );
	  }
	  else if ( (temp=strstr(Result[i][0],"/EFC+?"))!=NULL ) {  temp[3]='F'; }
	  else if ( (temp=strstr(Result[i][0],"/EFC+��"))!=NULL ) {  temp[3]='F'; }
	  else if ( (i>0)&&(strstr(Result[i-1][0],"/VV")==NULL)&&(strstr(Result[i-1][0],"/VJ")==NULL)&&(strstr(Result[i-1][0],"/VX")==NULL)&&(strstr(Result[i-1][0],"/XSVV")==NULL)&&(strstr(Result[i-1][0],"/XSVJ")==NULL)&&(strstr(Result[i][0],"/VV")==NULL)&&(strstr(Result[i][0],"/VJ")==NULL)&&(strstr(Result[i][0],"/XSVV")==NULL)&&(strstr(Result[i][0],"/XSVJ")==NULL)&&(strstr(Result[i][0],"/VX")==NULL)&&((temp=strstr(Result[i][0],"/VX"))!=NULL) ) { temp[2]='V'; }
	  else if ( (i<nResult-1) && ((temp=strstr(Result[i][0],"/EFF"))!=NULL)&&(Result[i+1][0]!=NULL)&&(strstr(Result[i+1][0],"/EFF")!=NULL) ) { temp[3]='C'; }

		j=strlen(Result[i][0]);
		if ( !strcmp(&Result[i][0][j-5],"��/UNK") ) { strcpy(&Result[i][0][j-5],"/UNK+��/PX"); continue; }
		else if ( !strcmp(&Result[i][0][j-5],"��/UNK") ) { strcpy(&Result[i][0][j-5],"/UNK+��/PX"); continue; }
		else if ( !strcmp(&Result[i][0][j-5],"��/UNK") ) { strcpy(&Result[i][0][j-5],"/UNK+��/PX"); continue; }
		else if ( !strcmp(&Result[i][0][j-5],"��/UNK") ) { strcpy(&Result[i][0][j-5],"/UNK+��/PX"); continue; }
		else if ( !strcmp(&Result[i][0][j-5],"��/UNK") ) { strcpy(&Result[i][0][j-5],"/UNK+��/PS"); continue; }
		else if ( !strcmp(&Result[i][0][j-5],"��/UNK") ) { strcpy(&Result[i][0][j-5],"/UNK+��/PS"); continue; }
		else if ( !strcmp(&Result[i][0][j-5],"��/UNK") ) { strcpy(&Result[i][0][j-5],"/UNK+��/PO"); continue; }
		else if ( !strcmp(&Result[i][0][j-5],"��/UNK") ) { strcpy(&Result[i][0][j-5],"/UNK+��/PO"); continue; }
		else if ( !strcmp(&Result[i][0][j-5],"��/UNK") ) { strcpy(&Result[i][0][j-5],"/UNK+��/PA"); continue; }
		else if ( !strcmp(&Result[i][0][j-7],"����/UNK") ) { strcpy(&Result[i][0][j-7],"/UNK+����/PA"); continue; }
		else if ( !strcmp(&Result[i][0][j-7],"����/UNK") ) { strcpy(&Result[i][0][j-7],"/UNK+����/PA"); continue; }
		else if ( !strcmp(&Result[i][0][j-7],"����/UNK") ) { strcpy(&Result[i][0][j-7],"/UNK+����/PA"); continue; }
		else if ( !strcmp(&Result[i][0][j-7],"����/UNK") ) { strcpy(&Result[i][0][j-7],"/UNK+����/PA"); continue; }
		else if ( !strcmp(&Result[i][0][j-7],"����/UNK") ) { strcpy(&Result[i][0][j-7],"/UNK+����/PA"); continue; }
		else if ( !strcmp(&Result[i][0][j-7],"����/UNK") ) { strcpy(&Result[i][0][j-7],"/UNK+����/PA"); continue; }

		// �ѱ��� �ƴ� ���ڷθ� �̷���� ������ NNG�±׷� ���� ����
		// int flag_num=0, flag_eng=0;
		// for ( j=0; (Wrd[i][j]!='\0')&&((Wrd[i][j]>='0')&&(Wrd[i][j]<='9'))	; j++ );
		// if ( (j>0)&&(Wrd[i][j]=='\0') ) { flag_num=1; }
		// for ( j=0; (Wrd[i][j]!='\0')&&(((Wrd[i][j]>='a')&&(Wrd[i][j]<='z'))||((Wrd[i][j]>='A')&&(Wrd[i][j]<='Z')))	; j++ ); 
		// if ( (j>0)&&(Wrd[i][j]=='\0') ) { flag_eng=1; }
		// for ( j=0; (Wrd[i][j]!='\0')&&(((Wrd[i][j]>='0')&&(Wrd[i][j]<='9'))||((Wrd[i][j]>='a')&&(Wrd[i][j]<='z'))||((Wrd[i][j]>='A')&&(Wrd[i][j]<='Z')))	; j++ ); 
		// if ((j>0)&&(Wrd[i][j]=='\0')&&(flag_num!=1)&&(flag_eng!=1) ) { strcpy(Result[i][0],Wrd[i]); strcat(Result[i][0],"/NNCG"); continue; }

		char tempStr[256]="";
		char *tempStrP1;
		char *tempStrP2;
		
		tempStrP1 = Result[i][0];
		while ( ( tempStrP1 = strstr(tempStrP1,"/SCF+"))!=NULL)  // ���ĺ�����,
		{
			if((tempStrP2=strchr(tempStrP1+5,'/'))!=NULL	// ���� �±װ� �ְ�,
			&&	strncmp(tempStrP2+1,"SCD",3)==0)	//  �� �±װ� ���ڸ�,
			{
				*tempStrP1='\0';
				*tempStrP2='\0';
				strcpy(tempStr, Result[i][0]);
				strcat(tempStr, tempStrP1+5);
				strcat(tempStr, "/NNCG" );
				strcat(tempStr, tempStrP2+4);
				strcpy(Result[i][0] , tempStr);
			}
			tempStrP1++;
		}

		tempStrP1 = Result[i][0];
		while ( ( tempStrP1 = strstr(tempStrP1,"/SCD+"))!=NULL)	// ���ڸ���,
		{
			if ((tempStrP2=strchr(tempStrP1+5,'/'))!=NULL	// ���� �±װ� �ְ�,
			&&	strncmp(tempStrP2+1,"SCF",3)==0)	//  �� �±װ� ���ĺ��̸�,
			{
				*tempStrP1='\0';
				*tempStrP2='\0';
				strcpy(tempStr, Result[i][0]);
				strcat(tempStr, tempStrP1+5);
				strcat(tempStr, "/NNCG" );
				strcat(tempStr, tempStrP2+4);
				strcpy(Result[i][0] , tempStr);
			}
			tempStrP1++;
		}


	  if ( (i<nResult-1) && !strcmp(Wrd[i],"��") && !strcmp(Wrd[i+1],"��") )
	  {
			strcpy(Result[i][0],"��/VV+��/EFC");
			strcpy(Result[i+1][0],"��/VX+��/EFF"); 
			i++;
	  }

/*
		// ��ǰ�±� ���� �˻� (�۵��ϴ����� �׽�Ʈ�ϰ� �ٷ� ���� ��!!!)
		char result_str[1024];
		result_str[0] = '\0';
		char	buf[1024];
		strcpy( buf, Result[i][0] );
		for( char *temp2=strtok_r(buf,"+",&temp); temp2!=NULL; temp2=strtok_r(NULL,"+",&temp) )
		{
			fprintf(stderr,"Result1: %s    temp2:%s\n", Result[i][0], temp2 );
			char tmpstr[1024];
			if( x->CheckPos( temp2, tmpstr) == true )	 {
				//strncpy(temp2,tmpstr
				fprintf(stderr,"buf: %s, Result[i][0]:%s\n",buf, Result[i][0]);
				strcat(result_str, tmpstr );
				strcat(result_str, "+" );
			}
			else	{
				strcat(result_str, temp2);
				strcat(result_str, "+" );
			}
		}
		result_str[ strlen(result_str)-1 ] = '\0';
		fprintf( stderr, "final: %s\n", result_str );
*/

	  //if( (!strcmp(Wrd[i], "��")) && (isEndofSentense(Result, nResult, i)==1) ){
	  //  strcpy(Result[i][0], "��/VV+��/EFF");
	 // }
  }
  // (END) - ���� �� EFC => EFF & ��=>��/NP+��/JX

/////////////////////////////////////////////////
////////////////////�±׼� ����//////////////////
/////////////////////////////////////////////////
    // 2006.07.11 - ���� �߰�
	// (START) - ���� �� ����� ���� ���� ���������� ó���� ��� ������, ���������� ��ȯ�� ���ش�.
	if (bSEJONG == 'T')
	{	
		for (i=0; Result[i][0]; i++)
		{                                                                                                             			
			for (j=0; Result[i][j]; j++) 
			{    
				strcpy(x->m_KomaResult.CndString[i][j], Result[i][j]);
				Result[i][j] = x->m_KomaResult.CndString[i][j];
			}                                            
			x->m_KomaResult.CndString[i][j][0] = '\0';	                                                               
		}   
		for (i=0; Result[i][0]; i++)
		{                                                                                                             
			for (int j=0; Result[i][j]; j++) 
			{     
				ConvertTagSetFromKU2SEJONG(Result[i][j]);
			}                                                                                                           
		}                                                                                                             
	}
	// ( END ) - ���� �� ����� ���� ���� ���������� ó���� ��� ������, ���������� ��ȯ�� ���ش�.

	HanTag_LOG_SVC(kulog_tagger, "Result of  %s",Sentence);
	for(i=0;i<nResult;i++)	HanTag_LOG_SVC(kulog_tagger, "\t\t%s\t%s\n",Wrd[i],Result[i][0]);

  return nResult;
}


// �̹� �Ҵ� ���� HanTag�� instance�� �̿��Ͽ� �� ������ ���¼� �м� ����
int DoHanTag(void *HanTag, TaggingMethod Method, char *Cnd[MaxNumWrd][MaxNumAmb], char *Result[MaxNumWrd][MaxNumAmb])
{
//if (time(NULL) > 1172674799) { HanTag_LOG_ERR(kulog_tagger, "Error : Expired, http://nlp.korea.ac.kr\n"); return 0; }
  CTagger *x=(CTagger *)HanTag;

  if (x==NULL) return 0;
  switch (Method) {
    case NO_TAGGING: 
      int ReturnValue;
      for (ReturnValue=0; Cnd[ReturnValue][0]; ReturnValue++);
      memcpy(Result, Cnd, sizeof(char *)*ReturnValue*MaxNumAmb);
      return ReturnValue;
    case PATH_BASED_TAGGING:
      return x->PathBasedTagging(Cnd, Result, 1);
    case PATH_BASED_TAGGING_KU:
      return x->PathBasedTagging(Cnd, Result, 1);
    case STATE_BASED_TAGGING:
      return x->StateBasedTagging(Cnd, Result, 1);
  }

  Result[0][0]=NULL;
  return 0;
}
