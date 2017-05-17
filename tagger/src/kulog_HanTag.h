#ifndef _KULOG_HANTAG_H_
#define _KULOG_HANTAG_H_

#include "kulog.h"

extern kulog_handler_t * kulog_tagger;

#define HanTag_LOG_ERR(slh, _fmt, _vaarg...) \
  KULOG_ERR(slh, "[HanTag] " "%s,%d:: " _fmt, __FILE__ , __LINE__ , ##_vaarg)
#define HanTag_LOG_SVC(slh, _fmt, _vaarg...) \
  KULOG_SVC(slh, "[HanTag] " _fmt, ##_vaarg)
#define HanTag_LOG_DBG(slh, _fmt, _vaarg...) \
  KULOG_DBG(slh, "[HanTag] " "%s,%d:: " _fmt, __FILE__ , __LINE__ , ##_vaarg)

#endif
