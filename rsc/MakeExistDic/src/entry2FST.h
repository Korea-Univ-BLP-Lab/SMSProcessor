#ifndef __entry2FST_H__
#define __entry2FST_H__

#pragma warning(disable: 4786)
#pragma warning(disable: 4503)

#include <map>
#include <stdio.h>
#include <string> 
#include <stdlib.h>
#include "FST.h"

using namespace std;

/******************************************************************************/
/* Ÿ�� ���� */
typedef map<string, int>             TAG_FREQ; /* �±� �� */
typedef map<string, TAG_FREQ>        WORD_TAG_FREQ; /* �ܾ� �±� �� */

extern int entry2FST(char *filename, char *list_filename, char *FST_filename, 
                     char *info_filename, char *freq_filename, 
                     int cutoff_threshold);

extern int FST_open(char *FST_Path, char *FST_INFO_Path, char *FST_FREQ_Path, 
                   FST **fst_fst, char ***fst_info, int **fst_freq);

extern void FST_close(FST *fst_fst, char **fst_info, int *fst_freq);

#endif
