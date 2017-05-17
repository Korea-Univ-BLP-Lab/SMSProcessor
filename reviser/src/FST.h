/* FST.h */
/* modified by dglee */
/* date : 29 Sep 2001 */
#ifndef __FST__
#define __FST__

#define MAX_CHAR   256
#define IsFinal    0

#define MaxStringLength 100	/* ã�����ϴ�  string�� �ִ���� */
#define FSTMaxMatch 100000

/* �ﰢ����� ���Ҹ� �Է¹޾� �������迭�� �����Ǵ� ��ġ�� ���� */
#define TabPos2(x, y, n) ((x)*(2*n-(x)-1)/2+(y)-1) /* dglee */

/* ���ڿ��� ũ�⸦ �Է¹޾� �ﰢ����� ������ ���� */
#define TabNum(x) ((x)*((x)+1)/2) /* added by dglee */


typedef struct {
  unsigned int CHAR:8;
  int Value:24;
  int Next;
} FST;


/******************************************************************************/
extern int MakeFST(char *SourceFilename, char *FSTFilename);

extern FST *LoadFST(char *FSTName);
extern void _FreeFST(FST *fst);


/* calculate hash value of the given string from FST */
/* nItem : String�� ��ġ�ϴ� ��Ʈ���� ���� */
/* ��ȯ�� : String�� ��ġ�ϴ� ù��° ��Ʈ���� Hash Value �Ǵ� -1(�˻� ����) */
extern int String2Hash(FST *fst, char *String, int *nItem);

/* find string with hash value */
/* 0 : invalid hash value */
/* 1 : find string corresponding to the hash value */
extern char *_Hash2String(FST *fst, int HashValue, char *String);

/* calculate hash values of all matched pattern */
extern int Pattern2Hash(FST *fst, char *String, int *IndexList);

/* calculate hash value located in the path of the given string from FST */
extern void Path2Hash(FST *fst, char *String, int *Flag);

/* calculate hash value of the string which is most similar to the given string */
extern int String2MostSimilarHash(FST *fst, char *String, int *nItem);

/* build tabular containing segmentation information for the given string */
extern void String2Tabular(FST *fst, char *String, int *Tabular);

/* build tabular containing segmentation information for the given string */
/* made by dglee */
extern int FindFSTTabular(FST *fst, char *String, int *Tabular);
extern int FindFSTTabular_hangul(FST *fst, char *String, int *Tabular);

/* change ASCII code to 4-bit code */
extern void H2to4(unsigned char *s, unsigned char *t);

/* change ASCII code(including wildcard ?) to 3-byte code */
extern void Q2to4(unsigned char *s, unsigned char *t);

/* change 4-bit code to ASCII code */
extern void H4to2(unsigned char *s, unsigned char *t);

#endif

