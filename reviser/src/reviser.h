#ifndef __REVISER_H__
#define __REVISER_H__

#include "FST.h"
#include "han.h"
#include "HString.h"

#define		MAX_STR_LEN		1024

/**
 @brief 입력 문자열 안에 흩어진 한글 음소들을 조합한다.
 @param input 처리 대상 문자열(사용자 입력 문자열)
 @param output 처리 결과 문자열이 저장될 위치
 @return 없음
 @remarks	예) ㄱ ㅏ ㅈ ㅏ => 가자
  */
void combinePhoneme( const AString & input , char * output);

/**
 @brief 입력 문자열 안에 반복구를 제거한다.
 @param input 처리 대상 문자열(사용자 입력 문자열)
 @param output 처리 결과 문자열이 저장될 위치
 @return 없음
 @remarks	예) ㄱㄱㄱㄱㄱㄱㄱㄱ => ㄱㄱㄱ , 바이바이바이 => 바이
  */
void RemoveRep( const HString & input , char * output);

/**
 @brief 입력 문자열 안에 최장일치 FST 키 대신 FST value(fst_info)를 치환하여 output에 넣는다.
 @param fst FST key data
 @param fst_info FST value data
 @param input 처리 대상 문자열(사용자 입력 문자열)
 @param output 처리 결과 문자열이 저장될 위치
 @param msg 처리 결과 저장 구조체
 @return 없음
 @remarks	예) 넌 오디 가니? => 넌 어디 가니?  ( 적용 : 오디 => 어디 )
  */
void ApplyRule( const HString & input , char * output, int o1, int o2, int o3, int o4);
//void ApplyRule( FST * fst, char **fst_info , const HString & input , char * output);

/**
 @brief 입력 문자열 안에 음소가 있는지 알려준다.
 @param input 처리 대상 문자열(사용자 입력 문자열)
 @return true - 정상적인 메시지 , false - 무의미한 메시지�
 @remarks	예) ㅏ러ㅣㄷㄱ => false
  */
bool HaveHanPhoneme( const char *input );

/**
 @brief 태그 밖의 음소를 제거한다.
 @param input 처리 대상 문자열(사용자 입력 문자열)
 @return 없음.
 @remarks	예) 먹ㄴ나 [냉담:ㅡㅡㅋㅋ]ㅋ	=> 먹나 [냉담:ㅡㅡㅋㅋ]
  */
void RemoveHanPhoneme(char *str);
//--------------------------------내부 함수----------------------------------------
/**
 @brief 입력 문자열과 최장 일치하는 
  */
int LongestMatchHash( const AString & str, HString & key, char * value, int o1, int o2, int o3, int o4 );

/**
 @brief 조합형 코드를 조합하여 완성형 한글을 만든다. 종성은 생략 가능.
 @param first 초성
 @param second 중성
 @param third 종성(생략 가능)
 @return 만들어진 한글 1글자 문자열
  */
AString makeHan( const byte first, const byte second, const byte third = 1);

#endif
