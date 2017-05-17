#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include "my-function.h"

void MyExit(int err_no, char *msg)
{

    printf("%s\n", msg);
    exit(err_no);
}

void *MyMalloc(int size)
{
    void *result;

    if ((result=malloc(size))==NULL) 
	MyExit(1, "Cannot allocate memory...");
    return result;
}

void MyFree(void *ptr)
{

    if (ptr==NULL)
	MyExit(1, "Cannot free null pointer");
    free(ptr);
}

int MyFseek(FILE *f, long offset, int ptrname)
{
    int result;

    if ((result=fseek(f, offset, ptrname))!=0)
	MyExit(1, "Cannot move file pointer");
    return result;
}

long GetFileSize(FILE *f)
{

    MyFseek(f, 0, 2);
    return ftell(f);
}
