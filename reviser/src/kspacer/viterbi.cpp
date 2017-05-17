#include <stdio.h>
#include <math.h>
#include <limits.h>
#include "hsplit.h" /* MAX_MJ */
#include "kulog_kspacer.h"

/* Foundations of Statistical Natural Language Processing
   p.350을 참조함 */

double syll_prob[MAX_MJ][2][2][2];
double ws_trans_prob[MAX_MJ][2][2][2];


/*****************************************************************************/
void unigram_viterbi_search(char splitchar[][3], int total_time, int total_class, int *state_sequence) {
  int i;
  int j, k;

  /* 2부터 total_time+1까지 저장 */
#ifdef WIN32
  double SeqPrb[100000][3];
  int SeqState[100000][3];
#else
  double SeqPrb[total_time + 1][total_class];
  int SeqState[total_time + 1][total_class];
#endif
  
  int max_prev_state=0;
  double max_prev_prob, cur_prev_prob;

  double cur_prob;

  /* 초기화 */
  for (i = 1; i < total_time + 1; i++)
    for (j = 0; j < total_class; j++)
      SeqPrb[i][j] = -LONG_MAX;

  SeqPrb[0][0] = 0.0;           /* log(1.0) */
  SeqPrb[0][1] = 0.0;

  /* Iteration Step */
  for (i = 1; i <= total_time; i++) {   /* 각 time(token) 마다 */
    for (j = 0; j < total_class; j++) { /* 각 class(tag) 마다 */

      /* 이전 상태까지의 최대 확률값과 경로 찾기 */
      max_prev_prob = -LONG_MAX;       /*log(0.0) *//* 초기화 */
      max_prev_state = 0;       /* 초기화 */

#ifdef _DEBUG_ /********************************************************/
      SPACER_LOG_DBG(kulog, "-----------------------------\n");
      SPACER_LOG_DBG(kulog, "get_probability(%s[%d], cur_t = %d)\n", splitchar[i], i, j);
#endif /********************************************************/

      /* 확률 갱신 부분 (가장 중요함 <- 사용하는 모델에 따라 수정해야 함) */
      /* bigram과 다른 점: 중복 연산을 피하기 위해 이 부분에 둠 */
      /* 이전 상태에 대해서는 영향을 받지 않기 때문 */
      cur_prob = log (syll_prob[i][j][0][0]) + log (ws_trans_prob[i][j][0][0]);

#ifdef _DEBUG_ /********************************************************/
       /**/ SPACER_LOG_DBG(kulog, "\n");
#endif /********************************************************/

      /* 최대값 찾기 */
      for (k = 0; k < total_class; k++) {
        cur_prev_prob = SeqPrb[i - 1][k] + cur_prob;

#ifdef _DEBUG_ /********************************************************/
        SPACER_LOG_DBG(kulog, "SeqPrb[%d][%d] * cur_prob = %lf\n", i - 1, k, cur_prev_prob);
#endif /********************************************************/

        if (max_prev_prob < cur_prev_prob) {
          max_prev_prob = cur_prev_prob;
          max_prev_state = k;
        }
      }
      SeqPrb[i][j] = max_prev_prob;
      SeqState[i][j] = max_prev_state;

#ifdef _DEBUG_ /********************************************************/
      SPACER_LOG_DBG(kulog, "MAX value : SeqPrb[%d][%d] = %lf, SeqState[%d][%d] = %d\n", i, j, SeqPrb[i][j], i, j, SeqState[i][j]);
#endif /********************************************************/
    }  /* end of for, 각 class(tag) 마다 */
  }    /* end of for, 각 time(token) 마다 */

  /* Termination and path-readout */
  max_prev_prob = -LONG_MAX;   /* 초기화 */
  for (i = 0; i < total_class; i++) {
    cur_prev_prob = SeqPrb[total_time][i];
    if (max_prev_prob < cur_prev_prob) {
      max_prev_prob = cur_prev_prob;
      max_prev_state = i;
    }
  }

  /* 경로 저장 */
  /* state_sequence에 1부터 total_time까지 저장됨 */
  state_sequence[total_time] = max_prev_state;

#ifdef _DEBUG_ /********************************************************/
  SPACER_LOG_DBG(kulog, "state_sequence[total_time] = %d\n", state_sequence[total_time]);
#endif /********************************************************/

  for (i = total_time - 1; i >= 1; i--) {
    state_sequence[i] = SeqState[i + 1][state_sequence[i + 1]];
  }

#ifdef _DEBUG_ /********************************************************/
  SPACER_LOG_DBG(kulog, "띄어쓰기 태그열\n");
  for (i = 1; i < total_time; i++) {    /* 마지막 음절의 띄어쓰기는 무조건 1이라고 간주하므로 total_time은 포함하지 않음 */
    SPACER_LOG_DBG(kulog, "%d", state_sequence[i]);
  }
  SPACER_LOG_DBG(kulog, "\n");
#endif /********************************************************/
}


/*****************************************************************************/
void bigram_viterbi_search(char splitchar[][3], int total_time, int total_class, int *state_sequence) {
  int i;
  int j, k;

  /* 2부터 total_time+1까지 저장 */
#ifdef WIN32
  double SeqPrb[100000][3];
  int SeqState[100000][3];
#else
  double SeqPrb[total_time + 1][total_class];
  int SeqState[total_time + 1][total_class];
#endif
  
  int max_prev_state=0;
  double max_prev_prob, cur_prev_prob;

  /* 초기화 */
  for (i = 1; i < total_time + 1; i++)
    for (j = 0; j < total_class; j++)
      SeqPrb[i][j] = -LONG_MAX;

  SeqPrb[0][0] = 0.0;           /* log(1.0) */
  SeqPrb[0][1] = 0.0;

  /* Iteration Step */
  for (i = 1; i <= total_time; i++) {   /* 각 time(token) 마다 */
    for (j = 0; j < total_class; j++) { /* 각 class(tag) 마다 */

      /* 이전 상태까지의 최대 확률값과 경로 찾기 */
      max_prev_prob = -LONG_MAX;       /*log(0.0) *//* 초기화 */
      max_prev_state = 0;       /* 초기화 */

      /* 최대값 찾기 */
      /* k는 이전 상태의 태그 */
      for (k = 0; k < total_class; k++) {
        /* 확률 갱신 부분 (가장 중요함 <- 사용하는 모델에 따라 수정해야 함) */

#ifdef _DEBUG_ /********************************************************/
        SPACER_LOG_DBG(kulog, "-----------------------------\n");
        SPACER_LOG_DBG(kulog, "get_probability(%s[%d], cur_t = %d, prev_t = %d)\n", splitchar[i], i, j, k);
#endif /********************************************************/

        cur_prev_prob = SeqPrb[i - 1][k] + log(syll_prob[i][j][k][0]) + log(ws_trans_prob[i][j][k][0]);

#ifdef _DEBUG_ /********************************************************/
        SPACER_LOG_DBG(kulog, "SeqPrb[%d][%d] * cur_prob = %lf\n", i - 1, k, cur_prev_prob);
#endif /********************************************************/

        if (max_prev_prob < cur_prev_prob) {
          max_prev_prob = cur_prev_prob;
          max_prev_state = k;
        }
      }
      SeqPrb[i][j] = max_prev_prob;
      SeqState[i][j] = max_prev_state;

#ifdef _DEBUG_ /********************************************************/
      SPACER_LOG_DBG(kulog, "SeqPrb[%d][%d] = %lf, SeqState[%d][%d] = %d\n", i, j, SeqPrb[i][j], i, j, SeqState[i][j]);
#endif /********************************************************/
    }
  }

  /* Termination and path-readout */
  max_prev_prob = -LONG_MAX;   /* 초기화 */
  for (i = 0; i < total_class; i++) {
    cur_prev_prob = SeqPrb[total_time][i];
    if (max_prev_prob < cur_prev_prob) {
      max_prev_prob = cur_prev_prob;
      max_prev_state = i;
    }
  }

  /* 경로 저장 */
  /* state_sequence에 1부터 total_time까지 저장됨 */
  state_sequence[total_time] = max_prev_state;

#ifdef _DEBUG_ /********************************************************/
  SPACER_LOG_DBG(kulog, "state_sequence[total_time] = %d\n", state_sequence[total_time]);
#endif /********************************************************/

  for (i = total_time - 1; i >= 1; i--) {
    state_sequence[i] = SeqState[i + 1][state_sequence[i + 1]];
  }

#ifdef _DEBUG_ /********************************************************/
  SPACER_LOG_DBG(kulog, "띄어쓰기 태그열\n");
  for (i = 1; i < total_time; i++) {
    SPACER_LOG_DBG(kulog, "%d", state_sequence[i]);
  }
  SPACER_LOG_DBG(kulog, "\n");
#endif /********************************************************/
}



/*****************************************************************************/
void trigram_viterbi_search(char splitchar[][3], int total_time, int total_class,
                            int *state_sequence) {
  int i;
  int j, k, l;

  int start_time = 2; /* 시작 위치 */
  int end_time = total_time+1; /* 끝 위치 */

#ifdef WIN32
  double SeqPrb[100000][2][2];
  int SeqState[100000][2][2];

  int max_prev_state[2];
  double max_prev_prob[2];
  double cur_prev_prob[2];
#else
  /* 3부터 total_time+2까지 저장 */
  double SeqPrb[total_time+3][total_class][total_class];
  int SeqState[total_time+3][total_class][total_class];

  int max_prev_state[total_class];
  double max_prev_prob[total_class];
  double cur_prev_prob[total_class];
#endif

  /* 초기화 */
  for (i = 2; i < total_time + 2; i++)
    for (j = 0; j < total_class; j++)
      for (l = 0; l < total_class; l++)
        SeqPrb[i][j][l] = -LONG_MAX;   /* log(0.0) *//* 초기화 */

  /* 첫 time에 대해 초기화 */
  SeqPrb[1][0][0] = 0.0;        /* log(1.0) */
  SeqPrb[1][0][1] = 0.0;
  SeqPrb[1][1][0] = 0.0;
  SeqPrb[1][1][1] = 0.0;

  /* Iteration Step */
  for (i = start_time; i <= end_time; i++) {   /* 각 time(token) 마다 */
    for (j = 0; j < total_class; j++) { /* 각 class(tag) 마다 */

      /* 초기화   */
      max_prev_prob[0] = -LONG_MAX; /* log(0.0) */
      max_prev_prob[1] = -LONG_MAX; 

      max_prev_state[0] = 0;    /* 초기화 */
      max_prev_state[1] = 0;    /* 초기화 */

      /* 이전 상태까지의 최대 확률값과 경로 찾기 */
      /* 최대값 찾기 */
      for (k = 0; k < total_class; k++) { /* 이전 상태 */
        for (l = 0; l < total_class; l++) { /* 이이전 상태 */

          /* 확률 갱신 부분 (가장 중요함 <- 사용하는 모델에 따라 수정해야 함) */

#ifdef _DEBUG_ /********************************************************/
          SPACER_LOG_DBG(kulog, "-----------------------------\n");
          SPACER_LOG_DBG(kulog, "get_probability(%s[%d], cur_t = %d, prev_t = %d, pprev_t = %d)\n", splitchar[i], i, j, k, l);
#endif /********************************************************/
          cur_prev_prob[k] = 
              SeqPrb[i - 1][k][l]
              + log (syll_prob[i][j][k][l])
              + log (ws_trans_prob[i][j][k][l]);

#ifdef _DEBUG_ /********************************************************/
          SPACER_LOG_DBG(kulog, "SeqPrb[%d][%d][%d](%lf) + syllable prob(%lf) + transition prob(%lf) = %lf\n", 
                        i-1, k, l, SeqPrb[i-1][k][l], 
                        log(syll_prob[i][j][k][l]), 
                        log(ws_trans_prob[i][j][k][l]), cur_prev_prob[k]);
#endif /********************************************************/

          if (max_prev_prob[k] < cur_prev_prob[k]) {
            max_prev_prob[k] = cur_prev_prob[k];
            max_prev_state[k] = l;
          }
        }  /* for (l = 0; l < total_class; l++) */
      }    /* for (k = 0; k < total_class; k++) */

      SeqPrb[i][j][0] = max_prev_prob[0];
      SeqPrb[i][j][1] = max_prev_prob[1];
      SeqState[i][j][0] = max_prev_state[0];
      SeqState[i][j][1] = max_prev_state[1];

#ifdef _DEBUG_ /********************************************************/
      if (SeqPrb[i][j][0] == -LONG_MAX)
        SPACER_LOG_DBG(kulog, "\nSeqPrb[%d][%d][0] = (absolutely) 0, SeqState[%d][%d][0] = %d\n", i, j, i, j, SeqState[i][j][0]);
      else SPACER_LOG_DBG(kulog, "\nSeqPrb[%d][%d][0] = %lf, SeqState[%d][%d][0] = %d\n", i, j, SeqPrb[i][j][0], i, j, SeqState[i][j][0]);

      if (SeqPrb[i][j][1] == -LONG_MAX)
        SPACER_LOG_DBG(kulog, "SeqPrb[%d][%d][1] = (absolutely) 0, SeqState[%d][%d][1] = %d\n", i, j, i, j, SeqState[i][j][1]);
      else SPACER_LOG_DBG(kulog, "SeqPrb[%d][%d][1] = %lf, SeqState[%d][%d][1] = %d\n", i, j, SeqPrb[i][j][1], i, j, SeqState[i][j][1]);
#endif /********************************************************/

    } /* for (j = 0; j < total_class; j++) */
  }   /* for (i = 1; i <= total_time; i++) */

  /* Termination and path-readout */
  {  /* start of block */
    double cur_prev_prob;
    int max_cur_state = 0;

    int max_prev_state = 0;
    double max_prev_prob = -LONG_MAX;  /* 초기화 */

    for (i = 0; i < total_class; i++) {
      for (j = 0; j < total_class; j++) {
        cur_prev_prob = SeqPrb[end_time][i][j];

        if (max_prev_prob < cur_prev_prob) {
          max_prev_prob = cur_prev_prob;
          max_cur_state = i;
          max_prev_state = j;
        }
      }  /* for (j = 0; j < total_class; j++) */
    }    /* for (i = 0; i < total_class; i++) */

    /* 경로 저장 */
    /* state_sequence에 1부터 total_time까지 저장됨 */
    state_sequence[end_time] = max_cur_state;

#ifdef _DEBUG_ /********************************************************/
    SPACER_LOG_DBG(kulog, "state_sequence[end_time] = %d\n", state_sequence[end_time]);
#endif /********************************************************/

    for (i = end_time - 1; i >= start_time; i--) {
      state_sequence[i] = max_prev_state;
      max_prev_state = SeqState[i + 1][state_sequence[i + 1]][max_prev_state];
    }
  } /* end of block */

#ifdef _DEBUG_ /********************************************************/
  SPACER_LOG_DBG(kulog, "띄어쓰기 태그열\n");
  for (i = start_time; i <= end_time ; i++) {
    SPACER_LOG_DBG(kulog, "%d", state_sequence[i]);
  }
  SPACER_LOG_DBG(kulog, "\n");
#endif /********************************************************/

}


