#ifndef _COMMON_H_
#define _COMMON_H_

//extern char SP_TAG[3];          /* ��� �±� */
//extern char NOSP_TAG[3];        /* ���̱� �±� */

//extern char BOS[3];             /* beginning of string mark */
//extern char EOS[3];             /* end of string mark */
//extern char SPACE[3];           /* space */
//extern char TAB[3];             /* TAB */


//extern int compare (const struct_split_gram * a, const struct_split_gram * b);
//extern int store_syllable (char *strptr, char splitchar[][3], int num); /* �ش� ���� ���� */
//extern int store_space_character (char *strptr);        /* ���� ���� */
extern int make_result_string (char splitchar[][3], int num_splitchar,
                               int *tag_sequence, char *target);
#endif
