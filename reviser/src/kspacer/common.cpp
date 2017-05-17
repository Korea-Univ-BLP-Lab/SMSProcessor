#include <stdio.h>
#include <string.h>
#include "hsplit.h"
//#include "get_probability.h" /* struct_split_gram */
//#include "spacer.h"

char STAG[2][2] = {"0", "1"};
char SPACE[3] = { FIL, ' ', 0 };        /* space */
char TAB[3] = { FIL, '\t', 0 }; /* TAB */

/*****************************************************************************/
/* 주어진 문자열 포인터 strptr의 끝에 splitchar의 num번째 음절을 저장한다. */
/* 리턴값 : strptr의 변화된 주소 */
int store_syllable(char *strptr, char splitchar[][3], int num) {

  if (splitchar[num][0] == FIL) {       /* 아스키 코드이면 */
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

/* 주어진 문자열 포인터 strptr의 끝에 공백을 저장한다. */
int store_space_character(char *strptr) {
  *strptr = ' ';                /* 공백 삽입 */
  *(++strptr) = (char) NULL;

  return (int) strptr;
}

/*****************************************************************************/

/* 원시 문장을 tag_sequence에 따라 띄어쓴 결과를 target에 저장 */
int make_result_string (char splitchar[][3], int num_splitchar, int *tag_sequence, 
                        char *target) {
  int i;
  char *strptr = target;
  int start_time = 2;
  int end_time = num_splitchar+1;

  strptr[0] = 0;

  for (i = start_time; i <= end_time; i++) {
    if (splitchar[i][0] == FIL) {       /* 아스키 코드이면 */
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
