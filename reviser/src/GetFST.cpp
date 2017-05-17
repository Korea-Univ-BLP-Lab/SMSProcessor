#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FST.h"
#include "GetFST.h"
#include "kulog_FST.h"

/* 변수 선언은 아래와 같이 해야 함 */
//FST *fst_fst;
//int *fst_freq;
//char **fst_info;

/*****************************************************************************/
char **entry_table=NULL;    /* 입력 단어들의 포인터 */
char *entry_string=NULL;    /* 입력 단어의 문자열 */

//extern FST *LoadFST(char *FSTName);
//extern void FreeFST(FST *fst);

static int make_entry_table(char *DicFile) {
  FILE *f;
  char *p;
  int i;
  long FileSize;
  int nEntry;

  if ((f=fopen(DicFile, "rb"))==NULL) {
    FST_LOG_ERR(kulog_reviser, "Cannot open entry file\n");
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
  char *tmp_buff;
//  if (strtok((char *)entry_string, "\r\n") != NULL)
  if (strtok_r((char *)entry_string, "\r\n", &tmp_buff) != NULL)

  do {
    nEntry++; 
//  } while (strtok(NULL, "\r\n") != NULL);
  } while (strtok_r(NULL, "\r\n", &tmp_buff ) != NULL);

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
/*static int *LoadINFO (char *path) {
  FILE *infofp;
  long FileSize = 0;
  int *fst_info;

  if ((infofp = fopen (path, "rb")) == NULL) {
    FST_LOG_ERR(kulog_reviser, "ERROR: Can't open information file [%s]!\n", path);
    return NULL;
  }

  fseek (infofp, 0, SEEK_END);
  FileSize = ftell (infofp);
  fseek (infofp, 0, SEEK_SET);
  fst_info = (int *) malloc (FileSize + 1);

  if (!fst_info) {
    FST_LOG_ERR(kulog_reviser, "ERROR: Not enough memory!\n");
    return NULL;
  }
  fread (fst_info, sizeof (int), FileSize / sizeof (int), infofp);
  fclose (infofp);
  return fst_info;
}
*/

/*****************************************************************************/
int FST_open(char *FST_Path, char *FST_INFO_Path, char *FST_FREQ_Path, 
                   FST **fst_fst, char ***fst_info, int **fst_freq) {

  int num;
  
  /* FST 열기 */
  if (!(*fst_fst = LoadFST (FST_Path))) {
    FST_LOG_ERR(kulog_reviser, "[ERROR] Cannot open FST! [%s]\n", FST_Path);
    return 0;
  }
  
//  /* frequency 화일 열기 */
//  if ((*fst_freq = LoadINFO (FST_FREQ_Path)) == NULL) {
//    FST_LOG_ERR(kulog_reviser, "[ERROR] Cannot open FST! [%s]\n", FST_FREQ_Path);
//    return 0;
//  }

  /* info 화일 열기 */
  if(!(num = make_entry_table(FST_INFO_Path)))
  {
	FST_LOG_ERR(kulog_reviser, "[ERROR] Cannot open FST_info file! [%s]\n", FST_INFO_Path);
    return 0;
  }
  
  *fst_info = entry_table;

  return 1;
}

/*****************************************************************************/
/* 이 함수의 호출은 : FST_close(fst_fst, fst_freq, fst_info); 와 같이 한다. */
void FST_close(FST *fst_fst, char **fst_info, int *fst_freq) {

  if (fst_fst) _FreeFST (fst_fst);      /* FST */

  /* 메모리 해제 */
  if (fst_freq) free (fst_freq);
  fst_freq = NULL;

  if (entry_string) free(entry_string);
  entry_string = NULL;

  if (entry_table) free(entry_table);
  entry_table = NULL;
}
