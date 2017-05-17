#ifndef __GETFST_H__
#define __GETFST_H__

int FST_open(char *FST_Path, char *FST_INFO_Path, char *FST_FREQ_Path, 
            FST **fst_fst, char ***fst_info, int **fst_freq);

void FST_close(FST *fst_fst, char **fst_info, int *fst_freq);

#endif

