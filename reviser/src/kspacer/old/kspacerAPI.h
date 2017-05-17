#ifndef _KSPACER_H_
#define _KSPACER_H_

//#include "definitions.h"
#include "kulog.h"

/* ���� Ÿ���� �����ϱ� ���� */
typedef struct {
//  int type;   /* �⺻ �� or ���� ������ ����ϴ� �� */
  int K;
  int J;
  int L;
  int I;
}
Model_Type;

int kspacer_init( kulog_handler_t *, char *trans_file, char *lexical_file);

int kspacer_run( char *input_sentence, char *new_sentence);


#endif
