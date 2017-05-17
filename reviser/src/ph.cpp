/* Phoneme-type hangul code processor for morphological analysis. */
/* DATE : 8/Oct/2001 */
/* modified by dglee */

#include <stdio.h>
#include <string.h>

#include "ph.h"
#include "hcc.h"

/* 문자열의 초성을 얻어내기 */
unsigned char GetInitialPhoneme(char *str) {
  unsigned char* tg = (unsigned char*)str;
  unsigned char ph;
  ph = (tg[0] >> 2) & 0x1f;
  return ph;
}

/* 문자열의 중성을 얻어내기 */
unsigned char GetMedialPhoneme(char *str) {
  unsigned char* tg = (unsigned char*)str;
  unsigned char ph;
  ph = ((tg[0] << 3) & 0x18) | ((tg[1] >> 5) & 0x7);
  return ph;
}

/* 문자열의 종성을 얻어내기 */
unsigned char GetFinalPhoneme(char *str) {
  unsigned char* tg = (unsigned char*)str;
  unsigned char ph;
  ph = (tg[1] & 0x1f);
  return ph;
}

/* added by dglee */
/* 완성형 음절의 종성을 얻어내기 */
unsigned char GetFinalPhoneme_KS(char *str) {
	unsigned char tg[4] = {0, }; /* 조합형 음절 */
	unsigned char ph;
	char tmp[4] = {0, };
	strncpy(tmp, str, 2);

	ks2tg((unsigned char*) tmp, tg);/* 조합형으로 변환 */

	ph = (tg[1] & 0x1f);
	return ph;
}
/*
 * 완성형 음절의 초성을 얻어내기
 * @author Lee, Joo-Young (trowind_at_gmail.com)
 * @version 0.1.0
 * @date 2005/03/15
 */
unsigned char GetInitialPhoneme_KS(const char *str)
{
	unsigned char tg[4] =  {0, };
	unsigned char ph;
	char tmp[4] = {0, };
	strncpy(tmp, str, 2);
	
	ks2tg((unsigned char*) tmp, tg);
	
	ph = (tg[0] >> 2) & 0x1f;
	return ph;
}

/*
 * 완성형 음절의 중성을 얻어내기
 * @author Lee, Joo-Young (trowind_at_gmail.com)
 * @version 0.1.0
 * @date 2005/03/15
 */
unsigned char GetMedialPhoneme_KS(const char *str)
{
	unsigned char tg[3] =  {0, };
	unsigned char ph;
	char tmp[4] = {0, };
	strncpy(tmp, str, 2);
	
	ks2tg((unsigned char*) tmp, tg);
	
	ph = ((tg[0] << 3) & 0x18) | ((tg[1] >> 5) & 0x7);

	return ph;
}


/* 조합형 코드 문자열을 자소형 코드 문자열로 변환. */
void ConvertToPhoneme (char* str, tPH* ph) 
{
  unsigned char* tg = (unsigned char*)str;
  ph->i = (tg[0] >> 2) & 0x1f;
  ph->m = ((tg[0] << 3) & 0x18) | ((tg[1] >> 5) & 0x7);
  ph->f = (tg[1] & 0x1f);
}

/* 자소형 코드 문자열을 조합형 코드 문자열로 변환. */
void ConvertToTrigem (char* str, tPH* ph)
{
  str[0] = (char) (0x80 | (ph->i << 2) | ((ph->m & 0x18) >> 3)); 
  str[1] = (char) (((ph->m & 0x7) << 5) | ph->f);
}

/* Check if its medial phoneme (vowel) is positive vowel. */
short IsPositiveVowel (unsigned char phm) 
{ 
  return (short)(phm == (unsigned char)PHM_A || phm == (unsigned char)PHM_O); 
}

/* Copy a given syllable to the string. */
void CopySyllable (char* str, unsigned char phi, unsigned char phm, unsigned char phf) 
{
  short len = (short) strlen(str);
  str [0] = (char) (0x80 | ((phi & 0x1f) << 2) | ((phm & 0x18) >> 3)); 
  str [1] = (char) (((phm & 0x7) << 5) | (phf & 0x1f));
  if (len<2) str [2] = (char)NULL;
}
