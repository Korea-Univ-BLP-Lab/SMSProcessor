#ifndef _KSPACER_H_
#define _KSPACER_H_

//#include "definitions.h"
#include "kulog.h"

/* 모델의 타입을 구분하기 위해 */
typedef struct {
//  int type;   /* 기본 모델 or 다중 음절을 고려하는 모델 */
  int K;
  int J;
  int L;
  int I;
}
Model_Type;

int kspacer_init( kulog_handler_t *, char *trans_file, char *lexical_file);

int kspacer_run( char *input_sentence, char *new_sentence);


#endif
