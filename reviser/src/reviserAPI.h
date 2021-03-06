/** 穿坦軒奄(句嬢床奄+嘘舛奄) 乞汲 奄沙 伯希 督析.
 	@file reviserAPI.h
 */
#ifndef __REVISER_API_H__
#define __REVISER_API_H__

#include "kulog.h"

// Config 切戟姥繕 舛税 --------------------------------------------------------------------
#define DRE_CONFIG_MAX_LEN 128 
  
 /**
   @brief 嘘舛奄 発井 竺舛 切戟姥繕
   @remarks  嘘舛 辛芝 竺舛
   @remarks  切据 戚遂 辛芝 竺舛
 */
typedef struct _reviser_config_t
{
	/**
	  @brief 戚坪銅嬬 嘘舛
	  @remarks 戚坪銅嬬 嘘舛 辛芝 :on(1), off(0), default(0).
	  */	
	int emoticon;
	
	/**
	  @brief 鋼差姥 坦軒
	  @remarks 鋼差姥 坦軒 辛芝 :on(1), off(0), default(0).
	  */
	int repeat;
	
	/**
	  @brief 水竺 嘘舛
	  @remarks 水竺 嘘舛 辛芝 :on(1), off(0), default(0).
	  */
	int insult;
	
	/**
	  @brief 旦切 嘘舛
	  @remarks 旦切 嘘舛 辛芝 :on(1), off(0), default(0).
	  */
	int speller;
	
	
	/**
	  @brief 製社 繕杯
	  @remarks 製社 繕杯 辛芝 :on(1), off(0), default(0).
	  */	  
	int phoneme;
	
	/**
	  @brief 句嬢床奄 嘘舛
	  @remarks 句嬢床奄 嘘舛 辛芝 :on(1), off(0), default(0).
	  */
	int spacer;
	
	/**
	  @brief 戚乞銅嬬 嘘舛 紫穿
	  @remarks 戚乞銅嬬 嘘舛 紫穿 辛芝 : default ("../rsc/emoticon") 
	  */	
	char emoticon_dic[DRE_CONFIG_MAX_LEN];
	
	/**
	  @brief 鋼差姥 差姥 紫穿
	  @remarks 鋼差姥 差姥 紫穿 辛芝 : default ("../rsc/repeat") 
	  */	
	char repeat_dic[DRE_CONFIG_MAX_LEN];
	
	/**
	  @brief 水竺 嘘舛 紫穿
	  @remarks 水竺 嘘舛 紫穿 辛芝 : default ("../rsc/insult") 
	  */	
	char insult_dic[DRE_CONFIG_MAX_LEN];
	
	/**
	  @brief 旦切 嘘舛 紫穿
	  @remarks 旦切 嘘舛 紫穿 辛芝 : default ("../rsc/speller") 
	  */	
	char speller_dic[DRE_CONFIG_MAX_LEN];
	
	/**
	  @brief 句嬢床奄 切据 井稽
	  @remarks 句嬢床奄 切据 井稽 辛芝 : default ("../rsc") 
	  */	
	char spacer_path[DRE_CONFIG_MAX_LEN];

	/**
	  @brief 莫殿社 紫穿
	  @remarks 莫殿社 紫穿 辛芝 : default ("../rsc") 
	  */	
	char morpheme_dic[DRE_CONFIG_MAX_LEN];
} reviser_config_t;

#define DEFAULT_EMOTICON_DIC	"../rsc/emoticon"
#define DEFAULT_REPEAT_DIC		"../rsc/repeat"
#define DEFAULT_INSULT_DIC		"../rsc/insult"
#define DEFAULT_SPELLER_DIC		"../rsc/speller"
#define DEFAULT_SPACER_PATH		"../rsc"
#define DEFAULT_MORPHEME_DIC	"../rsc/morpheme"



// message 切戟姥繕 舛税-----------------------------------------------------------------
/**
	@brief 置企 五獣走 掩戚.
 */
#define		MAX_MSG_LEN		1024

#define		NUMBER_OF_TAGS	8		// 殿益税 曽嫌

#define		INSULT			0		// 水竺
#define		UNKNOWN			1		// 乞硯

#define		JOY				2		// 奄旨
#define		SAD				3		// 十把
#define		SURPRISE		4		// 且寓
#define		COLD			5		// 鈎眼
#define		LOVE			6		// 紫櫛
#define		OTHERS			7		// 奄展

/**
 @brief 窒径 切戟姥繕.
 穿坦軒研 呪楳廃 衣引弘(庚切伸)聖 煽舌廃陥
 */
typedef struct	_Message
{
	/**
	  @brief 五獣走 据沙.
	  @remarks 
	  紫遂切 脊径 庚切伸.
	  @remarks
	  森) ぇ たさ び 獣拙馬壱 粛嬢 せせせせせせせ
	  */
	char	orgstr[MAX_MSG_LEN];				// 五獣走 据沙
	/**
	  @brief 五獣走 嘘舛沙.
	  @remarks 
	  神嫌嘘舛/句嬢床奄 衣引 庚切伸.
	  @remarks
	  森) 陥獣 獣拙馬壱 粛嬢 [奄旨:せせせ]
	  */
	char	revstr[MAX_MSG_LEN];				// 五獣走 嘘舛沙
	/**
	  @brief 殿益研 杏君浬 嘘舛沙.
	  @remarks 
	  revstr拭辞 殿益研 薦暗廃 庚切伸.
	  @remarks
	  森) 陥獣 獣拙馬壱 粛嬢 
	  */
	char	fltstr[MAX_MSG_LEN];				// 五獣走 嘘舛沙
	/**
	  @brief 殿益 昔縦 衣引.
	  @remarks 
		@remarks 	
		@remarks	- exp[INSULT]	: 水竺
		@remarks	- exp[UNKNOWN]	: 乞硯
		@remarks	- exp[JOY]	: 奄旨 			
		@remarks	- exp[SAD]	: 十把
		@remarks	- exp[SURPRISE] : 且寓
		@remarks	- exp[COLD]	: 鈎眼
		@remarks	- exp[LOVE]	: 紫櫛
		@remarks	- exp[OTHERS]	: 奄展	  
		@remarks	昔縦衣引 : 疑析廃 歳嫌拭 食君 鯵亜 赤聖 凶, 吐庚切(\\t)稽 姥歳廃陥.
		
		森)
		@remarks exp[JOY]	= "せせせ"
		@remarks exp[SAD] = "錠錠\t比比"
		@remarks exp[SAD] = "--" 廃匝廃匝原陥
	  */
	char    exp[NUMBER_OF_TAGS][MAX_MSG_LEN];
} Message;

// 敗呪 舛税--------------------------------------------------------------------
/** 
 @brief 嘘舛奄 乞汲, 句嬢床奄 嘘舛乞汲聖 段奄鉢廃陥.
 @param pStReviserConf : 嘘舛奄 発井 竺舛 structure.
 @param slh : log 輩級君.
 @return 0 : 舛雌坦軒 , -1 : 搾舛雌 坦軒
 @see reviser_config_t
 */
int reviser_init( reviser_config_t * pStReviserConf, kulog_handler_t * slh );

/**
 @brief input税 庚切伸聖 穿坦軒背辞 msg拭 煽舌廃陥.
 @param pStReviserConf : 嘘舛奄 発井 竺舛 structure.
 @param input 坦軒 企雌 庚切伸(紫遂切 脊径 庚切伸)
 @param msg 坦軒 衣引 煽舌 姥繕端
 @return 0 : 舛雌坦軒 , -1 : 搾舛雌 坦軒
 @see reviser_config_t, Message
  */
int reviser_run( reviser_config_t * pStReviserConf, const char * input, Message * msg );

/**
 @brief 嘘舛奄 乞汲, 句嬢床奄 乞汲聖 原巷軒廃陥..
 @param pStReviserConf : 嘘舛奄 発井 竺舛 structure.
 @return 0 : 舛雌坦軒
 @see reviser_config_t
 */
int reviser_finish( reviser_config_t * pStReviserConf );

#endif
