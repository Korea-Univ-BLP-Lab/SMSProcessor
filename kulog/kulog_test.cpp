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

#if defined(SOLARIS_OS)
#include <unistd.h>
#endif

#include "kulog.h"

/*
 * for test
 */

#define MY_LOG_ERR(slh, _fmt, _vaarg...) \
  KULOG_ERR(slh, "[MY] " "%s,%d:: " _fmt, __FILE__ , __LINE__ , ##_vaarg)
#define MY_LOG_SVC(slh, _fmt, _vaarg...) \
  KULOG_SVC(slh, "[MY] " _fmt, ##_vaarg)
#define MY_LOG_DBG(slh, _fmt, _vaarg...) \
  KULOG_DBG(slh, "[MY] " "%s,%d:: " _fmt, __FILE__ , __LINE__ , ##_vaarg)

static int
lap_time (unsigned int prev)
{
  return (unsigned int)time(NULL) - prev;
}

int
main(int argc, char *argv[])
{
  int			cnt = 0, total_cnt;
  int			start, end;
  kulog_handler_t	slh;

  slh.fp = stderr;
  slh.level = 3;

  start = lap_time(0);
  while (cnt++ < total_cnt) {
     MY_LOG_DBG(&slh, "KULOG DBG sam %s %d ...................................\n",
         "haha",1);
     MY_LOG_ERR(&slh, "KULOG_ERR sam %s %d ...................................\n",
         "puhaha",1);
     MY_LOG_SVC(&slh, "KULOG SVC sam %s %d ...................................\n",
         "puhehe",1);
  }
  end = lap_time(start);
 
  if ( end > 0 ) {
    print_std_err("TPS = %d, test cnt = %d, total sec = %d\n", 
      total_cnt  * 3 / end, total_cnt * 3 , end);
  }
  else {
     print_std_err("test count too small (%d)\n", total_cnt);
  }

  return 1;
}
