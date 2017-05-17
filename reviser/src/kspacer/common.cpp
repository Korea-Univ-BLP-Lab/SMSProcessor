#include <stdio.h>
#include <string.h>
#include "hsplit.h"
//#include "get_probability.h" /* struct_split_gram */
//#include "spacer.h"

char STAG[2][2] = {"0", "1"};
char SPACE[3] = { FIL, ' ', 0 };        /* space */
char TAB[3] = { FIL, '\t', 0 }; /* TAB */

/*****************************************************************************/
/* �־��� ���ڿ� ������ strptr�� ���� splitchar�� num��° ������ �����Ѵ�. */
/* ���ϰ� : strptr�� ��ȭ�� �ּ� */
int store_syllable(char *strptr, char splitchar[][3], int num) {

  if (splitchar[num][0] == FIL) {       /* �ƽ�Ű �ڵ��̸� */
    *strptr = splitchar[num][1];
    *(++strptr) = (char) NULL;
  }
  else {
    strcat (strptr, splitchar[num]);
    strptr += 2;
  }
  return (int) strptr;
}

/*****************************************************************************/

/* �־��� ���ڿ� ������ strptr�� ���� ������ �����Ѵ�. */
int store_space_character(char *strptr) {
  *strptr = ' ';                /* ���� ���� */
  *(++strptr) = (char) NULL;

  return (int) strptr;
}

/*****************************************************************************/

/* ���� ������ tag_sequence�� ���� �� ����� target�� ���� */
int make_result_string (char splitchar[][3], int num_splitchar, int *tag_sequence, 
                        char *target) {
  int i;
  char *strptr = target;
  int start_time = 2;
  int end_time = num_splitchar+1;

  strptr[0] = 0;

  for (i = start_time; i <= end_time; i++) {
    if (splitchar[i][0] == FIL) {       /* �ƽ�Ű �ڵ��̸� */
      *strptr = splitchar[i][1];
      *(++strptr) = (char) NULL;
    }
    else {
      strcpy (strptr, splitchar[i]);
      strptr += 2;
    }

    if (tag_sequence[i] == 1) {
      *strptr = ' ';
      strptr++;
    }
  }

  if (*(--strptr) == ' ')
    *strptr = 0;
  return 1;
}
