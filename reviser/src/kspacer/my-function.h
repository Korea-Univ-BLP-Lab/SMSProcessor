#ifndef _MYFUNCTION_H
#define _MYFUNCTION_H

#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////
// declaration of C type general purpose BTree function

#ifdef __cplusplus
extern "C"{
#endif

void MyExit(int, char *);
void *MyMalloc(int);
void MyFree(void *);
int MyFseek(FILE *, long, int);
long GetFileSize(FILE *);


#ifdef __cplusplus
}
#endif


#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#endif





