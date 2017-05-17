#ifndef __FST__
#define __FST__

#define NULL_INDEX (-1)

#define MAX_CHAR   256
#define IsFinal    0

#define MaxStringLength 100	/* ã�����ϴ�  string�� �ִ���� */
#define FSTMaxMatch 100000

typedef struct {
  unsigned int CHAR:8;
  int Value:24;
  int Next;
} FST;

FST *LoadFST(const char *FSTName);
void FreeFST(FST *fst);

/* calculate hash value of the given string from FST */
/* nItem : String�� ��ġ�ϴ� ��Ʈ���� ���� */
/* ��ȯ�� : String�� ��ġ�ϴ� ù��° ��Ʈ���� Hash Value �Ǵ� NULL_INDEX(�˻� ����) */
int String2Hash(FST *fst, const char *String, int *nItem=NULL);

/* calculate hash value of the entry most similare to given string from FST */
/* nItem : String�� ���� ������ ��Ʈ���� ���� */
/* Last : *Last-String�� �νĵ� ���ڿ��� ���� */
/* ��ȯ�� : String�� ���� ������ ù��° ��Ʈ���� Hash Value �Ǵ� NULL_INDEX(�˻� ����) */
int String2MostSimilarHash(FST *fst, const char *String, int *nItem=NULL, const char **Last=NULL);

/* fst�� ����ִ� ��Ʈ���� ���� */ 
int GetNumberOfEntry(FST *fst);

/* find string with hash value */
/* NULL : invalid hash value */
/* otherwise : find string corresponding to the hash value */
char *Hash2String(FST *fst, int HashValue, char *String);

/* calculate hash values of all matched pattern */
/* MaxMatch : �ִ�� ã�� ��Ʈ���� ���� */
/* IndexList : pattern�� ��ġ�Ǵ� ��Ʈ���� ���� �迭 */
/* ��ȯ�� : pattern�� ��ġ�Ǵ� ��Ʈ���� ���� */
int Pattern2Hash(FST *fst, const char *Pattern, int MaxMatch, int *IndexList);

/* calculate hash value located on the path of the given string from FST */
void Path2Hash(FST *fst, const char *String, int *ExistFlag);

/* build tabular containing segmentation information for the given string */
void String2Tabular(FST *fst, const char *String, int *Tabular);

#define CELL(Tabular, Size, i, j) Tabular[(2*Size-i+1)/2+j]

#endif
