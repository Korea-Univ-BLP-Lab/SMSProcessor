#ifndef _KULOG_KSPACER_H_
#define _KULOG_KSPACER_H_

#include "kulog.h"

extern kulog_handler_t * kulog_reviser;

#define KSPACER_LOG_ERR(slh, _fmt, _vaarg...) \
  KULOG_ERR(slh, "[KSPACER] " "%s,%d:: " _fmt, __FILE__ , __LINE__ , ##_vaarg)
#define KSPACER_LOG_SVC(slh, _fmt, _vaarg...) \
  KULOG_SVC(slh, "[KSPACER] " _fmt, ##_vaarg)
#define KSPACER_LOG_DBG(slh, _fmt, _vaarg...) \
  KULOG_DBG(slh, "[KSPACER] " "%s,%d:: " _fmt, __FILE__ , __LINE__ , ##_vaarg)

#endif
