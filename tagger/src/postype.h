/* definition for value of part-of-speech information. */

#ifndef POSTYPE_H
#define POSTYPE_H

/* lexicon information: part-of-speech. */
enum ePOS 
{
    P_NNCG,	/* nominal: noun: common: general */
    P_NNCV,	/* nominal: noun: common: verb */
    P_NNCJ,	/* nominal: noun: common: adjective */
    P_NNB,	/* nominal: noun: bound */
    P_NNBU,	/* nominal: noun: bound: unit */
    P_NNP,	/* nominal: noun: proper */
    P_NPP,	/* nominal: pronoun: personal */
    P_NPI,	/* nominal: pronoun: indicative */
    P_NU,	/* nominal: numeral */
    P_XSNN,	/* affix: suffix: nominal: noun */
    P_XSNND,	/* affix: suffix: nominal: noun: adnominal -jeok- */
    P_XSNP,	/* affix: suffix: nominal: pronoun */
    P_XSNU,	/* affix: suffix: nominal: numeral */
    P_XSNPL,	/* affix: suffix: nominal: plural -deul- */
    P_XPNN,	/* affix: prefix: nominal: noun */
    P_XPNU,	/* affix: prefix: nominal: numeral */
    P_PS,	/* particle: subjective */
    P_PC,	/* particle: complementive */
    P_PO,	/* particle: objective */
    P_PD,	/* particle: determinative */
    P_PA,	/* particle: adverbal */
    P_PV,	/* particle: vocative */
    P_PN,	/* particle: conjunctive */
    P_PX,	/* particle: auxiliary */
    P_DA,	/* adnoun: attributive */
    P_DI,	/* adnoun: indicative */
    P_DU,	/* adnoun: numberal */
    P_XSD,	/* affix: suffix: adnominal */
    P_AA,	/* adverb: attributive */
    P_AP,	/* adverb: predicative */
    P_AI,	/* adverb: indicative */
    P_AC,	/* adverb: conjunctive */
    P_AV,	/* adverb: verb */
    P_AJ,	/* adverb: adjective */
    P_XSA,	/* affix: suffix: adverbal */
    P_XSAH,	/* affix: suffix: adverbal: -hi- */
    P_C,	/* independental(exclamation) */
    P_I,	/* -i- predicative particle stem */
    P_VVG,	/* verbal: verb: general */
    P_VVX,	/* verbal: verb: auxiliary */
    P_VJG,	/* verbal: adjective: general */
    P_VJX,	/* verbal: adjective: auxiliary */
    P_XSVV,	/* affix: suffix: verbal: verb */
    P_XSVJ,	/* affix: suffix: verbal: adjective */
    P_XSVJD,	/* affix: suffix: verbal: adjective: -dab- */
    P_XSVJB,	/* affix: suffix: verbal: adjective: -srub- -rob- */
    P_EFF,	/* ending: final: finished */
    P_EFC,	/* ending: final: conjunctive */
    P_EFN,	/* ending: final: nominal */
    P_EFD,	/* ending: final: determinative */
    P_EFA,	/* ending: final: adverbial */
    P_EP,	/* ending: prefinal */
    P_UN,	/* unknown: noun */
    P_UV,	/* unknown: verb */
    P_SEVV,	/* surface: ending+auxiliary verb */
    P_SEVJ,	/* surface: ending+auxiliary adjective */
    P_SHO,	/* surface: 'ha' omit */
    P_SDB,	/* surface: adnoun+boundnoun */
    P_SDN,	/* surface: (2syllables)adnominal+noun */
    P_SBXV,	/* surface: boundnoun+aux.verb */
    P_SBXJ,	/* surface: boundnoun+aux.adjective */
    P_SCVV,	/* surface: noun+verb */
    P_SCVJ,	/* surface: noun+adjective */
};

/* lexicon information: feature. */
enum eFEATURE 
{
    F_N_IR,	    /* nominal: irregular */
    F_N_CM,	    /* nominal: compounding */
    F_VV_R,	    /* verb conjugation: regular */
    F_VV_I,	    /* verb conjugation: non-L-irregular */
    F_VV_L,	    /* verb conjugation: L-irregular */
    F_VJ_R,	    /* adjective conjugation: regular */
    F_VJ_I,	    /* adjective conjugation: non-L-irregular */
    F_VJ_L,	    /* adjective conjugation: L-irregular */
    F_V_NC,	    /* verbal: non contract */
    F_P_R_I,	    /* particle connectable: -i- predicative particle */
    F_P_V,	    /* particle: vowel */
    F_P_C,	    /* particle: consonant */
    F_P_L,	    /* particle: l-consonant */
    F_P_IR,	    /* particle: irregular */
    F_E_L_V,	    /* ending connectable: verbal */
    F_E_L_J,	    /* ending connectable: adjective */
    F_E_L_I,	    /* ending connectable: -i- predicative particle */
    F_E_L_IX,	    /* ending connectable: -i- omit */
    F_E_L_P,	    /* ending connectable: prefinal ending */
    F_E_R_P,	    /* ending connectable: particle */
    F_E_V,	    /* ending: vowel */
    F_E_C,	    /* ending: consonant */
    F_E_S,	    /* ending: ss|bs-consonant */
    F_E_L,	    /* ending: l-consonant */
    F_E_LX,	    /* ending: l omit */
    F_E_HX,	    /* ending: h omit */
    F_E_HC,	    /* ending: -ha- contract */
    F_E_IR,	    /* ending: irregular */
    F_E_RE,	    /* ending: regular */
    F_UNINF,	    /* uninflection */
    F_INF,	    /* inflection */
    F_DEF,	    /* default */
};

/* tag: part-of-speech tag. */
enum eTAG 
{
    T_NNCG,	/* nominal: noun: common: general */
    T_NNCV,	/* nominal: noun: common: verb */
    T_NNCJ,	/* nominal: noun: common: adjective */
    T_NNB,	/* nominal: noun: bound */
    T_NNBU,	/* nominal: noun: bound: unit */
    T_NNP,	/* nominal: noun: proper */
    T_NPP,	/* nominal: pronoun: personal */
    T_NPI,	/* nominal: pronoun: indicative */
    T_NU,	/* nominal: numeral */
    T_XSNN,	/* affix: suffix: nominal: noun */
    T_XSNND,	/* affix: suffix: nominal: noun: adnominal -jeok- */
    T_XSNP,	/* affix: suffix: nominal: pronoun */
    T_XSNU,	/* affix: suffix: nominal: numeral */
    T_XSNPL,	/* affix: suffix: nominal: plural -deul- */
    T_XPNN,	/* affix: prefix: nominal: noun */
    T_XPNU,	/* affix: prefix: nominal: numeral */
    T_PS,	/* particle: subjective */
    T_PC,	/* particle: complementive */
    T_PO,	/* particle: objective */
    T_PD,	/* particle: determinative */
    T_PA,	/* particle: adverbal */
    T_PV,	/* particle: vocative */
    T_PN,	/* particle: conjunctive */
    T_PX,	/* particle: auxiliary */
    T_DA,	/* adnoun: attributive */
    T_DI,	/* adnoun: indicative */
    T_DU,	/* adnoun: numeral */
    T_XSD,	/* affix: suffix: adnominal */
    T_AA,	/* adverb: attributive */
    T_AP,	/* adverb: predicative */
    T_AI,	/* adverb: indicative */
    T_AC,	/* adverb: conjunctive */
    T_AV,	/* adverb: verb */
    T_AJ,	/* adverb: adjective */
    T_XSA,	/* affix: suffix: adverbal */
    T_XSAH,	/* affix: suffix: adverbal: -hi- */
    T_C,	/* independental(exclamation) */
    T_I,	/* -i- predicative particle stem */
    T_VVG,	/* verbal: verb: general */
    T_VVX,	/* verbal: verb: auxiliary */
    T_VJG,	/* verbal: adjective: general */
    T_VJX,	/* verbal: adjective: auxiliary */
    T_XSVV,	/* affix: suffix: verbal: verb */
    T_XSVJ,	/* affix: suffix: verbal: adjective */
    T_XSVJD,	/* affix: suffix: verbal: adjective: -dab- */
    T_XSVJB,	/* affix: suffix: verbal: adjective: -srub- -rob- */
    T_EFF,	/* ending: final: finished */
    T_EFC,	/* ending: final: conjunctive */
    T_EFN,	/* ending: final: nominal */
    T_EFD,	/* ending: final: determinative */
    T_EFA,	/* ending: final: adverbial */
    T_EP,	/* ending: prefinal */
    T_UN,	/* unknown: noun */
    T_UV,	/* unknown: verb */
    T_SSP,	/* symbol: sentencial - . */
    T_SSQ,	/* symbol: sentencial - ? */
    T_SSE,	/* symbol: sentencial - ! */
    T_SSC,	/* symbol: sentencial - , */
    T_SSS,	/* symbol: sentencial - / */
    T_SSN,	/* symbol: sentencial - : */
    T_SSM,	/* symbol: sentencial - ; */
    T_SSLQ,	/* symbol: sentencial - ` */
    T_SSRQ,	/* symbol: sentencial - ' " */
    T_SSLP,	/* symbol: sentencial - ( [ { < */
    T_SSRP,	/* symbol: sentencial - ) ] } > */
    T_SSH,	/* symbol: sentencial - - ~ */
    T_SSA,	/* symbol: sentencial - ... ...... */
    T_SSX,	/* symbol: sentencial - ext. */
    T_SCF,	/* symbol: character - foreign language */
    T_SCH,	/* symbol: character - chinese */
    T_SCD,	/* symbol: character - digit */
    T_SPACE,	/* space */
};

#define LEN_POS		((short)3)
#define NUM_POS		((short)96)
#define NUM_TAG		(T_SPACE+1)

#endif
