#include <stdio.h>
#include <math.h>
#include <limits.h>
#include "hsplit.h" /* MAX_MJ */
#include "kulog_kspacer.h"

/* Foundations of Statistical Natural Language Processing
   p.350�� ������ */

double syll_prob[MAX_MJ][2][2][2];
double ws_trans_prob[MAX_MJ][2][2][2];


/*****************************************************************************/
void unigram_viterbi_search(char splitchar[][3], int total_time, int total_class, int *state_sequence) {
  int i;
  int j, k;

  /* 2���� total_time+1���� ���� */
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

  /* �ʱ�ȭ */
  for (i = 1; i < total_time + 1; i++)
    for (j = 0; j < total_class; j++)
      SeqPrb[i][j] = -LONG_MAX;

  SeqPrb[0][0] = 0.0;           /* log(1.0) */
  SeqPrb[0][1] = 0.0;

  /* Iteration Step */
  for (i = 1; i <= total_time; i++) {   /* �� time(token) ���� */
    for (j = 0; j < total_class; j++) { /* �� class(tag) ���� */

      /* ���� ���±����� �ִ� Ȯ������ ��� ã�� */
      max_prev_prob = -LONG_MAX;       /*log(0.0) *//* �ʱ�ȭ */
      max_prev_state = 0;       /* �ʱ�ȭ */

#ifdef _DEBUG_ /********************************************************/
      SPACER_LOG_DBG(kulog, "-----------------------------\n");
      SPACER_LOG_DBG(kulog, "get_probability(%s[%d], cur_t = %d)\n", splitchar[i], i, j);
#endif /********************************************************/

      /* Ȯ�� ���� �κ� (���� �߿��� <- ����ϴ� �𵨿� ���� �����ؾ� ��) */
      /* bigram�� �ٸ� ��: �ߺ� ������ ���ϱ� ���� �� �κп� �� */
      /* ���� ���¿� ���ؼ��� ������ ���� �ʱ� ���� */
      cur_prob = log (syll_prob[i][j][0][0]) + log (ws_trans_prob[i][j][0][0]);

#ifdef _DEBUG_ /********************************************************/
       /**/ SPACER_LOG_DBG(kulog, "\n");
#endif /********************************************************/

      /* �ִ밪 ã�� */
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
    }  /* end of for, �� class(tag) ���� */
  }    /* end of for, �� time(token) ���� */

  /* Termination and path-readout */
  max_prev_prob = -LONG_MAX;   /* �ʱ�ȭ */
  for (i = 0; i < total_class; i++) {
    cur_prev_prob = SeqPrb[total_time][i];
    if (max_prev_prob < cur_prev_prob) {
      max_prev_prob = cur_prev_prob;
      max_prev_state = i;
    }
  }

  /* ��� ���� */
  /* state_sequence�� 1���� total_time���� ����� */
  state_sequence[total_time] = max_prev_state;

#ifdef _DEBUG_ /********************************************************/
  SPACER_LOG_DBG(kulog, "state_sequence[total_time] = %d\n", state_sequence[total_time]);
#endif /********************************************************/

  for (i = total_time - 1; i >= 1; i--) {
    state_sequence[i] = SeqState[i + 1][state_sequence[i + 1]];
  }

#ifdef _DEBUG_ /********************************************************/
  SPACER_LOG_DBG(kulog, "���� �±׿�\n");
  for (i = 1; i < total_time; i++) {    /* ������ ������ ����� ������ 1�̶�� �����ϹǷ� total_time�� �������� ���� */
    SPACER_LOG_DBG(kulog, "%d", state_sequence[i]);
  }
  SPACER_LOG_DBG(kulog, "\n");
#endif /********************************************************/
}


/*****************************************************************************/
void bigram_viterbi_search(char splitchar[][3], int total_time, int total_class, int *state_sequence) {
  int i;
  int j, k;

  /* 2���� total_time+1���� ���� */
#ifdef WIN32
  double SeqPrb[100000][3];
  int SeqState[100000][3];
#else
  double SeqPrb[total_time + 1][total_class];
  int SeqState[total_time + 1][total_class];
#endif
  
  int max_prev_state=0;
  double max_prev_prob, cur_prev_prob;

  /* �ʱ�ȭ */
  for (i = 1; i < total_time + 1; i++)
    for (j = 0; j < total_class; j++)
      SeqPrb[i][j] = -LONG_MAX;

  SeqPrb[0][0] = 0.0;           /* log(1.0) */
  SeqPrb[0][1] = 0.0;

  /* Iteration Step */
  for (i = 1; i <= total_time; i++) {   /* �� time(token) ���� */
    for (j = 0; j < total_class; j++) { /* �� class(tag) ���� */

      /* ���� ���±����� �ִ� Ȯ������ ��� ã�� */
      max_prev_prob = -LONG_MAX;       /*log(0.0) *//* �ʱ�ȭ */
      max_prev_state = 0;       /* �ʱ�ȭ */

      /* �ִ밪 ã�� */
      /* k�� ���� ������ �±� */
      for (k = 0; k < total_class; k++) {
        /* Ȯ�� ���� �κ� (���� �߿��� <- ����ϴ� �𵨿� ���� �����ؾ� ��) */

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
  max_prev_prob = -LONG_MAX;   /* �ʱ�ȭ */
  for (i = 0; i < total_class; i++) {
    cur_prev_prob = SeqPrb[total_time][i];
    if (max_prev_prob < cur_prev_prob) {
      max_prev_prob = cur_prev_prob;
      max_prev_state = i;
    }
  }

  /* ��� ���� */
  /* state_sequence�� 1���� total_time���� ����� */
  state_sequence[total_time] = max_prev_state;

#ifdef _DEBUG_ /********************************************************/
  SPACER_LOG_DBG(kulog, "state_sequence[total_time] = %d\n", state_sequence[total_time]);
#endif /********************************************************/

  for (i = total_time - 1; i >= 1; i--) {
    state_sequence[i] = SeqState[i + 1][state_sequence[i + 1]];
  }

#ifdef _DEBUG_ /********************************************************/
  SPACER_LOG_DBG(kulog, "���� �±׿�\n");
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

  int start_time = 2; /* ���� ��ġ */
  int end_time = total_time+1; /* �� ��ġ */

#ifdef WIN32
  double SeqPrb[100000][2][2];
  int SeqState[100000][2][2];

  int max_prev_state[2];
  double max_prev_prob[2];
  double cur_prev_prob[2];
#else
  /* 3���� total_time+2���� ���� */
  double SeqPrb[total_time+3][total_class][total_class];
  int SeqState[total_time+3][total_class][total_class];

  int max_prev_state[total_class];
  double max_prev_prob[total_class];
  double cur_prev_prob[total_class];
#endif

  /* �ʱ�ȭ */
  for (i = 2; i < total_time + 2; i++)
    for (j = 0; j < total_class; j++)
      for (l = 0; l < total_class; l++)
        SeqPrb[i][j][l] = -LONG_MAX;   /* log(0.0) *//* �ʱ�ȭ */

  /* ù time�� ���� �ʱ�ȭ */
  SeqPrb[1][0][0] = 0.0;        /* log(1.0) */
  SeqPrb[1][0][1] = 0.0;
  SeqPrb[1][1][0] = 0.0;
  SeqPrb[1][1][1] = 0.0;

  /* Iteration Step */
  for (i = start_time; i <= end_time; i++) {   /* �� time(token) ���� */
    for (j = 0; j < total_class; j++) { /* �� class(tag) ���� */

      /* �ʱ�ȭ   */
      max_prev_prob[0] = -LONG_MAX; /* log(0.0) */
      max_prev_prob[1] = -LONG_MAX; 

      max_prev_state[0] = 0;    /* �ʱ�ȭ */
      max_prev_state[1] = 0;    /* �ʱ�ȭ */

      /* ���� ���±����� �ִ� Ȯ������ ��� ã�� */
      /* �ִ밪 ã�� */
      for (k = 0; k < total_class; k++) { /* ���� ���� */
        for (l = 0; l < total_class; l++) { /* ������ ���� */

          /* Ȯ�� ���� �κ� (���� �߿��� <- ����ϴ� �𵨿� ���� �����ؾ� ��) */

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
    double max_prev_prob = -LONG_MAX;  /* �ʱ�ȭ */

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

    /* ��� ���� */
    /* state_sequence�� 1���� total_time���� ����� */
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
  SPACER_LOG_DBG(kulog, "���� �±׿�\n");
  for (i = start_time; i <= end_time ; i++) {
    SPACER_LOG_DBG(kulog, "%d", state_sequence[i]);
  }
  SPACER_LOG_DBG(kulog, "\n");
#endif /********************************************************/

}


