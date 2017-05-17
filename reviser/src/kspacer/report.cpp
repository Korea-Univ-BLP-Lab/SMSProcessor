#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
//#include <sys/time.h>
//#include <sys/resource.h>
#include "kulog_kspacer.h"

extern int verbosity;
/******************************************************************************/
/*unsigned long used_ms() {
  struct rusage ru;

  if (getrusage(RUSAGE_SELF, &ru)) { report(0, "Can't get rusage: %s\n", strerror(errno)); }

  return 
    (ru.ru_utime.tv_sec+ru.ru_stime.tv_sec)*1000+
    (ru.ru_utime.tv_usec+ru.ru_stime.tv_usec)/1000;
}*/

/******************************************************************************/
void error(char *format, ...) {
  va_list ap;

//  KSPACER_LOG_ERR(kulog, "[%9ld ms::%d] ", used_ms(), verbosity); 
  KSPACER_LOG_ERR(kulog_reviser, "ERROR: ");
  va_start(ap, format);
  vfprintf(stderr, format, ap);
  va_end(ap);
  //exit(0);
}

/******************************************************************************/
/* verbosity가 주어진 mode보다 크거나 같을 때만 출력 */
void report(int mode, char *format, ...) {
  va_list ap;

  if (mode > verbosity) { return; }

//  if (mode>=0) { KSPACER_LOG_ERR(kulog, "[%9ld ms::%d] ", used_ms(), verbosity); } 
  va_start(ap, format);
  vfprintf(stderr, format, ap);
  va_end(ap);
}

/******************************************************************************/
void print_progress(int mode) {
  static int c=0;
  static char *wheel="|/-\\|/-\\";

  if (abs(mode)>verbosity) { return; }

  report(mode, "%c\r", wheel[c]);
  if (!wheel[++c]) { c=0; }
}

/******************************************************************************/
int intcompare(const void *ip, const void *jp) {
  int i = *((int *)ip);
  int j = *((int *)jp);

  if (i > j) { return 1; }
  if (i < j) { return -1; }
  return 0;
}

/******************************************************************************/
