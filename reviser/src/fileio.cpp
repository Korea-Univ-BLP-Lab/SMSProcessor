#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "FST.h"
#include "kulog_FST.h"

extern kulog_handler_t * kulog;

/******************************************************************************/
/* �Էµ� ȭ��(file_name)�� ���κ��� ����(EntryString) */
/* �� ������ EntryTable�� ������ �� �ִ�. */
/* �Լ� ���ο��� �޸𸮸� �Ҵ��ϹǷ�, �� �̻� EntryTable�� ������� �ʰ� �Ǹ� */
/* free(EntryTable)�� free(Entry)�� �ܺο��� �� ��� �Ѵ�. */
/* ���ϰ� : ������ �� */
/* EntryTable : �Է� ���ε��� ������ */
/******************************************************************************/
int file2lines(char *file_name, char ***EntryTable, char **Entry) {
  FILE *fp;
  char *ptr; /* �ӽ� ������ */
  int i;
  int nEntry = 0; /* ���� �� */
  long FileSize;

  char *entrystring = NULL;
  char **entrytable = NULL;

  if ((fp = fopen(file_name, "rb"))==NULL) { /* ȭ�� ���� */
    FST_LOG_ERR(kulog_reviser, "Cannot open file [%s]", file_name);
    return 0;
  }

  /* ȭ���� ũ�⸦ �˾Ƴ� */
  fseek(fp, 0, SEEK_END); /* ȭ���� �� */
  FileSize = ftell(fp);   /* ȭ���� ��ġ */
  fseek(fp, 0, SEEK_SET); /* ȭ���� ó�� */

  entrystring = (char *) malloc(FileSize+1); /* �޸� �Ҵ� */
  if (entrystring == NULL) {
    FST_LOG_ERR(kulog_reviser, "Error: memory not enough!\n");
    return 0;
  }

  fread(entrystring, FileSize, 1, fp); /* ȭ�� ��ü�� �о���� */
  entrystring[FileSize] = 0; /* NULL */
  fclose(fp); /* ȭ�� �ݱ� */

  /* ���� ���� ����. */
  /* strtok�� ���� ������ ���� �ڵ����� NULL�� �����Ѵ�. */

  char *tmp_buff;
//  if (strtok((char *)entrystring, "\r\n") != NULL) /* ���γ��� �и���(������) */
	if (strtok_r((char *)entrystring, "\r\n", &tmp_buff) != NULL) /* ���γ��� �и���(������) */
    do {
      nEntry++; /* ���� �� ���� */
//    } while (strtok(NULL, "\r\n") != NULL);
    } while (strtok_r(NULL, "\r\n",&tmp_buff) != NULL);

  /* �����͸� ���� �޸� �Ҵ� */
  entrytable = (char **) malloc(nEntry * sizeof(char *));
  if (entrytable == NULL) {
    FST_LOG_ERR(kulog_reviser, "Error: memory not enough!\n");
    return 0;
  }

  ptr = entrystring;
  for (i = 0; i < nEntry; i++) { /* ���� �� ��ŭ �ݺ� */
    entrytable[i] = ptr;

    if (i < nEntry-1)
      for (ptr += strlen((char *)ptr)+1; 
           (*ptr == 0) || (*ptr == '\r') ||(*ptr=='\n'); 
           ptr++); /* NULL�̳� ���� ���� ���� ������ */
  } 

  *EntryTable = entrytable;
  *Entry = entrystring;

  return (nEntry); /* ���� ���� ���� */
}
