/*
    ��¥ : 2006.09.07
    �ۼ� : �̽¿�
    ���� : ���ڿ��� Ȯ���� �����ϴ� �ڷ� ������ ����
           FST�� �̿�, stl ��� ����
*/

#ifndef ___STRING_PROB_FST___
#define ___STRING_PROB_FST___



#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "FST.h"


class CStrProbFST	{
public:
	CStrProbFST();
	~CStrProbFST();

public:
	bool Load( char arg_fn_prefix[] );
	double GetProb( char *str1  );
	
public:
	
	bool LoadProb( );

private:
	FST *fst;
	double *prob;
	char fst_fn[MaxStringLength];
	char prob_fn[MaxStringLength];
	/*
	Map map_str_prob;
	char str1[256];
	char str2[256];
	*/
	
};


#endif
