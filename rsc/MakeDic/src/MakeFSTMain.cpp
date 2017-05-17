#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "FST.h"

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
int MaxGroup=1000;                  /* minimize���� ���� �⺻ group �� */
#define GroupQuantum 1000           /* �⺻ group ���� �ʰ����� �� �� �Ҵ���� group �� */
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
void FreeAllMemory();
/* thread ����� ȣ��Ǵ� ��ũ�� */
#define ONEXIT(Message) {\
  if (Message&&Message[0])\
    fprintf(stderr, "%s\n", Message);\
  FreeAllMemory();\
  exit(1);\
}
/* Thread ���� ��ȣ�� �˻��ϴ� �Լ� */

void *MyMalloc(size_t Size)
{
  void *p=malloc(Size);

  if (p==NULL) 
    ONEXIT("Insufficient memory");
  return p;
}

void *MyRealloc(void *p, size_t Size, size_t Variance)
{
  p=realloc(p, Size);

  if (p==NULL) 
    ONEXIT("Insufficient memory");
  return p;
}

void MyFree(void *p, size_t Size)
{

  free(p);
}

void FreeAllMemory()
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

int MyStrCmp(const void *x, const void *y)
{
  return strcmp(*((char **)x), *((char **)y));
}

#define POSNAMEFILE "pos.nam"
#define MaxPOS 96

char POSTable[MaxPOS][10];

void LoadPOSTable()
{
  FILE *f;
  int i;

  if ((f=fopen(POSNAMEFILE, "rb"))==NULL) 
    ONEXIT("Cannot open pos.nam file");
  for (i=0; i<MaxPOS; i++)
    fscanf(f, "%s%*s", POSTable[i]);
  fclose(f);
}

int POS2Int(char *POS)
{
  int i;
  for (i=0; i<MaxPOS; i++)
    if (strcmp(POS, POSTable[i])==0) break;

  return i;
}

void MakeEntryTable(const char *DicFile, const char *DataFile)
{
  FILE *f;
  unsigned char *p;
  char *q;
  int i;

  LoadPOSTable();

  if ((f=fopen(DicFile, "rb"))==NULL) 
    ONEXIT("Cannot open source file");
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
  if (strtok_r((char *)EntryString, "\r\n", &tmp_buff )!=NULL)
    do {
      nEntry++; 
//    } while (strtok(NULL, "\r\n")!=NULL);
    } while (strtok_r(NULL, "\r\n", &tmp_buff)!=NULL);

  EntryTable=(unsigned char **)MyMalloc(nEntry*sizeof(unsigned char *));

  p=EntryString;
  for (i=0; i<nEntry; i++) {
    EntryTable[i]=p;
    if (strlen((char *)p)==0)
      ONEXIT("Invalid data");
    if (i<nEntry-1)
      for (p+=strlen((char *)p)+1; (*p==0)||(*p=='\r')||(*p=='\n'); p++);
    if ((q=strstr((char *)EntryTable[i], " {"))==NULL) 
      ONEXIT("Invalid data");
    *q=0;
  }

  qsort(EntryTable, nEntry, sizeof(char *), MyStrCmp);

  if ((f=fopen(DataFile, "wb"))==NULL) 
    ONEXIT("Cannot open data file");
  int POS[3], n;
  for (i=0; i<nEntry; i++) {
    memset(POS, 0, sizeof(int)*3);
    q=(char *)(EntryTable[i]+strlen((const char *)EntryTable[i])+1);
//    if ((q=strtok(q, " \r\n"))==NULL)
	  char *tmp_buff;	   
      if ((q=strtok_r(q, " \r\n", &tmp_buff))==NULL)
      ONEXIT("Invalid data");
    do {
      if ((strcmp(q, "{")==0)||(strcmp(q, "}")==0)) continue;
      if ((n=POS2Int(q))==MaxPOS)
        ONEXIT("Invalid data");
      POS[n/32]+=(1<<(n%32));
//    } while ((q=strtok(NULL, " \r\n"))!=NULL);
    } while ((q=strtok_r(NULL, " \r\n", &tmp_buff))!=NULL);
    fprintf(f, "%08X %08X %08X\n", POS[0], POS[1], POS[2]);
  }
  fclose(f);
}

/* IncompleteChar : ���� ���°� 2byte �ڵ��� ��������Ʈ�� �Է¹��� ���������� ��Ÿ�� */
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

  if (DFA[Root].Final&&IncompleteChar) {
    fprintf(stderr, "%d-th line : ", Start);
    ONEXIT("Invalid 2byte code is in the entry");
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

void MakeDFA(const char *DicFile, const char *DataFile)
{  

  MakeEntryTable(DicFile, DataFile);

  DFA=(StateType *)MyMalloc(sizeof(StateType)*MaxState);
  DFAGroup=(int *)MyMalloc(sizeof(int)*MaxState);
  TransitionTable=(FST *)MyMalloc(sizeof(FST)*MaxTransition);

  nState=1;
  nTransition=0;
  BuildDFA(0, 0, nEntry, 0, 0);

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

void Arrange()
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

void MinimizeDFA()
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

  for (i=0; i<nGroup; i++)
    while ((i<nGroup)&&(Groups[i].n==0)) {
      if (nGroup-i-1) 
        memmove(Groups+i, Groups+i+1, sizeof(GroupType)*(nGroup-i-1));
      nGroup--;
      Groups[nGroup].n=0;
      Groups[nGroup].Index=0;
    }

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
          fprintf(stderr, "\rCurrent Number of Groups : %d", nGroup);
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

void MakeFlatFST(const char *FSTName)
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
      fprintf(stderr, "\rFlattening : %d%% done", State*100/nGroup);
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
    fprintf(stderr, "%d : %s - %d %d\r\n", DFAGenNum[Root], s, Value, DFA[Root].Final);
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
    fprintf(stderr, "%s - %d\r\n", s, Value);
  }

  for (i=0; i<MinDFA[Root].n; i++) {
    if (MinTransitionTable[MinDFA[Root].Loc+i].Next==Root) continue;
    s[Pos]=MinTransitionTable[MinDFA[Root].Loc+i].CHAR;
    n=TraverseMinDFA(MinTransitionTable[MinDFA[Root].Loc+i].Next, Value+MinTransitionTable[MinDFA[Root].Loc+i].Value, s, Pos+1, n);
  }

  return n;
}

int MakeFST(const char *SourceFilename, const char *DataFilename, const char *FSTFilename)
{

  fprintf(stderr, "Building DFA...\n");
  MakeDFA(SourceFilename, DataFilename);   /* DFA�� ����� */

  DFAGenNum=(int *)MyMalloc(sizeof(int)*nState);
  memset(DFAGenNum, 0, sizeof(int)*nState);

  fprintf(stderr, "\nTotal number of entries : %d\nTotal number of states : %d\nCalculating hash values...\n", nEntry, nState);
  SetGenNum(0);         /* generation number set */
  fprintf(stderr, "\nTransform to FST...\n");
  AddHashValue(0);       /* FST output value set */

/*  int n;
  n=TraverseDFA(0, 0, s, 0, 0);
  if (n!=nEntry)
    ONEXIT("There must be some error on building DFA");
*/

  MyFree(DFAGenNum, sizeof(int)*nState); DFAGenNum=NULL;

  fprintf(stderr, "\nMinimizing FST...\n");

  MinimizeDFA(); 

  fprintf(stderr, "\nTotal number of groups : %d\nTotal Number of Transition : %d\n", nGroup, nMinTransition);

/*  n=TraverseMinDFA(0, 0, s, 0, 0);
  if (n!=nEntry) 
    ONEXIT("There must be some error on building DFA");
*/

  fprintf(stderr, "\nFlattening FST...\n");
  MakeFlatFST(FSTFilename);  /* 2���� �迭�� 1���� �迭�� */

  fprintf(stderr, "\nJob completed!\nSize of FST is %dbytes\n", SizeOfFST*sizeof(FST));
  return 0;
}
