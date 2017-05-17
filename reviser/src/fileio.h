/* Filename     : fileio.h */
/* Description  : external header of fileio.c */
/* Programmer   : Do-Gil Lee (Dept. of Computer Science, Korea University) */
/* Date         : 12 Dec 2002 */
#ifndef __FILEIO_H__
#define __FILEIO_H__

/******************************************************************************/
/* �Էµ� ȭ��(file_name)�� ���κ��� ���� */
/* �� ������ EntryTable�� ������ �� �ִ�. */
/* �Լ� ���ο��� �޸𸮸� �Ҵ��ϹǷ�, �� �̻� EntryTable�� ������� �ʰ� �Ǹ� */
/* free(EntryTable)�� �ܺο��� �� ��� �Ѵ�. */
/* ���ϰ� : ������ �� */
/* EntryTable : �Է� ���ε��� ������ */
/******************************************************************************/
extern int file2lines(char *file_name, char ***EntryTable, char **Entry);

#endif
