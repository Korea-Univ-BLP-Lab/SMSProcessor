/* Filename	: hcc98.h */
/* Version	: 98 */
/* Discription	: external header of hcc.c */
/* Programmer	: Sang-Zoo Lee (Dept. of Computer Science, Korea University) */
/* Date		: 2 July 1998 */

#ifndef __HCC98_H__
#define __HCC98_H__

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

#endif
