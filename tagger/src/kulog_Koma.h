#ifndef _KULOG_KOMA_H_
#define _KULOG_KOMA_H_

#include "kulog.h"

extern kulog_handler_t * kulog_tagger;

#define Koma_LOG_ERR(slh, _fmt, _vaarg...) \
  KULOG_ERR(slh, "[Koma] " "%s,%d:: " _fmt, __FILE__ , __LINE__ , ##_vaarg)
#define Koma_LOG_SVC(slh, _fmt, _vaarg...) \
  KULOG_SVC(slh, "[Koma] " _fmt, ##_vaarg)
#define Koma_LOG_DBG(slh, _fmt, _vaarg...) \
  KULOG_DBG(slh, "[Koma] " "%s,%d:: " _fmt, __FILE__ , __LINE__ , ##_vaarg)

#endif
