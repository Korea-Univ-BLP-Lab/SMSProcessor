/*
    날짜 : 2006.09.07
    작성 : 이승욱
    내용 : 문자열의 확률을 저장하는 자료 구조를 구현
           FST를 이용, stl 사용 안함
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
