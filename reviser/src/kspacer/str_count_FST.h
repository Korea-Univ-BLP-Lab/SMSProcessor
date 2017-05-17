/*
    ��¥ : 2006.08.02
    �ۼ� : �̽¿�
    ���� : ���ڿ��� �󵵸� �����ϴ� �ڷ� ������ ����
           Btree�� �̿�, stl ��� ����
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
