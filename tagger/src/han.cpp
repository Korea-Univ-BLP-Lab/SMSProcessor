#include "hcc.h"

#define CHOSEONG_FILL_CODE	0x0400
#define JUNGSEONG_FILL_CODE	0x0040
#define JONGSEONG_FILL_CODE	0x0001

#define CHOSEONG_MASK	0x7c00
#define JUNGSEONG_MASK	0x03e0
#define JONGSEONG_MASK	0x001f

#define FILL_CHAR_JOHAP	(0x8000 | CHOSEONG_FILL_CODE | JUNGSEONG_FILL_CODE | JONGSEONG_FILL_CODE)
#define FILL_CHAR_JOHAP_1ST 0x84
#define FILL_CHAR_JOHAP_2ND 0x41

#define FILL_CHAR	0xa4d4
#define FILL_CHAR_1ST 0xa4
#define FILL_CHAR_2ND 0xd4

int IsHan(unsigned char *s)
{
	return (*s>=0xb0 && *s<=0xc8 && *(s+1)>=0xa0);
}

extern void ks2tg (byte *ks , byte *tg);

// 완성형 음절로부터 각 자소 추출
int GetGrapheme(short *choseong, short *jungseong, short *jongseong, unsigned char *syllable)
{
	short syllable_value;
	unsigned char syllableTG[3];

	ks2tg(syllable, syllableTG);

	syllable_value=syllableTG[0]*256+syllableTG[1];
	*choseong=syllable_value & CHOSEONG_MASK; 
	*jungseong=syllable_value & JUNGSEONG_MASK;
	*jongseong=syllable_value & JONGSEONG_MASK;
	
	return 1;
}

// 조합형 음절로부터 각 자소 추출
int GetGraphemeJohap(short *choseong, short *jungseong, short *jongseong, unsigned char *syllable)
{
	short syllable_value;

	syllable_value=syllable[0]*256+syllable[1];
	*choseong=syllable_value & CHOSEONG_MASK; 
	*jungseong=syllable_value & JUNGSEONG_MASK;
	*jongseong=syllable_value & JONGSEONG_MASK;
	
	return 1;
}


// 종성이 있는가? (1: yes, 0: no)
int ExistJong(unsigned char *syll)
{
	short cho, jung, jong;
	GetGrapheme(&cho, &jung, &jong, syll);
	return !(jong == JONGSEONG_FILL_CODE);
}
