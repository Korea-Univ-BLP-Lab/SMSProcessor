#ifndef _KULOG_FST_H_
#define _KULOG_FST_H_

#include "kulog.h"

extern kulog_handler_t * kulog_reviser;

#define FST_LOG_ERR(slh, _fmt, _vaarg...) \
  KULOG_ERR(slh, "[FST] " "%s,%d:: " _fmt, __FILE__ , __LINE__ , ##_vaarg)
#define FST_LOG_SVC(slh, _fmt, _vaarg...) \
  KULOG_SVC(slh, "[FST] " _fmt, ##_vaarg)
#define FST_LOG_DBG(slh, _fmt, _vaarg...) \
  KULOG_DBG(slh, "[FST] " "%s,%d:: " _fmt, __FILE__ , __LINE__ , ##_vaarg)

#endif
