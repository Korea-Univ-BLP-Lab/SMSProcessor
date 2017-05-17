#ifndef __GET_PROBABILITY_FST_H__
#define __GET_PROBABILITY_FST_H__

#include "definitions.h"

/*typedef struct
{
  char str[13];                 // 부분 문자열
  int freq;                     // 빈도
}
struct_split_gram;
*/
//extern double get_probability42(PROB_MAP2 &probs, string num, string t1, string t2, string t3, string t4);


extern double get_probability4_FST(PROB_MAP_FST &probs, char *num, char *t1, char *t2, char *t3, char *t4);

#endif
