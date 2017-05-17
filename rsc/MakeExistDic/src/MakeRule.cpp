#include "entry2FST.h"

/*****************************************************************************/
/* Global variables */
char *Version = "(24/Oct/2006)";
char *Description = "MakeRule: Converting an text-rule file to a FST-rule file";

/*****************************************************************************/
void Help (char *RunName) {
  fprintf (stderr, "\n%s %s : %s", RunName, Version, Description);
  fprintf (stderr, "\n");
  fprintf (stderr, "\n[Usage]");
  fprintf (stderr, "\n%s FILE", RunName);
  fprintf (stderr, "\n\n[Description]");
  fprintf (stderr, "\nFILE refers to an entry file consisting of (key '\\t' value) pairs.");
  fprintf (stderr, "\n\n[Output]");
  fprintf (stderr, "\nFILE.FST, FILE.list, FILE.info, and FILE.freq");

  fprintf (stderr, "\n");
}

/*****************************************************************************/
int main(int argc, char *argv[]) {

  char FST_filename[100];
  char list_filename[100];
  char info_filename[100];
  char freq_filename[100];

  if (argc != 2) {
    Help (argv[0]);
    return 0;
  }

  /* 프로그램 정보 출력 */
  fprintf (stderr, "%s %s : %s\n", argv[0], Version, Description);      


  char ext_excluded_filename[100];
  
  { /* 확장자 제거 */
    char *p;
    strcpy(ext_excluded_filename, argv[1]);
    p = strrchr(ext_excluded_filename, '.');
    if (p != NULL) *p = 0;
  }

  sprintf(list_filename, "%s.list", ext_excluded_filename); /* list */
  sprintf(FST_filename, "%s.FST", ext_excluded_filename);   /* FST */
  sprintf(info_filename, "%s.dat", ext_excluded_filename); /* info */
  sprintf(freq_filename, "%s.freq", ext_excluded_filename); /* freq */

  entry2FST(argv[1], list_filename, FST_filename, info_filename, freq_filename, 0);

  return 1;
}
