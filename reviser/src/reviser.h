#ifndef __REVISER_H__
#define __REVISER_H__

#include "FST.h"
#include "han.h"
#include "HString.h"

#define		MAX_STR_LEN		1024

/**
 @brief ÀÔ·Â ¹®ÀÚ¿­ ¾È¿¡ Èğ¾îÁø ÇÑ±Û À½¼ÒµéÀ» Á¶ÇÕÇÑ´Ù.
 @param input Ã³¸® ´ë»ó ¹®ÀÚ¿­(»ç¿ëÀÚ ÀÔ·Â ¹®ÀÚ¿­)
 @param output Ã³¸® °á°ú ¹®ÀÚ¿­ÀÌ ÀúÀåµÉ À§Ä¡
 @return ¾øÀ½
 @remarks	¿¹) ¤¡ ¤¿ ¤¸ ¤¿ => °¡ÀÚ
  */
void combinePhoneme( const AString & input , char * output);

/**
 @brief ÀÔ·Â ¹®ÀÚ¿­ ¾È¿¡ ¹İº¹±¸¸¦ Á¦°ÅÇÑ´Ù.
 @param input Ã³¸® ´ë»ó ¹®ÀÚ¿­(»ç¿ëÀÚ ÀÔ·Â ¹®ÀÚ¿­)
 @param output Ã³¸® °á°ú ¹®ÀÚ¿­ÀÌ ÀúÀåµÉ À§Ä¡
 @return ¾øÀ½
 @remarks	¿¹) ¤¡¤¡¤¡¤¡¤¡¤¡¤¡¤¡ => ¤¡¤¡¤¡ , ¹ÙÀÌ¹ÙÀÌ¹ÙÀÌ => ¹ÙÀÌ
  */
void RemoveRep( const HString & input , char * output);

/**
 @brief ÀÔ·Â ¹®ÀÚ¿­ ¾È¿¡ ÃÖÀåÀÏÄ¡ FST Å° ´ë½Å FST value(fst_info)¸¦ Ä¡È¯ÇÏ¿© output¿¡ ³Ö´Â´Ù.
 @param fst FST key data
 @param fst_info FST value data
 @param input Ã³¸® ´ë»ó ¹®ÀÚ¿­(»ç¿ëÀÚ ÀÔ·Â ¹®ÀÚ¿­)
 @param output Ã³¸® °á°ú ¹®ÀÚ¿­ÀÌ ÀúÀåµÉ À§Ä¡
 @param msg Ã³¸® °á°ú ÀúÀå ±¸Á¶Ã¼
 @return ¾øÀ½
 @remarks	¿¹) ³Í ¿Àµğ °¡´Ï? => ³Í ¾îµğ °¡´Ï?  ( Àû¿ë : ¿Àµğ => ¾îµğ )
  */
void ApplyRule( const HString & input , char * output, int o1, int o2, int o3, int o4);
//void ApplyRule( FST * fst, char **fst_info , const HString & input , char * output);

/**
 @brief ÀÔ·Â ¹®ÀÚ¿­ ¾È¿¡ À½¼Ò°¡ ÀÖ´ÂÁö ¾Ë·ÁÁØ´Ù.
 @param input Ã³¸® ´ë»ó ¹®ÀÚ¿­(»ç¿ëÀÚ ÀÔ·Â ¹®ÀÚ¿­)
 @return true - Á¤»óÀûÀÎ ¸Ş½ÃÁö , false - ¹«ÀÇ¹ÌÇÑ ¸Ş½ÃÁöø
 @remarks	¿¹) ¤¿·¯¤Ó¤§¤¡ => false
  */
bool HaveHanPhoneme( const char *input );

/**
 @brief ÅÂ±× ¹ÛÀÇ À½¼Ò¸¦ Á¦°ÅÇÑ´Ù.
 @param input Ã³¸® ´ë»ó ¹®ÀÚ¿­(»ç¿ëÀÚ ÀÔ·Â ¹®ÀÚ¿­)
 @return ¾øÀ½.
 @remarks	¿¹) ¸Ô¤¤³ª [³Ã´ã:¤Ñ¤Ñ¤»¤»]¤»	=> ¸Ô³ª [³Ã´ã:¤Ñ¤Ñ¤»¤»]
  */
void RemoveHanPhoneme(char *str);
//--------------------------------³»ºÎ ÇÔ¼ö----------------------------------------
/**
 @brief ÀÔ·Â ¹®ÀÚ¿­°ú ÃÖÀå ÀÏÄ¡ÇÏ´Â 
  */
int LongestMatchHash( const AString & str, HString & key, char * value, int o1, int o2, int o3, int o4 );

/**
 @brief Á¶ÇÕÇü ÄÚµå¸¦ Á¶ÇÕÇÏ¿© ¿Ï¼ºÇü ÇÑ±ÛÀ» ¸¸µç´Ù. Á¾¼ºÀº »ı·« °¡´É.
 @param first ÃÊ¼º
 @param second Áß¼º
 @param third Á¾¼º(»ı·« °¡´É)
 @return ¸¸µé¾îÁø ÇÑ±Û 1±ÛÀÚ ¹®ÀÚ¿­
  */
AString makeHan( const byte first, const byte second, const byte third = 1);

#endif
