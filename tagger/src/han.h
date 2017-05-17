#ifndef	_HAN_H_
#define	_HAN_H_

#include "hcc.h"

#define CHOSEONG_FILL_CODE	0x0400
#define JUNGSEONG_FILL_CODE	0x0040
#define JONGSEONG_FILL_CODE	0x0001

// 한글인가?
int IsHan(unsigned char *s);

// 완성형 음절로부터 각 자소 추출
int GetGrapheme(short *choseong, short *jungseong, short *jongseong, unsigned char *syllable);

// 조합형 음절로부터 각 자소 추출
int GetGraphemeJohap(short *choseong, short *jungseong, short *jongseong, unsigned char *syllable);

// 종성이 있는가? (1: yes, 0: no)
int ExistJong(unsigned char *syll);


#endif
