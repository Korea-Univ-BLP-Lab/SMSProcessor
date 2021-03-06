#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "FST.h"
#include "kulog_FST.h"

extern kulog_handler_t * kulog;

/******************************************************************************/
/* 입력된 화일(file_name)을 라인별로 저장(EntryString) */
/* 각 라인은 EntryTable로 접근할 수 있다. */
/* 함수 내부에서 메모리를 할당하므로, 더 이상 EntryTable을 사용하지 않게 되면 */
/* free(EntryTable)과 free(Entry)를 외부에서 해 줘야 한다. */
/* 리턴값 : 라인의 수 */
/* EntryTable : 입력 라인들의 포인터 */
/******************************************************************************/
int file2lines(char *file_name, char ***EntryTable, char **Entry) {
  FILE *fp;
  char *ptr; /* 임시 포인터 */
  int i;
  int nEntry = 0; /* 라인 수 */
  long FileSize;

  char *entrystring = NULL;
  char **entrytable = NULL;

  if ((fp = fopen(file_name, "rb"))==NULL) { /* 화일 열기 */
    FST_LOG_ERR(kulog_reviser, "Cannot open file [%s]", file_name);
    return 0;
  }

  /* 화일의 크기를 알아냄 */
  fseek(fp, 0, SEEK_END); /* 화일의 끝 */
  FileSize = ftell(fp);   /* 화일의 위치 */
  fseek(fp, 0, SEEK_SET); /* 화일의 처음 */

  entrystring = (char *) malloc(FileSize+1); /* 메모리 할당 */
  if (entrystring == NULL) {
    FST_LOG_ERR(kulog_reviser, "Error: memory not enough!\n");
    return 0;
  }

  fread(entrystring, FileSize, 1, fp); /* 화일 전체를 읽어들임 */
  entrystring[FileSize] = 0; /* NULL */
  fclose(fp); /* 화일 닫기 */

  /* 라인 수를 센다. */
  /* strtok에 의해 라인의 끝에 자동으로 NULL을 삽입한다. */

  char *tmp_buff;
//  if (strtok((char *)entrystring, "\r\n") != NULL) /* 라인끝이 분리자(구분자) */
	if (strtok_r((char *)entrystring, "\r\n", &tmp_buff) != NULL) /* 라인끝이 분리자(구분자) */
    do {
      nEntry++; /* 라인 수 증가 */
//    } while (strtok(NULL, "\r\n") != NULL);
    } while (strtok_r(NULL, "\r\n",&tmp_buff) != NULL);

  /* 포인터를 위한 메모리 할당 */
  entrytable = (char **) malloc(nEntry * sizeof(char *));
  if (entrytable == NULL) {
    FST_LOG_ERR(kulog_reviser, "Error: memory not enough!\n");
    return 0;
  }

  ptr = entrystring;
  for (i = 0; i < nEntry; i++) { /* 라인 수 만큼 반복 */
    entrytable[i] = ptr;

    if (i < nEntry-1)
      for (ptr += strlen((char *)ptr)+1; 
           (*ptr == 0) || (*ptr == '\r') ||(*ptr=='\n'); 
           ptr++); /* NULL이나 라인 끝을 만날 때까지 */
  } 

  *EntryTable = entrytable;
  *Entry = entrystring;

  return (nEntry); /* 라인 수를 리턴 */
}
