#include <stdio.h>
#include <ctype.h>
#include "hsplit.h"

/****************************************************************************/
/* �־��� ���ڿ�(source_str)�� 1�� ���� �迭�� �� ���ڿ�(target_str)�� ��ȯ */
/* 1 byte ���ڴ� �տ� FIL�� ���� */
extern int convert_str(int num_char, char source_str[][3], char *target_str) {
  int i;
  int j = 0;
  
  target_str[0] = 0;
  
  for (i = 0; i < num_char; i++) {
    target_str[j++] = source_str[i][0];
    target_str[j++] = source_str[i][1];
  }
  target_str[j] = 0;
  
  return 1;
}

/****************************************************************************/
/* �־��� ���ڿ�(source_str)�� 1�� ���� �迭�� �� ���ڿ�(target_str)�� ��ȯ */
/* 1 byte ���ڴ� �տ� FIL�� ���� */
extern int convert_str_origin(int num_char, char source_str[][3], char *target_str) {
  int i;
  int j = 0;
  
  target_str[0] = 0;
  
  for (i = 0; i < num_char; i++) {

    if (source_str[i][0] != FIL && source_str[i][0] != FIL2) {
      target_str[j++] = source_str[i][0];
    }
    else i++;

    target_str[j++] = source_str[i][1];
  }

  target_str[j] = 0;
  
  return 1;
}

/****************************************************************************/
/* �־��� ���ڿ�(source_str)�� ���ڿ�(target_str)�� ��ȯ */
/* 1 byte ���ڴ� �տ� FIL�� ���� */
extern int convert_str_origin_array(char *source_str, char *target_str) {
  int i;
  int j = 0;
  int len;

  len = (int) strlen(source_str);

  for (i = 0; i < len; i++) {
    if (source_str[i] != FIL && source_str[i] != FIL2) {
      target_str[j++] = source_str[i];
    }
  }
  target_str[j] = 0;

  return 1;
}

/****************************************************************************/
short pushWord(word_type *sword, char *word, short type, short *count)
{
	if (*count < MAX_SPLIT) {
		strcpy(sword[*count].word, word);
		sword[*count].type = type;
		(*count)++;
		return 1;
	}
	
	return 0;
}

/****************************************************************************/
/* this function analyze a given string into several partable including Korean
   word-phrase, English string, digit string, punctuation, Chinese string,
   pictorial string, and so on. */
/* �ϼ��� */
short split_by_word_type (word_type *sword, char *hword) {
  char word[MAX_WORD];
  unsigned char* w;
  short start;
  short type;
  short len;
  short wi;
  short count=0;
  
  len = (short) strlen (hword);
  
  if(len >= MAX_WORD) {
  	 fprintf(stderr, "[Warning: Too long word] %s\n", hword);
  	 return 0;
  }
  
  wi = 0;
  w = (unsigned char*) hword;
  
  while (wi < len) {
    start = wi;
    if (isascii(w[wi])) { /* �ƽ�Ű �ڵ��ΰ�? */
      if (isupper(w[wi])) { /* ���ĺ��ΰ�? */
        /* foreign language - english */
        while (wi < len && isupper (w[wi])) wi++;
        strncpy (word, hword+start, wi-start);
        word[wi-start] = (char)NULL;
        type = T_ENG_UPPER; /* �ܱ��� �빮�� */
      } 
      else if (islower(w[wi])) { /* ���ĺ��ΰ�? */
        /* foreign language - english */
        while (wi < len && islower (w[wi])) wi++;
        strncpy (word, hword+start, wi-start);
        word[wi-start] = (char)NULL;
        type = T_ENG_LOWER; /* �ܱ��� �ҹ��� */
      } 
      else if (isdigit (w[wi])) { /* �����ΰ�? */
        /* digit */
        while (wi < len && isdigit (w[wi])) wi++;
        strncpy (word, hword+start, wi-start);
        word[wi-start] = (char)NULL;
        type = T_DIG; /* ���� */
      } 
      else if (w[wi] == FIL) { /* FIL �ΰ�? */
        while (wi < len && w[wi] == FIL) wi++;
        strncpy (word, hword+start, wi-start);
        word[wi-start] = (char)NULL;
        type = T_FIL; /* ���� */
      } 
      else { /* �׹��� ���� */
        while (wi < len && isascii(w[wi]) && !isdigit(w[wi]) && !isalpha(w[wi]) ) wi++;
        strncpy (word, hword+start, wi-start);
        word [wi-start] = (char)NULL;
        type = T_SYM;      /* ��ȣ */
      } 
    } 
    else { /* �ѱ� �Ǵ� ���� �Ǵ� 2����Ʈ ���� */
      if (isHangul(w[wi], w[wi+1])) { /* �ѱ��ΰ�? */
        /* korean */
        while (wi < len && isHangul(w[wi], w[wi+1])) { 
          wi += 2;
        }
        {
          strncpy (word, hword+start, wi-start);
          word [wi-start] = (char)NULL;
          type = T_HAN; /* �ѱ� */
        }
      } 
      else if (isHanja(w[wi], w[wi+1])) { /* �����ΰ�? */
        /* hanja */
        while (wi < len && isHanja(w[wi], w[wi+1])) wi += 2;
        strncpy (word, hword+start, wi-start);
        word [wi-start] = (char)NULL;
        type = T_HJ; /* �ڸ� ��� */
      } 
      else { /* 2����Ʈ ���� */
        word [0] = hword [wi++];
        word [1] = hword [wi++];
        word [2] = (char)NULL;
        type = T_2BSYM; /* 2 byte ��ȣ */
      }
    }
    
    /* ������ �ܾ� �� ���� ���� */
    if (!pushWord(sword, word, type, &count)) return 0;
  }
  
  return count;
}

/****************************************************************************/
/* �־��� ���ڿ��� �� ���ں��� ������. */
/* hword = �Է� ���ڿ� */
/* splitchar = ��� ���� */
/* 1 byte ���ڴ� �տ� FIL�� �ٿ��� 2byte�� �����. */
/* return value : �и��� ������ �� */
int split_by_char (char *hword, char splitchar[][3]) {
  unsigned char* w = (unsigned char*) hword;
  short len = (short) strlen (hword);
  short wi = 0;
  int num_splitchar = 0;
  
  while (wi < len) {
    if (isascii(w[wi])) { /* �ƽ�Ű �ڵ��ΰ�? */
      splitchar[num_splitchar][0] = FIL;
      splitchar[num_splitchar][1] = w[wi++];
      splitchar[num_splitchar++][2] = (char) NULL;
    } 
    else { /* �ѱ� �Ǵ� ���� �Ǵ� 2����Ʈ ���� */
      splitchar[num_splitchar][0] = w[wi++];
      splitchar[num_splitchar][1] = w[wi++];
      splitchar[num_splitchar++][2] = (char) NULL;
    }
  }
  
  return num_splitchar;
}

/****************************************************************************/
/* �־��� ���ڿ��� �� ���ں��� ������. */
/* hword = �Է� ���ڿ� */
/* splitchar = ��� ���� */
/* 1 byte ���ڴ� �տ� FIL�� ���� */
/* return value : �и��� ������ �� */
int split_by_char_origin (char *hword, char splitchar[][3]) {
  unsigned char* w = (unsigned char*) hword;
  short len = (short) strlen (hword);
  short wi = 0;
  int num_splitchar = 0;
  
  while (wi < len) {
    if (isascii(w[wi])) { /* �ƽ�Ű �ڵ��ΰ�? */
      splitchar[num_splitchar][0] = w[wi++];
      splitchar[num_splitchar++][1] = (char) NULL;
    } 
    else { /* �ѱ� �Ǵ� ���� �Ǵ� 2����Ʈ ���� */
      splitchar[num_splitchar][0] = w[wi++];
      splitchar[num_splitchar][1] = w[wi++];
      splitchar[num_splitchar++][2] = (char) NULL;
    }
  }
  
  return num_splitchar;
}

/****************************************************************************/
/* �־��� ���ڿ��� �� ���ں��� ������. */
/* hword = �Է� ���ڿ� */
/* splitchar = ��� ���� */
/* 1 byte ���ڴ� �տ� FIL�� �ٿ��� 2byte�� �����. */
/* return value : ���� = 1 */
int split_by_char_array (char *hword, char *splitchar) {
  unsigned char* w = (unsigned char*) hword;
  short len = (short) strlen (hword);
  short wi = 0;
  int i = 0;
  
  while (wi < len) {
    if (isascii(w[wi])) { /* �ƽ�Ű �ڵ��ΰ�? */
      splitchar[i++] = FIL;
      splitchar[i++] = w[wi++];
    } 
    else { /* �ѱ� �Ǵ� ���� �Ǵ� 2����Ʈ ���� */
      splitchar[i++] = w[wi++];
      splitchar[i++] = w[wi++];
    }
  }
  
  splitchar[i] = (char) NULL;
  return 1;
}
