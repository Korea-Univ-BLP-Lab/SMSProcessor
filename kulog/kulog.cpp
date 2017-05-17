#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "kulog.h"

static char *
get_time(char *yyyymmddhh, char *hhmmss)
{
  time_t      tick;
  struct tm   tms;

  tick = time(NULL);
  localtime_r(&tick, &tms);

  sprintf(yyyymmddhh, "%04d-%02d-%02d", tms.tm_year+1900, tms.tm_mon+1, tms.tm_mday);
  sprintf(hhmmss, "%02d:%02d:%02d", tms.tm_hour, tms.tm_min, tms.tm_sec);

  return yyyymmddhh;
}

int
kulog_write(kulog_handler_t *slh, int level, char *info, const char *format, ...)
{
  va_list  ap;
  char yyyymmddhh[32];
  char hhmmss[32];
  int ret = 0;
  int wsize = 0;
  
  if ( slh == NULL || level > slh->level ) {
    return 0;
  }
 
  get_time(yyyymmddhh, hhmmss);

  fprintf(slh->fp, "[%s %s] ", hhmmss, info );

  va_start(ap, format);
  if ( (wsize = vfprintf(slh->fp, format, ap)) < 0) {
      ret += vfprintf(slh->fp, format, ap);
  } else {
    ret += wsize;    
  }
  va_end(ap);
  
  fflush(slh->fp);
  
  return ret;
}
