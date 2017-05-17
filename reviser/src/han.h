#ifndef	_HAN_H_
#define	_HAN_H_

#include "hcc.h"

#define CHOSEONG_FILL_CODE	0x0400
#define JUNGSEONG_FILL_CODE	0x0040
#define JONGSEONG_FILL_CODE	0x0001

// �ѱ��ΰ�?
int IsHan(unsigned char *s);

// �ϼ��� �����κ��� �� �ڼ� ����
int GetGrapheme(short *choseong, short *jungseong, short *jongseong, unsigned char *syllable);

// ������ �����κ��� �� �ڼ� ����
int GetGraphemeJohap(short *choseong, short *jungseong, short *jongseong, unsigned char *syllable);

// ������ �ִ°�? (1: yes, 0: no)
int ExistJong(unsigned char *syll);


#endif
