/*
   Phoneme-type hangul code processor for morphological analysis.
   */

#include <stdio.h>
#include <string.h>

#include "ph.h"

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
    short len = strlen(str);
    str [0] = (char) (0x80 | ((phi & 0x1f) << 2) | ((phm & 0x18) >> 3)); 
    str [1] = (char) (((phm & 0x7) << 5) | (phf & 0x1f));
    if (len<2) str [2] = (char)NULL;
}
