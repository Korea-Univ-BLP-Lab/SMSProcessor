/** 전처리기(띄어쓰기+교정기) 모듈 기본 헤더 파일.
 	@file reviserAPI.h
 */
#ifndef __REVISER_API_H__
#define __REVISER_API_H__

#include "kulog.h"

// Config 자료구조 정의 --------------------------------------------------------------------
#define DRE_CONFIG_MAX_LEN 128 
  
 /**
   @brief 교정기 환경 설정 자료구조
   @remarks  교정 옵션 설정
   @remarks  자원 이용 옵션 설정
 */
typedef struct _reviser_config_t
{
	/**
	  @brief 이코티콘 교정
	  @remarks 이코티콘 교정 옵션 :on(1), off(0), default(0).
	  */	
	int emoticon;
	
	/**
	  @brief 반복구 처리
	  @remarks 반복구 처리 옵션 :on(1), off(0), default(0).
	  */
	int repeat;
	
	/**
	  @brief 욕설 교정
	  @remarks 욕설 교정 옵션 :on(1), off(0), default(0).
	  */
	int insult;
	
	/**
	  @brief 철자 교정
	  @remarks 철자 교정 옵션 :on(1), off(0), default(0).
	  */
	int speller;
	
	
	/**
	  @brief 음소 조합
	  @remarks 음소 조합 옵션 :on(1), off(0), default(0).
	  */	  
	int phoneme;
	
	/**
	  @brief 띄어쓰기 교정
	  @remarks 띄어쓰기 교정 옵션 :on(1), off(0), default(0).
	  */
	int spacer;
	
	/**
	  @brief 이모티콘 교정 사전
	  @remarks 이모티콘 교정 사전 옵션 : default ("../rsc/emoticon") 
	  */	
	char emoticon_dic[DRE_CONFIG_MAX_LEN];
	
	/**
	  @brief 반복구 복구 사전
	  @remarks 반복구 복구 사전 옵션 : default ("../rsc/repeat") 
	  */	
	char repeat_dic[DRE_CONFIG_MAX_LEN];
	
	/**
	  @brief 욕설 교정 사전
	  @remarks 욕설 교정 사전 옵션 : default ("../rsc/insult") 
	  */	
	char insult_dic[DRE_CONFIG_MAX_LEN];
	
	/**
	  @brief 철자 교정 사전
	  @remarks 철자 교정 사전 옵션 : default ("../rsc/speller") 
	  */	
	char speller_dic[DRE_CONFIG_MAX_LEN];
	
	/**
	  @brief 띄어쓰기 자원 경로
	  @remarks 띄어쓰기 자원 경로 옵션 : default ("../rsc") 
	  */	
	char spacer_path[DRE_CONFIG_MAX_LEN];

	/**
	  @brief 형태소 사전
	  @remarks 형태소 사전 옵션 : default ("../rsc") 
	  */	
	char morpheme_dic[DRE_CONFIG_MAX_LEN];
} reviser_config_t;

#define DEFAULT_EMOTICON_DIC	"../rsc/emoticon"
#define DEFAULT_REPEAT_DIC		"../rsc/repeat"
#define DEFAULT_INSULT_DIC		"../rsc/insult"
#define DEFAULT_SPELLER_DIC		"../rsc/speller"
#define DEFAULT_SPACER_PATH		"../rsc"
#define DEFAULT_MORPHEME_DIC	"../rsc/morpheme"



// message 자료구조 정의-----------------------------------------------------------------
/**
	@brief 최대 메시지 길이.
 */
#define		MAX_MSG_LEN		1024

#define		NUMBER_OF_TAGS	8		// 태그의 종류

#define		INSULT			0		// 욕설
#define		UNKNOWN			1		// 모름

#define		JOY				2		// 기쁨
#define		SAD				3		// 슬픔
#define		SURPRISE		4		// 놀람
#define		COLD			5		// 냉담
#define		LOVE			6		// 사랑
#define		OTHERS			7		// 기타

/**
 @brief 출력 자료구조.
 전처리를 수행한 결과물(문자열)을 저장한다
 */
typedef struct	_Message
{
	/**
	  @brief 메시지 원본.
	  @remarks 
	  사용자 입력 문자열.
	  @remarks
	  예) ㄷ ㅏㅅ ㅣ 시작하고 싶어 ㅋㅋㅋㅋㅋㅋㅋ
	  */
	char	orgstr[MAX_MSG_LEN];				// 메시지 원본
	/**
	  @brief 메시지 교정본.
	  @remarks 
	  오류교정/띄어쓰기 결과 문자열.
	  @remarks
	  예) 다시 시작하고 싶어 [기쁨:ㅋㅋㅋ]
	  */
	char	revstr[MAX_MSG_LEN];				// 메시지 교정본
	/**
	  @brief 태그를 걸러낸 교정본.
	  @remarks 
	  revstr에서 태그를 제거한 문자열.
	  @remarks
	  예) 다시 시작하고 싶어 
	  */
	char	fltstr[MAX_MSG_LEN];				// 메시지 교정본
	/**
	  @brief 태그 인식 결과.
	  @remarks 
		@remarks 	
		@remarks	- exp[INSULT]	: 욕설
		@remarks	- exp[UNKNOWN]	: 모름
		@remarks	- exp[JOY]	: 기쁨 			
		@remarks	- exp[SAD]	: 슬픔
		@remarks	- exp[SURPRISE] : 놀람
		@remarks	- exp[COLD]	: 냉담
		@remarks	- exp[LOVE]	: 사랑
		@remarks	- exp[OTHERS]	: 기타	  
		@remarks	인식결과 : 동일한 분류에 여러 개가 있을 때, 탭문자(\\t)로 구분한다.
		
		예)
		@remarks exp[JOY]	= "ㅋㅋㅋ"
		@remarks exp[SAD] = "엉엉\t흑흑"
		@remarks exp[SAD] = "--" 한줄한줄마다
	  */
	char    exp[NUMBER_OF_TAGS][MAX_MSG_LEN];
} Message;

// 함수 정의--------------------------------------------------------------------
/** 
 @brief 교정기 모듈, 띄어쓰기 교정모듈을 초기화한다.
 @param pStReviserConf : 교정기 환경 설정 structure.
 @param slh : log 핸들러.
 @return 0 : 정상처리 , -1 : 비정상 처리
 @see reviser_config_t
 */
int reviser_init( reviser_config_t * pStReviserConf, kulog_handler_t * slh );

/**
 @brief input의 문자열을 전처리해서 msg에 저장한다.
 @param pStReviserConf : 교정기 환경 설정 structure.
 @param input 처리 대상 문자열(사용자 입력 문자열)
 @param msg 처리 결과 저장 구조체
 @return 0 : 정상처리 , -1 : 비정상 처리
 @see reviser_config_t, Message
  */
int reviser_run( reviser_config_t * pStReviserConf, const char * input, Message * msg );

/**
 @brief 교정기 모듈, 띄어쓰기 모듈을 마무리한다..
 @param pStReviserConf : 교정기 환경 설정 structure.
 @return 0 : 정상처리
 @see reviser_config_t
 */
int reviser_finish( reviser_config_t * pStReviserConf );

#endif
