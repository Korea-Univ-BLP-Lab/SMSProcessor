#ifndef __definitions_H__
#define __definitions_H__

//#pragma warning(disable: 4786)
//#pragma warning(disable: 4503)

#include <string>
#include <map>
#include <stdio.h>
#include <stdlib.h>

#include "str_prob_FST.h"

using namespace std;

/******************************************************************************/
/* 타입 선언 */
/******************************************************************************/
/* 타입 선언 */




typedef map<string, int>       C_FREQ;     // 문자 빈도 
typedef map<string, C_FREQ>    CC_FREQ;    // 문자 문자 빈도 
typedef map<string, CC_FREQ>   CCC_FREQ;   // 문자 문자 문자 빈도 
typedef map<string, CCC_FREQ>  CCCC_FREQ;  // 문자 문자 문자 문자 빈도 
typedef map<string, CCCC_FREQ> CCCCC_FREQ; // 문자 문자 문자 문자 문자 빈도 

typedef map<string, double>    NUM_PROB; // 분자 확률값 
typedef map<string, NUM_PROB>  PROB_MAP; // 분모 분자 확률값 

typedef map <string, int>      TAGSET; // 태그 빈도 

extern C_FREQ     u_freq;
extern C_FREQ     c_freq;

extern CC_FREQ    cu_freq;
extern CC_FREQ    uc_freq;

extern CCC_FREQ   cuc_freq;
extern CCC_FREQ   ucu_freq;

extern CCCC_FREQ  ucuc_freq;
extern CCCC_FREQ  cucu_freq;

extern CCCCC_FREQ ucucu_freq;
extern CCCCC_FREQ cucuc_freq;



//typedef CStrProbBTree         PROB_MAP_BTREE;
typedef CStrProbFST           PROB_MAP_FST;

//typedef map <string, int>      TAGSET; /* 태그 빈도 */

/******************************************************************************/
/* 상수 선언 */
extern char STAG[2][2]; // STAG[0] = "0", STAG[1] = "1"

#define DELIM "_|"  /* 분리자 */

#define BOS_WSTAG_2 "1" /* 문장시작 띄어쓰기 태그 (따로 다른 태그를 붙이면 성능이 더 떨어짐) */
#define BOS_WSTAG_1 "1" /* 문장시작 띄어쓰기 태그 (따로 다른 태그를 붙이면 성능이 더 떨어짐) */

#define BOS_PTAG_2 "2|" /* 문장시작 품사 태그 */
#define BOS_PTAG_1 "1|" /* 문장시작 품사 태그 */

#define BOS_SYL_2 "|2"  /* 문장시작 음절 */
#define BOS_SYL_1 "|1"  /* 문장시작 음절 */

#define MLE 1
#define LINEAR_INTERPOL1 2
#define MAXENT  3

#define MAX_LINE 10000
#define MAX_WORD_LEN 100 
#define MAX_TAG_LEN 100 

#define ALMOST_ZERO 1.0e-100 /* 완전히 0을 쓰게 되면 log(0) = inf가 되어서 문제 발생 가능 */

/******************************************************************************/
extern int verbosity;


/*
typedef CStrCountBTree	CStrCount;

extern CStrCount     u_freq2;
extern CStrCount     c_freq2;

extern CStrCount    cu_freq2;
extern CStrCount    uc_freq2;

extern CStrCount   cuc_freq2;
extern CStrCount   ucu_freq2;

extern CStrCount  ucuc_freq2;
extern CStrCount  cucu_freq2;

extern CStrCount ucucu_freq2;
extern CStrCount cucuc_freq2;
*/


#endif
