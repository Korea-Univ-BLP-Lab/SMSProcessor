/** 형태소 분석 및 품사 부착기 모듈 기본 API 헤더 파일
 	@file HanTagAPI.h
 */
#ifndef __HANTAGAPI__
#define __HANTAGAPI__

#include "nlp.def"
///////////////////////////////////////////////////////////////
#include "kulog.h"

/**
 @brief 형태소 분석기 환경 설정 자료구조.
 */
typedef struct _HanTag_config_t
{
	/**
	  @brief 태거 실행 여부.
	  @remarks 
	  on(1) /off(0)
	  */
	int tagger;
	/**
	  @brief 태거 리소스 경로.
	  @remarks 
	  예) ../tagger/rsc
	  */
	char tagger_path[DRE_CONFIG_MAX_LEN];
} HanTag_config_t;

#define DEFAULT_TAGGER_PATH		"../rsc/tagger"
/** 
 @brief 형태소 분석기 및 품사 부착 모듈을 초기화
 @param pStHanTagConf : 형태소 분석기 및 품사 부착기 환경 설정 정보
 @param HanTag_log : 형태소 분석기 및 품사 부착기 로그 핸들러
 @param HanTag : 형태소 분석 및 품사 부착기
 @return 0 : 정상처리 , -1 : 비정상 처리
 @see HanTag_config_t
 */
int HanTag_init( HanTag_config_t * pStHanTagConf, kulog_handler_t * HanTag_log, void ** HanTag );

/**
 @brief 한 문장을 입력받아 형태소 분석 및 품사 부착을 수행
 @param pStHanTagConf : 형태소 분석기 및 품사 부착기 환경 설정 정보
 @param HanTag : 형태소 분석과 품사태깅을 위한 객체
 @param Sentence : 형태소 분석 또는 품사 태깅할 문장을 저장한 버퍼 예) Sentence="나도 좋아"
 @param Wrd[] : 문장을 어절단위로 차례로 저장 예) Wrd[0]="나도", Wrd[1]="좋아"
 @param Result[] : 각 어절에 대한 형태소 분석 결과를 저장 예) Result[0]="나/NP+도/JX", Result[1]="좋/VA+아/EF"
 @return 형태소 분석을 수행한 어절의 수를 반환
 @see HanTag_config_t
 */ 
int HanTag_run( HanTag_config_t * pStHanTagConf, void *HanTag, char *Sentence, char *Wrd[MaxNumWrd], char *Result[MaxNumWrd][MaxNumAmb] );


/**
 @brief 사용자 정의 품사로 변환
**/
bool Convert2Usertag ( void *HanTag,  char Result[MaxNumAmb] );

/**
 @brief 형태소 분석기 및 품사 부착기 모듈을 마무리
 @param HanTag : 형태소 분석과 품사 태깅을 위한 객체
 @return 0 : 정상처리 , -1 : 비정상 처리
 */
int HanTag_finish(void * HanTag);

///////////////////////////////////////////////////////////////

#ifndef __TaggingMethod__
#define __TaggingMethod__
typedef enum {
  NO_TAGGING,           // 형태소 분석만
  PATH_BASED_TAGGING,   // 경로 기반 태깅
  STATE_BASED_TAGGING   // 상태 기반 태깅
} TaggingMethod;
#endif

// 파일에서 한 문장을 읽어들이기
int LoadSentenceFromStream(FILE *f, char *Sentence, int MaxLength);

// 스트링(*pString)에서 한 문장을 읽어들이기
// 문장을 읽은 후 *pString의 값은 다음 문장의 시작 위치로 옮겨짐
int LoadSentenceFromString(char **pString, char *Sentence, int MaxLength);

// HanTag 실행을 위한 사전 및 정보 읽기
// 반환값 : 1 - 성공, 0 - 실패
int LoadHanTagEngine(const char *ProbEntryFilename, const char *ProbDataFilename, 
								const char *PosEntryFilename, const char *PosDataFilename,
								const char *ExistEntryFilename, const char *ExistDataFilename
);

// 사전 및 정보를 위한 메모리 해제
void EndHanTagEngine(void);

// HanTag class의 instance를 할당 받기
// 반환값 : NULL - 실패, 기타 - 성공
void *CreateHanTag(void);

// HanTag class의 instance를 해제
void FreeHanTag(void *HanTag);

// 이미 할당 받은 HanTag의 instance를 이용하여 한 문장을 형태소 분석 및 태깅
// 반환값 : 문장내 어절의 개수
int DoKomaAndHanTag(void *HanTag, TaggingMethod Method, char *Sentence, char *Wrd[MaxNumWrd], char *Result[MaxNumWrd][MaxNumAmb], char bSEJONG='T');

// 이미 할당 받은 HanTag의 instance를 이용하여 한 문장을 형태소 분석 및 태깅
// 반환값 : 문장내 어절의 개수
int DoHanTag(void *HanTag, TaggingMethod Method, char *Cnd[MaxNumWrd][MaxNumAmb], char *Result[MaxNumWrd][MaxNumAmb]);

#endif
