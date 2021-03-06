// DATE : 2004. 11. 6

#ifndef HSPLIT_H
#define HSPLIT_H

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_MJ 1024
#define MAX_WORD 1024 /* 분리할 단어의 최장 길이 */
#define MAX_SPLIT 100 /* 분리된 단어의 최대수 */

#define FIL (char) 127 /* 127:, 128:� : 채움 코드 */
#define FIL2 (char) 128 /* 127:, 128:� : 채움 코드 */

/* 완성형 */
#ifndef isHanja /* 한자 */
#define isHanja(str0, str1) ( (str0 >= 0xCA) && (str0 <= 0xFD) && (str1 >= 0xA1) && (str1 <= 0xFE) )
#endif
 
#ifndef isHangul /* 한글 */
#define isHangul(str0, str1) ( (str0 >= 0xB0) && (str0 <= 0xC8) && (str1 >= 0xA1) && (str1 <= 0xFE) )
#endif
 
#ifndef is2Byte /* 2byte Code */
#define is2Byte(str0, str1) ( (str0 >= 0xA1) && (str0 <= 0xAC) && (str1 >= 0xA1) && (str1 <= 0xFE) )
#endif


#define T_ENG_UPPER 0 /* english upper case */
#define T_ENG_LOWER 1 /* english lower case */
#define T_HAN       2 /* hangul */
#define T_HJ        3 /* hanja */
#define T_DIG       4 /* digit */
#define T_SYM       5 /* symbol */
#define T_2BSYM     6 /* 2byte symbol */
#define T_FIL      7 /* FIL */

typedef struct Word_type {
  char word[MAX_WORD];
  int type;
} word_type;

/* hword를 입력받아 word의 type에 따라 나눈 후 sword에 저장 */
/* return value : 분리된 word의 수 */
extern short split_by_word_type(word_type *sword, char *hword);

/* 주어진 문자열을 각 문자별로 나눈다. */
/* hword = 입력 문자열 */
/* splitchar = 결과 저장 */
/* 1 byte 문자는 앞에 FIL을 붙여서 2byte로 만든다. */
/* return value : 분리된 문자의 수 */
extern int split_by_char (char *hword, char splitchar[][3]);

/* 주어진 문자열을 각 문자별로 나눈다. */
/* hword = 입력 문자열 */
/* splitchar = 결과 저장 */
/* 1 byte 문자는 앞에 FIL을 제거 */
/* return value : 분리된 문자의 수 */
extern int split_by_char_origin (char *hword, char splitchar[][3]);

/* 주어진 문자열을 각 문자별로 나눈다. */
/* hword = 입력 문자열 */
/* splitchar = 결과 저장 */
/* return value : 성공 = 1 */
extern int split_by_char_array (char *hword, char *splitchar);

/* 주어진 문자열(source_str)을 1차 문자 배열로 된 문자열(target_str)로 변환 */
/* 1 byte 문자는 앞에 FIL을 유지 */
extern int convert_str(int num_char, char source_str[][3], char *target_str);

/* 주어진 문자열(source_str)을 1차 문자 배열로 된 문자열(target_str)로 변환 */
/* 1 byte 문자는 앞에 FIL을 제거 */
extern int convert_str_origin(int num_char, char source_str[][3], char *target_str);

/* 주어진 문자열(source_str)을 문자열(target_str)로 변환 */
/* 1 byte 문자는 앞에 FIL을 제거 */
extern int convert_str_origin_array(char *source_str, char *target_str);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
