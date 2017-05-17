#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "FST.h"
#include "kulog_FST.h"

extern kulog_handler_t * kulog;

/* �Է� entry */
unsigned char **EntryTable=NULL;    /* �Է� �ܾ���� ������ */
unsigned char *EntryString=NULL;    /* �Է� �ܾ��� ���ڿ� */
int nEntry;

/* DFA�� FST ���� �ڷ� ���� */
typedef struct StateStruct {
  int Final:16;                     /* ���� state�� ���� ���·� ������ entry�� ���� */
  int n:16;                         /* Acceptable Input�� ���� = ���� state���� �Ļ��Ǵ� Transition�� ���� */
  int Loc;                          /* ���� state���� �Ļ��ϴ� Transition���� TransitionTable �������� ���� ��ġ */
} StateType;

/* state�� ���� �ӽ÷� ���Ǵ� Field�� : memory ����� ���̱� ���� ���� �� */
int *DFAGroup=NULL;                 /* minimize �������� ���� state�� ���ϴ� group ��ȣ */
int *DFAGenNum=NULL;                /* DFA -> FST ��ȯ �������� �ӽ÷� �����Ǵ� ��(���� state�� ��ġ�� entry�� ��) */
int *DFANext=NULL;                  /* ���� group�� ���ϴ� ���� state�� index */

/* DFA �� minimize�ϱ� ���� FST�� ���� ����/��� */
StateType *DFA=NULL;                /* DFA �� Minimize�ϱ� ���� FST */
int MaxState=200000;                /* DFA�� �⺻ state �� */
#define StateQuantum 200000         /* ���� ������ �⺻ state ���� �ʰ��� �� �� �Ҵ���� state �� */
int nState;                         /* ������� ������� state �� */

/* DFA�� ������ Transition�� ǥ���ϱ� ���� ����/��� */
FST *TransitionTable=NULL;          /* Transition���� table */
int MaxTransition=200000;           /* TransitionTable�� �⺻ transition �� */
#define TransitionQuantum 200000    /* ���� ������ �⺻ transition ���� �ʰ��� �� �� �Ҵ���� transition �� */
int nTransition;                    /* ������� ����� �� transition �� */

/* Minimize�� ���� FST �� TransitionTable */
StateType *MinDFA=NULL;             /* minimize�� ���� FST */
FST *MinTransitionTable=NULL;       /* minimize�� ���� transition���� table */
int nMinTransition;                 /* MinTransitionTable�� transition���� ���� */

/* Minimize �������� �ӽ÷� �����Ǵ� ���� */
typedef struct GroupType {
  int n;                            /* �ش� group�� ���ϴ� state�� �� */
  int Index;                        /* �ش� group�� ���ϴ� state�� (DFA������) index */
} GroupType;
GroupType *Groups=NULL;             /* group���� ���� */
int MaxGroup=10000;                  /* minimize���� ���� �⺻ group �� */
#define GroupQuantum 10000           /* �⺻ group ���� �ʰ����� �� �� �Ҵ���� group �� */
int nGroup=0;                       /* ��ü group�� ���� = MinDFA�� ���¼� */

/* Flattening ���� FST */
FST *FlatFST=NULL;
/* ���������� ������� FST�� ũ�� */
int SizeOfFST;
/* Flattening �������� �ӽ÷� ���Ǵ� ���� */
int *StatePos;                      /* �� state�� �����ϴ� ��ġ */
char *Used;                         /* �ش� ��ġ�� ��� ���� */

/* �Է� ������ ũ�� */
long FileSize;

/* �������� �Ҵ���� ��ü �޸𸮸� �����ϴ� �Լ� */
void FreeAllMemory(void);
/* thread ����� ȣ��Ǵ� ��ũ�� */
#define ONEXIT(Message) {\
  if (Message&&Message[0]) FST_LOG_ERR(kulog_reviser, "%s\n");\
  FreeAllMemory();\
}
#define MaxMessage 100
char MessageBuffer[MaxMessage];

/* ���� ������� �޸��� �� */
int MemoryUsed=0;

/* ���� ������� �޸��� ���� ����ϴ� ��ũ�� */
#define MEMORYCHECK {\
  FST_LOG_SVC(kulog_reviser, "\r%d bytes %-20s\n", MemoryUsed, "used");\
}

void *MyMalloc(size_t Size)
{
  void *p=malloc(Size);
  MemoryUsed+=Size;

  MEMORYCHECK;
  if (p==NULL) 
    {
    	ONEXIT("Insufficient memory");
    	return NULL;
    }
  return p;
}

void *MyRealloc(void *p, size_t Size, size_t Variance)
{
  p=realloc(p, Size);
  MemoryUsed+=Variance;

  MEMORYCHECK;
  if (p==NULL) 
  {
  	  ONEXIT("Insufficient memory");
  	  return NULL;
  }
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

/* State�� n��° transition�� �� �ʵ�鿡 ���� ��ũ�� */
#define OFCHAR(State, n) (TransitionTable[DFA[State].Loc+n].CHAR)
#define OFVALUE(State, n) (TransitionTable[DFA[State].Loc+n].Value)
#define OFNEXT(State, n) (TransitionTable[DFA[State].Loc+n].Next)

void MakeEntryTable(char *DicFile)
{
  FILE *f;
  unsigned char *p;
  int i;

  if ((f=fopen(DicFile, "rb"))==NULL) 
  {
  	  ONEXIT("Cannot open entry file");
  	  return ;
  }
  fseek(f, 0, SEEK_END);
  FileSize=ftell(f);
  fseek(f, 0, SEEK_SET);

  EntryString=(unsigned char *)MyMalloc(FileSize+1);
  fread(EntryString, FileSize, 1, f);
  EntryString[FileSize]=0;
  fclose(f);

  nEntry=0;
char *tmp_buff;
//  if (strtok((char *)EntryString, "\r\n")!=NULL)
  if (strtok_r((char *)EntryString, "\r\n", &tmp_buff )!=NULL)
    do {
      nEntry++; 
//    } while (strtok(NULL, "\r\n")!=NULL);
    } while (strtok_r(NULL, "\r\n", &tmp_buff )!=NULL);

  ///**/FST_LOG_SVC(kulog_reviser, "nEntry = %d\n", nEntry);
  EntryTable=(unsigned char **)MyMalloc(nEntry*sizeof(unsigned char *));

  p=EntryString;
  for (i=0; i<nEntry; i++) {
    EntryTable[i]=p;
    if (i<nEntry-1)
      for (p+=strlen((char *)p)+1; (*p==0)||(*p=='\r')||(*p=='\n'); p++);
  } 
}

/* IncompleteChar : ���� ���°� 2byte �ڵ��� ��������Ʈ�� �Է¹��� ���������� ��Ÿ�� */
void BuildDFA(int Root, int Start, int Size, int StrPos, int IncompleteChar)
{
  int i, SubStart[MAX_CHAR], SubSize[MAX_CHAR];
  unsigned char LastChar=0;

  DFA[Root].Final=0;
  while ((DFA[Root].Final<Size)&&(EntryTable[Start+DFA[Root].Final][StrPos]==0))
    DFA[Root].Final++;
  if (DFA[Root].Final&&IncompleteChar) 
  {
  	ONEXIT("Invalid 2byte code is in the entry");
  	return ;
}

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
      {
      	ONEXIT("Input entries are not sorted");
      	return ;
    }
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

  /**/FST_LOG_SVC(kulog_reviser, "MakeEntryTable completed..\n");

  DFA=(StateType *)MyMalloc(sizeof(StateType)*MaxState);
  DFAGroup=(int *)MyMalloc(sizeof(int)*MaxState);
  TransitionTable=(FST *)MyMalloc(sizeof(FST)*MaxTransition);

  nState=1;
  nTransition=0;
  
  /**/FST_LOG_SVC(kulog_reviser, "before BuildDFA..\n");
  BuildDFA(0, 0, nEntry, 0, 0);
  /**/FST_LOG_SVC(kulog_reviser, "after BuildDFA..\n");

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
{                               /* state�� Group�� �߰� */

  DFANext[NewState]=Groups[GroupNum].Index;
  DFAGroup[NewState]=GroupNum;
  Groups[GroupNum].Index=NewState;
  ++Groups[GroupNum].n;
}

int DelState(int GroupNum, int PreState)
{                               /* State�� Group���� ���� */
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
          FST_LOG_SVC(kulog_reviser, "\r# of Groups : %d", nGroup);
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
      FST_LOG_SVC(kulog_reviser, "\rFlattening : %d%% %-20s", State*100/nGroup, "done");
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
    FST_LOG_SVC(kulog_reviser, "%d : %s - %d %d\n", DFAGenNum[Root], s, Value, DFA[Root].Final);
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
    FST_LOG_SVC(kulog_reviser, "%s - %d\n", s, Value);
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

  FST_LOG_SVC(kulog_reviser, "Building DFA...\n");
  MakeDFA(SourceFilename);   /* DFA�� ����� */

  DFAGenNum=(int *)MyMalloc(sizeof(int)*nState);
  memset(DFAGenNum, 0, sizeof(int)*nState);

  FST_LOG_SVC(kulog_reviser, "Total number of entries : %d\nTotal number of states : %d\nCalculating hash values...\n", nEntry, nState);
  SetGenNum(0);         /* generation number set */
  FST_LOG_SVC(kulog_reviser, "Transform to FST...\n");
  AddHashValue(0);       /* FST output value set */

/*  int n;
  n=TraverseDFA(0, 0, s, 0, 0);
  if (n!=nEntry)
    {
    ONEXIT("There must be some error on building DFA");
    return -1;
}*/

  MyFree(DFAGenNum, sizeof(int)*nState); DFAGenNum=NULL;

  FST_LOG_SVC(kulog_reviser, "Minimizing FST...\n");

  MinimizeDFA(); 

  FST_LOG_SVC(kulog_reviser, "\rTotal number of groups : %-20d\nTotal Number of Transition : %d\n", nGroup, nMinTransition);

/*  n=TraverseMinDFA(0, 0, s, 0, 0);
  if (n!=nEntry) 
    {ONEXIT("There must be some error on building DFA"); return ; }

  FST_LOG_SVC(kulog_reviser, "Flattening FST...\n");
  MakeFlatFST(FSTFilename);  // 2���� �迭�� 1���� �迭�� */

  FST_LOG_SVC(kulog_reviser, "\rJob completed!\nSize of FST is %dbytes\n", SizeOfFST*sizeof(FST));
  return 0;
}



