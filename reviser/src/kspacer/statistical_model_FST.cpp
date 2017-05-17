#include <stdio.h>
#include <stdlib.h>

#include "get_probability_FST.h"
#include "hsplit.h"
#include "common.h"
#include "viterbi.h"
#include "spacer_FST.h"  /* extern Model_Type model_type; 선언을 위해 */
#include "kulog_kspacer.h"

extern Model_Type model_type;

extern double syll_prob[MAX_MJ][2][2][2];
extern double ws_trans_prob[MAX_MJ][2][2][2];

extern void get_all_syllable_probability_FST(PROB_MAP_FST &syllable_prob, char splitchar[][3], int total_time, int total_class);
extern void get_all_ws_transition_probability_FST(PROB_MAP_FST &ws_transition_prob, char splitchar[][3], int total_time, int total_class);

/*
void restore_alpha_space( char *input_sentence, char *new_sentence )
{

	char restore_sentence[4096];
	int restore_sentence_len = 0;
	restore_sentence[0] = '\0';
	
	size_t input_len = strlen( input_sentence );
	size_t new_len = strlen( new_sentence );
	
	size_t alpha_start_idx=0;	// 교정문의 알파벳 시작 인덱스
	size_t alpha_start_nth=0; // 교정문의 알파벳 시작 글자의 번호
	size_t alpha_end_idx=0;
	size_t alpha_end_nth=0;
	size_t nth_char = 0;
	
	size_t input_nth_char = 0;
	size_t input_pos = 0;
	

	size_t i=0;
	while(1)	{
			if( i>= new_len )	break;


			// 한글
			if( new_sentence[i] < 0 )	{
				restore_sentence[restore_sentence_len] = new_sentence[i];
				restore_sentence_len++;
				i++;
				restore_sentence[restore_sentence_len] = new_sentence[i];
				restore_sentence_len++;
				i++;
				nth_char++;

			}
			// 아스키
			else	{
				
				
				// 알파벳 시작 지점
				if( (new_sentence[i] >= 'a' && new_sentence[i] <= 'z') ||
					  (new_sentence[i] >= 'A' && new_sentence[i] <= 'Z')  )	{

					  	// 해당하는 입력문의 위치로 이동
					  	while( input_nth_char < nth_char )	{
								if( input_sentence[input_pos] < 0 )	{
									input_pos+=2;
								}
								else	{
									input_pos++;
								}
								input_nth_char++;

					  	}



					  	// 입력문에서 알파벳이 계속 될때까지 복사
					  	for(; input_pos<input_len; input_pos++ )	{
					  			if( (input_sentence[input_pos] >= 'a' && input_sentence[input_pos] <= 'z') ||
					  					(input_sentence[input_pos] >= 'A' && input_sentence[input_pos] <= 'Z')  )	{
					  						input_nth_char++;

											restore_sentence[restore_sentence_len] = input_sentence[input_pos];
											restore_sentence_len++;

					  			}
					  			else	{
					  				break;
					  			}
					  	}


					  	// 해당하는 교정문에 위치를 이동
					  	while( nth_char < input_nth_char )	{
					  		if( new_sentence[i] < 0 )	{
								nth_char++;
					  			i+=2;
								
					  		}
					  		else	{
								if( new_sentence[i] != ' ' )	{
									nth_char++;
								}
								i++;

								
					  		}
					  		
					  	}

//break;					  	

				}
				// 특수기호, 공백 등
				else	{
					restore_sentence[restore_sentence_len] = new_sentence[i];
					restore_sentence_len++;
					i++;
					nth_char++;
				}
				
			}
	
	}
	restore_sentence[restore_sentence_len] = '\0';
	strcpy( new_sentence, restore_sentence );

	
}
*/

/*****************************************************************************/
/* 띄어쓰기 교정 */
/*****************************************************************************/
int statistical_spacer_FST(PROB_MAP_FST &ws_transition_prob2, PROB_MAP_FST &syllable_prob2, 
                       char *input_sentence, char *new_sentence) {

  char splitchar[MAX_MJ][3]; /* 어절에서 분리된 개개의 문자를 모두 2바이트로 변환하여 저장 */
  int num_splitchar = 0;
  int tag_sequence[MAX_MJ] = { 0, }; /* 띄어쓰기 태그열 */

  int gram_num; /* 띄어쓰기 태그에 대해 unigram, bigram, trigram 결정 */

#ifdef MAX_MSG_LEN
 char org[MAX_MSG_LEN];
#else
char org[1024];
#endif
sprintf( org, "%s", input_sentence );

	// if (time(NULL) > 1167577199) { KSPACER_LOG_ERR(kulog_reviser, "Error : Expired, http://nlp.korea.ac.kr"); return -1; }

  model_type.K = 2;
  model_type.J = 2;
  model_type.L = 1;
  model_type.I = 2;

  new_sentence[0] = (char) NULL;

  /* 입력문장(실제 입력단위는 어절)을 각 문자별로 나눈다. */
  /* 나누어진 문자의 수는 num_splitchar에 저장된다. */
  num_splitchar = split_by_char(input_sentence, &splitchar[2]);

  strcpy (splitchar[0], BOS_SYL_2);   /* 문장 시작 표시 */
  strcpy (splitchar[1], BOS_SYL_1);   /* 문장 시작 표시 */

  gram_num = (model_type.J > model_type.I) ? (model_type.J + 1) : (model_type.I + 1);
  // assert( gram_num == 3 );


  /*****************************************************************************/
  /* 음절 발생 확률 */
  get_all_syllable_probability_FST(syllable_prob2, splitchar, num_splitchar, 2);

  /* 띄어쓰기 태그 전이 확률 */
  get_all_ws_transition_probability_FST(ws_transition_prob2, splitchar, num_splitchar, 2);

  /*****************************************************************************/
  switch (gram_num) {

  case 3: /* 이전 두 상태를 고려함 (trigram) */
  	trigram_viterbi_search(splitchar, num_splitchar, 2, tag_sequence);
    break;

  case 2: /* 이전 하나의 상태를 고려함 (bigram) */
    bigram_viterbi_search(splitchar, num_splitchar, 2, tag_sequence);
    break;

  case 1: /* 이전 상태를 고려하지 않음 */
    unigram_viterbi_search(splitchar, num_splitchar, 2, tag_sequence);
    break;

  }

  /* 최종 결과 생성 */
  make_result_string (splitchar, num_splitchar, tag_sequence, new_sentence);

#ifdef KU_RESTORE_ALPHA_SPACE
 // restore_alpha_space( org, new_sentence );
#endif
  KSPACER_LOG_SVC(kulog_reviser, "kspacer complete :%s\n",new_sentence);

  return 1;
}



/*****************************************************************************/
/* 음절 발생 확률 */
void get_all_syllable_probability_FST(PROB_MAP_FST &syllable_prob2, 
                                  char splitchar[][3], int total_time, int total_class) {

  int i, j;
  int k, l;
  int start_time = 2;
  int end_time = total_time+1;

#ifdef _DEBUG_ /********************************************************/
  KSPACER_LOG_DBG(kulog_reviser, "model type = [g,%d,%d,%d,%d]\n", model_type.K, model_type.J, model_type.L, model_type.I);
#endif /********************************************************/

  for (i = start_time; i <= end_time; i++) { /* 각 time(token) 마다 */
    for (k = 0; k < total_class; k++) {       /* 이전 각 class(tag) 마다 */
      for (l = 0; l < total_class; l++) {     /* 이전 두 번째 각 class(tag) 마다 */


        syll_prob[i][0][k][l] = get_probability4_FST(syllable_prob2, 
                                     splitchar[i], // 현재 음절(분자)
                                     splitchar[i-2], // 이이전 음절
                                     STAG[l], // 이이전 태그
                                     splitchar[i-1], // 이전 음절
                                     STAG[k]  // 이전 태그
                                     );

        for (j = 0; j < total_class; j++) {   /* 각 class(tag) 마다 */
          syll_prob[i][j][k][l] = syll_prob[i][0][k][l];
        }
      }
    }
  }
}



/*****************************************************************************/
/* 띄어쓰기 태그 전이 확률 */
void get_all_ws_transition_probability_FST(PROB_MAP_FST &ws_transition_prob2, 
                                    char splitchar[][3], int total_time, int total_class) {

  int i, j;
  int k, l;
  int start_time = 2;
  int end_time = total_time+1;

#ifdef _DEBUG_ /********************************************************/
  KSPACER_LOG_DBG(kulog_reviser, "model type = [g,%d,%d,%d,%d]\n", model_type.K, model_type.J, model_type.L, model_type.I);
#endif /********************************************************/

  for (i = start_time; i <= end_time; i++) { /* 각 time(token) 마다 */
    for (j = 0; j < total_class; j++) {       /* 각 class(tag) 마다 */
      for (k = 0; k < total_class; k++) {     /* 이전 각 class(tag) 마다 */
        for (l = 0; l < total_class; l++) {   /* 이전 두 번째 각 class(tag) 마다 */

          ws_trans_prob[i][j][k][l] = get_probability4_FST(ws_transition_prob2, 
                                     STAG[j], // 현재 태그(분자)

                                     STAG[l], // 이이전 태그
                                     splitchar[i-1], // 이전 음절
                                     STAG[k],  // 이전 태그
                                     splitchar[i] // 현재 음절
                                     );
        }
      }
    }
  }
}

/*****************************************************************************/

