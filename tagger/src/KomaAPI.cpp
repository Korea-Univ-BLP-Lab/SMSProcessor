#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Koma.h"
#include "kulog_Koma.h"

int LoadKomaEngine(const char *MainEntryFilename, const char *MainDataFilename,
					const char *UserEntryFilename, const char *UserDataFilename,
                    const char *ConnectionTableFilename,
                    const char *TagFilename, const char *TagOutFilename )
{
// if (time(NULL) > 1172674799) { Koma_LOG_ERR(kulog_tagger, "Error : Expired, http://nlp.korea.ac.kr\n"); return 0; }
  if (!CKoma::m_Rsc.Load(MainEntryFilename, MainDataFilename, UserEntryFilename, UserDataFilename, ConnectionTableFilename, TagFilename, TagOutFilename ))	{
    return 0;
 }
  return 1;
}

void EndKomaEngine(void)
{
// if (time(NULL) > 1172674799) { Koma_LOG_ERR(kulog_tagger, "Error : Expired, http://nlp.korea.ac.kr\n"); return; }
  CKoma::m_Rsc.UnLoad();
}

void *CreateKoma(int CodeType)
{
// if (time(NULL) > 1172674799) { Koma_LOG_ERR(kulog_tagger, "Error : Expired, http://nlp.korea.ac.kr\n"); return NULL; }
  CKoma *x=new CKoma;
  if (x!=NULL) x->m_HcodeType=CodeType;
  return x;
}

void FreeKoma(void *Koma)
{

// if (time(NULL) > 1172674799) { Koma_LOG_ERR(kulog_tagger, "Error : Expired, http://nlp.korea.ac.kr\n"); return; }
  delete ((CKoma *)Koma);
}

int DoKomaOneWord(void *Koma, char *Eojeol, char Result[MaxNumAmb][BuffSize])
{

// if (time(NULL) > 1172674799) { Koma_LOG_ERR(kulog_tagger, "Error : Expired, http://nlp.korea.ac.kr\n"); return 0; }
  if (Koma==NULL) return 0;
  return ((CKoma *)Koma)->AnalyzeWord(Eojeol, Result);
}

int DoKomaOneSentence(void *Koma, char *Sentence, void *Result)
{

// if (time(NULL) > 1172674799) { Koma_LOG_ERR(kulog_tagger, "Error : Expired, http://nlp.korea.ac.kr\n"); return 0; }
  if (Koma==NULL) return 0;
  return ((CKoma *)Koma)->AnalyzeSentence(Sentence, (KomaResultType *)Result);
}

int GetNumResult(void *Koma)
{
// if (time(NULL) > 1172674799) { Koma_LOG_ERR(kulog_tagger, "Error : Expired, http://nlp.korea.ac.kr\n"); return 0; }
  if (Koma==NULL) return 0;
  return ((CKoma *)Koma)->GetNumResult();
}

// wai��° �м� ����� wcol��° ���¼��� �ε��� ��������
int GetPair(void *Koma, short wai, short wcol)
{
  if (Koma==NULL) return 0;
  return ((CKoma *)Koma)->GetPair(wai, wcol);
}

// pi��° �ε����� �ش��ϴ� ���¼��� ���ڿ�
const char *GetMorph(void *Koma, short pi)
{
  if (Koma==NULL) return NULL;
  return ((CKoma *)Koma)->GetMorph(pi);
}
// pi��° �ε����� �ش��ϴ� ���¼��� ǰ��
const char *GetTag(void *Koma, short pi)
{
  if (Koma==NULL) return NULL;
  return ((CKoma *)Koma)->GetTag(pi);
}
