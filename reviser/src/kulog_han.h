#ifndef _KULOG_HAN_H_
#define _KULOG_HAN_H_

#include "kulog.h"

extern kulog_handler_t * kulog_reviser;

#define HAN_LOG_ERR(slh, _fmt, _vaarg...) \
  KULOG_ERR(slh, "[HAN] " "%s,%d:: " _fmt, __FILE__ , __LINE__ , ##_vaarg)
#define HAN_LOG_SVC(slh, _fmt, _vaarg...) \
  KULOG_SVC(slh, "[HAN] " _fmt, ##_vaarg)
#define HAN_LOG_DBG(slh, _fmt, _vaarg...) \
  KULOG_DBG(slh, "[HAN] " "%s,%d:: " _fmt, __FILE__ , __LINE__ , ##_vaarg)

#endif
