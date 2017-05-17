#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FST.h"

/* ���� ������ �Ʒ��� ���� �ؾ� �� */
//FST *fst_fst;
//int *fst_freq;
//char **fst_info;

/*****************************************************************************/
char **entry_table=NULL;    /* �Է� �ܾ���� ������ */
char *entry_string=NULL;    /* �Է� �ܾ��� ���ڿ� */

static int make_entry_table(char *DicFile) {
  FILE *f;
  char *p;
  int i;
  long FileSize;
  int nEntry;

  if ((f=fopen(DicFile, "rb"))==NULL) {
    fprintf(stderr, "Cannot open entry file");
    return 0;
  }
  
  fseek(f, 0, SEEK_END);
  FileSize=ftell(f);
  fseek(f, 0, SEEK_SET);

  entry_string=(char *)malloc(FileSize+1);
  fread(entry_string, FileSize, 1, f);
  entry_string[FileSize]=0;
  fclose(f);

  nEntry=0;
//  if (strtok((char *)entry_string, "\r\n") != NULL)
char *tmp_buff;
  if (strtok_r((char *)entry_string, "\r\n", &tmp_buff) != NULL)

  do {
    nEntry++; 
//  } while (strtok(NULL, "\r\n") != NULL);
  } while (strtok_r(NULL, "\r\n",&tmp_buff) != NULL);

  ///**/fprintf(stderr, "nEntry = %d\n", nEntry);
  entry_table=(char **)malloc(nEntry * sizeof(char *));

  p = entry_string;

  for (i = 0; i < nEntry; i++) {
    entry_table[i] = p;
    if (i<nEntry-1)
      for (p += strlen((char *)p)+1; (*p==0)||(*p=='\r')||(*p=='\n'); p++);
  } 
  
  return (nEntry);
}

/*****************************************************************************/
static int *LoadINFO (char *path) {
  FILE *infofp;
  long FileSize = 0;
  int *fst_info;

  if ((infofp = fopen (path, "rb")) == NULL) {
    fprintf (stderr, "ERROR: Can't open information file [%s]!\n", path);
    return NULL;
  }

  fseek (infofp, 0, SEEK_END);
  FileSize = ftell (infofp);
  fseek (infofp, 0, SEEK_SET);
  fst_info = (int *) malloc (FileSize + 1);

  if (!fst_info) {
    fprintf (stderr, "ERROR: Not enough memory!\n");
    return NULL;
  }
  fread (fst_info, sizeof (int), FileSize / sizeof (int), infofp);
  fclose (infofp);
  return fst_info;
}


/*****************************************************************************/
int FST_open(char *FST_Path, char *FST_INFO_Path, char *FST_FREQ_Path, 
                   FST **fst_fst, char ***fst_info, int **fst_freq) {

  int num;
  
  /* FST ���� */
  fprintf(stderr, "\tReading FST file..");
  if (!(*fst_fst = LoadFST (FST_Path))) {
    fprintf (stderr, "[ERROR] Cannot open FST! [%s]\n", FST_Path);
    return 0;
  }
  fprintf(stderr, "\t[done]\n");
  
  /* frequency ȭ�� ���� */
  fprintf(stderr, "\tReading frequency file..");
  if ((*fst_freq = LoadINFO (FST_FREQ_Path)) == NULL) {
    fprintf (stderr, "[ERROR] Cannot open FST! [%s]\n", FST_FREQ_Path);
    return 0;
  }
  fprintf(stderr, "\t[done]\n");

  /* info ȭ�� ���� */
  fprintf(stderr, "\tReading info file..");
  num = make_entry_table(FST_INFO_Path);
  
  *fst_info = entry_table;
  fprintf(stderr, "\t[done]\n");

  return 1;
}

/*****************************************************************************/
/* �� �Լ��� ȣ���� : FST_close(fst_fst, fst_freq, fst_info); �� ���� �Ѵ�. */
void FST_close(FST *fst_fst, char **fst_info, int *fst_freq) {

  if (fst_fst) FreeFST (fst_fst);      /* FST */

  /* �޸� ���� */
  if (fst_freq) free (fst_freq);
  fst_freq = NULL;

  if (entry_string) free(entry_string);
  entry_string = NULL;

  if (entry_table) free(entry_table);
  entry_table = NULL;
}
