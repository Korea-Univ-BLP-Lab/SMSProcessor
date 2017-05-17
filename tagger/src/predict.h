#ifndef PREDICT_H
#define PREDICT_H

#include "Koma.h"

extern unsigned p_sev[2];
extern unsigned p_sevv[2];
extern unsigned p_sevj[2];
extern unsigned p_sho[2];
extern unsigned p_sd[2];
extern unsigned p_sdb[2];
extern unsigned p_sdn[2];
extern unsigned p_sbx[2];
extern unsigned p_sbxv[2];
extern unsigned p_sbxj[2];
extern unsigned p_scvv[2];
extern unsigned p_scvj[2];

extern bool SplitHangulbyPrediction(CKoma *koma, short wi);

#endif
