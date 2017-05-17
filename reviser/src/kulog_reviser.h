#ifndef _KULOG_REVISER_H_
#define _KULOG_REVISER_H_

#include "kulog.h"

extern kulog_handler_t * kulog_reviser;

#define REVISER_LOG_ERR(slh, _fmt, _vaarg...) \
  KULOG_ERR(slh, "[REVISER] " "%s,%d:: " _fmt, __FILE__ , __LINE__ , ##_vaarg)
#define REVISER_LOG_SVC(slh, _fmt, _vaarg...) \
  KULOG_SVC(slh, "[REVISER] " _fmt, ##_vaarg)
#define REVISER_LOG_DBG(slh, _fmt, _vaarg...) \
  KULOG_DBG(slh, "[REVISER] " "%s,%d:: " _fmt, __FILE__ , __LINE__ , ##_vaarg)

#endif
