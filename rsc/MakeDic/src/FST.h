#ifndef __FST__
#define __FST__

#define NULL_INDEX (-1)

#define MAX_CHAR   256
#define IsFinal    0

#define MaxStringLength 100	/* 찾고자하는  string의 최대길이 */

typedef struct {
  unsigned int CHAR:8;
  int Value:24;
  int Next;
} FST;

FST *LoadFST(const char *FSTName);
void FreeFST(FST *fst);

/* calculate hash value of the given string from FST */
/* nItem : String에 일치하는 엔트리의 개수 */
/* 반환값 : String에 일치하는 첫번째 엔트리의 Hash Value 또는 NULL_INDEX(검색 실패) */
int String2Hash(FST *fst, const char *String, int *nItem=NULL);

/* calculate hash value of the entry most similare to given string from FST */
/* nItem : String에 가장 유사한 엔트리의 개수 */
/* Last : *Last-String은 인식된 문자열의 길이 */
/* 반환값 : String에 가장 유사한 첫번째 엔트리의 Hash Value 또는 NULL_INDEX(검색 실패) */
int String2MostSimilarHash(FST *fst, const char *String, int *nItem=NULL, const char **Last=NULL);

/* fst에 들어있는 엔트리의 개수 */ 
int GetNumberOfEntry(FST *fst);

/* find string with hash value */
/* NULL : invalid hash value */
/* otherwise : find string corresponding to the hash value */
char *Hash2String(FST *fst, int HashValue, char *String);

/* calculate hash values of all matched pattern */
/* MaxMatch : 최대로 찾을 엔트리의 개수 */
/* IndexList : pattern에 일치되는 엔트리를 담을 배열 */
/* 반환값 : pattern에 일치되는 엔트리의 개수 */
int Pattern2Hash(FST *fst, const char *Pattern, int MaxMatch, int *IndexList);

/* calculate hash value located on the path of the given string from FST */
void Path2Hash(FST *fst, const char *String, int *ExistFlag);

/* build tabular containing segmentation information for the given string */
void String2Tabular(FST *fst, const char *String, int *Tabular);

#define CELL(Tabular, Size, i, j) Tabular[(2*Size-i+1)/2+j]

#endif
