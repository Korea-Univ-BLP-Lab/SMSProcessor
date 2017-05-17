#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "FST.h"

/* 입력 entry */
unsigned char **EntryTable=NULL;    /* 입력 단어들의 포인터 */
unsigned char *EntryString=NULL;    /* 입력 단어의 문자열 */
int nEntry;

/* DFA와 FST 공용 자료 구조 */
typedef struct StateStruct {
  int Final:16;                     /* 현재 state를 종료 상태로 가지는 entry의 개수 */
  int n:16;                         /* Acceptable Input의 개수 = 현재 state에서 파생되는 Transition의 개수 */
  int Loc;                          /* 현재 state에서 파생하는 Transition들의 TransitionTable 내에서의 시작 위치 */
} StateType;

/* state에 대해 임시로 사용되는 Field들 : memory 사용을 줄이기 위해 따로 둠 */
int *DFAGroup=NULL;                 /* minimize 과정에서 현재 state가 속하는 group 번호 */
int *DFAGenNum=NULL;                /* DFA -> FST 변환 과정에서 임시로 생성되는 값(현재 state를 거치는 entry의 수) */
int *DFANext=NULL;                  /* 같은 group에 속하는 다음 state의 index */

/* DFA 및 minimize하기 전의 FST를 위한 변수/상수 */
StateType *DFA=NULL;                /* DFA 및 Minimize하기 전의 FST */
int MaxState=200000;                /* DFA의 기본 state 수 */
#define StateQuantum 200000         /* 현재 설정된 기본 state 수를 초과할 때 더 할당받을 state 수 */
int nState;                         /* 현재까지 만들어진 state 수 */

/* DFA와 연관된 Transition을 표현하기 위한 변수/상수 */
FST *TransitionTable=NULL;          /* Transition들의 table */
int MaxTransition=200000;           /* TransitionTable의 기본 transition 수 */
#define TransitionQuantum 200000    /* 현재 설정된 기본 transition 수를 초과할 때 더 할당받을 transition 수 */
int nTransition;                    /* 현재까지 만들어 진 transition 수 */

/* Minimize한 후의 FST 및 TransitionTable */
StateType *MinDFA=NULL;             /* minimize한 후의 FST */
FST *MinTransitionTable=NULL;       /* minimize한 후의 transition들의 table */
int nMinTransition;                 /* MinTransitionTable의 transition들의 개수 */

/* Minimize 과정에서 임시로 생성되는 변수 */
typedef struct GroupType {
  int n;                            /* 해당 group에 속하는 state의 수 */
  int Index;                        /* 해당 group에 속하는 state의 (DFA에서의) index */
} GroupType;
GroupType *Groups=NULL;             /* group들의 정보 */
int MaxGroup=10000;                  /* minimize했을 때의 기본 group 수 */
#define GroupQuantum 10000           /* 기본 group 수를 초과했을 때 더 할당받을 group 수 */
int nGroup=0;                       /* 전체 group의 개수 = MinDFA의 상태수 */

/* Flattening 후의 FST */
FST *FlatFST=NULL;
/* 최종적으로 만들어진 FST의 크기 */
int SizeOfFST;
/* Flattening 과정에서 임시로 사용되는 변수 */
int *StatePos;                      /* 각 state가 시작하는 위치 */
char *Used;                         /* 해당 위치가 사용 여부 */

/* 입력 파일의 크기 */
long FileSize;

/* 동적으로 할당받은 전체 메모리를 제거하는 함수 */
void FreeAllMemory(void);
/* thread 종료시 호출되는 매크로 */
#define ONEXIT(Message) {\
  if (Message&&Message[0]) fprintf(stderr, "%s\n", Message);\
  FreeAllMemory();\
  exit(1);\
}
#define MaxMessage 100
char MessageBuffer[MaxMessage];

/* 현재 사용중인 메모리의 양 */
int MemoryUsed=0;

/* 현재 사용중인 메모리의 양을 출력하는 매크로 */
#define MEMORYCHECK {\
  fprintf(stderr, "\r%d bytes %-20s\n", MemoryUsed, "used");\
}

void *MyMalloc(size_t Size)
{
  void *p=malloc(Size);
  MemoryUsed+=Size;

  MEMORYCHECK;
  if (p==NULL) 
    ONEXIT("Insufficient memory");
  return p;
}

void *MyRealloc(void *p, size_t Size, size_t Variance)
{
  p=realloc(p, Size);
  MemoryUsed+=Variance;

  MEMORYCHECK;
  if (p==NULL) 
    ONEXIT("Insufficient memory");
  return p;
}

void MyFree(void *p, size_t Size)
{

  MemoryUsed-=Size;
  MEMORYCHECK;
  free(p);
}

void FreeAllMemory(void)
{
  if (EntryString) { MyFree(EntryString, FileSize+1); EntryString=NULL; }
  if (EntryTable) { MyFree(EntryTable, sizeof(unsigned char *)*nEntry); EntryTable=NULL; }
  if (DFA) { MyFree(DFA, sizeof(StateType)*MaxState); DFA=NULL; }
  if (DFAGroup) { MyFree(DFAGroup, sizeof(int)*MaxState); DFAGroup=NULL; }
  if (TransitionTable) { MyFree(TransitionTable, sizeof(FST)*MaxTransition); TransitionTable=NULL; }
  if (Groups) { MyFree(Groups, sizeof(GroupType)*MaxGroup); Groups=NULL; }
  if (DFANext) { MyFree(DFANext, sizeof(int)*nState); DFANext=NULL; }
  if (Used) { MyFree(Used, MaxTransition); Used=NULL; }
  if (StatePos) { MyFree(StatePos, sizeof(int)*nGroup); StatePos=NULL; }
  if (MinDFA) { MyFree(MinDFA, sizeof(StateType)*nGroup); MinDFA=NULL; }
  if (MinTransitionTable) { MyFree(MinTransitionTable, sizeof(FST)*nMinTransition); MinTransitionTable=NULL; }
  if (FlatFST) { MyFree(FlatFST, sizeof(FST)*MaxTransition); FlatFST=NULL; }
}

/* State의 n번째 transition의 각 필드들에 대한 매크로 */
#define OFCHAR(State, n) (TransitionTable[DFA[State].Loc+n].CHAR)
#define OFVALUE(State, n) (TransitionTable[DFA[State].Loc+n].Value)
#define OFNEXT(State, n) (TransitionTable[DFA[State].Loc+n].Next)

void MakeEntryTable(char *DicFile)
{
  FILE *f;
  unsigned char *p;
  int i;

  if ((f=fopen(DicFile, "rb"))==NULL) 
    ONEXIT("Cannot open entry file");
  fseek(f, 0, SEEK_END);
  FileSize=ftell(f);
  fseek(f, 0, SEEK_SET);

  EntryString=(unsigned char *)MyMalloc(FileSize+1);
  fread(EntryString, FileSize, 1, f);
  EntryString[FileSize]=0;
  fclose(f);

  nEntry=0;

//  if (strtok((char *)EntryString, "\r\n")!=NULL)
    char *tmp_buff;
    if (strtok_r((char *)EntryString, "\r\n", &tmp_buff)!=NULL)
    do {
      nEntry++; 
//    } while (strtok(NULL, "\r\n")!=NULL);
    } while (strtok_r(NULL, "\r\n", &tmp_buff)!=NULL);

  ///**/fprintf(stderr, "nEntry = %d\n", nEntry);
  EntryTable=(unsigned char **)MyMalloc(nEntry*sizeof(unsigned char *));

  p=EntryString;
  for (i=0; i<nEntry; i++) {
    EntryTable[i]=p;
    if (i<nEntry-1)
      for (p+=strlen((char *)p)+1; (*p==0)||(*p=='\r')||(*p=='\n'); p++);
  } 
}

/* IncompleteChar : 현재 상태가 2byte 코드의 상위바이트만 입력받은 상태인지를 나타냄 */
void BuildDFA(int Root, int Start, int Size, int StrPos, int IncompleteChar)
{
  int i, SubStart[MAX_CHAR], SubSize[MAX_CHAR];
  unsigned char LastChar=0;

  DFA[Root].Final=0;
  while ((DFA[Root].Final<Size)&&(EntryTable[Start+DFA[Root].Final][StrPos]==0))
    DFA[Root].Final++;
  if (DFA[Root].Final>1) 
  {
	fprintf(stderr, "%s : ", EntryTable[Start+DFA[Root].Final-1] );
    ONEXIT( "redundant entry was detected" );
  }


  if (DFA[Root].Final&&IncompleteChar) 
    ONEXIT("Invalid 2byte code is in the entry");

  if (DFA[Root].Final)
    DFAGroup[Root]=DFA[Root].Final+1;
  else DFAGroup[Root]=IncompleteChar;
  if (DFAGroup[Root]>nGroup-1)
    nGroup=DFAGroup[Root]+1;
  DFA[Root].n=0;
  DFA[Root].Loc=nTransition;

  if (StrPos&&(EntryTable[Start][StrPos-1]==' ')) {
    TransitionTable[nTransition].CHAR=' ';
    TransitionTable[nTransition].Next=Root;
    TransitionTable[nTransition].Value=0;
    nTransition++;
    if (nTransition==MaxTransition) {
      MaxTransition+=TransitionQuantum;
      TransitionTable=(FST *)MyRealloc(TransitionTable, sizeof(FST)*MaxTransition, sizeof(FST)*TransitionQuantum);
      memset(TransitionTable+MaxTransition-TransitionQuantum, 0, sizeof(FST)*TransitionQuantum);
    }
    DFA[Root].n++;
  }

  for (i=DFA[Root].Final; i<Size; i++) {
    if (EntryTable[Start+i][StrPos]!=LastChar) {
      SubStart[DFA[Root].n]=Start+i;
      SubSize[DFA[Root].n]=0;
      TransitionTable[nTransition].CHAR=EntryTable[Start+i][StrPos];
      TransitionTable[nTransition].Next=nState;
      TransitionTable[nTransition].Value=0;
      nTransition++;
      if (nTransition==MaxTransition) {
        MaxTransition+=TransitionQuantum;
        TransitionTable=(FST *)MyRealloc(TransitionTable, sizeof(FST)*MaxTransition, sizeof(FST)*TransitionQuantum);
        memset(TransitionTable+MaxTransition-TransitionQuantum, 0, sizeof(FST)*TransitionQuantum);
      }
      nState++;
      if (nState==MaxState) {
        MaxState+=StateQuantum;
        DFA=(StateType *)MyRealloc(DFA, sizeof(StateType)*MaxState, sizeof(StateType)*StateQuantum);
        memset(DFA+MaxState-StateQuantum, 0, sizeof(StateType)*StateQuantum);
        DFAGroup=(int *)MyRealloc(DFAGroup, sizeof(int)*MaxState, sizeof(int)*StateQuantum);
        memset(DFAGroup+MaxState-StateQuantum, 0, sizeof(int)*StateQuantum);
      }
      DFA[Root].n++;
      if (EntryTable[Start+i][StrPos]<LastChar) 
        ONEXIT("Input entries are not sorted");
      LastChar=EntryTable[Start+i][StrPos];
    }
    SubSize[DFA[Root].n-1]++;
  }

  for (i=0; i<DFA[Root].n; i++) {
    if (OFNEXT(Root, i)==Root) continue;
    if ((OFCHAR(Root, i)<0x80)||IncompleteChar)
      BuildDFA(OFNEXT(Root, i), SubStart[i], SubSize[i], StrPos+1, 0);
    else BuildDFA(OFNEXT(Root, i), SubStart[i], SubSize[i], StrPos+1, 1);
  }
}

void MakeDFA(char *DicFile)
{  
  MakeEntryTable(DicFile);

  /**/fprintf(stderr, "MakeEntryTable completed..\n");

  DFA=(StateType *)MyMalloc(sizeof(StateType)*MaxState);
  DFAGroup=(int *)MyMalloc(sizeof(int)*MaxState);
  TransitionTable=(FST *)MyMalloc(sizeof(FST)*MaxTransition);

  nState=1;
  nTransition=0;
  
  /**/fprintf(stderr, "before BuildDFA..\n");
  BuildDFA(0, 0, nEntry, 0, 0);
  /**/fprintf(stderr, "after BuildDFA..\n");

  MyFree(EntryString, FileSize+1); EntryString=NULL;
  MyFree(EntryTable, sizeof(unsigned char *)*nEntry); EntryTable=NULL;
}

int SetGenNum(int State)
{
  int i, NextValue, CurrentValue;
  
  CurrentValue=0;
  
  for (i=0; i<DFA[State].n; i++) {
    if (OFNEXT(State, i)==State) 
      continue;
    NextValue=DFAGenNum[OFNEXT(State, i)];
    if (NextValue!=0) CurrentValue+=NextValue;
    else CurrentValue+=SetGenNum(OFNEXT(State, i));
  }
  
  CurrentValue+=DFA[State].Final;
  DFAGenNum[State]=CurrentValue;
  
  return CurrentValue;
}

void AddHashValue(int State)
{
  int i;
  
  for (i=0; i<DFA[State].n; i++) {
    if (OFNEXT(State, i)==State) 
      OFVALUE(State, i)=0;
    else if ((i==0)||((i==1)&&(OFNEXT(State, i-1)==State))) 
        OFVALUE(State, i)=DFA[State].Final;
    else OFVALUE(State, i)=
        OFVALUE(State, i-1-(OFNEXT(State, i-1)==State))+
        DFAGenNum[OFNEXT(State, i-1-(OFNEXT(State, i-1)==State))];
    if (OFNEXT(State, i)!=State)
      AddHashValue(OFNEXT(State, i));
  }
}

void AddState(int GroupNum, int NewState)
{                               /* state를 Group에 추가 */

  DFANext[NewState]=Groups[GroupNum].Index;
  DFAGroup[NewState]=GroupNum;
  Groups[GroupNum].Index=NewState;
  ++Groups[GroupNum].n;
}

int DelState(int GroupNum, int PreState)
{                               /* State를 Group에서 삭제 */
  int StateToDelete;
  
  StateToDelete=DFANext[PreState];
  DFANext[PreState]=DFANext[StateToDelete];
  --Groups[GroupNum].n;
  
  return StateToDelete;
}

void InitGroups(int Root)
{
  int i;
  
  for (i=0; i<DFA[Root].n; i++) {
    if (OFNEXT(Root, i)==Root) continue;
    InitGroups(OFNEXT(Root, i));
  }

  DFANext[Root]=0;
  AddState(DFAGroup[Root], Root);
}

FILE *tmpfile(void);

void Arrange(void)
{
  int i, Group, Loc;
  FILE *fState=tmpfile(), *fTransition=tmpfile();
  FST OneTransition;

  nMinTransition=0;
  for (Group=0; Group<nGroup; Group++) {
    Loc=nMinTransition;
    for (i=0; i<DFA[Groups[Group].Index].n; i++) {
      OneTransition.CHAR=(unsigned int)OFCHAR(Groups[Group].Index, i);
      OneTransition.Value=OFVALUE(Groups[Group].Index, i);
      OneTransition.Next=DFAGroup[OFNEXT(Groups[Group].Index, i)];
      fwrite(&OneTransition, sizeof(FST), 1, fTransition);
      nMinTransition++;
    }
    DFA[Groups[Group].Index].Loc=Loc;
    fwrite(DFA+Groups[Group].Index, sizeof(StateType), 1, fState);
  }

  MyFree(DFA, sizeof(StateType)*MaxState); DFA=NULL;
  MyFree(DFAGroup, sizeof(int)*MaxState); DFAGroup=NULL;
  MyFree(TransitionTable, sizeof(FST)*MaxTransition); TransitionTable=NULL;
  MyFree(Groups, sizeof(GroupType)*MaxGroup); Groups=NULL;

  fseek(fState, 0, SEEK_SET);
  fseek(fTransition, 0, SEEK_SET);

  MinDFA=(StateType *)MyMalloc(nGroup*sizeof(StateType));
  MinTransitionTable=(FST *)MyMalloc(sizeof(FST)*nMinTransition);

  fread(MinDFA, sizeof(StateType), nGroup, fState);
  fread(MinTransitionTable, sizeof(FST), nMinTransition, fTransition);

  fclose(fState);
  fclose(fTransition);
}

void MinimizeDFA(void)
{
  int i, j, k, LastnGroup;
  int Head, Current, Last;
  
  Groups=(GroupType *)MyMalloc(sizeof(GroupType)*MaxGroup);
  memset(Groups, 0, sizeof(GroupType)*MaxGroup); 

  DFANext=(int *)MyMalloc(sizeof(int)*nState);
  memset(DFANext, 0, sizeof(int)*nState);

  InitGroups(0);

  j=0;
  for (i=0; i<nGroup; i++) {
    if (Groups[i].n==0) continue;
    if (i!=j) {
      memcpy(Groups+j, Groups+i, sizeof(GroupType));
      for (k=0, Current=Groups[j].Index; k<Groups[j].n; k++, Current=DFANext[Current])
        DFAGroup[Current]=j;
    }
    j++;
  }
  memset(Groups+j, 0, sizeof(GroupType)*(nGroup-j));
  nGroup=j;

  do {
    LastnGroup=nGroup;
    for (i=0; i<nGroup; i++) {
      if (Groups[i].n==1) continue;
      Head=Groups[i].Index;
      Last=Head;
      do {
        Current=DFANext[Last];
        if (DFA[Head].n==DFA[Current].n) 
          for (j=0; j<DFA[Head].n; j++) {
            if (OFCHAR(Head, j)!=OFCHAR(Current, j)) break;
            if (OFVALUE(Head, j)!=OFVALUE(Current, j)) break;
            if (DFAGroup[OFNEXT(Head, j)]!=DFAGroup[OFNEXT(Current, j)]) break;
          }
        if ((DFA[Head].n!=DFA[Current].n)||(j<DFA[Head].n)) 
          AddState(nGroup, DelState(i, Last));
        else 
          Last=DFANext[Last];
      } while (DFANext[Last]);

      if (Groups[nGroup].n!=0) {
        if (nGroup%10==0) 
          fprintf(stderr, "\r# of Groups : %d", nGroup);
        nGroup++;
        if (nGroup==MaxGroup-1) {
          MaxGroup+=GroupQuantum;
          Groups=(GroupType *)MyRealloc(Groups, sizeof(GroupType)*MaxGroup, sizeof(GroupType)*GroupQuantum);
          memset(Groups+MaxGroup-GroupQuantum, 0, sizeof(GroupType)*GroupQuantum);
        }
      }
    }
  } while (LastnGroup!=nGroup);
  
  MyFree(DFANext, sizeof(int)*nState); DFANext=NULL;

  Arrange();
}

void MakeFlatFST(char *FSTName)
{
  int State, Pos, MinPos, MaxPos, i;
  FILE *Dic;
  
  MinPos=0;
  MaxPos=0;
  
  StatePos=(int *)MyMalloc(sizeof(int)*nGroup);
  Used=(char *)MyMalloc(MaxTransition);
  FlatFST=(FST *)MyMalloc(sizeof(FST)*MaxTransition);
  memset(StatePos, 0, sizeof(int)*nGroup);
  memset(Used, 0, MaxTransition);
  memset(FlatFST, 0, sizeof(FST)*MaxTransition);

  for (State=0; State<nGroup; State++) {
    if (State%100==0) 
      fprintf(stderr, "\rFlattening : %d%% %-20s", State*100/nGroup, "done");
    Pos=MinPos-1;
    do {
      do {
        for (Pos++; Used[Pos]; Pos++);
      } while ((MinDFA[State].Final)&&(FlatFST[Pos+IsFinal].Next!=0));

      for (i=0; i<MinDFA[State].n; i++)
        if (FlatFST[Pos+MinTransitionTable[MinDFA[State].Loc+i].CHAR].Next!=0) 
          break;
    } while (i<MinDFA[State].n);
    
    for (i=0; i<MinDFA[State].n; i++) {
      FlatFST[Pos+MinTransitionTable[MinDFA[State].Loc+i].CHAR].CHAR=MinTransitionTable[MinDFA[State].Loc+i].CHAR;
      FlatFST[Pos+MinTransitionTable[MinDFA[State].Loc+i].CHAR].Next=MinTransitionTable[MinDFA[State].Loc+i].Next;
      FlatFST[Pos+MinTransitionTable[MinDFA[State].Loc+i].CHAR].Value=MinTransitionTable[MinDFA[State].Loc+i].Value;
    }
    if (MinDFA[State].Final) {
      FlatFST[Pos+IsFinal].CHAR=MinDFA[State].Final;
      FlatFST[Pos+IsFinal].Next=-1;
      FlatFST[Pos+IsFinal].Value=-1;
    }
  
    StatePos[State]=Pos;
    Used[Pos]=1;
    
    if (Pos>MaxPos) MaxPos=Pos;
    for (; Used[MinPos]; MinPos++);

    if (MaxPos+MAX_CHAR*2>MaxTransition) {
      MaxTransition+=TransitionQuantum;
      FlatFST=(FST *)MyRealloc(FlatFST, sizeof(FST)*MaxTransition, sizeof(FST)*TransitionQuantum);
      memset(FlatFST+MaxTransition-TransitionQuantum, 0, sizeof(FST)*TransitionQuantum);
      Used=(char *)MyRealloc(Used, MaxTransition, TransitionQuantum);
      memset(Used+MaxTransition-TransitionQuantum, 0, TransitionQuantum);
    }
  }
  
  MyFree(Used, MaxTransition); Used=NULL;
  
  SizeOfFST=MaxPos+MAX_CHAR;
  for (i=0; i<SizeOfFST; i++) 
    if ((FlatFST[i].Next!=0)&&(FlatFST[i].Next!=-1))
      FlatFST[i].Next=StatePos[FlatFST[i].Next];
  
  MyFree(StatePos, sizeof(int)*nGroup); StatePos=NULL;
  MyFree(MinDFA, sizeof(StateType)*nGroup); MinDFA=NULL;
  MyFree(MinTransitionTable, sizeof(FST)*nMinTransition); MinTransitionTable=NULL;
  
  Dic=fopen(FSTName, "wb");
  fwrite(FlatFST, sizeof(FST), SizeOfFST, Dic);
  fclose(Dic);
  MyFree(FlatFST, sizeof(FST)*MaxTransition); FlatFST=NULL;
}

int TraverseDFA(int Root, int Value, char *s, int Pos, int n)
{
  int i;

  if (DFA[Root].Final) {
    s[Pos]=0;
    n+=DFA[Root].Final;
    fprintf(stderr, "%d : %s - %d %d\n", DFAGenNum[Root], s, Value, DFA[Root].Final);
  }

  for (i=0; i<DFA[Root].n; i++) {
    if (OFNEXT(Root, i)==Root) continue;
    s[Pos]=OFCHAR(Root, i);
    n=TraverseDFA(OFNEXT(Root, i), Value+OFVALUE(Root, i), s, Pos+1, n);
  }

  return n;
}

int TraverseMinDFA(int Root, int Value, char *s, int Pos, int n)
{
  int i;

  if (MinDFA[Root].Final) {
    s[Pos]=0;
    n+=MinDFA[Root].Final;
    fprintf(stderr, "%s - %d\n", s, Value);
  }

  for (i=0; i<MinDFA[Root].n; i++) {
    if (MinTransitionTable[MinDFA[Root].Loc+i].Next==Root) continue;
    s[Pos]=MinTransitionTable[MinDFA[Root].Loc+i].CHAR;
    n=TraverseMinDFA(MinTransitionTable[MinDFA[Root].Loc+i].Next, Value+MinTransitionTable[MinDFA[Root].Loc+i].Value, s, Pos+1, n);
  }

  return n;
}

int MakeFST(char *SourceFilename, char *FSTFilename)
{
//  char s[256];

  fprintf(stderr, "Building DFA...\n");
  MakeDFA(SourceFilename);   /* DFA를 만든다 */

  DFAGenNum=(int *)MyMalloc(sizeof(int)*nState);
  memset(DFAGenNum, 0, sizeof(int)*nState);

  fprintf(stderr, "Total number of entries : %d\nTotal number of states : %d\nCalculating hash values...\n", nEntry, nState);
  SetGenNum(0);         /* generation number set */
  fprintf(stderr, "Transform to FST...\n");
  AddHashValue(0);       /* FST output value set */

/*  int n;
  n=TraverseDFA(0, 0, s, 0, 0);
  if (n!=nEntry)
    ONEXIT("There must be some error on building DFA");*/

  MyFree(DFAGenNum, sizeof(int)*nState); DFAGenNum=NULL;

  fprintf(stderr, "Minimizing FST...\n");

  MinimizeDFA(); 

  fprintf(stderr, "\rTotal number of groups : %-20d\nTotal Number of Transition : %d\n", nGroup, nMinTransition);

/*  n=TraverseMinDFA(0, 0, s, 0, 0);
  if (n!=nEntry) 
    ONEXIT("There must be some error on building DFA");*/

  fprintf(stderr, "Flattening FST...\n");
  MakeFlatFST(FSTFilename);  /* 2차원 배열을 1차원 배열로 */

  fprintf(stderr, "\rJob completed!\nSize of FST is %dbytes\n", SizeOfFST*sizeof(FST));
  return 0;
}



