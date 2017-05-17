#ifndef _SPACER_FST_H_
#define _SPACER_FST_H_

#include "definitions.h"

#ifndef _DEBUG_
//#define _DEBUG_ /* for debugging */
#endif

/* 모델의 타입을 구분하기 위해 */
typedef struct {
//  int type;   /* 기본 모델 or 다중 음절을 고려하는 모델 */
  int K;
  int J;
  int L;
  int I;
}
Model_Type;

extern Model_Type model_type;
extern int spacer_open_FST(char *trans_file, char *lexical_file, 
                       PROB_MAP_FST &ws_transition_prob, PROB_MAP_FST &syllable_prob);                                              

extern int statistical_spacer_FST (PROB_MAP_FST &ws_transition_prob, PROB_MAP_FST &syllable_prob, char *input_sentence, char *new_sentence);

#endif
