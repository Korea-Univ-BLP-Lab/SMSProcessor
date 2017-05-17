/* Filename	: hcc.h */
/* Version	: 02 (2002) */
/* Discription	: external header of hcc.c */
/* Programmer	: Sang-Zoo Lee (Dept. of Computer Science, Korea University) */
/* Date		: 2 July 1998 */
/* Modifier	: Won-Ho Ryu, Do-Gil Lee */
/* Date		: 19 October 2001 - 확장 완성형 변환 코드 추가 */
/* Modifier	: Sang-Zoo Lee */
/* Date		: 19 January 2002 - 코드 정리 및 완성형 문자 집합 출력 함수 추가 */
/* Modifier	: Sang-Zoo Lee */
/* Date		: 06 February 2002 - 완성형 음절 2-그램 출력 함수 추가 */

#ifndef __HCC_H__
#define __HCC_H__

#include "stdio.h"

typedef unsigned char	byte;

/* KS 코드 문자열을 TG 코드 문자열로 변환한다.
    ks : KS 문자열 
    tg : TriGem 문자열 */
extern void ks2tg (byte *ks , byte *tg);

/* TG 코드 문자열을 KS 코드 문자열로 변환한다.
    tg : TriGem 문자열
    ks : KS 문자열 */
extern void tg2ks (byte *tg , byte *ks);

/* KS 코드 입력 스트림을 TG 코드 출력 스트림으로 변환한다.
    ks_in : KS 코드 입력 스트림 
    tg_out : TG 코드 출력 스트림 */
extern void stream_ks2tg (FILE *ks_in , FILE *tg_out);

/* TG 코드 입력 스트림을 KS 코드 출력 스트림으로 변환한다.
    tg_in : TG 코드 입력 스트림 
    ks_out : KS 코드 출력 스트림 */
extern void stream_tg2ks (FILE *tg_in , FILE *ks_out);

/* 상용 완성형 문자 집합을 완성형 코드로 출력 */
extern void show_all_KS_set_in_KS_CODE (FILE* outfp);

/* 상용 완성형 문자 집합을 조합형 코드로 출력 */
extern void show_all_KS_set_in_TG_CODE (FILE* outfp);

/* 상용 완성형 음절 2-그램을 완성형 코드로 출력 */
extern void show_all_syllable_2_gram_in_KS_CODE (FILE* outfp);

/* 상용 완성형 음절 2-그램을 조합형 코드로 출력 */
extern void show_all_syllable_2_gram_in_TG_CODE (FILE* outfp);

#endif
