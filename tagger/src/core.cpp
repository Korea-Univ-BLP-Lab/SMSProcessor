/* Core engine of korean morphological analyzer. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "postype.h"
#include "ph.h"
#include "predict.h"
#include "Koma.h"

/* Korean character area of Trigem-code(combination-type code) */
#ifndef isKOR
#define isKOR(x1,x2) ((x1 >= 0x84 && x1 <= 0xD3) && ((x2 >= 0x41 && x2 <= 0x7E) || (x2 >= 0x81 && x2 <= 0xFE)))
#endif
/* Chinese character area of Trigem-code(combination-type code) */
#ifndef isHJA
#define isHJA(x1,x2) ((x1 >= 0xE0 && x1 <= 0xF9) && ((x2 >= 0x31 && x2 <= 0x7E) || (x2 >= 0x91 && x2 <= 0xFE)))
#endif

#ifndef isJAP
#define isJAP(x1,x2) ((x1 == 0xDD && x2 >= 0xA1 && x2 <=0xF3 ) || (x1 == 0xDE && x2 >= 0x31 && x2 <=0x98 ))
#endif

#ifndef isDAlpha
#define isDAlpha(x1, x2) ((x1 == 0xDA) && ((x2 >= 0x51 && x2 <= 0x6A) || (x2 >= 0x71 && x2 <= 0x7E) || (x2 >= 0x91 && x2 <= 0x9C)))
#endif

#ifndef isDDigit
#define isDDigit(x1, x2) ((x1 == 0xDA) && (x2 >= 0x40 && x2 <= 0x49))
#endif

#ifndef isGDigit
#define isGDigit(x1, x2) ((x1 == 0xDB) && ((x2 >= 0x31 && x2 <= 0x3A) || (x2 >= 0x40 && x2 <= 0x49)))
#endif

#define SYMBOL_SF1 ". ! ?"
#define SYMBOL_SF2 "Ú> Ú1 ÚO Ù3 Ù6"
#define SYMBOL_SP1 ", : ; /"
#define SYMBOL_SP2 "Ù4 Ú< ÚJ ÚK Ú?"
#define SYMBOL_SL1 "` ( { ["
#define SYMBOL_SL2 "Ù> Ù@ ÙB ÙD ÙF ÙH ÙJ ÙL Úp Ú8 Úk Ú ÚL"
#define SYMBOL_SR1 "' ) } ] \""
#define SYMBOL_SR2 "Ù? ÙA ÙC ÙE ÙG ÙI ÙK ÙM ÙW ÙX Ù¥ Ù© Ú2 Ú7 Ú9 ÚN Úm ÚŸ"
#define SYMBOL_SD1 "- ~"
#define SYMBOL_SD2 "Ù9 Ù: Ù= Ú="
#define SYMBOL_SC1 "$"
#define SYMBOL_SC2 "Ú4 Úl Ù[ Ù\ Ù]"
#define SYMBOL_SU1 "l m g A W V %"
#define SYMBOL_SU2 "Ü1 Ü2 ml Ü3 dl Ü4 Ü5 kl Ü6 cc Ü7 Ü8 Ü9 Ü: Ü; fm Ü< nm Ü= Ü> mm Ü? cm Ü@ km \
ÜA ÜB ÜC ÜD ÜE ha ÜF ÜG mg ÜH kg ÜI kt ÜJ cal ÜK kcal ÜL dB ÜM ÜN ÜO ps ÜP ns ÜQ ÜR ms ÜS pV ÜT nV \
ÜU ÜV mV ÜW kV ÜX MV ÜY pA ÜZ nA Ü[ Ü\ mA Ü] kA Ü^ pW Ü_ nW Ü` Üa mW Üb kW Üc MW Üd Hz Üe kHz Üf \
Üg GHz Üh THz Üi Üj Ük Ül pF Üm nF Ün Üo mol Üp cd Üq rad Ür Üs Üt sr Üu Pa Üv Üw Üx Üy Wb Üz lm \
Ü{ lx Ü| Bq Ü} Gy Ü~ Sv Ü‘ ÙY ÙZ Ùµ Ù¶ Ú5"
#define SYMBOL_KDIGIT "¹¡ ´â  e Àå ¤‚ ¯³"

/* this function analyze a given string into several partable including Korean
   word-phrase, English string,digit string, punctuation, Chinese string,
   pictorial string, and so on. */
bool SplitHangulSymbol(CKoma *koma, char* hword) 
{
  char word[XL_WORD];
  unsigned char* w;
  short start;
  short tag;
  short len;
  short wi;
  
  len = strlen (hword);
  wi = 0;
  w = (unsigned char*) hword;
  while (wi < len) {
    start = wi;
    if (isascii(w[wi])) {

      if (isalpha(w[wi])) {
        /* foreign language - english */
        while (wi < len && isalpha (w[wi])) wi++;
        strncpy (word, hword+start, wi-start);
        word[wi-start] = (char)NULL;
        tag = T_SCF;
      } 
      else if (isdigit (w[wi])) {
        /* digit */
        while (wi < len && isdigit (w[wi])) wi++;
        strncpy (word, hword+start, wi-start);
        word[wi-start] = (char)NULL;
        tag = T_SCD;
      } 
      else if (hword[wi] == '.') {
        /* period or abbreviation mark */
        while (wi < len && (hword[wi] == '.')) wi++;
        strncpy (word, hword+start, wi-start);
        word[wi-start] = (char)NULL;
        if (wi-start == 1) tag = T_SSP;
        else tag = T_SSA;
      } 
      else {
        if (hword[wi] == '?') tag = T_SSQ;
        else if (hword[wi] == '!') tag = T_SSE;
        else if (hword[wi] == ',') tag = T_SSC;
        else if (hword[wi] == '/') tag = T_SSS;
        else if (hword[wi] == ':') tag = T_SSN;
        else if (hword[wi] == ';') tag = T_SSM;
        else if (hword[wi] == '`') tag = T_SSLQ;
        else if (hword[wi] == ' ') tag = T_SPACE;
        else if (strchr (" \" \' ", hword[wi]) != NULL) tag = T_SSRQ;
        else if (strchr (" ( [ { < ", hword[wi]) != NULL) tag = T_SSLP;
        else if (strchr (" ) ] } > ", hword[wi]) != NULL) tag = T_SSRP;
        else if (strchr (" - ~ ", hword[wi]) != NULL) tag = T_SSH;
        else tag = T_SSX;
        word [0] = hword[wi++]; 
        word [1] = (char)NULL;
      }
    } 
    else {
      if (isKOR(w[wi], w[wi+1])) {
        /* korean */
        while (wi < len && isKOR(w[wi], w[wi+1])) wi += 2;
        strncpy (word, hword+start, wi-start);
        word [wi-start] = (char)NULL;
        tag = T_UN;
      } 
      else if (isHJA(w[wi], w[wi+1])) {
        /* hanja */
        while (wi < len && isHJA(w[wi], w[wi+1])) wi += 2;
        strncpy (word, hword+start, wi-start);
        word [wi-start] = (char)NULL;
        tag = T_SCH;
      } 
      else {
        word [0] = hword [wi++];
        word [1] = hword [wi++];
        word [2] = (char)NULL;
        tag = T_SSX;
      }
    }
    if (!koma->PushWord(word, tag)) return false;
  }
  return true;
}

/* analyze symbol(non-hangul) string. */
bool AnalyzeSymbol(CKoma *koma, short wi) 
{
  short pseq [XL_PSEQ];
  short pi;
  
  koma->m_Table.w[wi].bi = koma->m_Table.topa - 1;
  if (!(pi=koma->PushPair(koma->m_Table.w[wi].word, koma->m_Table.w[wi].tag, F_DEF))) return false;
  pseq[0] = pi - 1;
  pseq[1] = BOTTOM;
  if (!koma->PushAnal(pseq)) return false;
  koma->m_Table.w[wi].ti = koma->m_Table.topa - 1;
  return true;
}

/* si : startable index.
   col : column.
   pseq : sequence of pair.
   known=1 : use known startable table.
   known=0 : use unknown startable table. */
bool ConcateHangulPair(CKoma *koma, short si, short col, short* pseq, short known) 
{
  short mi;
  short pi;
  char* morph;
  short tag;
  unsigned long tags[2];
  unsigned long feat;
  short pptag;
  short ptag;
/* modified start */
  char surf [200];
  unsigned surf_tags[2];
  unsigned surf_feat;
  short ci, num_i;
  /* modified end */

  /* check overflow */
  if (col >= XL_PSEQ) return false;
  /* get index */
  if (known) mi = koma->m_Table.k[si].mseq[col];
  else mi = koma->m_Table.u[si].mseq[col];
  if (mi == BOTTOM) {
    pseq [col] = BOTTOM;
    if (!koma->PushAnal(pseq)) return false;
    return true;
  }
  morph = koma->m_Table.m[mi].morph;
  tags[0] = koma->m_Table.m[mi].tags[0];
  tags[1] = koma->m_Table.m[mi].tags[1];
  feat = koma->m_Table.m[mi].feat;
  for (tag=T_NNCG; tag<=T_UV; tag++) {
    if (tags[tag<32?0:1] & ((unsigned long)1 << (tag<32?tag:(tag-32)))) {
      /* beginning of modified part at 15 Sep 1998. */
      /* to prevent more than two predicative postposition in a single korean string. */
      if ( tag == T_I ) {
        num_i = 0;
        for (ci=0; ci < col; ci++)
          if ( (koma->m_Table.p[pseq[ci]].tag) == T_I ) num_i++;
        if ( num_i > 1 ) continue;
      }
      /* end of modified part at 15 Sep 1998. */
      if (col) {
        ptag = koma->m_Table.p[pseq[col-1]].tag;
        /* modified start */ 
        if ((ptag==T_EFC) && ((tag==T_VVX) || (tag==T_VJX))) {
          sprintf (surf, "%s%s", koma->m_Table.p[pseq[col-1]].morph, morph);
          if (!koma->m_Rsc.GetPos(surf, surf_tags, &surf_feat)) return false;
          if ((tag==T_VVX) && !(surf_tags[1] & p_sevv[1])) continue;
          if ((tag==T_VJX) && !(surf_tags[1] & p_sevj[1])) continue;
        } 
        else if ((ptag==T_NNCG) && ((tag==T_VVG) || (tag==T_VJG))) {
          sprintf (surf, "%s%s", koma->m_Table.p[pseq[col-1]].morph, morph);
          if (!koma->m_Rsc.GetPos(surf, surf_tags, &surf_feat)) return false;
          if ((tag==T_VVG) && !(surf_tags[1] & p_scvv[1])) continue;
          if ((tag==T_VJG) && !(surf_tags[1] & p_scvj[1])) continue;
        }
        /* modified end */ 
        else if (!koma->m_Rsc.IsConnectable(ptag, tag)) continue;
        /* modified start */ 
        /* bigram */
        /* negative lexical constraint */
        if ( (tag==T_XSNN) || 
             (tag==T_PX) || 
             (tag==T_PO) ||
             (ptag==T_XPNN) ) {
          sprintf (surf, "%s_%s+%s_%s",
          koma->m_Table.p[pseq[col-1]].morph, TagName [ptag],
          morph, TagName [tag]);
          if (!koma->m_Rsc.GetPos(surf, surf_tags, &surf_feat)) return false;
          if ( surf_tags[1] & p_sdb[1] ) continue;
        }
        /* positive lexical constraint */
/*      if ( (tag==T_NNCG || tag==T_NNCV || tag==T_NNCJ) &&
           (tag==T_NNCG || tag==T_NNCV || tag==T_NNCJ) ) {
          sprintf (surf, "%s_%s+%s_%s",
          koma->m_Table.p[pseq[col-1]].morph, TagName [ptag],
          morph, TagName [tag]);
          if (!koma->m_Rsc.GetPos(surf, surf_tags, &surf_feat)) return false;
          if (! (surf_tags[1] & p_sdn[1]) ) continue;
        }*/
        /* trigram */
        if (col>1) {
          pptag = koma->m_Table.p[pseq[col-2]].tag;
          if ( ((pptag==T_I) && (ptag==T_EFC) && (tag==T_VVX || tag==T_VJX)) ||
               (pptag==T_EFC && ptag==T_VVX && tag==T_EFD) ||
               ((pptag==T_EFC || pptag==T_EFF || pptag==T_PA || pptag==T_EFN ) && 
                (ptag==T_I) && 
                (tag==T_EFC || tag==T_EFF || tag==T_EFD || tag==T_EFN)) ) {
            sprintf (surf, "%s_%s+%s_%s+%s_%s",
                     koma->m_Table.p[pseq[col-2]].morph, TagName [pptag],
                     koma->m_Table.p[pseq[col-1]].morph, TagName [ptag],
                     morph, TagName [tag]);
            if (!koma->m_Rsc.GetPos(surf, surf_tags, &surf_feat)) return false;
            if ( surf_tags[1] & p_sdb[1] ) continue;
          } 
        } 
        /* modified end */ 
      } 

      if (!(pi=koma->PushPair(morph, tag, F_DEF))) return false;
      pseq [col] = pi-1;
      if (!ConcateHangulPair(koma, si, (short)(col+1), pseq, known)) return false;
    }
  }
  return true;
}

bool ConcateHangul(CKoma *koma, short wi) 
{
  short ki;    /* known start index */
  short ui;    /* unknown start index */
  short pseq [XL_PSEQ];

  koma->m_Table.w[wi].bi = koma->m_Table.topa - 1;
  /* known startable stack */
  if (!koma->BuildKnown()) return false;
  for (ki=0; ki<koma->m_Table.topk; ki++)
    if (!ConcateHangulPair(koma, ki, 0, pseq, 1)) return false;
  /* if it is impossible to morphologically analyze the word, unknown startable stack */
  /* original begin */
  if (koma->m_Table.topa-1 == koma->m_Table.w[wi].bi) {
    if (!koma->BuildUnknown()) return false;
    for (ui=0; ui<koma->m_Table.topu; ui++)
      if (!ConcateHangulPair(koma, ui, 0, pseq, 0)) return false;
  }
  /* original end */
  /* modified begin
  if (!koma->BuildUnknown()) return false;
  for (ui=0; ui<koma->m_Table.topu; ui++)
    if (!ConcateHangulPair(koma, ui, 0, pseq, 0)) return false;
  modified end */
  koma->m_Table.w[wi].ti = koma->m_Table.topa - 1;
  return true;
}

bool AnalyzeHangul(CKoma *koma, short wi) 
{
  koma->ResetHangul();
  if (!SplitHangulbyPrediction(koma, wi)) {
	return false;
  }
  if (!ConcateHangul(koma, wi)){
     return false;
  }
  return true;
}

/* wi : word index.
   wcol : column index of word anal table.
   pseq : sequence of pair.
   ptag : previous tag.*/
bool ConcateSymbolPair(CKoma *koma, short wi, short wcol, short* pseq, short ptag) 
{
  short ai;
  short col;
  short pi;
  short tag, pptag;
/* modified start */
  char surf [200];
  unsigned surf_tags[2];
  unsigned surf_feat;
/* modified end */

  /* check overflow */
  if (wcol >= XL_PSEQ) return false;
  /* ending of word */
  if (wi == koma->m_Table.topw) {
    /* check if adjacent two tags are able to be concated. */
    if (!koma->m_Rsc.IsConnectable(ptag, T_SPACE)) return true;
    pseq [wcol] = BOTTOM;
    if (!koma->PushWordAnal(pseq)) return false;
    return true;
  }
  for (ai=koma->m_Table.w[wi].bi+1; ai<koma->m_Table.w[wi].ti+1; ai++) {
    col = 0;
    pi = koma->m_Table.a[ai].pseq[col];
    tag = koma->m_Table.p[pi].tag;
    /* check if adjacent two tags are able to be concated. */
    if (!koma->m_Rsc.IsConnectable(ptag, tag)) continue;
    /* modified bigin */
    /* negative lexical constraint */
    /* bigram */
    if (wcol>0) {
      if ( ptag==T_SSLP || ptag==T_SSRP || ptag==T_SSLQ || ptag==T_SSRQ ||
           tag==T_SSX || ptag==T_SCF || ptag==T_SSP ||
           tag==T_SSLP || tag==T_SSRP || tag==T_SSP || tag==T_SSQ ) {
        sprintf (surf, "%s_%s+%s_%s",
                 koma->m_Table.p[pseq[wcol-1]].morph, TagName [ptag],
                 koma->m_Table.p[pi].morph, TagName [tag]);
        if (!koma->m_Rsc.GetPos(surf, surf_tags, &surf_feat)) return false;
        if ( surf_tags[1] & p_sdb[1] ) continue;
      } 
      if ( ptag==T_SCD ) {
        sprintf (surf, "_%s+%s_%s",
                 TagName [ptag],
                 koma->m_Table.p[pi].morph, TagName [tag]);
        if (!koma->m_Rsc.GetPos(surf, surf_tags, &surf_feat)) return false;
        if ( surf_tags[1] & p_sdb[1] ) continue;
      } 
      if ( tag==T_SCD ) {
        sprintf (surf, "%s_%s+_%s",
                 koma->m_Table.p[pseq[wcol-1]].morph, TagName [ptag],
                 TagName [tag]);
        if (!koma->m_Rsc.GetPos(surf, surf_tags, &surf_feat)) return false;
        if ( surf_tags[1] & p_sdb[1] ) continue;
      } 
    } 
    /* modified end */ 
    /* trigram */
    if (wcol>1) 
    {
      pptag = koma->m_Table.p[pseq[wcol-2]].tag;
      if ( (pptag==T_SSLP && tag==T_SSRP) ||
           (ptag==T_SSLP || ptag==T_SSRP || ptag==T_SSLQ || ptag==T_SSRQ) ||
           (pptag==T_SSP && tag==T_SSP) ) {
        sprintf (surf, "%s_%s+%s_%s+%s_%s",
                 koma->m_Table.p[pseq[wcol-2]].morph, TagName [pptag],
                 koma->m_Table.p[pseq[wcol-1]].morph, TagName [ptag],
                 koma->m_Table.p[pi].morph, TagName [tag]);
        if (!koma->m_Rsc.GetPos(surf, surf_tags, &surf_feat)) return false;
        if ( surf_tags[1] & p_sd[1] ) continue;
      } 
    } 
    /* modified end */ 

    while (pi != BOTTOM) {
      pseq [wcol+col] = pi;
      tag = koma->m_Table.p[pi].tag;
      col++;
      pi = koma->m_Table.a[ai].pseq[col];
    }
    if (!ConcateSymbolPair(koma, (short)(wi+1), (short)(wcol+col), pseq, tag)) return false;
  }
  return true;
}

bool ConcateHangulSymbol(CKoma *koma) 
{
  short pseq[XL_PSEQ];
  if (!ConcateSymbolPair(koma, 0, 0, pseq, T_SPACE)) return false;
  return true;
}

bool AnalyzeHangulSymbol(CKoma *koma, char* hword) 
{
  short wi;    /* subword index */
//  short wacnt = 0;  /* number of subwords */

  koma->ResetSymbol();
  if (!SplitHangulSymbol(koma, hword)) return false;
  for (wi=0; wi<koma->m_Table.topw; wi++) {
    if (koma->m_Table.w[wi].tag == T_UN) {
      if (!AnalyzeHangul(koma, wi)) return false;
    } 
    else {
      if (!AnalyzeSymbol(koma, wi)) return false;
    }
  }
  if (!ConcateHangulSymbol(koma)) return false;
  return true;
}
