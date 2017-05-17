#ifndef _KULOG_H_
#define _KULOG_H_

/* log levels */
#define KULOG_LEVEL_NONE          0
#define KULOG_LEVEL_ERR           1
#define KULOG_LEVEL_SVC           2
#define KULOG_LEVEL_DBG           3

typedef struct _kulog_handler_t kulog_handler_t;
struct _kulog_handler_t {
  FILE *fp;
  int  level;
//  char log_path[256];
//  char file_name[256];
//  char module_name[256];
};

int kulog_write(kulog_handler_t *, int, char *, const char *, ...);

/* user macro samples */
#define KULOG_ERR(slh, vaargs...)  \
  kulog_write(slh, KULOG_LEVEL_ERR, "ERR" , ##vaargs)
#define KULOG_SVC(slh, vaargs...)  \
  kulog_write(slh, KULOG_LEVEL_SVC, "SVC" , ##vaargs)
#define KULOG_DBG(slh, vaargs...)  \
  kulog_write(slh, KULOG_LEVEL_DBG, "DBG" , ##vaargs)
#define KULOG_DUMP(slh, vaargs...)  \
  kulog_write(slh, KULOG_LEVEL_NONE, NULL , ##vaargs)

#define print_std_err(vaargs...) \
  do { \
    fprintf(stderr , ##vaargs); \
    fflush(stderr); \
  } while(0)

#define print_std_out(vaargs...) \
  do { \
    fprintf(stdout , ##vaargs); \
    fflush(stdout); \
  } while(0)

#endif

