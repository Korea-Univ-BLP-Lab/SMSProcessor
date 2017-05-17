#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "FST.h"

FST *LoadFST(const char *FSTName)
{				/* FST file을 open */
  FILE *Dic;
  FST *fst;
  int FileSize;

  /* open FST file */
  if ((Dic = fopen(FSTName, "rb")) == NULL)
	  return NULL;

  /* calculate filesize */
  fseek(Dic, 0, 2);		/* SEEK_END */
  FileSize = ftell(Dic);
  fseek(Dic, 0, 0);

  /* memory allocate */
  if ((fst=(FST *)malloc(FileSize))==NULL) {
    fclose(Dic);
    return NULL;
  }

  /* load FST */
  fread(fst, sizeof(FST), FileSize/sizeof(FST), Dic);
  fclose(Dic);

  return fst;
}

/* free memory for FST */
void FreeFST(FST *fst)
{				
    free(fst);
}

/* calculate hash value of the given string from FST */
/* nItem : String에 일치하는 엔트리의 개수 */
/* 반환값 : String에 일치하는 첫번째 엔트리의 Hash Value 또는 NULL_INDEX(검색 실패) */
int String2Hash(FST *fst, const char *String, int *nItem)
{                              
  int CurrentState=0, Result=0;
  unsigned char *p=(unsigned char *)String;
  
  if (nItem) *nItem=0;
  /* transition among states */
  while (*p!=0) {
    /* if there is no transition when *Key is given */
    if ((*p!=fst[CurrentState+*p].CHAR)||(fst[CurrentState+*p].Next==0)||(fst[CurrentState+*p].Next==-1))
      return NULL_INDEX;
    else {
      /* add hash value for given transition */
      Result+=fst[CurrentState+*p].Value;
      /* transit to mathing state */
      CurrentState=fst[CurrentState+*p].Next;
      p++;
    }
  }
  
  /* is current state one of the final state? */
  if (fst[CurrentState+IsFinal].Next==-1) {
    if (nItem) *nItem=fst[CurrentState+IsFinal].CHAR;
    return Result;
  }
  else return NULL_INDEX;
}

int GetLongestMatch(FST *fst, const char *String, int *nItem, const char **Last, int CurrentState, int Result)
{
  int Hash;

  if (String[0]==0) {
    if (fst[CurrentState+IsFinal].Next==-1) {
      if (Last) *Last=String;
      if (nItem) *nItem=fst[CurrentState+IsFinal].CHAR;
      return Result;
    }
    return NULL_INDEX;
  }

  if (String[0]!=' ')
    if ((fst[CurrentState+' '].CHAR==' ')&&(fst[CurrentState+' '].Next>0))
      if (((unsigned char)String[0]==fst[fst[CurrentState+' '].Next+(unsigned char)String[0]].CHAR)&&
          (fst[fst[CurrentState+' '].Next+(unsigned char)String[0]].Next>0))
        if ((Hash=GetLongestMatch(fst, String+1, nItem, Last, fst[fst[CurrentState+' '].Next+(unsigned char)String[0]].Next, 
                                  Result+fst[CurrentState+' '].Value+fst[fst[CurrentState+' '].Next+(unsigned char)String[0]].Value))!=NULL_INDEX) 
          return Hash;
  if (((unsigned char)String[0]==fst[CurrentState+(unsigned char)String[0]].CHAR)&&
      (fst[CurrentState+(unsigned char)String[0]].Next>0))
    if ((Hash=GetLongestMatch(fst, String+1, nItem, Last, fst[CurrentState+(unsigned char)String[0]].Next, Result+fst[CurrentState+(unsigned char)String[0]].Value))!=NULL_INDEX) 
      return Hash;

  if (fst[CurrentState+IsFinal].Next==-1) {
    if (Last) *Last=String;
    if (nItem) *nItem=fst[CurrentState+IsFinal].CHAR;
    return Result;
  }

  return NULL_INDEX;
}

/* calculate hash value of the entry most similare to given string from FST */
/* nItem : String에 가장 유사한 엔트리의 개수 */
/* Last : *Last-String은 인식된 문자열의 길이 */
/* 반환값 : String에 가장 유사한 첫번째 엔트리의 Hash Value 또는 NULL_INDEX(검색 실패) */
int String2MostSimilarHash(FST *fst, const char *String, int *nItem, const char **Last)
{                              

  if (nItem) *nItem=0;
  return GetLongestMatch(fst, String, nItem, Last, 0, 0);
}

/* fst에 들어있는 엔트리의 개수 */
int GetNumberOfEntry(FST *fst)
{
  unsigned int i;
  int Hash=0, CurrentState=0;

  while (1) {
    /* find matching state */
    for (i=MAX_CHAR-1; i>=1; i--)
      if ((i==fst[CurrentState+i].CHAR)&&(fst[CurrentState+i].Next!=-1)) {
        Hash+=fst[CurrentState+i].Value;
        break;
      }
    if (i==0) break;
    CurrentState=fst[CurrentState+i].Next;
  }
  if (fst[CurrentState+IsFinal].Next==-1) return Hash+fst[CurrentState+IsFinal].CHAR;
  else return 0;
}

/* find string with hash value */
/* 0 : invalid hash value */
/* 1 : find string corresponding to the hash value */
char *Hash2String(FST *fst, int HashValue, char *String)
{
  unsigned int i;
  int CurrentState=0;
  unsigned char *p=(unsigned char *)String;

  while (1) {
    /* find matching state */
    *p=0;
    for (i=1; i<MAX_CHAR; i++)
      if ((i==fst[CurrentState+i].CHAR)&&(fst[CurrentState+i].Next!=-1))
        if (fst[CurrentState+i].Value>HashValue) break;
        else *p=i;
    /* not found */
    if (*p==0) return NULL;
    HashValue-=fst[CurrentState+*p].Value;
    CurrentState=fst[CurrentState+*p].Next;
    p++;
    if (((unsigned int)HashValue<fst[CurrentState+IsFinal].CHAR)&&(fst[CurrentState+IsFinal].Next==-1))
      break;
  }

  *p=0;
  return String;
}

int FindAll(FST *fst, unsigned char *p, int CurrentState, int Result, int n, int MaxMatch, int *IndexList, int IncompleteChar)
{
  unsigned int i;

  if (CurrentState==-1) return n;
  if (!IncompleteChar&&(*p==0)) {
    if (fst[CurrentState+IsFinal].Next==-1) 
      if (n<MaxMatch) {
        IndexList[n]=Result;
        n++;
      }
    return n;
  }

  if (!IncompleteChar&&(*p=='*'))
    n=FindAll(fst, p+1, CurrentState, Result, n, MaxMatch, IndexList, 0);

  for (i=1; i<MAX_CHAR; i++)
    if ((*p=='*')||(*p=='?')||(*p==i)) 
      if ((i==fst[CurrentState+i].CHAR)&&fst[CurrentState+i].Next&&(fst[CurrentState+i].Next!=CurrentState))
        if (*p=='*')
          n=FindAll(fst, p, fst[CurrentState+i].Next, Result+fst[CurrentState+i].Value, n, MaxMatch, IndexList, (!IncompleteChar)&&(i>=0x80));
        else {
          if ((*p=='?')&&!IncompleteChar&&(*(p+1)!='?')&&(i>=0x80))
            continue;
          if (IncompleteChar)
            n=FindAll(fst, p+1, fst[CurrentState+i].Next, Result+fst[CurrentState+i].Value, n, MaxMatch, IndexList, 0);
          else n=FindAll(fst, p+1, fst[CurrentState+i].Next, Result+fst[CurrentState+i].Value, n, MaxMatch, IndexList, i>=0x80);
        }

  return n;
}

/* calculate hash values of all matched pattern */
int Pattern2Hash(FST *fst, const char *Pattern, int MaxMatch, int *IndexList)
{				

  return FindAll(fst, (unsigned char *)Pattern, 0, 0, 0, MaxMatch, IndexList, 0);
}

/* calculate hash value located on the path of the given string from FST */
void Path2Hash(FST *fst, const char *String, int *ExistFlag)
{                              
  int CurrentState=0, Result=0, i, n=strlen(String);
  unsigned char *p=(unsigned char *)String;
  
  for (i=0; i<n; i++)
    ExistFlag[i]=NULL_INDEX;

  /* transition among states */
  while (*p!=0) {
    /* if there is no transition when *Key is given */
    if ((*p!=fst[CurrentState+*p].CHAR)||(fst[CurrentState+*p].Next==0)||(fst[CurrentState+*p].Next==-1))
      return;
    else {
      /* add hash value for given transition */
      Result+=fst[CurrentState+*p].Value;
      /* transit to mathing state */
      CurrentState=fst[CurrentState+*p].Next;
      p++;
    }
    if (fst[CurrentState+IsFinal].Next==-1)
      ExistFlag[p-(unsigned char *)String-1]=Result;
  }
}

/* build tabular containing segmentation information for the given string */
void String2Tabular(FST *fst, const char *String, int *Tabular)
{
  int CurrentState, Result, i, j, n=strlen(String);
  unsigned char *p=(unsigned char *)String;
  
  for (i=0; i<n; i++) {
    CurrentState=0;
    p=(unsigned char *)String+i;
    Result=0;
    /* transition among states */
    for (j=0; p[0]; j++, p++) {
      /* if there is no transition when *Key is given */
      if ((*p!=fst[CurrentState+*p].CHAR)||(fst[CurrentState+*p].Next==0)||(fst[CurrentState+*p].Next==-1))
      	break;
      else { /* transit to mathing state */
        Result+=fst[CurrentState+*p].Value;
        CurrentState=fst[CurrentState+*p].Next;
      }
      if (fst[CurrentState+IsFinal].Next==-1)
        CELL(Tabular, n, i, j)=Result;
      else CELL(Tabular, n, i, j)=-1;
    }
  }
}
