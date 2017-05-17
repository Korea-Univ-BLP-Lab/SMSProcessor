/* predictively morphological analysis */

#include <stdio.h>
#include <string.h>

#include "ph.h"
#include "postype.h"
#include "Koma.h"

#define NEXT	((short)(koma->m_Table.topm-1))

unsigned p_n[2]	    = { 0x00003fff, 0x00000000 };
unsigned p_n0[2]    = { 0x000001ff, 0x00000000 };
unsigned p_n1[2]    = { 0x000001ef, 0x00000000 };
unsigned p_n2[2]    = { 0x00003fcf, 0x00000000 };
unsigned p_n3[2]    = { 0x000000cf, 0x00000000 };

//Edited By Wonho
unsigned p_n4[2]    = { 0x000001cf, 0x00000000 };

unsigned p_nncg[2]  = { 0x00000001, 0x00000000 };
unsigned p_nncv[2]  = { 0x00000002, 0x00000000 };
unsigned p_nncj[2]  = { 0x00000004, 0x00000000 };
unsigned p_nnb[2]   = { 0x00000018, 0x00000000 };
unsigned p_nnbu[2]  = { 0x00000010, 0x00000000 };
unsigned p_nnp[2]   = { 0x00000020, 0x00000000 };
unsigned p_np[2]    = { 0x000000c0, 0x00000000 };
unsigned p_nu[2]    = { 0x00000100, 0x00000000 };
unsigned p_xsn[2]   = { 0x00003e10, 0x00000000 };
unsigned p_xsnn[2]  = { 0x00000200, 0x00000000 };
unsigned p_xsnnd[2] = { 0x00000400, 0x00000000 };
unsigned p_xsnp[2]  = { 0x00000800, 0x00000000 };
unsigned p_xsnu[2]  = { 0x00001000, 0x00000000 };
unsigned p_xsnpl[2] = { 0x00002000, 0x00000000 };
unsigned p_xpnn[2]  = { 0x00004000, 0x00000000 };
unsigned p_xpnu[2]  = { 0x00008000, 0x00000000 };
unsigned p_pie[2]   = { 0x00ff0000, 0x000fc020 };
unsigned p_p[2]	    = { 0x00ff0000, 0x00000000 };
unsigned p_ps[2]    = { 0x00010000, 0x00000000 };
unsigned p_pc[2]    = { 0x00020000, 0x00000000 };
unsigned p_po[2]    = { 0x00040000, 0x00000000 };
unsigned p_pd[2]    = { 0x00080000, 0x00000000 };
unsigned p_pa[2]    = { 0x00100000, 0x00000000 };
unsigned p_pv[2]    = { 0x00200000, 0x00000000 };
unsigned p_pn[2]    = { 0x00400000, 0x00000000 };
unsigned p_px[2]    = { 0x00800000, 0x00000000 };
unsigned p_dac[2]   = { 0xff000000, 0x0000001f };
unsigned p_du[2]    = { 0x04000000, 0x00000000 };
unsigned p_d[2]	    = { 0x07000000, 0x00000000 };
unsigned p_xsm[2]   = { 0x08002000, 0x0000000c };
unsigned p_xsd[2]   = { 0x08000000, 0x00000000 };
unsigned p_a[2]     = { 0xf0000000, 0x00000003 };
unsigned p_av2[2]   = { 0x00000000, 0x00000003 };
unsigned p_av[2]    = { 0x00000000, 0x00000001 };
unsigned p_aj[2]    = { 0x00000000, 0x00000002 };
unsigned p_xsa[2]   = { 0x10000000, 0x00000004 };
unsigned p_xsah[2]  = { 0x20000000, 0x00000008 };
unsigned p_c[2]     = { 0x00000000, 0x00000010 };
unsigned p_i[2]	    = { 0x00000000, 0x00000020 };
unsigned p_v[2]	    = { 0x00000000, 0x18003fc0 };
unsigned p_vg[2]    = { 0x00000000, 0x00000140 };
unsigned p_vv[2]    = { 0x00000000, 0x080004c0 };
unsigned p_vvg[2]   = { 0x00000000, 0x00000040 };
unsigned p_vx[2]    = { 0x00000000, 0x00000280 };
unsigned p_vvx[2]   = { 0x00000000, 0x00000080 };
unsigned p_vj[2]    = { 0x00000000, 0x10003b00 };
unsigned p_vjg[2]   = { 0x00000000, 0x00000100 };
unsigned p_vjx[2]   = { 0x00000000, 0x00000200 };
unsigned p_xsv[2]   = { 0x00000000, 0x00003c00 };
unsigned p_xsvv[2]  = { 0x00000000, 0x00000400 };
unsigned p_xsvj[2]  = { 0x00000000, 0x00003800 };
unsigned p_xsvjg[2] = { 0x00000000, 0x00000800 };
unsigned p_xsvjd[2] = { 0x00000000, 0x00001000 };
unsigned p_xsvjb[2] = { 0x00000000, 0x00002000 };
unsigned p_e[2]	    = { 0x00000000, 0x000fc000 };
unsigned p_ef[2]    = { 0x00000000, 0x0007c000 };
unsigned p_efc[2]   = { 0x00000000, 0x00008000 };
unsigned p_ep[2]    = { 0x00000000, 0x00080000 };
unsigned p_un[2]    = { 0x00000000, 0x00100000 };
unsigned p_uv[2]    = { 0x00000000, 0x00200000 };
unsigned p_sev[2]   = { 0x00000000, 0x00c00000 };
unsigned p_sevv[2]  = { 0x00000000, 0x00400000 };
unsigned p_sevj[2]  = { 0x00000000, 0x00800000 };
unsigned p_sho[2]   = { 0x00000000, 0x01000000 };
unsigned p_sd[2]    = { 0x00000000, 0x06000000 };
unsigned p_sdb[2]   = { 0x00000000, 0x02000000 };
unsigned p_sdn[2]   = { 0x00000000, 0x04000000 };
unsigned p_sbx[2]   = { 0x00000000, 0x18000000 };
unsigned p_sbxv[2]  = { 0x00000000, 0x08000000 };
unsigned p_sbxj[2]  = { 0x00000000, 0x10000000 };
unsigned p_scvv[2]  = { 0x00000000, 0x20000000 };
unsigned p_scvj[2]  = { 0x00000000, 0x40000000 };
unsigned p_we[2]    = { 0xf7ffbfff, 0x0607c01f };

unsigned f_n_ir	    = 0x00000001;
unsigned f_n_cm	    = 0x00000002;
unsigned f_vv_r	    = 0x00000004;
unsigned f_vv_i	    = 0x00000008;
unsigned f_vv_l	    = 0x00000010;
unsigned f_vj_r	    = 0x00000020;
unsigned f_vj_i	    = 0x00000040;
unsigned f_vj_l	    = 0x00000080;
unsigned f_v_nc	    = 0x00000100;
unsigned f_p_r_i    = 0x00000200;
unsigned f_p_v	    = 0x00000400;
unsigned f_p_c	    = 0x00000800;
unsigned f_p_l	    = 0x00001000;
unsigned f_p_ir	    = 0x00002000;
unsigned f_e_l_v    = 0x00004000;
unsigned f_e_l_j    = 0x00008000;
unsigned f_e_l_i    = 0x00010000;
unsigned f_e_l_ix   = 0x00020000;
unsigned f_e_l_p    = 0x00040000;
unsigned f_e_r_p    = 0x00080000;
unsigned f_e_v	    = 0x00100000;
unsigned f_e_c	    = 0x00200000;
unsigned f_e_s	    = 0x00400000;
unsigned f_e_l	    = 0x00800000;
unsigned f_e_lx	    = 0x01000000;
unsigned f_e_hx	    = 0x02000000;
unsigned f_e_hc	    = 0x04000000;
unsigned f_e_ir	    = 0x08000000;
unsigned f_e_p	    = 0x10000000;
unsigned f_e_n	    = 0x20000000;
unsigned f_inf	    = 0x40000000;
unsigned f_def	    = 0x80000000;

unsigned f_v_r	    = 0x00000024;
unsigned f_v_i	    = 0x00000048;
unsigned f_v_l	    = 0x00000090;
unsigned f_p_pc	    = 0x00001c00;
unsigned f_e_cc	    = 0x0007c000;
unsigned f_e_pc	    = 0x00f00000;

/* state prediction for predictive morphological analysis. */
enum eSTATE 
{
    S_WE,
    S_P,
    S_I,
    S_E,
    S_XSM,
    S_XSN,
    S_XSV,
    S_N,
    S_VX,
};

bool engine(CKoma *koma);

/* split all constituents(hanguls) from word by using prediction. */
bool SplitHangulbyPrediction(CKoma *koma, short wi) 
{
  char* word;

  word = koma->m_Table.w[wi].word;
  koma->m_Table.i.wi = wi;
  /* conceptual WORD_END state */
  koma->m_Table.i.rfeat = f_def;
  koma->m_Table.i.rtags[0] = p_we[0]; // 어절끝에 대한 예측 정보 
  koma->m_Table.i.rtags[1] = p_we[1];
  if (!koma->PushHead(word, S_WE, BOTTOM)) return false;
  while (koma->PopHead())
	  if (!engine(koma)) return false; 
  return true;
}

/* split a head with real part and formal part. */
void split(CKoma *koma)
{
  strncpy (koma->m_Table.i.real, koma->m_Table.i.head, koma->m_Table.i.rlen); 
  koma->m_Table.i.real [koma->m_Table.i.rlen] = (char)0;
  strcpy (koma->m_Table.i.form, koma->m_Table.i.head+koma->m_Table.i.rlen);
}

/* look up morpheme in dictionary. */
bool lookup(CKoma *koma) 
{
  if (!koma->m_Rsc.GetPos(koma->m_Table.i.form, koma->m_Table.i.ftags, &(koma->m_Table.i.ffeat))) return false;
  // 사전 정보 보정
  /* add unregistered feature: particle: phonological connectivity */
  if (((koma->m_Table.i.ftags[0] & p_p[0]) || (koma->m_Table.i.ftags[1] & p_p[1])) && 
      !(koma->m_Table.i.ffeat & f_p_pc)) 
    koma->m_Table.i.ffeat |= f_p_pc;
  /* add unregistered feature: ending: categorial connectivity */
  if (((koma->m_Table.i.ftags[0] & p_e[0]) || (koma->m_Table.i.ftags[1] & p_e[1])) && 
      !(koma->m_Table.i.ffeat & f_e_cc)) 
    koma->m_Table.i.ffeat |= f_e_cc;
  /* add unregistered feature: ending: phonological connectivity */
  if (((koma->m_Table.i.ftags[0] & p_e[0]) || (koma->m_Table.i.ftags[1] & p_e[1])) && 
      !(koma->m_Table.i.ffeat & f_e_pc)) 
    koma->m_Table.i.ffeat |= f_e_pc;
  return true;
}

/************************/
/* states by prediction */
/************************/

bool unknown_estimation(CKoma *koma);
bool rule_itself(CKoma *koma);
bool rule_initial(CKoma *koma);
bool rule_final(CKoma *koma);
bool rule_inf(CKoma *koma);

bool engine(CKoma *koma)
{
  koma->m_isUnknown = 0;
  koma->m_isHaOmit = 0;
  koma->m_Table.i.hlen = strlen(koma->m_Table.i.head);
  koma->m_Table.i.rlen = 0;
  ConvertToPhoneme (koma->m_Table.i.head+koma->m_Table.i.hlen-2, &koma->m_fp);
  
  /* 어절을 좌측에서 우측으로 읽어가며 형태소 분석을 실시 */
  while (koma->m_Table.i.rlen<koma->m_Table.i.hlen) {
    koma->m_Table.i.flen = koma->m_Table.i.hlen - koma->m_Table.i.rlen;
    ConvertToPhoneme (koma->m_Table.i.head+koma->m_Table.i.rlen, &koma->m_cp); // 형식형태소의 첫번재 문자
    /* 현재 음절을 자소 코드로 변환 */
    if (koma->m_Table.i.rlen >= 2) // 실질 형태소의 길이가 2이상 일 때
      ConvertToPhoneme (koma->m_Table.i.head+koma->m_Table.i.rlen-2, &koma->m_pp); // 실질형태소의 마지막 문자
    // 실질 형태소의 마지막 음절을 자소 코드로 변환
    if (koma->m_Table.i.flen >= 2) // 형식 형태소의 길이가 2이상 일 때
      ConvertToPhoneme (koma->m_Table.i.head+koma->m_Table.i.rlen+2, &koma->m_np); // 형식형태소의 두번째 문자
    // 형식 형태소의 두번째 음절을 자소 코드로 변환
    split(koma);  // head를 실질 형태소와 형식 형태소로 분리함.
          
    /* 어절 전체가 단독 형태소인가를 검사 */
    if (!rule_itself (koma)) return false;
    /*  형태소 시작 부분부터 끝까지를 이용해 분석 */
    if (!rule_initial (koma)) return false;
    /*  형태소의 끝 음소를 이용한 분석 */
    if (!rule_final (koma)) return false;
    /* rule_inflection_ending과 rule_inflection_particle을 수행 */
    if (!rule_inf (koma)) return false;
    koma->m_Table.i.rlen += 2;
    while (koma->m_Table.i.head[koma->m_Table.i.rlen]>0) koma->m_Table.i.rlen++;
    if (koma->m_Table.i.rlen==koma->m_Table.i.hlen) break;
  }

  if (!unknown_estimation(koma)) return false;
  return true;
}

bool unknown_estimation(CKoma *koma)
{
  if (!koma->m_isUnknown) return true;
  ConvertToPhoneme (koma->m_Table.i.head+koma->m_Table.i.hlen-2, &koma->m_cp);
  if ((koma->m_cp.i == PHI_NG || koma->m_cp.i == PHI_N || koma->m_cp.i == PHI_R) && 
      koma->m_cp.m == PHM_EU && 
      (koma->m_cp.f == PHF_FIL || koma->m_cp.f == PHF_N || koma->m_cp.f == PHF_R)) return true;
  if (((koma->m_Table.i.ptags[0] & p_n[0]) || (koma->m_Table.i.ptags[1] & p_n[1])) &&
      !(koma->m_Table.i.pfeat & f_n_ir)) {
    koma->m_Table.i.mfeat = f_def;
    koma->m_Table.i.mtags[0] = p_un[0];
    koma->m_Table.i.mtags[1] = p_un[1];
    if (!koma->PushMorph(koma->m_Table.i.head, koma->m_Table.i.next)) return false;
    if (!koma->PushUnknown(NEXT)) return false;
  }
  if ((koma->m_Table.i.ptags[0] & p_v[0]) || (koma->m_Table.i.ptags[1] & p_v[1])) {
    koma->m_Table.i.mfeat = f_def;
    koma->m_Table.i.mtags[0] = p_uv[0];
    koma->m_Table.i.mtags[1] = p_uv[1];
    if (!koma->PushMorph(koma->m_Table.i.head, koma->m_Table.i.next)) return false;
    if (!koma->PushUnknown(NEXT)) return false;
  }
  return true;
}

bool rule_itself_inf_s_geo(CKoma *koma);
bool rule_itself_inf_gi(CKoma *koma);

bool rule_itself_nominal(CKoma *koma);
bool rule_itself_nprefix(CKoma *koma);
bool rule_itself_modifier(CKoma *koma);
bool rule_itself_particle(CKoma *koma);
bool rule_itself_verbal(CKoma *koma);
bool rule_itself_ending(CKoma *koma);
bool rule_itself_i(CKoma *koma);
bool rule_itself(CKoma *koma)
{
  if (koma->m_Table.i.rlen > 0) return true;
  if (!lookup(koma)) return false;

  /*  if (!rule_itself_inf_s_geo(koma)) return false;
  if (!rule_itself_inf_gi(koma)) return false;*/

  if (!rule_itself_nominal(koma)) return false;
  if (!rule_itself_nprefix(koma)) return false;
  if (!rule_itself_modifier(koma)) return false;
  if (!rule_itself_verbal(koma)) return false;
  if (!rule_itself_particle(koma)) return false;
  if (!rule_itself_ending(koma)) return false;
  if (!rule_itself_i(koma)) return false;
  return true;
}

/* s-omission: WE
    (G|NG),EO,FIL = (G|NG),EO,S  
    M,UEO,FIL = M,UEO,S 
   geo-contraction: WE
    G,EO,FIL = G,EU,FIL,G,EO,S  
    G,EO,FIL = G,EO,FIL,G,I,FIL
*/
bool rule_itself_inf_s_geo(CKoma *koma)
{
    /* condition */
    if (koma->m_Table.i.flen != 2) return true;
    if (!(koma->m_Table.i.pred == S_WE)) return true;
    if (!(koma->m_cp.f == PHF_FIL)) return true; 
    if (!((koma->m_cp.i == PHI_M && koma->m_cp.m == PHM_UEO) || 
	((koma->m_cp.i == PHI_G || koma->m_cp.i == PHI_NG) && koma->m_cp.m == PHM_EO))) return true; 
    split(koma);
    /* left morpheme */
    if (koma->m_cp.m == PHM_EO)
	CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_EO, PHF_S);
    else 
	CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_UEO, PHF_S);
    koma->m_Table.i.rfeat = f_n_ir;
    koma->m_Table.i.rtags[0] = p_np[0] | p_nnb[0];
    koma->m_Table.i.rtags[1] = p_np[1] | p_nnb[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_WE, koma->m_Table.i.next)) return false;
    if (! (koma->m_Table.i.rlen == 0 && koma->m_cp.i == PHI_G && koma->m_cp.m == PHM_EO) ) return true; 
    /* G,EO,FIL = G,EU,FIL,G,EO,S */
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_EU, PHF_FIL);
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen+2, koma->m_cp.i, PHM_EO, PHF_S);
    koma->m_Table.i.rfeat = f_n_ir;
    koma->m_Table.i.rtags[0] = p_np[0];
    koma->m_Table.i.rtags[1] = p_np[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_WE, koma->m_Table.i.next)) return false;
    /* G,EO,FIL = G,EO,FIL,G,I,FIL */
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_EO, PHF_FIL);
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen+2, koma->m_cp.i, PHM_I, PHF_FIL);
    koma->m_Table.i.rfeat = f_n_ir;
    koma->m_Table.i.rtags[0] = p_np[0] | p_a[0];
    koma->m_Table.i.rtags[1] = p_np[1] | p_a[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_WE, koma->m_Table.i.next)) return false;
    return true;
}

/* gi-omission:
    (G|J),EOI = (G|J),EO,FIL,G,I,FIL
    NG,YEOI = NG,YEO,FIL,G,I,FIL */ 
bool rule_itself_inf_gi(CKoma *koma)
{
    /* condition */
    if (koma->m_Table.i.flen != 2) return true;
    if (!(koma->m_Table.i.pred == S_WE)) return true;
    if (!((koma->m_cp.i == PHI_NG && koma->m_cp.m == PHM_YEOI) || 
	((koma->m_cp.i == PHI_G || koma->m_cp.i == PHI_J) && koma->m_cp.m == PHM_EOI))) return true; 
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_EOI, koma->m_cp.f);
    if (!lookup(koma)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_p[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_p[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    koma->m_Table.i.mfeat = f_inf;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    if (koma->m_cp.m == PHM_EOI)
	CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_EO, PHF_FIL);
    else 
	CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_YEO, PHF_FIL);
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen+2, PHI_G, PHM_I, PHF_FIL);
    koma->m_Table.i.rfeat = f_n_ir;
    koma->m_Table.i.rtags[0] = p_np[0];
    koma->m_Table.i.rtags[1] = p_np[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_P, NEXT)) return false;
    return true;
}

bool rule_itself_nominal(CKoma *koma)
{
    /* condition */
    if (!((koma->m_Table.i.ptags[0] & p_n[0]) ||
	(koma->m_Table.i.ptags[1] & p_n[1]))) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_n[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_n[1];
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) 
    {
/* begging of modifed part at 15 Sep 1998
	koma->m_Table.i.ptags[0] |= p_xsn[0];
	koma->m_Table.i.ptags[1] |= p_xsn[1];
end of modifed part at 15 Sep 1998 */
	koma->m_isUnknown = 1;
	return true;
    }
    if ((koma->m_Table.i.pfeat & f_n_ir) && !(koma->m_Table.i.ffeat & f_n_ir)) return true;
    if ((koma->m_Table.i.pfeat & f_n_cm) && (koma->m_Table.i.ffeat & f_n_cm)) 
    {
	koma->m_Table.i.mtags[0] &= ~p_n3[0];
	koma->m_Table.i.mtags[1] &= ~p_n3[1];
    }
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    koma->m_Table.i.ptags[0] &= ~p_n2[0];
    koma->m_Table.i.ptags[1] &= ~p_n2[1];
    /* right morpheme */
    koma->m_Table.i.mfeat = koma->m_Table.i.pfeat & (f_inf | f_def);
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    if (!koma->PushKnown(NEXT)) return false;
    return true;
}

bool rule_itself_nprefix(CKoma *koma)
{
    /* condition */
    if (koma->m_Table.i.pred != S_N) return true;
    if (!((koma->m_Table.i.ptags[0] & p_xpnn[0]) ||
	(koma->m_Table.i.ptags[1] & p_xpnn[1]) )) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_xpnn[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_xpnn[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* right morpheme */
    koma->m_Table.i.mfeat = f_def;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    if (!koma->PushKnown(NEXT)) return false;
    return true;
}

bool rule_itself_modifier(CKoma *koma)
{
    /* condition */
  /*  if (koma->m_Table.i.pred != S_WE && koma->m_Table.i.pred != S_XSV && koma->m_Table.i.pred != S_N && 
	koma->m_Table.i.pred != S_P) return true;*/
    if (!((koma->m_Table.i.ptags[0] & (p_dac[0] | p_xpnu[0])) ||
	(koma->m_Table.i.ptags[1] & (p_dac[1] | p_xpnu[1])))) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & (p_dac[0] | p_xpnu[0]);
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & (p_dac[1] | p_xpnu[1]);
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) 
    {
	koma->m_Table.i.ptags[0] |= p_xsm[0];
	koma->m_Table.i.ptags[1] |= p_xsm[1];
	return true;
    }
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* right morpheme */
    koma->m_Table.i.mfeat = f_def;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    if (!koma->PushKnown(NEXT)) return false;
    return true;
}

bool rule_itself_verbal(CKoma *koma)
{
    unsigned tags[2];
    /* condition */
    if (koma->m_Table.i.pred != S_E) return true;
    if (!((koma->m_Table.i.ptags[0] & p_v[0]) ||
	(koma->m_Table.i.ptags[1] & p_v[1]))) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_v[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_v[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) 
    {
	koma->m_Table.i.ptags[0] |= p_xsv[0];
	koma->m_Table.i.ptags[1] |= p_xsv[1];
	koma->m_isUnknown = 1;
	return true;
    }
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    /*    koma->m_Table.i.ptags[0] &= ~p_v[0];
    koma->m_Table.i.ptags[1] &= ~p_v[1];*/
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if ((koma->m_Table.i.pfeat & f_v_nc) && (koma->m_Table.i.ffeat & f_v_nc)) return true;
    if (!(((koma->m_Table.i.pfeat & f_vv_r) && (koma->m_Table.i.ffeat & f_vv_r)) ||
	((koma->m_Table.i.pfeat & f_vv_i) && (koma->m_Table.i.ffeat & f_vv_i)) ||
	((koma->m_Table.i.pfeat & f_vv_l) && (koma->m_Table.i.ffeat & f_vv_l)))) 
    {
	koma->m_Table.i.mtags[0] &= ~p_vv[0];
	koma->m_Table.i.mtags[1] &= ~p_vv[1];
    }
    if (!(((koma->m_Table.i.pfeat & f_vj_r) && (koma->m_Table.i.ffeat & f_vj_r)) ||
	((koma->m_Table.i.pfeat & f_vj_i) && (koma->m_Table.i.ffeat & f_vj_i)) ||
	((koma->m_Table.i.pfeat & f_vj_l) && (koma->m_Table.i.ffeat & f_vj_l)))) 
    {
	koma->m_Table.i.mtags[0] &= ~p_vj[0];
	koma->m_Table.i.mtags[1] &= ~p_vj[1];
    }
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    tags[0] = koma->m_Table.i.mtags[0];
    tags[1] = koma->m_Table.i.mtags[1];
    /* right morpheme */
    koma->m_Table.i.mtags[0] &= ~p_sbx[0];
    koma->m_Table.i.mtags[1] &= ~p_sbx[1];
    koma->m_Table.i.mfeat = koma->m_Table.i.pfeat & (f_inf | f_def);
    if (koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1]) 
    {
	if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
	/* left morpheme */
	if (!koma->PushKnown(NEXT)) return false;
    }
    /* right morpheme: boundnoun + verbal suffix */
    koma->m_Table.i.mtags[0] = tags[0] & p_sbx[0];
    koma->m_Table.i.mtags[1] = tags[1] & p_sbx[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    koma->m_Table.i.mtags[0] = 0;
    koma->m_Table.i.mtags[1] = 0;
    if ((tags[0] & p_sbxv[0]) || (tags[1] & p_sbxv[1])) 
    {
	koma->m_Table.i.mtags[0] = p_xsvv[0];
	koma->m_Table.i.mtags[1] = p_xsvv[1];
    }
    if ((tags[0] & p_sbxj[0]) || (tags[1] & p_sbxj[1])) 
    {
	koma->m_Table.i.mtags[0] = p_xsvjg[0];
	koma->m_Table.i.mtags[1] = p_xsvjg[1];
    }
    strcpy (koma->m_Table.i.morph, koma->m_Table.i.head+2);
    if (!koma->PushMorph(koma->m_Table.i.morph, koma->m_Table.i.next)) return false;
    /* left morpheme */
    strncpy (koma->m_Table.i.morph, koma->m_Table.i.head, 2); 
    koma->m_Table.i.morph [2] = (char)0;
    koma->m_Table.i.mtags[0] = p_nnb[0];
    koma->m_Table.i.mtags[1] = p_nnb[1];
    if (!koma->PushMorph(koma->m_Table.i.morph, NEXT)) return false;
    if (!koma->PushKnown(NEXT)) return false;
    return true;
}

bool rule_itself_particle(CKoma *koma)
{
    /* condition */
    if (!(koma->m_Table.i.pred == S_WE || koma->m_Table.i.pred == S_I)) return true;
    if (!((koma->m_Table.i.ptags[0] & p_p[0]) ||
	(koma->m_Table.i.ptags[1] & p_p[1]))) return true;
    if (koma->m_Table.i.wi == 0) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_p[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_p[1];
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if ((koma->m_Table.i.pfeat & f_p_r_i) && !(koma->m_Table.i.ffeat & f_p_r_i)) return true;
    /* right morpheme */
    koma->m_Table.i.mfeat = f_def;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    if (!koma->PushKnown(NEXT)) return false;
    return true;
}

bool rule_itself_ending(CKoma *koma)
{
    /* condition */
    if (!(koma->m_Table.i.pred == S_WE || koma->m_Table.i.pred == S_E || koma->m_Table.i.pred == S_VX ||
	koma->m_Table.i.pred == S_I || koma->m_Table.i.pred == S_P)) return true;
    if (!((koma->m_Table.i.ptags[0] & p_e[0]) ||
	(koma->m_Table.i.ptags[1] & p_e[1]))) return true;
    if ( (koma->m_Table.i.ftags[0] & p_sho[0])	||
	(koma->m_Table.i.ftags[1] & p_sho[1])	) { koma->m_isHaOmit = 1; }
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (! (koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1]) ) return true;
    if ( (koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p) ) return true;
    if (! (koma->m_Table.i.ffeat & f_e_l_ix) ) return true;
    /* right morpheme */
    koma->m_Table.i.mfeat = f_def;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme: i-omission */
    strcpy (koma->m_Table.i.morph, koma->m_Table.i.real);
    CopySyllable (koma->m_Table.i.morph, PHI_NG, PHM_I, PHF_FIL);
    koma->m_Table.i.mfeat = f_inf;
    koma->m_Table.i.mtags[0] = p_i[0];
    koma->m_Table.i.mtags[1] = p_i[1];
    if (!koma->PushMorph(koma->m_Table.i.morph, NEXT)) return false;
    if (!koma->PushKnown(NEXT)) return false;
    return true;
}

bool rule_itself_i(CKoma *koma)
{
    /* condition */
    if (!(koma->m_Table.i.pred == S_E)) return true;
    if (!((koma->m_Table.i.ptags[0] & p_i[0]) || (koma->m_Table.i.ptags[1] & p_i[1]))) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_i[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_i[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* right morpheme */
    koma->m_Table.i.mfeat = f_def;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    if (!koma->PushKnown(NEXT)) return false;
    return true;
}

bool rule_initial_unitnoun(CKoma *koma);
bool rule_initial_nsuffix(CKoma *koma);
bool rule_initial_msuffix(CKoma *koma);
bool rule_initial_auxverbal(CKoma *koma);
bool rule_initial_vsuffix(CKoma *koma);
bool rule_initial_particle(CKoma *koma);
bool rule_initial_ending(CKoma *koma);
bool rule_initial_i(CKoma *koma);
bool rule_initial(CKoma *koma)
{
    if (koma->m_Table.i.rlen < 2) return true;
    if (!lookup(koma)) return false;
    if (!rule_initial_unitnoun(koma)) return false;
    if (!rule_initial_nsuffix(koma)) return false;
    if (!rule_initial_msuffix(koma)) return false;
    if (!rule_initial_auxverbal(koma)) return false;
    if (!rule_initial_vsuffix(koma)) return false;
    if (!rule_initial_particle(koma)) return false;
    if (!rule_initial_ending(koma)) return false;
    if (!rule_initial_i(koma)) return false;
    return true;
}

bool rule_initial_unitnoun(CKoma *koma)
{
    /* condition */
    if (!((koma->m_Table.i.ptags[0] & p_n1[0]) ||
	(koma->m_Table.i.ptags[1] & p_n1[1]))) return true;
    if ((koma->m_Table.i.pfeat & f_n_ir)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_n1[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_n1[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
/* modified begin */
    if ( koma->m_Table.i.ffeat & f_n_cm ) return true;
/* modified end */	 
/* old begin
    if ( !( (koma->m_Table.i.mtags[0] & p_nnbu[0]) || (koma->m_Table.i.mtags[1] & p_nnbu[1]) ) &&
	 (koma->m_Table.i.ffeat & f_n_cm) ) return true;
old end */	 
    /* right morpheme */
    koma->m_Table.i.mfeat = koma->m_Table.i.pfeat & (f_inf | f_def);
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    koma->m_Table.i.rfeat = f_n_cm;
    koma->m_Table.i.rtags[0] = p_n4[0] | p_xpnn[0];
    koma->m_Table.i.rtags[1] = p_n4[1] | p_xpnn[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_N, NEXT)) return false;
    return true;
}

bool rule_initial_nsuffix(CKoma *koma)
{
    /* condition */
    if (!((koma->m_Table.i.ptags[0] & p_xsn[0]) || 
	  (koma->m_Table.i.ptags[1] & p_xsn[1]))) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_xsn[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_xsn[1];
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* right morpheme */
    koma->m_Table.i.mfeat = f_def;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    koma->m_Table.i.rfeat = 0;
    if ( (koma->m_Table.i.mtags[0] & p_nnbu[0]) || (koma->m_Table.i.mtags[1] & p_nnbu[1]) ) 
    {
	koma->m_Table.i.rtags[0] = p_n[0] | p_du[0];
	koma->m_Table.i.rtags[1] = p_n[1] | p_du[1];
    }
    else
    {
	koma->m_Table.i.rtags[0] = p_n[0];
	koma->m_Table.i.rtags[1] = p_n[1];
    }
    if (!koma->PushHead(koma->m_Table.i.real, S_XSN, NEXT)) return false;
    return true;
}

bool rule_initial_msuffix(CKoma *koma)
{
    /* condition */
    if (!(koma->m_Table.i.pred == S_WE)) return true;
    if (!((koma->m_Table.i.ptags[0] & p_a[0]) || (koma->m_Table.i.ptags[1] & p_a[1]))) return true;
    if (!((koma->m_Table.i.ptags[0] & p_xsm[0]) || (koma->m_Table.i.ptags[1] & p_xsm[1]))) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_xsm[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_xsm[1];
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* right morpheme */
    koma->m_Table.i.mfeat = f_def;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    koma->m_Table.i.rfeat = 0;
    if ((koma->m_Table.i.mtags[0] & p_px[0]) || (koma->m_Table.i.mtags[1] & p_px[1]) ||
	(koma->m_Table.i.mtags[0] & p_xsnpl[0]) || (koma->m_Table.i.mtags[1] & p_xsnpl[1])) 
    {
	koma->m_Table.i.rtags[0] = p_a[0];
	koma->m_Table.i.rtags[1] = p_a[1];
    } 
    else 
    {
	koma->m_Table.i.rtags[0] = p_n[0];
	koma->m_Table.i.rtags[1] = p_n[1];
    }
    if (!koma->PushHead(koma->m_Table.i.real, S_XSM, NEXT)) return false;
    return true;
}

bool rule_initial_auxverbal(CKoma *koma)
{
    /* condition */
    if (koma->m_Table.i.pred != S_E) return true;
    if (!((koma->m_Table.i.ptags[0] & p_vx[0]) ||
	(koma->m_Table.i.ptags[1] & p_vx[1]))) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_vx[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_vx[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    if ((koma->m_Table.i.pfeat & f_v_nc) && (koma->m_Table.i.ffeat & f_v_nc)) return true;
    if (!(((koma->m_Table.i.pfeat & f_vv_r) && (koma->m_Table.i.ffeat & f_vv_r)) ||
	((koma->m_Table.i.pfeat & f_vv_i) && (koma->m_Table.i.ffeat & f_vv_i)) ||
	((koma->m_Table.i.pfeat & f_vv_l) && (koma->m_Table.i.ffeat & f_vv_l)))) 
    {
	koma->m_Table.i.mtags[0] &= ~p_vv[0];
	koma->m_Table.i.mtags[1] &= ~p_vv[1];
    }
    if (!(((koma->m_Table.i.pfeat & f_vj_r) && (koma->m_Table.i.ffeat & f_vj_r)) ||
	((koma->m_Table.i.pfeat & f_vj_i) && (koma->m_Table.i.ffeat & f_vj_i)) ||
	((koma->m_Table.i.pfeat & f_vj_l) && (koma->m_Table.i.ffeat & f_vj_l)))) 
    {
	koma->m_Table.i.mtags[0] &= ~p_vj[0];
	koma->m_Table.i.mtags[1] &= ~p_vj[1];
    }
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* right morpheme */
    koma->m_Table.i.mfeat = koma->m_Table.i.pfeat & (f_inf | f_def);
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    koma->m_Table.i.rfeat = 0;
    koma->m_Table.i.rtags[0] = p_efc[0];
    koma->m_Table.i.rtags[1] = p_efc[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_VX, NEXT)) return false;
    return true;
}

bool rule_initial_vsuffix(CKoma *koma)
{
    unsigned tags[2];
    /* condition */
    if (!(koma->m_Table.i.pred == S_E)) return true;
    if (!((koma->m_Table.i.ptags[0] & p_v[0]) || (koma->m_Table.i.ptags[1] & p_v[1]))) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_v[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_v[1];
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* right morpheme */
    koma->m_Table.i.mfeat = f_def;
    tags[0] = 0;
    tags[1] = 0;
    if (((koma->m_Table.i.pfeat & f_vv_r) && (koma->m_Table.i.ffeat & f_vv_r)) ||
	((koma->m_Table.i.pfeat & f_vv_i) && (koma->m_Table.i.ffeat & f_vv_i))) 
    {
	tags[0] |= koma->m_Table.i.mtags[0] & p_vv[0];
	tags[1] |= koma->m_Table.i.mtags[1] & p_vv[1];
    }
    if (((koma->m_Table.i.pfeat & f_vj_r) && (koma->m_Table.i.ffeat & f_vj_r)) ||
	((koma->m_Table.i.pfeat & f_vj_i) && (koma->m_Table.i.ffeat & f_vj_i))) 
    {
	tags[0] |= koma->m_Table.i.mtags[0] & p_vj[0];
	tags[1] |= koma->m_Table.i.mtags[1] & p_vj[1];
    }
    if (!(tags[0] || tags[1])) return true;
    /* left morpheme */
    koma->m_Table.i.rfeat = 0;
    koma->m_Table.i.mtags[0] = tags[0];
    koma->m_Table.i.mtags[1] = tags[1];
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    if ( koma->m_pp.i == PHI_H && koma->m_pp.m == PHM_OA && koma->m_pp.f == PHF_FIL	&& 
	( (koma->m_cp.i == PHI_H && koma->m_cp.m == PHM_A && koma->m_cp.f == PHF_FIL) ||
	  (koma->m_cp.i == PHI_D && koma->m_cp.m == PHM_OI && koma->m_cp.f == PHF_FIL) ) )
    {
	koma->m_Table.i.rtags[0] = p_n0[0] | p_av2[0] | p_xsnn[0];
	koma->m_Table.i.rtags[1] = p_n0[1] | p_av2[1] | p_xsnn[1];
    }
    else
    {
	koma->m_Table.i.rtags[0] = p_n0[0] | p_av2[0];
	koma->m_Table.i.rtags[1] = p_n0[1] | p_av2[1];
    }
    if (!koma->PushHead(koma->m_Table.i.real, S_XSV, NEXT)) return false;
    return true;
}

long phonemic_constraint_particle(CKoma *koma);
bool rule_initial_particle_s(CKoma *koma);
bool rule_initial_particle_sr(CKoma *koma);
bool rule_initial_particle_gu(CKoma *koma);
bool rule_initial_particle_ii(CKoma *koma);
bool rule_initial_particle(CKoma *koma)
{
    /* condition */
    if (!(koma->m_Table.i.pred == S_WE || koma->m_Table.i.pred == S_I)) return true;
    if ((koma->m_Table.i.pfeat & f_p_r_i) && !(koma->m_Table.i.ffeat & f_p_r_i)) return true;
    if (!((koma->m_Table.i.ptags[0] & p_p[0]) || (koma->m_Table.i.ptags[1] & p_p[1]))) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_p[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_p[1];
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if (!phonemic_constraint_particle(koma)) return true;
    /* right morpheme */
    koma->m_Table.i.mfeat = f_def;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    koma->m_Table.i.rfeat = f_e_r_p;
    koma->m_Table.i.rtags[0] = p_n[0] | p_ef[0] | p_a[0];
    koma->m_Table.i.rtags[1] = p_n[1] | p_ef[1] | p_a[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_P, NEXT)) return false;
    /* inflection */
    if (!rule_initial_particle_s(koma)) return false;
    if (!rule_initial_particle_sr(koma)) return false;
    if (!rule_initial_particle_gu(koma)) return false;
    if (!rule_initial_particle_ii(koma)) return false;
    return true;
}

long phonemic_constraint_particle(CKoma *koma)
{
    if (koma->m_pp.f == PHF_FIL) return (koma->m_Table.i.ffeat & f_p_v);
    else if (koma->m_pp.f == PHF_R) return (koma->m_Table.i.ffeat & f_p_l);
    return (koma->m_Table.i.ffeat & f_p_c);
}

/* s-omission: 
    M,UEO,FIL,?(p_v) = M,UEO,S + ?(p_v)
    (G|NG),EO,FIL,?(p_v) = (G|NG),EO,S + ?(p_v) */
bool rule_initial_particle_s(CKoma *koma)
{
    /* condition */
    if (!((koma->m_Table.i.ffeat & f_p_v) && (koma->m_pp.f == PHF_FIL) &&
	((koma->m_pp.i == PHI_M && koma->m_pp.m == PHM_UEO) || 
	((koma->m_pp.i == PHI_G || koma->m_pp.i == PHI_NG) && koma->m_pp.m == PHM_EO)))) return true;
    /* right morpheme */
    koma->m_Table.i.mfeat = f_inf;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    strcpy (koma->m_Table.i.morph, koma->m_Table.i.real);
    CopySyllable (koma->m_Table.i.morph+koma->m_Table.i.rlen-2, koma->m_pp.i, koma->m_pp.m, PHF_S);
    koma->m_Table.i.rfeat = f_n_ir;
    koma->m_Table.i.rtags[0] = p_np[0] | p_nnb[0];
    koma->m_Table.i.rtags[1] = p_np[1] | p_nnb[1];
    if (!koma->PushHead(koma->m_Table.i.morph, S_P, NEXT)) return false;
    return true;
}

/* s-omission: r
    M,UEO,R,R,O = M,UEO,S + R,O
    (G|NG),EO,R,R,O = (G|NG),EO,S + R,O */
bool rule_initial_particle_sr(CKoma *koma)
{
    /* condition */
    if (!(koma->m_cp.i == PHI_R && koma->m_cp.m == PHM_O)) return true;
    if (!(koma->m_pp.f == PHF_R)) return true;
    if (!((koma->m_pp.i == PHI_M && koma->m_pp.m == PHM_UEO) || 
	((koma->m_pp.i == PHI_G || koma->m_pp.i == PHI_NG) && koma->m_pp.m == PHM_EO))) return true; 
    /* right morpheme */
    koma->m_Table.i.mfeat = f_inf;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    strcpy (koma->m_Table.i.morph, koma->m_Table.i.real);
    CopySyllable (koma->m_Table.i.morph+koma->m_Table.i.rlen-2, koma->m_pp.i, koma->m_pp.m, PHF_S);
    koma->m_Table.i.rfeat = f_n_ir;
    koma->m_Table.i.rtags[0] = p_np[0] | p_nnb[0];
    koma->m_Table.i.rtags[1] = p_np[1] | p_nnb[1];
    if (!koma->PushHead(koma->m_Table.i.morph, S_P, NEXT)) return false;
    return true;
}

/* -gu- ommission:
    N,U,FIL,G,A,FIL = N,U,FIL,G,U,FIL + G,A,FIL */
bool rule_initial_particle_gu(CKoma *koma)
{
    /* condition */
    if (koma->m_Table.i.rlen != 2) return true;
    if (koma->m_Table.i.flen != 2) return true;
    if (!(koma->m_cp.i == PHI_G && koma->m_cp.m == PHM_A && koma->m_cp.f == PHF_FIL)) return true;
    if (!(koma->m_pp.i == PHI_N && koma->m_pp.m == PHM_U && koma->m_pp.f == PHF_FIL)) return true;
    /* right morpheme */
    koma->m_Table.i.mfeat 
		= f_inf;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    strcpy (koma->m_Table.i.morph, koma->m_Table.i.real);
    CopySyllable (koma->m_Table.i.morph+koma->m_Table.i.rlen, PHI_G, PHM_U, PHF_FIL);
    koma->m_Table.i.rfeat = f_n_ir;
    koma->m_Table.i.rtags[0] = p_np[0];
    koma->m_Table.i.rtags[1] = p_np[1];
    if (!koma->PushHead(koma->m_Table.i.morph, S_P, NEXT)) return false;
    return true;
}

/* i-insertion:
    N,AI,FIL,G,A,FIL = N,A,FIL + G,A,FIL
    (N|J),EOI,FIL,G,A,FIL = (N|J),EO,FIL + G,A,FIL */
bool rule_initial_particle_ii(CKoma *koma)
{
    /* condition */
    if (koma->m_Table.i.rlen != 2) return true;
    if (koma->m_Table.i.flen != 2) return true;
    if (!(koma->m_cp.i == PHI_G && koma->m_cp.m == PHM_A && koma->m_cp.f == PHF_FIL)) return true;
    if (!(koma->m_pp.f == PHF_FIL)) return true;
    if (!((koma->m_pp.i == PHI_N && koma->m_pp.m == PHM_AI) || 
	((koma->m_pp.i == PHI_N || koma->m_pp.i == PHI_J) && koma->m_pp.m == PHM_EOI))) return true; 
    /* right morpheme */
    koma->m_Table.i.mfeat = f_inf;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    strcpy (koma->m_Table.i.morph, koma->m_Table.i.real);
    if (koma->m_pp.m == PHM_AI)
	CopySyllable (koma->m_Table.i.morph+koma->m_Table.i.rlen-2, koma->m_pp.i, PHM_A, koma->m_pp.f);
    else
	CopySyllable (koma->m_Table.i.morph+koma->m_Table.i.rlen-2, koma->m_pp.i, PHM_EO, koma->m_pp.f);
    koma->m_Table.i.rfeat = f_n_ir;
    koma->m_Table.i.rtags[0] = p_np[0];
    koma->m_Table.i.rtags[1] = p_np[1];
    if (!koma->PushHead(koma->m_Table.i.morph, S_P, NEXT)) return false;
    return true;
}

bool rule_initial_i(CKoma *koma)
{
    /* condition */
    if (!(koma->m_Table.i.pred == S_E)) return true;
    if (!((koma->m_Table.i.ptags[0] & p_i[0]) || (koma->m_Table.i.ptags[1] & p_i[1]))) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_i[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_i[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* left morpheme */
    koma->m_Table.i.mfeat = f_def;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* right morpheme */
    koma->m_Table.i.rfeat = f_e_r_p | f_p_r_i;
    koma->m_Table.i.rtags[0] = p_n[0] | p_ef[0] | p_p[0] | p_a[0];
    koma->m_Table.i.rtags[1] = p_n[1] | p_ef[1] | p_p[1] | p_a[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_I, NEXT)) return false;
    return true;
}

long phonemic_constraint_ending(CKoma *koma);
long vowel_harmony (CKoma *koma);
bool rule_initial_ending_io(CKoma *koma);
bool rule_initial_ending_s(CKoma *koma);
bool rule_initial_ending_d(CKoma *koma);
bool rule_initial_ending_h(CKoma *koma);
bool rule_initial_ending_r(CKoma *koma);
bool rule_initial_ending_r_omit(CKoma *koma);
bool rule_initial_ending_ha_jjeo(CKoma *koma);
bool rule_initial_ending_ha_omit(CKoma *koma);
bool rule_initial_ending(CKoma *koma)
{
    /* condition */
    if (!(koma->m_Table.i.pred == S_WE || koma->m_Table.i.pred == S_E || koma->m_Table.i.pred == S_VX ||
	koma->m_Table.i.pred == S_I || koma->m_Table.i.pred == S_P)) return true;
    if (!((koma->m_Table.i.ptags[0] & p_e[0]) || (koma->m_Table.i.ptags[1] & p_e[1]))) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    if (phonemic_constraint_ending(koma)) 
    {
	if (!vowel_harmony(koma)) return true;
	/* left morpheme */
	koma->m_Table.i.mfeat = f_def;
	if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
        /* right morpheme */
	if (koma->m_cp.i == PHI_NG && (koma->m_cp.m == PHM_A || koma->m_cp.m == PHM_EO)) 
	{
	    koma->m_Table.i.rfeat = f_v_r;
	    if (koma->m_pp.f == PHF_FIL) koma->m_Table.i.rfeat |= f_v_nc;
        } 
	else if (koma->m_cp.i == PHI_NG && koma->m_cp.m == PHM_EU) koma->m_Table.i.rfeat = f_v_r;
	else koma->m_Table.i.rfeat = f_v_r | f_v_i | f_v_l;
	if (koma->m_Table.i.rlen == 4) 
	    ConvertToPhoneme (koma->m_Table.i.head+koma->m_Table.i.rlen-4, &koma->m_ppp);
	koma->m_Table.i.rtags[0] = 0;
        koma->m_Table.i.rtags[1] = 0;
	if (koma->m_Table.i.ffeat & f_e_l_v) 
	{
	    koma->m_Table.i.rtags[0] |= p_vv[0];
	    koma->m_Table.i.rtags[1] |= p_vv[1];
	}
	if ((koma->m_Table.i.ffeat & f_e_l_v) &&
	    (koma->m_pp.i == PHI_NG && ((koma->m_pp.m == PHM_I && koma->m_pp.f == PHF_SS) ||
	    (koma->m_pp.m == PHM_EO && koma->m_pp.f == PHF_BS)))) 
	{
	    koma->m_Table.i.rtags[0] |= p_vj[0];
	    koma->m_Table.i.rtags[1] |= p_vj[1];
	}
	if ((koma->m_Table.i.ffeat & f_e_l_j) &&
	    !(koma->m_pp.i == PHI_NG && ((koma->m_pp.m == PHM_I && koma->m_pp.f == PHF_SS) ||
	    (koma->m_pp.m == PHM_EO && koma->m_pp.f == PHF_BS))) &&
	    !((koma->m_Table.i.rlen == 4) && 
	    koma->m_pp.i == PHI_N && koma->m_pp.m == PHM_I && koma->m_pp.f == PHF_FIL &&
	    koma->m_ppp.i == PHI_NG && koma->m_ppp.m == PHM_A && koma->m_ppp.f == PHF_FIL)) 
	{
	    koma->m_Table.i.rtags[0] |= p_vj[0];
	    koma->m_Table.i.rtags[1] |= p_vj[1];
	}
	if ((koma->m_Table.i.ffeat & f_e_l_i) && 
	    (koma->m_pp.i == PHI_NG && koma->m_pp.m == PHM_I && koma->m_pp.f == PHF_FIL)) 
	{
	    koma->m_Table.i.rtags[0] |= p_i[0];
	    koma->m_Table.i.rtags[1] |= p_i[1];
	}
	if ((koma->m_Table.i.ffeat & f_e_l_i) && (koma->m_Table.i.rlen == 4) &&
	    (koma->m_pp.i == PHI_N && koma->m_pp.m == PHM_I && koma->m_pp.f == PHF_FIL &&
	    koma->m_ppp.i == PHI_NG && koma->m_ppp.m == PHM_A && koma->m_ppp.f == PHF_FIL)) 
	{
	    koma->m_Table.i.rtags[0] |= p_vj[0];
	    koma->m_Table.i.rtags[1] |= p_vj[1];
	}
	if (!((koma->m_Table.i.mtags[0] & p_ep[0]) || (koma->m_Table.i.mtags[1] & p_ep[1])) &&
	    (koma->m_Table.i.ffeat & f_e_l_p)) 
	{
	    koma->m_Table.i.rtags[0] |= p_ep[0];
	    koma->m_Table.i.rtags[1] |= p_ep[1];
	}
	if (koma->m_Table.i.rtags[0] || koma->m_Table.i.rtags[1]) 
	{
	    if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
	}
    }
    /* inflection */
    if (!rule_initial_ending_s(koma)) return false;
    if (!rule_initial_ending_d(koma)) return false;
    if (!rule_initial_ending_h(koma)) return false;
    if (!rule_initial_ending_r(koma)) return false;
    if (!rule_initial_ending_r_omit(koma)) return false;
    if (!rule_initial_ending_ha_jjeo(koma)) return false;
    if (!rule_initial_ending_ha_omit(koma)) return false;
    if (!rule_initial_ending_io(koma)) return false;
    return true;
}

long phonemic_constraint_ending(CKoma *koma)
{
    if (koma->m_pp.f == PHF_FIL) return (koma->m_Table.i.ffeat & f_e_v);
    else if (koma->m_pp.f == PHF_R) return (koma->m_Table.i.ffeat & f_e_l);
    else if (koma->m_pp.f == PHF_SS || koma->m_pp.f == PHF_BS) return (koma->m_Table.i.ffeat & f_e_s);
    return (koma->m_Table.i.ffeat & f_e_c);
}

long vowel_harmony (CKoma *koma)
{
    if (koma->m_cp.i == PHI_NG && koma->m_cp.m == PHM_A)
	return (koma->m_pp.m == PHM_A || koma->m_pp.m == PHM_O || koma->m_pp.m == PHM_YA || koma->m_pp.m == PHM_AI);
    return 1;
}

/* s-irregularity: 
	FIL = S
    (A|EO|U|EU|I),_,NG,EU = (A|EO|U|EU|I),_ + NG,EU
    A,_,NG,A = A,_ + NG,A
    (EO|U|EU|I),_,NG,EO = (EO|U|EU|I),_ + NG,EO */
bool rule_initial_ending_s(CKoma *koma)
{
    /* condition */
    if (!(koma->m_Table.i.ffeat & f_e_ir)) return true;
    if (!(koma->m_pp.f == PHF_FIL)) return true;
    if (!(koma->m_cp.i == PHI_NG && (koma->m_cp.m == PHM_EU || koma->m_cp.m == PHM_A || 
	koma->m_cp.m == PHM_EO))) return true; 
    if (!(koma->m_pp.m == PHM_A || koma->m_pp.m == PHM_EO || koma->m_pp.m == PHM_U || 
	koma->m_pp.m == PHM_EU || koma->m_pp.m == PHM_I)) return true; 
    if (koma->m_cp.m == PHM_A && koma->m_pp.m != PHM_A) return true; 
    if (koma->m_cp.m == PHM_EO && koma->m_pp.m == PHM_A) return true; 
    /* right morpheme */
    koma->m_Table.i.mfeat = f_inf;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    strcpy (koma->m_Table.i.morph, koma->m_Table.i.real);
    CopySyllable (koma->m_Table.i.morph+koma->m_Table.i.rlen-2, koma->m_pp.i, koma->m_pp.m, PHF_S);
    koma->m_Table.i.rfeat = f_vv_i | f_vj_i;
    koma->m_Table.i.rtags[0] = p_vv[0] | p_vj[0];
    koma->m_Table.i.rtags[1] = p_vv[1] | p_vj[1];
    return koma->PushHead(koma->m_Table.i.morph, S_E, NEXT);
}

/* d-irregularity: R = D
    (G|N|D|M|B|S|K),=(A|EO|U|YEO|EU|I),_,NG,EU = 
	(G|N|D|M|B|S|K),=(A|EO|U|YEO|EU|I),_ + NG,EU
    D,A,_,NG,A = D,A,_ + NG,A
    (G|N|D|M|B|S|K),(EO|U|YEO|EU|I),_,NG,EO = 
	(G|N|D|M|B|S|K),(EO|U|YEO|EU|I),_ + NG,EO */
bool rule_initial_ending_d(CKoma *koma)
{
    /* condition */
    if (!(koma->m_Table.i.ffeat & f_e_ir)) return true;
    if (!(koma->m_pp.f == PHF_R)) return true;
    if (!(koma->m_cp.i == PHI_NG && (koma->m_cp.m == PHM_EU || koma->m_cp.m == PHM_A || 
	koma->m_cp.m == PHM_EO))) return true; 
    if (!(koma->m_pp.m == PHM_A || koma->m_pp.m == PHM_EO || koma->m_pp.m == PHM_U || 
	koma->m_pp.m == PHM_YEO || koma->m_pp.m == PHM_EU || koma->m_pp.m == PHM_I)) return true; 
    if (koma->m_cp.m == PHM_A && koma->m_pp.m != PHM_A) return true; 
    if (koma->m_cp.m == PHM_EO && koma->m_pp.m == PHM_A) return true; 
    if (!(koma->m_pp.i == PHI_G || koma->m_pp.i == PHI_N || koma->m_pp.i == PHI_D || koma->m_pp.i == PHI_M ||
	koma->m_pp.i == PHI_B || koma->m_pp.i == PHI_S || koma->m_pp.i == PHI_K)) return true; 
    /* right morpheme */
    koma->m_Table.i.mfeat = f_inf;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    strcpy (koma->m_Table.i.morph, koma->m_Table.i.real);
    CopySyllable (koma->m_Table.i.morph+koma->m_Table.i.rlen-2, koma->m_pp.i, koma->m_pp.m, PHF_D);
    koma->m_Table.i.rfeat = f_vv_i | f_vj_i;
    koma->m_Table.i.rtags[0] = p_vv[0] | p_vj[0];
    koma->m_Table.i.rtags[1] = p_vv[1] | p_vj[1];
    return koma->PushHead(koma->m_Table.i.morph, S_E, NEXT);
}

/* h-irregularity: FIL = H
    (G|D|DD|R|M),(A|EO),_,? = (G|D|DD|R|M),(A|EO),_ + ?(e_hx) 
    NG,(YA|YEO),_,? = NG,(YA|YEO),_ + ?(e_hx) */
bool rule_initial_ending_h(CKoma *koma)
{
    /* condition */
    if (!(koma->m_Table.i.ffeat & f_e_hx)) return true;
    if (!(koma->m_pp.f == PHF_FIL)) return true;
    if (!(koma->m_pp.m == PHM_A || koma->m_pp.m == PHM_EO || koma->m_pp.m == PHM_YA || 
	koma->m_pp.m == PHM_YEO)) return true; 
    if (!(koma->m_pp.i == PHI_G || koma->m_pp.i == PHI_D || koma->m_pp.i == PHI_DD || 
	koma->m_pp.i == PHI_R || koma->m_pp.i == PHI_M || koma->m_pp.i == PHI_NG)) return true; 
    if ((koma->m_pp.m == PHM_A || koma->m_pp.m == PHM_EO) && koma->m_pp.i == PHI_NG) return true; 
    if (!(koma->m_pp.m == PHM_A || koma->m_pp.m == PHM_EO) && koma->m_pp.i != PHI_NG) return true; 
    /* right morpheme */
    koma->m_Table.i.mfeat = f_inf;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    strcpy (koma->m_Table.i.morph, koma->m_Table.i.real);
    CopySyllable (koma->m_Table.i.morph+koma->m_Table.i.rlen-2, koma->m_pp.i, koma->m_pp.m, PHF_H);
    koma->m_Table.i.real[koma->m_Table.i.rlen] = (char)0;
    koma->m_Table.i.rfeat = f_vj_i | f_vv_i;
    koma->m_Table.i.rtags[0] = p_vj[0] | p_vv[0];
    koma->m_Table.i.rtags[1] = p_vj[1] | p_vv[1];
    return koma->PushHead(koma->m_Table.i.morph, S_E, NEXT);
}

/* r-regularity: FIL = R
    _,? = _ + ?(e_lx) */
bool rule_initial_ending_r(CKoma *koma)
{
    /* condition */
    if (!(koma->m_Table.i.ffeat & f_e_lx)) return true;
    if (!(koma->m_pp.f == PHF_FIL)) return true;
    /* right morpheme */
    koma->m_Table.i.mfeat = f_def;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    strcpy (koma->m_Table.i.morph, koma->m_Table.i.real);
    CopySyllable (koma->m_Table.i.morph+koma->m_Table.i.rlen-2, koma->m_pp.i, koma->m_pp.m, PHF_R);
    koma->m_Table.i.rfeat = f_vv_r | f_vj_r;
    koma->m_Table.i.rtags[0] = p_vv[0] | p_vj[0];
    koma->m_Table.i.rtags[1] = p_vv[1] | p_vj[1];
    return koma->PushHead(koma->m_Table.i.morph, S_E, NEXT);
}

/* r-omission: FIL = R
    M,(EO|A),_,J,I,FIL = M,(EO|A),_ + J,I,FIL */
bool rule_initial_ending_r_omit(CKoma *koma)
{
    /* condition */
    if (koma->m_Table.i.rlen != 2) return true;
    if (koma->m_Table.i.flen != 2) return true;
    if (!(koma->m_pp.i == PHI_M && (koma->m_pp.m == PHM_A || koma->m_pp.m == PHM_EO) && koma->m_pp.f == PHF_FIL)) return true;
    if (!(koma->m_cp.i == PHI_J && koma->m_cp.m == PHM_I && koma->m_cp.f == PHF_FIL)) return true;
    /* right morpheme */
    koma->m_Table.i.mfeat = f_def;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    strcpy (koma->m_Table.i.morph, koma->m_Table.i.real);
    CopySyllable (koma->m_Table.i.morph+koma->m_Table.i.rlen-2, koma->m_pp.i, koma->m_pp.m, PHF_R);
    koma->m_Table.i.rfeat = f_vv_r | f_vj_r;
    koma->m_Table.i.rtags[0] = p_vv[0] | p_vj[0];
    koma->m_Table.i.rtags[1] = p_vv[1] | p_vj[1];
    return koma->PushHead(koma->m_Table.i.morph, S_E, NEXT);
}

/* irregularity: ha: jjeo
	EO,FIL = I,FIL,H,A,FIL
    NG,EO,FIL,JJ,_,? = NG,EO,FIL,JJ,_ + ? */
bool rule_initial_ending_ha_jjeo(CKoma *koma)
{
    /* condition */
    if (koma->m_Table.i.rlen != 4) return true;
    if (!(koma->m_Table.i.ffeat & f_e_l_v)) return true;
    if (koma->m_cp.i == PHI_NG && (koma->m_cp.m == PHM_A || koma->m_cp.m == PHM_EO)) return true;
    if (!(koma->m_pp.i == PHI_JJ && koma->m_pp.m == PHM_EO && koma->m_pp.f == PHF_FIL)) return true;
    if (!(koma->m_ppp.i == PHI_NG && koma->m_ppp.m == PHM_EO && koma->m_ppp.f == PHF_FIL)) return true;
    /* right morpheme */
    koma->m_Table.i.mfeat = f_def;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    strcpy (koma->m_Table.i.morph, koma->m_Table.i.real);
    CopySyllable (koma->m_Table.i.morph+koma->m_Table.i.rlen-2, koma->m_pp.i, PHM_I, PHF_FIL);
    CopySyllable (koma->m_Table.i.morph+koma->m_Table.i.rlen, PHI_H, PHM_A, PHF_FIL);
    koma->m_Table.i.rfeat = f_vv_i;
    koma->m_Table.i.rtags[0] = p_vv[0];
    koma->m_Table.i.rtags[1] = p_vv[1];
    if (!koma->PushHead(koma->m_Table.i.morph, S_E, NEXT)) return false;
    return true;
}

/* irregularity: ha: omit
	?,(D,A)|(J|I),FIL = ?,H,A,FIL + (D,A)|(J|I),FIL */
bool rule_initial_ending_ha_omit(CKoma *koma)
{
    /* condition */
    if (!koma->m_isHaOmit) return true;
    if (koma->m_Table.i.rlen < 2) return true;
    if (koma->m_Table.i.flen != 2) return true;
    if (!(koma->m_cp.f == PHF_FIL)) return true;
    if (!((koma->m_cp.i == PHI_D && koma->m_cp.m == PHM_A) || (koma->m_cp.i == PHI_J && koma->m_cp.m == PHM_I))) return true;
    /* right morpheme */
    koma->m_Table.i.mfeat = f_def;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    strcpy (koma->m_Table.i.morph, koma->m_Table.i.real);
    CopySyllable (koma->m_Table.i.morph+koma->m_Table.i.rlen, PHI_H, PHM_A, PHF_FIL);
    koma->m_Table.i.rfeat = f_vv_i | f_vj_i ;
    koma->m_Table.i.rtags[0] = p_vg[0] | p_xsv[0];
    koma->m_Table.i.rtags[1] = p_vg[1] | p_xsv[1];
    if (!koma->PushHead(koma->m_Table.i.morph, S_E, NEXT)) return false;
    return true;
}

/* i-omission:
    FIL,? = FIL + NG,I,FIL + ?(e_l_ix) */
bool rule_initial_ending_io(CKoma *koma)
{
    /* condition */
    if ((koma->m_pp.f != PHF_FIL) || !(koma->m_Table.i.ffeat & f_e_l_ix)) return true;
    /* right morpheme */
    koma->m_Table.i.mfeat = f_inf;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    koma->m_Table.i.morph[0] = (char)0;
    CopySyllable (koma->m_Table.i.morph, PHI_NG, PHM_I, PHF_FIL);
    koma->m_Table.i.mfeat = f_inf;
    koma->m_Table.i.mtags[0] = p_i[0];
    koma->m_Table.i.mtags[1] = p_i[1];
    if (!koma->PushMorph(koma->m_Table.i.morph, NEXT)) return false;
    koma->m_Table.i.rfeat = f_e_r_p | f_p_r_i;
    koma->m_Table.i.rtags[0] = p_n[0] | p_ef[0] | p_a[0] | p_p[0];
    koma->m_Table.i.rtags[1] = p_n[1] | p_ef[1] | p_a[1] | p_p[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_I, NEXT)) return false;
    /* s-omission:
	(G|NG),EO,FIL,? = (G|NG),EO,S(n_ir) + NG,I,FIL + ?(e_l_ix)
	M,UEO,FIL,? = M,UEO,S(n_ir) + NG,I,FIL + ?(e_l_ix) */
    if ((koma->m_pp.i == PHI_M && koma->m_pp.m == PHM_UEO) || 
	((koma->m_pp.i == PHI_G || koma->m_pp.i == PHI_NG) && koma->m_pp.m == PHM_EO)) 
    {
	strcpy (koma->m_Table.i.morph, koma->m_Table.i.real);
	CopySyllable (koma->m_Table.i.morph+koma->m_Table.i.rlen-2, koma->m_pp.i, koma->m_pp.m, PHF_S);
	koma->m_Table.i.rfeat = f_n_ir;
	koma->m_Table.i.rtags[0] = p_np[0] | p_nnb[0];
	koma->m_Table.i.rtags[1] = p_np[1] | p_nnb[1];
	if (!koma->PushHead(koma->m_Table.i.morph, S_I, NEXT)) return false;
	return true;
    }
    /* i-contraction:
	G,EOI,FIL,? = G,EO,S + NG,I,FIL + ?(e_l_ix)
	T,EOI,FIL,? = T,EO,FIL + NG,I,FIL + ?(e_l_ix) */
    if (koma->m_Table.i.rlen != 2) return true;
    if (!((koma->m_pp.i == PHI_T || koma->m_pp.i == PHI_G) && koma->m_pp.m == PHM_EOI)) return true; 
    strcpy (koma->m_Table.i.morph, koma->m_Table.i.real);
    if (koma->m_pp.i == PHI_T)
	CopySyllable (koma->m_Table.i.morph+koma->m_Table.i.rlen-2, koma->m_pp.i, PHM_EO, PHF_FIL);
    else
	CopySyllable (koma->m_Table.i.morph+koma->m_Table.i.rlen-2, koma->m_pp.i, PHM_EO, PHF_S);
    koma->m_Table.i.rfeat = f_n_ir;
    koma->m_Table.i.rtags[0] = p_nnb[0];
    koma->m_Table.i.rtags[1] = p_nnb[1];
    if (!koma->PushHead(koma->m_Table.i.morph, S_I, NEXT)) return false;
    return true;
}

bool rule_final_particle(CKoma *koma);
bool rule_final_ending(CKoma *koma);
bool rule_final(CKoma *koma)
{
    if (koma->m_cp.f == PHF_N || koma->m_cp.f == PHF_R || koma->m_cp.f == PHF_M || 
	koma->m_cp.f == PHF_B || koma->m_cp.f == PHF_NG) 
    {
	CopySyllable (koma->m_Table.i.form, PHI_FIL, PHM_FIL, koma->m_cp.f);
	CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, koma->m_cp.m, PHF_FIL);
    } 
    else if (koma->m_cp.f == PHF_RM) 
    {
	CopySyllable (koma->m_Table.i.form, PHI_FIL, PHM_FIL, PHF_M);
	CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, koma->m_cp.m, PHF_R);
    }
    else return true;
    if (!lookup(koma)) return false;
    if (!(koma->m_Table.i.ftags[0] || koma->m_Table.i.ftags[1])) return true;
    if (!rule_final_particle(koma)) return false;
    if (!rule_final_ending(koma)) return false;
    return true;
}

bool rule_final_particle_s(CKoma *koma);
bool rule_final_particle(CKoma *koma)
{
    /* condition */
    if (!(koma->m_Table.i.pred == S_WE)) return true;
    if (!(koma->m_cp.f == PHF_N || koma->m_cp.f == PHF_R)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_p[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_p[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if ((koma->m_Table.i.pfeat & f_p_r_i) && !(koma->m_Table.i.ffeat & f_p_r_i)) return true;
    /* right morpheme */
    koma->m_Table.i.mfeat = f_def;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    koma->m_Table.i.rfeat = f_e_r_p;
    koma->m_Table.i.rtags[0] = p_n[0] | p_ef[0] | p_a[0];
    koma->m_Table.i.rtags[1] = p_n[1] | p_ef[1] | p_a[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_P, NEXT)) return false;
    /* inflection */
    if (!rule_final_particle_s(koma)) return false;
    return true;
}

/* s-omission: 
    M,UEO,?(p_v) = M,UEO,S + ?(p_v)
    (G|NG),EO,?(p_v) = (G|NG),EO,S + ?(p_v) */
bool rule_final_particle_s(CKoma *koma)
{
    /* condition */
    if (!((koma->m_Table.i.ffeat & f_p_v) && 
	((koma->m_cp.i == PHI_M && koma->m_cp.m == PHM_UEO) || 
	((koma->m_cp.i == PHI_G || koma->m_cp.i == PHI_NG) && koma->m_cp.m == PHM_EO)))) return true;
    /* right morpheme */
    koma->m_Table.i.mfeat = f_inf;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    strcpy (koma->m_Table.i.morph, koma->m_Table.i.real);
    CopySyllable (koma->m_Table.i.morph+koma->m_Table.i.rlen, koma->m_cp.i, koma->m_cp.m, PHF_S);
    koma->m_Table.i.rfeat = f_n_ir;
    koma->m_Table.i.rtags[0] = p_np[0] | p_nnb[0];
    koma->m_Table.i.rtags[1] = p_np[1] | p_nnb[1];
    if (!koma->PushHead(koma->m_Table.i.morph, S_P, NEXT)) return false;
    return true;
}

bool rule_final_ending_io(CKoma *koma);
bool rule_final_ending_h(CKoma *koma);
bool rule_final_ending_r(CKoma *koma);
bool rule_final_ending_b(CKoma *koma);
bool rule_final_ending_ha_jjeo(CKoma *koma);
bool rule_final_ending(CKoma *koma)
{
    /* condition */
    if (!(koma->m_Table.i.pred == S_WE || koma->m_Table.i.pred == S_E || koma->m_Table.i.pred == S_VX ||
	koma->m_Table.i.pred == S_I || koma->m_Table.i.pred == S_P)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    /* left morpheme */
    koma->m_Table.i.mfeat = f_def;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* right morpheme */
    koma->m_Table.i.rfeat = f_v_r | f_v_i | f_v_l;
    koma->m_Table.i.rtags[0] = 0;
    koma->m_Table.i.rtags[1] = 0;
    if (koma->m_Table.i.ffeat & f_e_l_v) 
    {
	koma->m_Table.i.rtags[0] |= p_vv[0];
	koma->m_Table.i.rtags[1] |= p_vv[1];
    }
    if ((koma->m_Table.i.ffeat & f_e_l_j) &&
	!((koma->m_Table.i.rlen == 2) && 
	    koma->m_cp.i == PHI_N && koma->m_cp.m == PHM_I &&
	    koma->m_pp.i == PHI_NG && koma->m_pp.m == PHM_A && koma->m_pp.f == PHF_FIL)) 
    {
        koma->m_Table.i.rtags[0] |= p_vj[0];
        koma->m_Table.i.rtags[1] |= p_vj[1];
    }
    if ((koma->m_Table.i.ffeat & f_e_l_i) &&
	(koma->m_cp.i == PHI_NG && koma->m_cp.m == PHM_I)) 
    {
        koma->m_Table.i.rtags[0] |= p_i[0];
        koma->m_Table.i.rtags[1] |= p_i[1];
    }
    if ((koma->m_Table.i.ffeat & f_e_l_i) && (koma->m_Table.i.rlen == 2) &&
	(koma->m_cp.i == PHI_N && koma->m_cp.m == PHM_I &&
	koma->m_pp.i == PHI_NG && koma->m_pp.m == PHM_A && koma->m_pp.f == PHF_FIL)) 
    {
        koma->m_Table.i.rtags[0] |= p_vj[0];
        koma->m_Table.i.rtags[1] |= p_vj[1];
    }
    if (!((koma->m_Table.i.mtags[0] & p_ep[0]) || (koma->m_Table.i.mtags[1] & p_ep[1])) &&
        (koma->m_Table.i.ffeat & f_e_l_p)) 
    {
        koma->m_Table.i.rtags[0] |= p_ep[0];
        koma->m_Table.i.rtags[1] |= p_ep[1];
    }
    if (koma->m_Table.i.rtags[0] || koma->m_Table.i.rtags[1]) 
    {
	if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    }
    /* inflection */
    if (!rule_final_ending_h(koma)) return false;
    if (!rule_final_ending_r(koma)) return false;
    if (!rule_final_ending_b(koma)) return false;
    if (!rule_final_ending_ha_jjeo(koma)) return false;
    if (!rule_final_ending_io(koma)) return false;
    return true;
}

/* h-irregularity: FIL = H
    (N|G|D|DD|R|M),(A|EO),_,? = (N|G|D|DD|R|M),(A|EO),_ + ?(e_hx) 
    NG,(YA|YEO),_,? = NG,(YA|YEO),_ + ?(e_hx) */
bool rule_final_ending_h(CKoma *koma)
{
    /* condition */
    if (!(koma->m_Table.i.ffeat & f_e_hx)) return true;
    if (!(koma->m_cp.m == PHM_A || koma->m_cp.m == PHM_EO || koma->m_cp.m == PHM_YA || 
	koma->m_cp.m == PHM_YEO)) return true; 
    if (!(koma->m_cp.i == PHI_N || koma->m_cp.i == PHI_G || koma->m_cp.i == PHI_D || koma->m_cp.i == PHI_DD || 
	koma->m_cp.i == PHI_R || koma->m_cp.i == PHI_M || koma->m_cp.i == PHI_NG)) return true; 
    if ((koma->m_cp.m == PHM_A || koma->m_cp.m == PHM_EO) && koma->m_cp.i == PHI_NG) return true; 
    if (!(koma->m_cp.m == PHM_A || koma->m_cp.m == PHM_EO) && koma->m_cp.i != PHI_NG) return true; 
    /* right morpheme */
    koma->m_Table.i.mfeat = f_inf;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    strcpy (koma->m_Table.i.morph, koma->m_Table.i.real);
    CopySyllable (koma->m_Table.i.morph+koma->m_Table.i.rlen, koma->m_cp.i, koma->m_cp.m, PHF_H);
    koma->m_Table.i.rfeat = f_vj_i | f_vv_i ;
    koma->m_Table.i.rtags[0] = p_vj[0] | p_vv[0];
    koma->m_Table.i.rtags[1] = p_vj[1] | p_vv[1];
    if (!koma->PushHead(koma->m_Table.i.morph, S_E, NEXT)) return false;
    return true;
}

/* r-regularity: FIL = R
    _,? = _ + ?(e_lx) */
bool rule_final_ending_r(CKoma *koma)
{
    /* condition */
    if (!(koma->m_Table.i.ffeat & f_e_lx)) return true;
    if (koma->m_cp.f == PHF_RM || koma->m_cp.f == PHF_M) return true;
    /* right morpheme */
    koma->m_Table.i.mfeat = f_def;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    strcpy (koma->m_Table.i.morph, koma->m_Table.i.real);
    CopySyllable (koma->m_Table.i.morph+koma->m_Table.i.rlen, koma->m_cp.i, koma->m_cp.m, PHF_R);
    koma->m_Table.i.rfeat = f_vv_r | f_vj_r;
    koma->m_Table.i.rtags[0] = p_vv[0] | p_vj[0];
    koma->m_Table.i.rtags[1] = p_vv[1] | p_vj[1];
    if (!koma->PushHead(koma->m_Table.i.morph, S_E, NEXT)) return false;
    return true;
}

/* b-regularity: 
	FIL = B
    S,EU,FIL,R,EO,_,N = S,EU,FIL,R,EO,_ + N */
bool rule_final_ending_b(CKoma *koma)
{
    /* condition */
    if (koma->m_Table.i.flen != 2) return true;
    if (koma->m_cp.f != PHF_N) return true;
    if (!(koma->m_cp.i == PHI_R && koma->m_cp.m == PHM_EO)) return true;
    if (!(koma->m_pp.i == PHI_S && koma->m_pp.m == PHM_EU && koma->m_pp.f == PHF_FIL)) return true;
    /* right morpheme */
    koma->m_Table.i.mfeat = f_inf;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    strcpy (koma->m_Table.i.morph, koma->m_Table.i.real);
    CopySyllable (koma->m_Table.i.morph+koma->m_Table.i.rlen, koma->m_cp.i, koma->m_cp.m, PHF_B);
    koma->m_Table.i.rfeat = f_vv_i | f_vj_i;
    koma->m_Table.i.rtags[0] = p_vv[0] | p_vj[0];
    koma->m_Table.i.rtags[1] = p_vv[1] | p_vj[1];
    if (!koma->PushHead(koma->m_Table.i.morph, S_E, NEXT)) return false;
    return true;
}

/* irregularity: ha: jjeo
	EO = I,FIL,H,A,FIL + 
    NG,EO,FIL,JJ,_,? = NG,EO,FIL,JJ,_,_,_,_,_ + ? */
bool rule_final_ending_ha_jjeo(CKoma *koma)
{
    /* condition */
    if (!(koma->m_Table.i.ffeat & f_e_l_v)) return true;
    if (koma->m_Table.i.rlen != 2) return true;
    if (!(koma->m_cp.i == PHI_JJ && koma->m_cp.m == PHM_EO)) return true;
    if (!(koma->m_pp.i == PHI_NG && koma->m_pp.m == PHM_EO && koma->m_pp.f == PHF_FIL)) return true;
    /* right morpheme */
    koma->m_Table.i.mfeat = f_def;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    strcpy (koma->m_Table.i.morph, koma->m_Table.i.real);
    CopySyllable (koma->m_Table.i.morph+koma->m_Table.i.rlen, koma->m_cp.i, PHM_I, PHF_FIL);
    CopySyllable (koma->m_Table.i.morph+koma->m_Table.i.rlen+2, PHI_H, PHM_A, PHF_FIL);
    koma->m_Table.i.rfeat = f_vv_i;
    koma->m_Table.i.rtags[0] = p_vv[0];
    koma->m_Table.i.rtags[1] = p_vv[1];
    if (!koma->PushHead(koma->m_Table.i.morph, S_E, NEXT)) return false;
    return true;
}

/* i-omission:
    FIL,? = FIL + NG,I,FIL + ?(e_l_ix) */
bool rule_final_ending_io(CKoma *koma)
{
    /* condition */
    if (!(koma->m_Table.i.ffeat & f_e_l_ix)) return true;
    /* right morpheme */
    koma->m_Table.i.mfeat = f_inf;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    koma->m_Table.i.morph[0] = (char)0;
    CopySyllable (koma->m_Table.i.morph, PHI_NG, PHM_I, PHF_FIL);
    koma->m_Table.i.mfeat = f_inf;
    koma->m_Table.i.mtags[0] = p_i[0];
    koma->m_Table.i.mtags[1] = p_i[1];
    if (!koma->PushMorph(koma->m_Table.i.morph, NEXT)) return false;
    koma->m_Table.i.rfeat = f_e_r_p | f_p_r_i;
    koma->m_Table.i.rtags[0] = p_n[0] | p_ef[0] | p_a[0] | p_p[0];
    koma->m_Table.i.rtags[1] = p_n[1] | p_ef[1] | p_a[1] | p_p[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_I, NEXT)) return false;
    /* s-omission:
	(G|NG),EO,FIL,? = (G|NG),EO,S(n_ir) + NG,I,FIL + ?(e_l_ix)
	M,UEO,FIL,? = M,UEO,S(n_ir) + NG,I,FIL + ?(e_l_ix) */
    if ((koma->m_cp.i == PHI_M && koma->m_cp.m == PHM_UEO) || 
	((koma->m_cp.i == PHI_G || koma->m_cp.i == PHI_NG) && koma->m_cp.m == PHM_EO)) 
    {
	strcpy (koma->m_Table.i.morph, koma->m_Table.i.real);
	CopySyllable (koma->m_Table.i.morph+koma->m_Table.i.rlen, koma->m_cp.i, koma->m_cp.m, PHF_S);
	koma->m_Table.i.rfeat = f_n_ir;
	koma->m_Table.i.rtags[0] = p_np[0] | p_nnb[0];
	koma->m_Table.i.rtags[1] = p_np[1] | p_nnb[1];
	if (!koma->PushHead(koma->m_Table.i.morph, S_I, NEXT)) return false;
	return true;
    }
    /* i-contraction:
	G,EOI,FIL,? = G,EO,S + NG,I,FIL + ?(e_l_ix)
	T,EOI,FIL,? = T,EO,FIL + NG,I,FIL + ?(e_l_ix) */
    if (koma->m_Table.i.rlen != 0) return true;
    if (!((koma->m_cp.i == PHI_T || koma->m_cp.i == PHI_G) && koma->m_cp.m == PHM_EOI)) return true; 
    strcpy (koma->m_Table.i.morph, koma->m_Table.i.real);
    if (koma->m_cp.i == PHI_T)
	CopySyllable (koma->m_Table.i.morph+koma->m_Table.i.rlen, koma->m_cp.i, PHM_EO, PHF_FIL);
    else
	CopySyllable (koma->m_Table.i.morph+koma->m_Table.i.rlen, koma->m_cp.i, PHM_EO, PHF_S);
    koma->m_Table.i.rfeat = f_n_ir;
    koma->m_Table.i.rtags[0] = p_nnb[0];
    koma->m_Table.i.rtags[1] = p_nnb[1];
    if (!koma->PushHead(koma->m_Table.i.morph, S_I, NEXT)) return false;
    return true;
}

bool rule_inf_particle(CKoma *koma);
bool rule_inf_ending(CKoma *koma);
bool rule_inf(CKoma *koma)
{
    if (!rule_inf_particle(koma)) return false;
    if (!rule_inf_ending(koma)) return false;
    return true;
}

bool rule_inf_particle_ii_eui(CKoma *koma);
bool rule_inf_particle_ii_geoi(CKoma *koma);
bool rule_inf_particle_s_i(CKoma *koma);
bool rule_inf_particle_s_WE(CKoma *koma);
bool rule_inf_particle_g(CKoma *koma);
bool rule_inf_particle_gi(CKoma *koma);
bool rule_inf_particle(CKoma *koma)
{
  /*  if (koma->m_Table.i.wi != 0 || koma->m_Table.topw != 1) return true; */
    if (!(koma->m_Table.i.pred == S_WE)) return true;
    if ((koma->m_Table.i.pfeat & f_p_r_i)) return true;
    /*    if (koma->m_Table.i.wi+1 != koma->m_Table.topw) return true;*/
    if (!((koma->m_Table.i.ptags[0] & p_p[0]) || 
	(koma->m_Table.i.ptags[1] & p_p[0]))) return true;
    /* inflection */
    if (!rule_inf_particle_ii_eui(koma)) return false;
    if (!rule_inf_particle_ii_geoi(koma)) return false;
    if (!rule_inf_particle_s_i(koma)) return false;
    if (!rule_inf_particle_s_WE(koma)) return false;
    if (!rule_inf_particle_g(koma)) return false;
    if (!rule_inf_particle_gi(koma)) return false;
    return true;
}

/* i-insertion: eui
    N,AI,FIL = N,A,FIL + NG,EUI,FIL 
    (N|J),EOI,FIL = (N|J),EO,FIL + NG,EUI,FIL */
bool rule_inf_particle_ii_eui(CKoma *koma)
{
    /* condition */
    if (koma->m_Table.i.rlen != 0 || koma->m_Table.i.flen != 2) return true;
    if (!(koma->m_cp.f == PHF_FIL)) return true; 
    if (!((koma->m_cp.i == PHI_N && koma->m_cp.m == PHM_AI) || 
	((koma->m_cp.i == PHI_N || koma->m_cp.i == PHI_J) && koma->m_cp.m == PHM_EOI))) return true; 
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_EUI, PHF_FIL);
    koma->m_Table.i.mfeat = f_inf;
    koma->m_Table.i.mtags[0] = p_pd[0];
    koma->m_Table.i.mtags[1] = p_pd[1];
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    if (koma->m_cp.m == PHM_AI)
	CopySyllable (koma->m_Table.i.real, koma->m_cp.i, PHM_A, PHF_FIL);
    else
	CopySyllable (koma->m_Table.i.real, koma->m_cp.i, PHM_EO, PHF_FIL);
    koma->m_Table.i.rfeat = f_n_ir;
    koma->m_Table.i.rtags[0] = p_np[0];
    koma->m_Table.i.rtags[1] = p_np[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_P, NEXT)) return false;
    return true;
}

/* i-insertion: geoi
    N,AI,FIL,G,EOI = N,A,FIL + NG,EOI,FIL,G,EOI 
    (N|J),EOI,FIL,G,EOI = (N|J),EO,FIL + NG,EOI,FIL,G,EOI */
bool rule_inf_particle_ii_geoi(CKoma *koma)
{
    /* condition */
    if (koma->m_Table.i.rlen != 0 || koma->m_Table.i.flen < 4) return true;
    if (!(koma->m_np.i == PHI_G && koma->m_np.m == PHM_EOI && koma->m_cp.f == PHF_FIL)) return true; 
    if (!((koma->m_cp.i == PHI_N && koma->m_cp.m == PHM_AI) || 
	((koma->m_cp.i == PHI_N || koma->m_cp.i == PHI_J) && koma->m_cp.m == PHM_EOI))) return true; 
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_EOI, PHF_FIL);
    if (!lookup(koma)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_p[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_p[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    koma->m_Table.i.mfeat = f_inf;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    if (koma->m_cp.m == PHM_AI)
	CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_A, PHF_FIL);
    else 
	CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_EO, PHF_FIL);
    koma->m_Table.i.rfeat = f_n_ir;
    koma->m_Table.i.rtags[0] = p_np[0];
    koma->m_Table.i.rtags[1] = p_np[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_P, NEXT)) return false;
    return true;
}

/* s-omission: i
    (G|NG),EOI,FIL = (G|NG),EO,S + NG,I,FIL 
    M,UEOI,FIL = M,UEO,S + NG,I,FIL */
bool rule_inf_particle_s_i(CKoma *koma)
{
    /* condition */
    if (koma->m_Table.i.flen != 2) return true;
    if (!(koma->m_cp.f == PHF_FIL)) return true; 
    if (!((koma->m_cp.i == PHI_M && koma->m_cp.m == PHM_UEOI) || 
	((koma->m_cp.i == PHI_G || koma->m_cp.i == PHI_NG) && koma->m_cp.m == PHM_EOI))) return true; 
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_I, PHF_FIL);
    koma->m_Table.i.mfeat = f_inf;
    koma->m_Table.i.mtags[0] = p_ps[0] | p_pc[0];
    koma->m_Table.i.mtags[1] = p_ps[1] | p_pc[1];
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    if (koma->m_cp.m == PHM_EOI)
	CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_EO, PHF_S);
    else 
	CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_UEO, PHF_S);
    koma->m_Table.i.rfeat = f_n_ir;
    koma->m_Table.i.rtags[0] = p_np[0] | p_nnb[0];
    koma->m_Table.i.rtags[1] = p_np[1] | p_nnb[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_P, NEXT)) return false;
    return true;
}

/* g-omission:
    J,EOI = J,EO,G + NG,EOI(p_ir) */ 
bool rule_inf_particle_g(CKoma *koma)
{
    /* condition */
    if (koma->m_Table.i.rlen != 0) return true;
    if (!(koma->m_cp.i == PHI_J && koma->m_cp.m == PHM_EOI)) return true;
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_EOI, koma->m_cp.f);
    if (!lookup(koma)) return true;
    if (!(koma->m_Table.i.ffeat & f_p_ir)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_p[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_p[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    koma->m_Table.i.mfeat = f_inf;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_EO, PHF_G);
    koma->m_Table.i.rfeat = f_n_ir;
    koma->m_Table.i.rtags[0] = p_nnb[0];
    koma->m_Table.i.rtags[1] = p_nnb[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_P, NEXT)) return false;
    return true;
}

/* s-omission: WE
    (G|NG),EO,FIL = (G|NG),EO,S  
    M,UEO,FIL = M,UEO,S 
   geo-contraction: WE
    G,EO,FIL = G,EU,FIL,G,EO,S  
    G,EO,FIL = G,EO,FIL,G,I,FIL
*/
bool rule_inf_particle_s_WE(CKoma *koma)
{
    /* condition */
    if (koma->m_Table.i.flen != 2) return true;
    if (!(koma->m_Table.i.pred == S_WE)) return true;
    if (!(koma->m_cp.f == PHF_FIL)) return true; 
    if (!((koma->m_cp.i == PHI_M && koma->m_cp.m == PHM_UEO) || 
	((koma->m_cp.i == PHI_G || koma->m_cp.i == PHI_NG) && koma->m_cp.m == PHM_EO))) return true; 
    split(koma);
    /* left morpheme */
    if (koma->m_cp.m == PHM_EO)
	CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_EO, PHF_S);
    else 
	CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_UEO, PHF_S);
    koma->m_Table.i.rfeat = f_n_ir;
    koma->m_Table.i.rtags[0] = p_np[0] | p_nnb[0];
    koma->m_Table.i.rtags[1] = p_np[1] | p_nnb[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_WE, koma->m_Table.i.next)) return false;
    if (! (koma->m_Table.i.rlen == 0 && koma->m_cp.i == PHI_G && koma->m_cp.m == PHM_EO) ) return true; 
    /* G,EO,FIL = G,EU,FIL,G,EO,S */
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_EU, PHF_FIL);
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen+2, koma->m_cp.i, PHM_EO, PHF_S);
    koma->m_Table.i.rfeat = f_n_ir;
    koma->m_Table.i.rtags[0] = p_np[0];
    koma->m_Table.i.rtags[1] = p_np[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_WE, koma->m_Table.i.next)) return false;
    /* G,EO,FIL = G,EO,FIL,G,I,FIL */
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_EO, PHF_FIL);
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen+2, koma->m_cp.i, PHM_I, PHF_FIL);
    koma->m_Table.i.rfeat = f_n_ir;
    koma->m_Table.i.rtags[0] = p_np[0] | p_a[0];
    koma->m_Table.i.rtags[1] = p_np[1] | p_a[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_WE, koma->m_Table.i.next)) return false;
    return true;
}

/* gi-omission:
    (G|J),EOI = (G|J),EO,FIL,G,I,FIL
    NG,YEOI = NG,YEO,FIL,G,I,FIL */ 
bool rule_inf_particle_gi(CKoma *koma)
{
    /* condition */
    if (koma->m_Table.i.rlen != 0) return true;
    if (koma->m_Table.i.flen != 2) return true;
    if (!(koma->m_Table.i.pred == S_WE)) return true;
    if (!((koma->m_cp.i == PHI_NG && koma->m_cp.m == PHM_YEOI) || 
	((koma->m_cp.i == PHI_G || koma->m_cp.i == PHI_J) && koma->m_cp.m == PHM_EOI))) return true; 
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_EOI, koma->m_cp.f);
    if (!lookup(koma)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_p[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_p[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    koma->m_Table.i.mfeat = f_inf;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    if (koma->m_cp.m == PHM_EOI)
	CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_EO, PHF_FIL);
    else 
	CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_YEO, PHF_FIL);
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen+2, PHI_G, PHM_I, PHF_FIL);
    koma->m_Table.i.rfeat = f_n_ir;
    koma->m_Table.i.rtags[0] = p_np[0];
    koma->m_Table.i.rtags[1] = p_np[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_P, NEXT)) return false;
    return true;
}

bool rule_inf_ending_b_eu(CKoma *koma);
bool rule_inf_ending_b_a(CKoma *koma);
bool rule_inf_ending_b_eo(CKoma *koma);
bool rule_inf_ending_h_ai_a(CKoma *koma);
bool rule_inf_ending_h_ai_eo(CKoma *koma);
bool rule_inf_ending_h_yai_a(CKoma *koma);
bool rule_inf_ending_h_yai_eo(CKoma *koma);
bool rule_inf_ending_reu_a(CKoma *koma);
bool rule_inf_ending_reu_eo(CKoma *koma);
bool rule_inf_ending_u(CKoma *koma);
bool rule_inf_ending_neora(CKoma *koma);
bool rule_inf_ending_n(CKoma *koma);
bool rule_inf_ending_ha_ai(CKoma *koma);
bool rule_inf_ending_ha_yeo(CKoma *koma);
bool rule_inf_ending_ha_jjai(CKoma *koma);
bool rule_inf_ending_ha_ktch(CKoma *koma);
bool rule_inf_ending_reo(CKoma *koma);
bool rule_inf_ending_rai(CKoma *koma);
bool rule_inf_ending_a_eu(CKoma *koma);
bool rule_inf_ending_a_a(CKoma *koma);
bool rule_inf_ending_a_r(CKoma *koma);
bool rule_inf_ending_a_o(CKoma *koma);
bool rule_inf_ending_a_oh(CKoma *koma);
bool rule_inf_ending_a_ah(CKoma *koma);
bool rule_inf_ending_eo_eu(CKoma *koma);
bool rule_inf_ending_eo_s(CKoma *koma);
bool rule_inf_ending_eo_eo(CKoma *koma);
bool rule_inf_ending_eo_u(CKoma *koma);
bool rule_inf_ending_eo_ai(CKoma *koma);
bool rule_inf_ending_eo_oai(CKoma *koma);
bool rule_inf_ending_eo_eoi(CKoma *koma);
bool rule_inf_ending_eo_yeo(CKoma *koma);
bool rule_inf_ending_eo_i(CKoma *koma);
bool rule_inf_ending_i_eoi(CKoma *koma);
bool rule_inf_ending_i_ya(CKoma *koma);
bool rule_inf_ending_i_yo(CKoma *koma);
bool rule_inf_ending(CKoma *koma)
{
    if (!(koma->m_Table.i.pred == S_WE || koma->m_Table.i.pred == S_E || 
	koma->m_Table.i.pred == S_P || koma->m_Table.i.pred == S_I || 
	koma->m_Table.i.pred == S_VX)) return true;
    if (!((koma->m_Table.i.ptags[0] & p_e[0]) || 
	(koma->m_Table.i.ptags[1] & p_e[1]))) return true;
    /* inflection */
    if (!rule_inf_ending_b_eu(koma)) return false;
    if (!rule_inf_ending_b_a(koma)) return false;
    if (!rule_inf_ending_b_eo(koma)) return false;
    if (!rule_inf_ending_h_ai_a(koma)) return false;
    if (!rule_inf_ending_h_ai_eo(koma)) return false;
    if (!rule_inf_ending_h_yai_a(koma)) return false;
    if (!rule_inf_ending_h_yai_eo(koma)) return false;
    if (!rule_inf_ending_reu_a(koma)) return false;
    if (!rule_inf_ending_reu_eo(koma)) return false;
    if (!rule_inf_ending_u(koma)) return false;
    if (!rule_inf_ending_neora(koma)) return false;
    if (!rule_inf_ending_n(koma)) return false;
    if (!rule_inf_ending_ha_ai(koma)) return false;
    if (!rule_inf_ending_ha_yeo(koma)) return false;
    if (!rule_inf_ending_ha_jjai(koma)) return false;
    if (!rule_inf_ending_ha_ktch(koma)) return false;
    if (!rule_inf_ending_reo(koma)) return false;
    if (!rule_inf_ending_rai(koma)) return false;
    if (!rule_inf_ending_a_eu(koma)) return false;
    if (!rule_inf_ending_a_a(koma)) return false;
    if (!rule_inf_ending_a_r(koma)) return false;
    if (!rule_inf_ending_a_o(koma)) return false;
    if (!rule_inf_ending_a_oh(koma)) return false;
    if (!rule_inf_ending_a_ah(koma)) return false;
    if (!rule_inf_ending_eo_eu(koma)) return false;
    if (!rule_inf_ending_eo_s(koma)) return false;
    if (!rule_inf_ending_eo_eo(koma)) return false;
    if (!rule_inf_ending_eo_u(koma)) return false;
    if (!rule_inf_ending_eo_ai(koma)) return false;
    if (!rule_inf_ending_eo_oai(koma)) return false;
    if (!rule_inf_ending_eo_eoi(koma)) return false;
    if (!rule_inf_ending_eo_yeo(koma)) return false;
    if (!rule_inf_ending_eo_i(koma)) return false;
    if (!rule_inf_ending_i_eoi(koma)) return false;
    if (!rule_inf_ending_i_ya(koma)) return false;
    if (!rule_inf_ending_i_yo(koma)) return false;
    return true;
}

/* irregularity: b: eu
	FIL,NG,U = B + NG,EU
	modified -> FIL,NG,U = B + FIL,FIL
    _,_,_,(FIL|N|R|M|B|NG) = _ + _,_,(FIL|N|R|M|B|NG) */
bool rule_inf_ending_b_eu(CKoma *koma)
{
    char temp[40];
    /* condition */
    if (koma->m_Table.i.rlen < 2) return true;
    if (!(koma->m_pp.f == PHF_FIL && koma->m_cp.i == PHI_NG && koma->m_cp.m == PHM_U)) return true;
    if (!(koma->m_cp.f == PHF_FIL || koma->m_cp.f == PHF_N || koma->m_cp.f == PHF_R || koma->m_cp.f == PHF_M || 
	koma->m_cp.f == PHF_B || koma->m_cp.f == PHF_NG)) return true;
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_EU, koma->m_cp.f);
    if (!lookup(koma)) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    if ((koma->m_Table.i.ffeat & f_e_s) && !(koma->m_Table.i.ffeat & f_e_c)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    koma->m_Table.i.mfeat = f_inf;
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if (koma->m_cp.f != PHF_FIL)
        CopySyllable (koma->m_Table.i.form, PHI_FIL, PHM_FIL, koma->m_cp.f);
    else
    {
        strcpy (temp, (koma->m_Table.i.form)+2);
        strcpy (koma->m_Table.i.form, temp);
    }
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen-2, koma->m_pp.i, koma->m_pp.m, PHF_B);
    koma->m_Table.i.rfeat = f_vv_i | f_vj_i;
    koma->m_Table.i.rtags[0] = p_vv[0] | p_vj[0];
    koma->m_Table.i.rtags[1] = p_vv[1] | p_vj[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    return true;
}

/* irregularity: b: a
    (G|D|R),(O|A),FIL,NG,OA,(FIL|SS) = (G|D|R),(O|A),B + NG,A,(FIL|SS) */
bool rule_inf_ending_b_a(CKoma *koma)
{
    /* condition */
    if (koma->m_Table.i.rlen < 2) return true;
    if (!(koma->m_cp.m == PHM_OA && (koma->m_cp.f == PHF_FIL || koma->m_cp.f == PHF_SS))) return true;
    if (!(koma->m_cp.i == PHI_NG && koma->m_pp.f == PHF_FIL)) return true;
    if (!((koma->m_pp.m == PHM_O || koma->m_pp.m == PHM_A) && 
	(koma->m_pp.i == PHI_G || koma->m_pp.i == PHI_D || koma->m_pp.i == PHI_R))) return true;
    if (!(koma->m_pp.m == PHM_O || koma->m_pp.m == PHM_A)) return true;
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_A, koma->m_cp.f);
    if (!lookup(koma)) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    if (!(koma->m_Table.i.ffeat & f_e_ir)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    koma->m_Table.i.mfeat = f_inf;
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen-2, koma->m_pp.i, koma->m_pp.m, PHF_B);
    koma->m_Table.i.rfeat = f_vv_i | f_vj_i;
    koma->m_Table.i.rtags[0] = p_vv[0] | p_vj[0];
    koma->m_Table.i.rtags[1] = p_vv[1] | p_vj[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    return true;
}

/* irregularity: b: eo
    ~((G|D),O,)FIL,NG,UEO,(FIL|SS) = ~((G|D),O,)B + NG,EO,(FIL|SS) */
bool rule_inf_ending_b_eo(CKoma *koma)
{
    /* condition */
    if (koma->m_Table.i.rlen < 2) return true;
    if (!(koma->m_cp.m == PHM_UEO && (koma->m_cp.f == PHF_FIL || koma->m_cp.f == PHF_SS))) return true;
    if (!(koma->m_cp.i == PHI_NG && koma->m_pp.f == PHF_FIL)) return true;
    if ((koma->m_pp.m == PHM_O && (koma->m_pp.i == PHI_G || koma->m_pp.i == PHI_D))) return true;
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_EO, koma->m_cp.f);
    if (!lookup(koma)) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    if (!(koma->m_Table.i.ffeat & f_e_ir)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    koma->m_Table.i.mfeat = f_inf;
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen-2, koma->m_pp.i, koma->m_pp.m, PHF_B);
    koma->m_Table.i.rfeat = f_vv_i | f_vj_i;
    koma->m_Table.i.rtags[0] = p_vv[0] | p_vj[0];
    koma->m_Table.i.rtags[1] = p_vv[1] | p_vj[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    return true;
}

/* irregularity: h: ai: a
    (G|D|DD|R|M),AI,(FIL|SS) = (G|D|DD|R|M),A,H + NG,A,(FIL|SS) */
bool rule_inf_ending_h_ai_a(CKoma *koma)
{
    /* condition */
    if (koma->m_Table.i.rlen<2) return true;
    if (!(koma->m_cp.m == PHM_AI && (koma->m_cp.f == PHF_FIL || koma->m_cp.f == PHF_SS))) return true;
    if (!(koma->m_cp.i == PHI_G || koma->m_cp.i == PHI_D || koma->m_cp.i == PHI_DD || koma->m_cp.i == PHI_R || koma->m_cp.i == PHI_M)) 
	return true;
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_A, koma->m_cp.f);
    if (!lookup(koma)) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    if (!(koma->m_Table.i.ffeat & f_e_ir)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    koma->m_Table.i.mfeat = f_inf;
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_A, PHF_H);
    koma->m_Table.i.rfeat = f_vj_i;
    koma->m_Table.i.rtags[0] = p_vj[0];
    koma->m_Table.i.rtags[1] = p_vj[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    return true;
}

/* irregularity: h: ai: eo
    (G|D|DD|R|M),AI,(FIL|SS) = (G|D|DD|R|M),EO,H + NG,EO,(FIL|SS) */
bool rule_inf_ending_h_ai_eo(CKoma *koma)
{
    /* condition */
    if (koma->m_Table.i.rlen<2) return true;
    if (!(koma->m_cp.m == PHM_AI && (koma->m_cp.f == PHF_FIL || koma->m_cp.f == PHF_SS))) return true;
    if (!(koma->m_cp.i == PHI_G || koma->m_cp.i == PHI_D || koma->m_cp.i == PHI_DD || koma->m_cp.i == PHI_R || koma->m_cp.i == PHI_M)) 
	return true;
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_EO, koma->m_cp.f);
    if (!lookup(koma)) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    if (!(koma->m_Table.i.ffeat & f_e_ir)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    koma->m_Table.i.mfeat = f_inf;
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_EO, PHF_H);
    koma->m_Table.i.rfeat = f_vj_i;
    koma->m_Table.i.rtags[0] = p_vj[0];
    koma->m_Table.i.rtags[1] = p_vj[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    return true;
}

/* irregularity: h: yai: a
    NG,YAI,(FIL|SS) = NG,YA,H + NG,A,(FIL|SS) */
bool rule_inf_ending_h_yai_a(CKoma *koma)
{
    /* condition */
    if (koma->m_Table.i.rlen<2) return true;
    if (!(koma->m_cp.i == PHI_NG && koma->m_cp.m == PHM_YAI && (koma->m_cp.f == PHF_FIL || koma->m_cp.f == PHF_SS))) 
	return true;
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_A, koma->m_cp.f);
    if (!lookup(koma)) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    if (!(koma->m_Table.i.ffeat & f_e_ir)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    koma->m_Table.i.mfeat = f_inf;
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_YA, PHF_H);
    koma->m_Table.i.rfeat = f_vj_i;
    koma->m_Table.i.rtags[0] = p_vj[0];
    koma->m_Table.i.rtags[1] = p_vj[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    return true;
}

/* irregularity: h: yai: eo 
    NG,YAI,(FIL|SS) = NG,YEO,H + NG,EO,(FIL|SS) */
bool rule_inf_ending_h_yai_eo(CKoma *koma)
{
    /* condition */
    if (koma->m_Table.i.rlen<2) return true;
    if (!(koma->m_cp.i == PHI_NG && koma->m_cp.m == PHM_YAI && (koma->m_cp.f == PHF_FIL || koma->m_cp.f == PHF_SS))) 
	return true;
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_EO, koma->m_cp.f);
    if (!lookup(koma)) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    if (!(koma->m_Table.i.ffeat & f_e_ir)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    koma->m_Table.i.mfeat = f_inf;
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_YEO, PHF_H);
    koma->m_Table.i.rfeat = f_vj_i;
    koma->m_Table.i.rtags[0] = p_vj[0];
    koma->m_Table.i.rtags[1] = p_vj[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    return true;
}

/* irregularity: reu: a
    (A|O),R,R,A,(FIL|SS) = (A|O),FIL,R,EU,FIL + NG,A,(FIL|SS) */
bool rule_inf_ending_reu_a(CKoma *koma)
{
    /* condition */
    if (koma->m_Table.i.rlen<2) return true;
    if (!(koma->m_cp.i == PHI_R && koma->m_cp.m == PHM_A && (koma->m_cp.f == PHF_FIL || koma->m_cp.f == PHF_SS))) return true; 
    if (!(koma->m_pp.f == PHF_R && (koma->m_pp.m == PHM_A || koma->m_pp.m == PHM_O))) return true;
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_A, koma->m_cp.f);
    if (!lookup(koma)) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    if (!(koma->m_Table.i.ffeat & f_e_ir)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    koma->m_Table.i.mfeat = f_inf;
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen-2, koma->m_pp.i, koma->m_pp.m, PHF_FIL);
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_EU, PHF_FIL);
    koma->m_Table.i.rfeat = f_vv_i | f_vj_i;
    koma->m_Table.i.rtags[0] = p_vv[0] | p_vj[0];
    koma->m_Table.i.rtags[1] = p_vv[1] | p_vj[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    return true;
}

/* irregularity: reu: eo
    ~(A|O),R,R,EO,(FIL|SS) = ~(A|O),FIL,R,EU,FIL + NG,EO,(FIL|SS) */
bool rule_inf_ending_reu_eo(CKoma *koma)
{
    /* condition */
    if (koma->m_Table.i.rlen<2) return true;
    if (!(koma->m_cp.i == PHI_R && koma->m_cp.m == PHM_EO && (koma->m_cp.f == PHF_FIL || koma->m_cp.f == PHF_SS))) return true; 
    if (!(koma->m_pp.f == PHF_R)) return true;
    /*    if ( koma->m_pp.m == PHM_A || koma->m_pp.m == PHM_O ) return true;*/
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_EO, koma->m_cp.f);
    if (!lookup(koma)) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    if (!(koma->m_Table.i.ffeat & f_e_ir)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    koma->m_Table.i.mfeat = f_inf;
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen-2, koma->m_pp.i, koma->m_pp.m, PHF_FIL);
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_EU, PHF_FIL);
    koma->m_Table.i.rfeat = f_vv_i | f_vj_i;
    koma->m_Table.i.rtags[0] = p_vv[0] | p_vj[0];
    koma->m_Table.i.rtags[1] = p_vv[1] | p_vj[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    return true;
}

/* irregularity: u
    P,EO,(FIL|SS) = P,U,FIL + NG,EO,(FIL|SS) */
bool rule_inf_ending_u(CKoma *koma)
{
    /* condition */
    if (!(koma->m_cp.i == PHI_P && koma->m_cp.m == PHM_EO && (koma->m_cp.f == PHF_FIL || koma->m_cp.f == PHF_SS))) return true; 
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_EO, koma->m_cp.f);
    if (!lookup(koma)) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    if (!(koma->m_Table.i.ffeat & f_e_ir)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    koma->m_Table.i.mfeat = f_inf;
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_U, PHF_FIL);
    koma->m_Table.i.rfeat = f_vv_i | f_vj_i;
    koma->m_Table.i.rtags[0] = p_vv[0] | p_vj[0];
    koma->m_Table.i.rtags[1] = p_vv[1] | p_vj[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    return true;
}

/* irregularity: neora
    NG,O,FIL,N,EO,FIL,R,A,FIL = NG,O,FIL + NG,A,FIL,R,A,FIL */
bool rule_inf_ending_neora(CKoma *koma)
{
    /* condition */
    if (koma->m_Table.i.rlen < 2 || koma->m_Table.i.flen != 4) return true;
    if (!(koma->m_cp.i == PHI_N && koma->m_cp.m == PHM_EO && koma->m_cp.f == PHF_FIL)) return true; 
    if (!(koma->m_np.i == PHI_R && koma->m_np.m == PHM_A && koma->m_np.f == PHF_FIL)) return true;
    if (!(koma->m_pp.i == PHI_NG && koma->m_pp.m == PHM_O && koma->m_pp.f == PHF_FIL)) return true;
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_A, koma->m_cp.f);
    if (!lookup(koma)) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    if (!(koma->m_Table.i.ffeat & f_e_ir)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    koma->m_Table.i.mfeat = f_inf;
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    koma->m_Table.i.rfeat = f_vv_r | f_vj_r;
    koma->m_Table.i.rtags[0] = p_vv[0] | p_vj[0];
    koma->m_Table.i.rtags[1] = p_vv[1] | p_vj[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    return true;
}

/* irregularity: n
    NG,O,N = NG,O,FIL + NG,A,FIL,R,A,FIL */
bool rule_inf_ending_n(CKoma *koma)
{
    /* condition */
    if (!(koma->m_cp.i == PHI_NG && koma->m_cp.m == PHM_O && koma->m_cp.f == PHF_N)) return true; 
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_A, PHF_FIL);
    CopySyllable (koma->m_Table.i.form+2, PHI_R, PHM_A, PHF_FIL);
    if (!lookup(koma)) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    if (!(koma->m_Table.i.ffeat & f_e_ir)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    koma->m_Table.i.mfeat = f_inf;
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, koma->m_cp.m, PHF_FIL);
    koma->m_Table.i.rfeat = f_vv_r | f_vj_r;
    koma->m_Table.i.rtags[0] = p_vv[0] | p_vj[0];
    koma->m_Table.i.rtags[1] = p_vv[1] | p_vj[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    return true;
}

/* irregularity: ha: ai
    H,AI,(FIL|SS) = H,A,FIL + NG,A,(FIL|SS) */
bool rule_inf_ending_ha_ai(CKoma *koma)
{
    /* condition */
    if (!(koma->m_cp.i == PHI_H && koma->m_cp.m == PHM_AI && (koma->m_cp.f == PHF_FIL || koma->m_cp.f == PHF_SS))) 
	return true;
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_A, koma->m_cp.f);
    if (!lookup(koma)) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    if (!(koma->m_Table.i.ffeat & f_e_ir)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    koma->m_Table.i.mfeat = f_inf;
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_A, PHF_FIL);
    koma->m_Table.i.rfeat = f_vv_i | f_vj_i;
    koma->m_Table.i.rtags[0] = p_vv[0] | p_vj[0];
    koma->m_Table.i.rtags[1] = p_vv[1] | p_vj[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    return true;
}

/* irregularity: ha: yeo
    H,A,FIL,NG,YEO,(FIL|SS) = H,A,FIL + NG,A,(FIL|SS) */
bool rule_inf_ending_ha_yeo(CKoma *koma)
{
    /* condition */
    if (koma->m_Table.i.rlen<2) return true;
    if (!(koma->m_cp.i == PHI_NG && koma->m_cp.m == PHM_YEO && (koma->m_cp.f == PHF_FIL || koma->m_cp.f == PHF_SS))) 
	return true;
    if (!(koma->m_pp.i == PHI_H && koma->m_pp.m == PHM_A && koma->m_pp.f == PHF_FIL)) return true;
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_A, koma->m_cp.f);
    if (!lookup(koma)) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    if (!(koma->m_Table.i.ffeat & f_e_ir)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    koma->m_Table.i.mfeat = f_inf;
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    koma->m_Table.i.rfeat = f_vv_i | f_vj_i;
    koma->m_Table.i.rtags[0] = p_vv[0] | p_vj[0];
    koma->m_Table.i.rtags[1] = p_vv[1] | p_vj[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    return true;
}

/* irregularity: ha: jjai 
    NG,EO,FIL,JJ,YAI,(FIL|SS) = NG,EO,FIL,JJ,I,FIL,H,A,FIL + NG,A,(FIL|SS) */
bool rule_inf_ending_ha_jjai(CKoma *koma)
{
    /* condition */
    if (koma->m_Table.i.rlen != 2) return true;
    if (!(koma->m_cp.i == PHI_JJ && koma->m_cp.m == PHM_AI && (koma->m_cp.f == PHF_FIL || koma->m_cp.f == PHF_SS))) 
	return true;
    if (!(koma->m_pp.i == PHI_NG && koma->m_pp.m == PHM_EO && koma->m_pp.f == PHF_FIL)) return true;
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_A, koma->m_cp.f);
    if (!lookup(koma)) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    if (!(koma->m_Table.i.ffeat & f_e_ir)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    koma->m_Table.i.mfeat = f_inf;
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_I, PHF_FIL);
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen+2, PHI_H, PHM_A, PHF_FIL);
    koma->m_Table.i.rfeat = f_vv_i | f_vj_i;
    koma->m_Table.i.rtags[0] = p_vv[0] | p_vj[0];
    koma->m_Table.i.rtags[1] = p_vv[1] | p_vj[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    return true;
}

/* irregularity: ha: k,t,ch
    K = H,A,FIL + G 
    T = H,A,FIL + D 
    CH = H,A,FIL + J */
bool rule_inf_ending_ha_ktch(CKoma *koma)
{
    /* condition */
    if (koma->m_Table.i.rlen<2) return true;
    if (!(koma->m_cp.i == PHI_K || koma->m_cp.i == PHI_T || koma->m_cp.i == PHI_CH)) return true;
    /* right morpheme */
    split(koma);
    if (koma->m_cp.i == PHI_K)
	CopySyllable (koma->m_Table.i.form, PHI_G, koma->m_cp.m, koma->m_cp.f);
    else if (koma->m_cp.i == PHI_T)
	CopySyllable (koma->m_Table.i.form, PHI_D, koma->m_cp.m, koma->m_cp.f);
    else
	CopySyllable (koma->m_Table.i.form, PHI_J, koma->m_cp.m, koma->m_cp.f);
    if (!lookup(koma)) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    if (!(koma->m_Table.i.ffeat & f_e_hc)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    koma->m_Table.i.mfeat = f_inf;
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, PHI_H, PHM_A, PHF_FIL);
    koma->m_Table.i.rfeat = f_vv_i | f_vj_i;
    koma->m_Table.i.rtags[0] = p_vv[0] | p_vj[0];
    koma->m_Table.i.rtags[1] = p_vv[1] | p_vj[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    return true;
}

/* irregularity: reo
    R,EU,FIL,R,EO,(FIL|SS) = R,EU,FIL + NG,EO,(FIL|SS) */
bool rule_inf_ending_reo(CKoma *koma)
{
    /* condition */
    if (koma->m_Table.i.rlen<4) return true;
    if (!(koma->m_cp.i == PHI_R && koma->m_cp.m == PHM_EO && (koma->m_cp.f == PHF_FIL || koma->m_cp.f == PHF_SS))) return true; 
    if (!(koma->m_pp.i == PHI_R && koma->m_pp.m == PHM_EU && koma->m_pp.f == PHF_FIL)) return true;
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, koma->m_cp.m, koma->m_cp.f);
    if (!lookup(koma)) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    if (!(koma->m_Table.i.ffeat & f_e_ir)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    koma->m_Table.i.mfeat = f_inf;
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    koma->m_Table.i.rfeat = f_vv_l | f_vj_l;
    koma->m_Table.i.rtags[0] = p_vv[0] | p_vj[0];
    koma->m_Table.i.rtags[1] = p_vv[1] | p_vj[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    return true;
}

/* irregularity: rai
    FIL,R,AI,(FIL|SS) = FIL,R,EO,FIL + NG,EO,(FIL|SS) */
bool rule_inf_ending_rai(CKoma *koma)
{
    /* condition */
    if (koma->m_Table.i.rlen!=2) return true;
    if (!(koma->m_cp.i == PHI_R && koma->m_cp.m == PHM_AI && (koma->m_cp.f == PHF_FIL || koma->m_cp.f == PHF_SS))) return true; 
    if (!(koma->m_pp.f == PHF_FIL)) return true;
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_EO, koma->m_cp.f);
    if (!lookup(koma)) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    if (!(koma->m_Table.i.ffeat & f_e_ir)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    koma->m_Table.i.mfeat = f_inf;
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_EO, PHF_FIL);
    koma->m_Table.i.rfeat = f_vv_r;
    koma->m_Table.i.rtags[0] = p_vv[0];
    koma->m_Table.i.rtags[1] = p_vv[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    return true;
}

/* irregularity: a: eu
    (A|O),?,(G|N|R|BB|NG|P),A,(FIL|SS) 
    = (A|O),?,(G|N|R|BB|NG|P),EU,FIL + NG,A,(FIL|SS) */
bool rule_inf_ending_a_eu(CKoma *koma)
{
    /* condition */
    if (koma->m_Table.i.rlen < 2) return true;
    if (!(koma->m_cp.m == PHM_A && (koma->m_cp.f == PHF_FIL || koma->m_cp.f == PHF_SS))) return true; 
    if (!(koma->m_cp.i == PHI_G || koma->m_cp.i == PHI_N || koma->m_cp.i == PHI_R || koma->m_cp.i == PHI_BB || 
	koma->m_cp.i == PHI_NG || koma->m_cp.i == PHI_P)) return true;
    if (!(koma->m_pp.m == PHM_A || koma->m_pp.m == PHM_O)) return true;
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_A, koma->m_cp.f);
    if (!lookup(koma)) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    if (!(koma->m_Table.i.ffeat & f_e_ir)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    koma->m_Table.i.mfeat = f_inf;
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_EU, PHF_FIL);
    koma->m_Table.i.rfeat = f_vv_r | f_vj_r;
    koma->m_Table.i.rtags[0] = p_vv[0] | p_vj[0];
    koma->m_Table.i.rtags[1] = p_vv[1] | p_vj[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    return true;
}

/* irregularity: a: a
    ~(D|B|BB|NG|K|H),A,(FIL|SS) = ~(D|B|BB|NG|K|H),A,FIL + NG,A,(FIL|SS) */
bool rule_inf_ending_a_a(CKoma *koma)
{
    /* condition */
    if (!(koma->m_cp.m == PHM_A && (koma->m_cp.f == PHF_FIL || koma->m_cp.f == PHF_SS))) return true; 
    if ((koma->m_cp.i == PHI_D || koma->m_cp.i == PHI_B || koma->m_cp.i == PHI_BB || koma->m_cp.i == PHI_NG || 
	koma->m_cp.i == PHI_K || koma->m_cp.i == PHI_H)) return true;
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_A, koma->m_cp.f);
    if (!lookup(koma)) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    if (!(koma->m_Table.i.ffeat & f_e_ir)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    koma->m_Table.i.mfeat = f_inf;
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_A, PHF_FIL);
    koma->m_Table.i.rfeat = f_vv_r | f_vj_r;
    koma->m_Table.i.rtags[0] = p_vv[0] | p_vj[0];
    koma->m_Table.i.rtags[1] = p_vv[1] | p_vj[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    return true;
}

/* irregularity: a: r
    M,A,FIL = M,A,R + NG,A,FIL
    M,A,FIL[,R,A,FIL] = M,A,R + NG,A,FIL[,R,A,FIL] */
bool rule_inf_ending_a_r(CKoma *koma)
{
    /* condition */
    if (koma->m_Table.i.flen>4) return true;
    if (!(koma->m_cp.i == PHI_M && koma->m_cp.m == PHM_A && koma->m_cp.f == PHF_FIL)) return true; 
    if (koma->m_Table.i.flen==4 &&
	!(koma->m_np.i == PHI_R && koma->m_np.m == PHM_A && koma->m_np.f == PHF_FIL)) return true;
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_A, koma->m_cp.f);
    if (!lookup(koma)) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    if (!(koma->m_Table.i.ffeat & f_e_ir)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    koma->m_Table.i.mfeat = f_inf;
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_A, PHF_R);
    koma->m_Table.i.rfeat = f_vv_r;
    koma->m_Table.i.rtags[0] = p_vvx[0];
    koma->m_Table.i.rtags[1] = p_vvx[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    return true;
}

/* irregularity: a: o
    (G|GG|B|SS|NG|J|JJ|H),OA,(FIL|SS) = (G|GG|B|SS|NG|J|JJ|H),O,FIL + NG,A,(FIL|SS) */
bool rule_inf_ending_a_o(CKoma *koma)
{
    /* condition */
    if (!(koma->m_cp.m == PHM_OA && (koma->m_cp.f == PHF_FIL || koma->m_cp.f == PHF_SS))) return true; 
    if (!((koma->m_cp.i == PHI_G || koma->m_cp.i == PHI_GG || koma->m_cp.i == PHI_B || koma->m_cp.i == PHI_SS || 
	koma->m_cp.i == PHI_NG || koma->m_cp.i == PHI_J || koma->m_cp.i == PHI_JJ || koma->m_cp.i == PHI_H))) return true;
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_A, koma->m_cp.f);
    if (!lookup(koma)) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    if (!(koma->m_Table.i.ffeat & f_e_ir)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    koma->m_Table.i.mfeat = f_inf;
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_O, PHF_FIL);
    koma->m_Table.i.rfeat = f_vv_r | f_vj_r;
    koma->m_Table.i.rtags[0] = p_vv[0] | p_vj[0];
    koma->m_Table.i.rtags[1] = p_vv[1] | p_vj[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    return true;
}

/* irregularity: a: oh
    N,OA,(FIL|SS) = N,O,H + NG,A,(FIL|SS) */
bool rule_inf_ending_a_oh(CKoma *koma)
{
    /* condition */
    if (!(koma->m_cp.m == PHM_OA && (koma->m_cp.f == PHF_FIL || koma->m_cp.f == PHF_SS))) return true; 
    if (!(koma->m_cp.i == PHI_N)) return true;
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_A, koma->m_cp.f);
    if (!lookup(koma)) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    if (!(koma->m_Table.i.ffeat & f_e_ir)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    koma->m_Table.i.mfeat = f_inf;
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_O, PHF_H);
    koma->m_Table.i.rfeat = f_vv_r | f_vj_r;
    koma->m_Table.i.rtags[0] = p_vv[0] | p_vj[0];
    koma->m_Table.i.rtags[1] = p_vv[1] | p_vj[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    return true;
}

/* irregularity: a: ah
    (N|DD),A,(FIL|SS) = (N|DD),A,H + NG,A,(FIL|SS) */
bool rule_inf_ending_a_ah(CKoma *koma)
{
    /* condition */
    if (!(koma->m_cp.m == PHM_A && (koma->m_cp.f == PHF_FIL || koma->m_cp.f == PHF_SS))) return true; 
    if (!(koma->m_cp.i == PHI_N || koma->m_cp.i == PHI_DD)) return true;
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_A, koma->m_cp.f);
    if (!lookup(koma)) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    if (!(koma->m_Table.i.ffeat & f_e_ir)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    koma->m_Table.i.mfeat = f_inf;
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_A, PHF_H);
    koma->m_Table.i.rfeat = f_vv_r | f_vj_r;
    koma->m_Table.i.rtags[0] = p_vv[0] | p_vj[0];
    koma->m_Table.i.rtags[1] = p_vv[1] | p_vj[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    return true;
}

/* irregularity: eo: eu
    (G|GG|N|DD|R|BB|SS|K|T|P),EO,(FIL|SS) 
    = (G|GG|N|DD|R|BB|SS|K|T|P),EU,FIL + NG,EO,(FIL|SS) */
bool rule_inf_ending_eo_eu(CKoma *koma)
{
    /* condition */
    if (!(koma->m_cp.m == PHM_EO && (koma->m_cp.f == PHF_FIL || koma->m_cp.f == PHF_SS))) return true; 
    if (!(koma->m_cp.i == PHI_G || koma->m_cp.i == PHI_GG || koma->m_cp.i == PHI_N || koma->m_cp.i == PHI_DD || 
	koma->m_cp.i == PHI_R || koma->m_cp.i == PHI_BB || koma->m_cp.i == PHI_SS || koma->m_cp.i == PHI_K || 
	koma->m_cp.i == PHI_T || koma->m_cp.i == PHI_P)) return true;
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_EO, koma->m_cp.f);
    if (!lookup(koma)) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    if (!(koma->m_Table.i.ffeat & f_e_ir)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    koma->m_Table.i.mfeat = f_inf;
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_EU, PHF_FIL);
    koma->m_Table.i.rfeat = f_vv_r | f_vj_r;
    koma->m_Table.i.rtags[0] = p_vv[0] | p_vj[0];
    koma->m_Table.i.rtags[1] = p_vv[1] | p_vj[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    return true;
}

/* irregularity: eo: s
    G,EO,(FIL|SS) = G,EU,S + NG,EO,(FIL|SS) */
bool rule_inf_ending_eo_s(CKoma *koma)
{
    /* condition */
    if (!(koma->m_cp.i == PHI_G && koma->m_cp.m == PHM_EO && (koma->m_cp.f == PHF_FIL || koma->m_cp.f == PHF_SS))) return true; 
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_EO, koma->m_cp.f);
    if (!lookup(koma)) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    if (!(koma->m_Table.i.ffeat & f_e_ir)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    koma->m_Table.i.mfeat = f_inf;
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_EU, PHF_S);
    koma->m_Table.i.rfeat = f_vv_i;
    koma->m_Table.i.rtags[0] = p_vv[0];
    koma->m_Table.i.rtags[1] = p_vv[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    return true;
}

/* irregularity: eo: eo
    (N|S|SS),EO,(FIL|SS) = (N|S|SS),EO,FIL + NG,EO,(FIL|SS) */
bool rule_inf_ending_eo_eo(CKoma *koma)
{
    /* condition */
    if (!(koma->m_cp.m == PHM_EO && (koma->m_cp.f == PHF_FIL || koma->m_cp.f == PHF_SS))) return true; 
    if (!(koma->m_cp.i == PHI_N || koma->m_cp.i == PHI_S || koma->m_cp.i == PHI_SS)) return true;
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_EO, koma->m_cp.f);
    if (!lookup(koma)) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    if (!(koma->m_Table.i.ffeat & f_e_ir)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    koma->m_Table.i.mfeat = f_inf;
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_EO, PHF_FIL);
    koma->m_Table.i.rfeat = f_vv_r | f_vj_r;
    koma->m_Table.i.rtags[0] = p_vv[0] | p_vj[0];
    koma->m_Table.i.rtags[1] = p_vv[1] | p_vj[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    return true;
}

/* irregularity: eo: u
    ~(DD|M|B|BB|P|H),UEO,(FIL|SS) = ~(DD|M|B|BB|P|H),U,FIL + NG,EO,(FIL|SS) */
bool rule_inf_ending_eo_u(CKoma *koma)
{
    /* condition */
    if (!(koma->m_cp.m == PHM_UEO && (koma->m_cp.f == PHF_FIL || koma->m_cp.f == PHF_SS))) return true;
    if ((koma->m_cp.i == PHI_DD || koma->m_cp.i == PHI_M || koma->m_cp.i == PHI_B || koma->m_cp.i == PHI_BB || 
	koma->m_cp.i == PHI_P || koma->m_cp.i == PHI_H)) return true;
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_EO, koma->m_cp.f);
    if (!lookup(koma)) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    if (!(koma->m_Table.i.ffeat & f_e_ir)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    koma->m_Table.i.mfeat = f_inf;
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_U, PHF_FIL);
    koma->m_Table.i.rfeat = f_vv_r | f_vj_r;
    koma->m_Table.i.rtags[0] = p_vv[0] | p_vj[0];
    koma->m_Table.i.rtags[1] = p_vv[1] | p_vj[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    return true;
}

/* irregularity: eo: ai
    ~H,AI,(FIL|SS) = ~H,AI,FIL + NG,EO,(FIL|SS) */
bool rule_inf_ending_eo_ai(CKoma *koma)
{
    /* condition */
    if (!(koma->m_cp.m == PHM_AI && (koma->m_cp.f == PHF_FIL || koma->m_cp.f == PHF_SS))) return true;
    if (koma->m_cp.i == PHI_H) return true;
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_EO, koma->m_cp.f);
    if (!lookup(koma)) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    if (!(koma->m_Table.i.ffeat & f_e_ir)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    koma->m_Table.i.mfeat = f_inf;
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_AI, PHF_FIL);
    koma->m_Table.i.rfeat = f_vv_r | f_vj_r;
    koma->m_Table.i.rtags[0] = p_vv[0] | p_vj[0];
    koma->m_Table.i.rtags[1] = p_vv[1] | p_vj[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    return true;
}

/* irregularity: eo: oai
    ~(M|BB|CH|K|T|P|H),OAI,(FIL|SS) = ~(M|BB|CH|K|T|P|H),OI,FIL + NG,EO,(FIL|SS) */
bool rule_inf_ending_eo_oai(CKoma *koma)
{
    /* condition */
    if (!(koma->m_cp.m == PHM_OAI && (koma->m_cp.f == PHF_FIL || koma->m_cp.f == PHF_SS))) return true;
    if ((koma->m_cp.i == PHI_M || koma->m_cp.i == PHI_BB || koma->m_cp.i == PHI_CH || koma->m_cp.i == PHI_K || 
	koma->m_cp.i == PHI_T || koma->m_cp.i == PHI_P || koma->m_cp.i == PHI_H)) return true;
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_EO, koma->m_cp.f);
    if (!lookup(koma)) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    if (!(koma->m_Table.i.ffeat & f_e_ir)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    koma->m_Table.i.mfeat = f_inf;
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_OI, PHF_FIL);
    koma->m_Table.i.rfeat = f_vv_r | f_vj_r;
    koma->m_Table.i.rtags[0] = p_vv[0] | p_vj[0];
    koma->m_Table.i.rtags[1] = p_vv[1] | p_vj[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    return true;
}

/* irregularity: eo: eoi
    (G|N|D|DD|R|M|B|S),EOI,(FIL|SS) = (G|N|D|DD|R|M|B|S),EOI,FIL + NG,EO,(FIL|SS)
   irregularity: eo: seoiyo
    S,EOI,FIL,NG,YO,FIL = S,I,FIL + NG,EO,FIL,NG,YO,FIL */
bool rule_inf_ending_eo_eoi(CKoma *koma)
{
    /* condition */
    if (!(koma->m_cp.m == PHM_EOI && (koma->m_cp.f == PHF_FIL || koma->m_cp.f == PHF_SS))) return true;
    if (!(koma->m_cp.i == PHI_G || koma->m_cp.i == PHI_N || koma->m_cp.i == PHI_D || koma->m_cp.i == PHI_DD || 
	koma->m_cp.i == PHI_R || koma->m_cp.i == PHI_M || koma->m_cp.i == PHI_B || koma->m_cp.i == PHI_S)) return true;
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_EO, koma->m_cp.f);
    if (!lookup(koma)) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    if (!(koma->m_Table.i.ffeat & f_e_ir)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    koma->m_Table.i.mfeat = f_inf;
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_EOI, PHF_FIL);
    koma->m_Table.i.rfeat = f_vv_r | f_vj_r;
    koma->m_Table.i.rtags[0] = p_vv[0] | p_vj[0];
    koma->m_Table.i.rtags[1] = p_vv[1] | p_vj[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    /* irregularity: eo: seoiyo
       S,EOI,FIL,NG,YO,FIL = S,I,FIL + NG,EO,FIL,NG,YO,FIL */
    if (koma->m_Table.i.flen != 4) return true;
    if (!(koma->m_cp.i == PHI_S && koma->m_cp.f == PHF_FIL)) return true;
    if (!(koma->m_np.i == PHI_NG && koma->m_np.m == PHM_YO && koma->m_np.f == PHI_FIL)) return true;
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_I, PHF_FIL);
    koma->m_Table.i.rfeat = f_vv_r | f_vj_r;
    koma->m_Table.i.rtags[0] = p_vv[0] | p_vj[0] | p_ep[0];
    koma->m_Table.i.rtags[1] = p_vv[1] | p_vj[1] | p_ep[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    return true;
}

/* irregularity: eo: yeo
    (K|P),YEO,(FIL|SS) = (K|P),YEO,FIL + NG,EO,(FIL|SS) */
bool rule_inf_ending_eo_yeo(CKoma *koma)
{
    /* condition */
    if (!(koma->m_cp.m == PHM_YEO && (koma->m_cp.f == PHF_FIL || koma->m_cp.f == PHF_SS))) return true;
    if (!(koma->m_cp.i == PHI_K || koma->m_cp.i == PHI_P)) return true;
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_EO, koma->m_cp.f);
    if (!lookup(koma)) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    if (!(koma->m_Table.i.ffeat & f_e_ir)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    koma->m_Table.i.mfeat = f_inf;
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_YEO, PHF_FIL);
    koma->m_Table.i.rfeat = f_vv_r | f_vj_r;
    koma->m_Table.i.rtags[0] = p_vv[0] | p_vj[0];
    koma->m_Table.i.rtags[1] = p_vv[1] | p_vj[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    return true;
}

/* irregularity: eo: i
    ~(SS),YEO,(FIL|SS) = ~(SS),I,FIL + NG,EO,(FIL|SS) */
bool rule_inf_ending_eo_i(CKoma *koma)
{
    /* condition */
    if (!(koma->m_cp.m == PHM_YEO && (koma->m_cp.f == PHF_FIL || koma->m_cp.f == PHF_SS))) return true;
    if (koma->m_cp.i == PHI_SS) return true;
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_EO, koma->m_cp.f);
    if (!lookup(koma)) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    if (!(koma->m_Table.i.ffeat & f_e_ir)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    koma->m_Table.i.mfeat = f_inf;
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_I, PHF_FIL);
    koma->m_Table.i.rfeat = f_vv_r | f_vj_r;
    if (koma->m_Table.i.pred != S_E) 
    {
	koma->m_Table.i.rtags[0] = p_vv[0] | p_vj[0] | p_ep[0];
	koma->m_Table.i.rtags[1] = p_vv[1] | p_vj[1] | p_ep[1];
    } 
    else 
    {
	koma->m_Table.i.rtags[0] = p_vv[0] | p_vj[0];
	koma->m_Table.i.rtags[1] = p_vv[1] | p_vj[1];
    }
    if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    /* left morpheme: i-contraction */
    if (!(koma->m_Table.i.ffeat & f_e_l_i)) return true;
    /* gyeo-contraction:
       G,YEO,FIL = G,EO,S + NG,I,FIL + NG,EO,FIL */
    if (koma->m_cp.i == PHI_G && koma->m_cp.f == PHF_FIL && koma->m_Table.i.rlen == 0) 
    {
        koma->m_Table.i.morph[0] = (char)0;
	CopySyllable (koma->m_Table.i.morph, PHI_NG, PHM_I, PHF_FIL);
	koma->m_Table.i.mfeat = f_inf;
	koma->m_Table.i.mtags[0] = p_i[0];
	koma->m_Table.i.mtags[1] = p_i[1];
	if (!koma->PushMorph(koma->m_Table.i.morph, NEXT)) return false;
	CopySyllable (koma->m_Table.i.real, koma->m_cp.i, PHM_EO, PHF_S);
	koma->m_Table.i.rfeat = f_n_ir;
	koma->m_Table.i.rtags[0] = p_np[0] | p_nnb[0];
	koma->m_Table.i.rtags[1] = p_np[1] | p_nnb[1];
	if (!koma->PushHead(koma->m_Table.i.real, S_I, NEXT)) return false;
	return true;
    }
    if (! (koma->m_cp.i == PHI_NG && ( koma->m_Table.i.rlen == 0 || koma->m_pp.f == PHF_FIL ) ) ) return true;
    koma->m_Table.i.morph[0] = (char)0;
    CopySyllable (koma->m_Table.i.morph, PHI_NG, PHM_I, PHF_FIL);
    koma->m_Table.i.mfeat = f_inf;
    koma->m_Table.i.mtags[0] = p_i[0];
    koma->m_Table.i.mtags[1] = p_i[1];
    if (!koma->PushMorph(koma->m_Table.i.morph, NEXT)) return false;
    if (koma->m_Table.i.rlen == 0) 
	return koma->PushKnown(NEXT);
    koma->m_Table.i.real[koma->m_Table.i.rlen] = (char)0;
    koma->m_Table.i.rfeat = f_e_r_p | f_p_r_i;
    koma->m_Table.i.rtags[0] = p_n[0] | p_ef[0] | p_a[0] | p_p[0];
    koma->m_Table.i.rtags[1] = p_n[1] | p_ef[1] | p_a[1] | p_p[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_I, NEXT)) return false;
    /* s-omission:
	(G|NG),EO,FIL,? = (G|NG),EO,S(n_ir) + NG,I,FIL + ?
	M,UEO,FIL,? = M,UEO,S(n_ir) + NG,I,FIL + ? */
    if ((koma->m_pp.i == PHI_M && koma->m_pp.m == PHM_UEO) || 
	((koma->m_pp.i == PHI_G || koma->m_pp.i == PHI_NG) && koma->m_pp.m == PHM_EO)) 
    {
	CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen-2, koma->m_pp.i, koma->m_pp.m, PHF_S);
	koma->m_Table.i.rfeat = f_n_ir;
	koma->m_Table.i.rtags[0] = p_np[0] | p_nnb[0];
	koma->m_Table.i.rtags[1] = p_np[1] | p_nnb[1];
	if (!koma->PushHead(koma->m_Table.i.real, S_I, NEXT)) return false;
    }
    return true;
}

/* irregularity: i: eoi
	YEOI = I,FIL + NG,EOI
    [NG,A,FIL,]N,_,FIL,NG,YO,FIL = [NG,A,FIL,]N,_,_ + _,_,FIL,NG,YO,FIL
    NG,_,FIL,NG,YO,FIL = NG,_,_ + _,_,FIL,NG,YO,FIL */
bool rule_inf_ending_i_eoi(CKoma *koma)
{
    /* condition */
    if (koma->m_Table.i.flen != 4) return true;
    if (!((koma->m_cp.i == PHI_NG || koma->m_cp.i == PHI_N) && koma->m_cp.m == PHM_YEOI && koma->m_cp.f == PHF_FIL)) 
	return true; 
    if (!(koma->m_np.i == PHI_NG && koma->m_np.m == PHM_YO && koma->m_np.f == PHF_FIL)) return true;
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_EOI, PHF_FIL);
    if (!lookup(koma)) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    koma->m_Table.i.mfeat = f_inf;
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    if (koma->m_cp.i == PHI_N && koma->m_pp.i == PHI_NG && koma->m_pp.m == PHM_A && 
	koma->m_pp.f == PHF_FIL) 
    {
	CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_I, PHF_FIL);
	koma->m_Table.i.rfeat = f_vj_r;
	koma->m_Table.i.rtags[0] = p_vj[0];
	koma->m_Table.i.rtags[1] = p_vj[1];
	if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    }
    /* left morpheme: i-contraction */
    if (!(koma->m_cp.i == PHI_NG && koma->m_pp.f == PHI_FIL)) return true;
    koma->m_Table.i.morph[0] = (char)0;
    CopySyllable (koma->m_Table.i.morph, PHI_NG, PHM_I, PHF_FIL);
    koma->m_Table.i.mfeat = f_inf;
    koma->m_Table.i.mtags[0] = p_i[0];
    koma->m_Table.i.mtags[1] = p_i[1];
    if (!koma->PushMorph(koma->m_Table.i.morph, NEXT)) return false;
    if (koma->m_Table.i.rlen == 0) 
	return koma->PushKnown(NEXT);
    koma->m_Table.i.real[koma->m_Table.i.rlen] = (char)0;
    koma->m_Table.i.rfeat = f_e_r_p | f_p_r_i;
    koma->m_Table.i.rtags[0] = p_n[0] | p_ef[0] | p_a[0] | p_p[0];
    koma->m_Table.i.rtags[1] = p_n[1] | p_ef[1] | p_a[1] | p_p[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_I, NEXT)) return false;
    /* s-omission:
	(G|NG),EO,FIL,? = (G|NG),EO,S(n_ir) + NG,I,FIL + ?
	M,UEO,FIL,? = M,UEO,S(n_ir) + NG,I,FIL + ? */
    if ((koma->m_pp.i == PHI_M && koma->m_pp.m == PHM_UEO) || 
	((koma->m_pp.i == PHI_G || koma->m_pp.i == PHI_NG) && koma->m_pp.m == PHM_EO)) 
    {
	CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen-2, koma->m_pp.i, koma->m_pp.m, PHF_S);
	koma->m_Table.i.rfeat = f_n_ir;
	koma->m_Table.i.rtags[0] = p_np[0] | p_nnb[0];
	koma->m_Table.i.rtags[1] = p_np[1] | p_nnb[1];
	if (!koma->PushHead(koma->m_Table.i.real, S_I, NEXT)) return false;
    }
    return true;
}

/* irregularity: i: ya
	YA = I,FIL + NG,YA
    [NG,A,FIL,]N,_,FIL = [NG,A,FIL,]N,_,_ + _,_,FIL */
bool rule_inf_ending_i_ya(CKoma *koma)
{
    /* condition */
    if (koma->m_Table.i.flen != 2) return true;
    if (koma->m_Table.i.rlen != 2) return true;
    if (!(koma->m_cp.i == PHI_N && koma->m_cp.m == PHM_YA && koma->m_cp.f == PHF_FIL)) return true; 
    if (!(koma->m_pp.i == PHI_NG && koma->m_pp.m == PHM_A && koma->m_pp.f == PHF_FIL)) return true; 
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_YA, PHF_FIL);
    if (!lookup(koma)) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    koma->m_Table.i.mfeat = f_inf;
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_I, PHF_FIL);
    koma->m_Table.i.rfeat = f_vj_r;
    koma->m_Table.i.rtags[0] = p_vj[0];
    koma->m_Table.i.rtags[1] = p_vj[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    return true;
}

/* irregularity: i: yo
	YO = I,FIL + NG,O
    (N|NG),_,FIL = (N|NG),_,_ + _,_,FIL */
bool rule_inf_ending_i_yo(CKoma *koma)
{
    /* condition */
    if (koma->m_Table.i.flen != 2) return true;
    if (!((koma->m_cp.i == PHI_N || koma->m_cp.i == PHI_NG) && koma->m_cp.m == PHM_YO && koma->m_cp.f == PHF_FIL)) return true; 
    /* right morpheme */
    split(koma);
    CopySyllable (koma->m_Table.i.form, PHI_NG, PHM_O, PHF_FIL);
    if (!lookup(koma)) return true;
    if ((koma->m_Table.i.pfeat & f_e_r_p) && !(koma->m_Table.i.ffeat & f_e_r_p)) return true;
    koma->m_Table.i.mtags[0] = koma->m_Table.i.ftags[0] & p_e[0];
    koma->m_Table.i.mtags[1] = koma->m_Table.i.ftags[1] & p_e[1];
    koma->m_Table.i.mfeat = f_inf;
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    /* mask by prediction */
    koma->m_Table.i.mtags[0] &= koma->m_Table.i.ptags[0];
    koma->m_Table.i.mtags[1] &= koma->m_Table.i.ptags[1];
    if (!(koma->m_Table.i.mtags[0] || koma->m_Table.i.mtags[1])) return true;
    if (!koma->PushMorph(koma->m_Table.i.form, koma->m_Table.i.next)) return false;
    /* left morpheme */
    CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen, koma->m_cp.i, PHM_I, PHF_FIL);
    koma->m_Table.i.rfeat = f_vj_r;
    koma->m_Table.i.rtags[0] = p_vj[0];
    koma->m_Table.i.rtags[1] = p_vj[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_E, NEXT)) return false;
    /* left morpheme: i-contraction */
    if (!(koma->m_cp.i == PHI_NG && 
	  ( koma->m_Table.i.rlen == 0 || 
	    ( koma->m_Table.i.rlen > 0 && koma->m_pp.f == PHI_FIL ) ) )) return true;
    koma->m_Table.i.morph[0] = (char)0;
    CopySyllable (koma->m_Table.i.morph, PHI_NG, PHM_I, PHF_FIL);
    koma->m_Table.i.mfeat = f_inf;
    koma->m_Table.i.mtags[0] = p_i[0];
    koma->m_Table.i.mtags[1] = p_i[1];
    if (!koma->PushMorph(koma->m_Table.i.morph, NEXT)) return false;
    if (koma->m_Table.i.rlen == 0) 
	return koma->PushKnown(NEXT);
    koma->m_Table.i.real[koma->m_Table.i.rlen] = (char)0;
    koma->m_Table.i.rfeat = f_e_r_p | f_p_r_i;
    koma->m_Table.i.rtags[0] = p_n[0] | p_ef[0] | p_a[0] | p_p[0];
    koma->m_Table.i.rtags[1] = p_n[1] | p_ef[1] | p_a[1] | p_p[1];
    if (!koma->PushHead(koma->m_Table.i.real, S_I, NEXT)) return false;
    /* s-omission:
	(G|NG),EO,FIL,? = (G|NG),EO,S(n_ir) + NG,I,FIL + ?
	M,UEO,FIL,? = M,UEO,S(n_ir) + NG,I,FIL + ? */
    if ((koma->m_pp.i == PHI_M && koma->m_pp.m == PHM_UEO) || 
	((koma->m_pp.i == PHI_G || koma->m_pp.i == PHI_NG) && koma->m_pp.m == PHM_EO)) 
    {
	CopySyllable (koma->m_Table.i.real+koma->m_Table.i.rlen-2, koma->m_pp.i, koma->m_pp.m, PHF_S);
	koma->m_Table.i.rfeat = f_n_ir;
	koma->m_Table.i.rtags[0] = p_np[0] | p_nnb[0];
	koma->m_Table.i.rtags[1] = p_np[1] | p_nnb[1];
	if (!koma->PushHead(koma->m_Table.i.real, S_I, NEXT)) return false;
    }
    return true;
}
