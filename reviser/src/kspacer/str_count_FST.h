/*
    날짜 : 2006.08.02
    작성 : 이승욱
    내용 : 문자열의 빈도를 저장하는 자료 구조를 구현
           Btree를 이용, stl 사용 안함
*/

#ifndef ___STRING_CNT_BTREE___
#define ___STRING_CNT_BTREE__



#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "CBTree.h"

#ifndef ___WORDTYPE___
#define  ___WORDTYPE___
typedef struct {
	int count;
	char key[200];
}wordtype;
#endif


class CStrCountBTree	{
public:
	CStrCountBTree();
	~CStrCountBTree();

public:
	void AddStr( char *str1 );
	int GetCount( char *str1  );
	void SaveToFile( char *filename );
	void LoadFromFile( char *filename );
	

//private:
public:
	CBTree btree;
};


#endif
