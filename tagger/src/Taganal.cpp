#include <stdio.h>
#include <string.h>
#include "Taganal.h"
#include "nlp.def"

int chkanl(char *Anl) 
{
  int NumMrp=0;
  int i;

  enum {T, M} state=M;   /* Tag or Morpheme */
  if (!Anl) return -1;
  for (i=0; Anl[i]!='\0'; i++) {
    if (Anl[i]==MrpTagMark[0]) {
      if (state==M) state=T;
      else return -1;
    } else if (Anl[i]==MrpDlmMark[0]) {
      if (state==T) {
        NumMrp++;
        state=M;
      }
      else return -1;
    } else if (Anl[i]==EscMark[0]) {
      if (state==M) i++;
      else return -1;
    } /* else ... if */
  } /* for (i) */

  if (state==T) NumMrp++;
  else return -1;

  return NumMrp;
} /* chkanl */

char *anltok (char *Anl) 
{
  char *start;
  static char *last;
  int loc;

  if (Anl) start=Anl;
  else start=last;

  if (start[0]=='\0') return NULL;
  for (loc=1; (start[loc]!='\0')&&(start[loc]!=MrpTagMark[0])&&(start[loc]!=MrpDlmMark[0]); loc++) {
    if (start[loc]==EscMark[0]) loc++;
  } /* for (loc) */

  if ((start[loc]==MrpTagMark[0])||(start[loc]==MrpDlmMark[0])) start[loc++]='\0';
  last=start+loc;
  return start;
} /* anltok */


char *anltok_r (char *Anl, char **last) 
{
  char *start;
  int loc;

  if (Anl) start=Anl;
  else start=(*last);
  if (start[0]=='\0') return NULL;
  for (loc=1; (start[loc]!='\0')&&(start[loc]!=MrpTagMark[0])&&(start[loc]!=MrpDlmMark[0]); loc++) {
    if (start[loc]==EscMark[0]) loc++;
  } /* for (i) */

  if ((start[loc]==MrpTagMark[0])||(start[loc]==MrpDlmMark[0])) start[loc++]='\0';
  (*last)=start+loc;

  return start;
} /* anltok_r */


char *GetPosHead (char *Anl, char *Head) 
{
  int i, j;
  int len;
  char UnkTag[5];

  len=strlen((char *)Anl);

  //원호가 고침
  for(i=0;i<4;i++)
    UnkTag[i] = Anl[len-4+i];
  UnkTag[i]=NULL;

  if (strcmp(UnkTag,"/UNK")==0)
  {
      for(i=0;i<4;i++)
          Head[i]=UnkTag[i+1];
      Head[i]=NULL;
      return Head;
  }  
  //여기까지.

  for (i=0; (i<len)&&(Anl[i]!=MrpTagMark[0]); i++);
  for (j=i; (j<len)&&(Anl[j]!=MrpConMark[0]); j++);

  if (j==i) Head[0]=0;
  else {
    strncpy((char *)Head, (char *) Anl+i+1, j-i-1);
    Head[j-i-1]='\0';
  }

  return Head;
} /* GetPosHead */

char *GetPosTail (char *Anl, char *Tail) 
{
  int i;

  for (i=strlen((char *)Anl)-1; (i>=0)&&(Anl[i]!=MrpTagMark[0]); i--);

  strcpy((char *)Tail, (char *)Anl+i+1);

  return Tail;
} /* GetPosTail */
