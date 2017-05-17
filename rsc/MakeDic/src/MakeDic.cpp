#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int MakeFST(const char *SourceFilename, const char *DataFilename, const char *FSTFilename);

void PrintUsage(char *Program)
{

  fprintf(stderr, "Dictionary compilation program\n");
  fprintf(stderr, " USAGE : %s <Text Dictionary> <FST Filename> <DATA Filename>\n", Program);
  fprintf(stderr, "    ex) %s main.txt main.FST main.dat\n\n", Program);
}

int main(int argc, char *argv[])
{
  if (argc!=4) {
    PrintUsage(argv[0]);
    return 1;
  }
  MakeFST(argv[1], argv[3], argv[2]);
  return 0;
}
