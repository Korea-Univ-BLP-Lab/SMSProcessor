/** ��ó����(����+������) ��� �⺻ ��� ����.
 	@file reviserAPI.h
 */
#ifndef __REVISER_API_H__
#define __REVISER_API_H__

#include "kulog.h"

// Config �ڷᱸ�� ���� --------------------------------------------------------------------
#define DRE_CONFIG_MAX_LEN 128 
  
 /**
   @brief ������ ȯ�� ���� �ڷᱸ��
   @remarks  ���� �ɼ� ����
   @remarks  �ڿ� �̿� �ɼ� ����
 */
typedef struct _reviser_config_t
{
	/**
	  @brief ����Ƽ�� ����
	  @remarks ����Ƽ�� ���� �ɼ� :on(1), off(0), default(0).
	  */	
	int emoticon;
	
	/**
	  @brief �ݺ��� ó��
	  @remarks �ݺ��� ó�� �ɼ� :on(1), off(0), default(0).
	  */
	int repeat;
	
	/**
	  @brief �弳 ����
	  @remarks �弳 ���� �ɼ� :on(1), off(0), default(0).
	  */
	int insult;
	
	/**
	  @brief ö�� ����
	  @remarks ö�� ���� �ɼ� :on(1), off(0), default(0).
	  */
	int speller;
	
	
	/**
	  @brief ���� ����
	  @remarks ���� ���� �ɼ� :on(1), off(0), default(0).
	  */	  
	int phoneme;
	
	/**
	  @brief ���� ����
	  @remarks ���� ���� �ɼ� :on(1), off(0), default(0).
	  */
	int spacer;
	
	/**
	  @brief �̸�Ƽ�� ���� ����
	  @remarks �̸�Ƽ�� ���� ���� �ɼ� : default ("../rsc/emoticon") 
	  */	
	char emoticon_dic[DRE_CONFIG_MAX_LEN];
	
	/**
	  @brief �ݺ��� ���� ����
	  @remarks �ݺ��� ���� ���� �ɼ� : default ("../rsc/repeat") 
	  */	
	char repeat_dic[DRE_CONFIG_MAX_LEN];
	
	/**
	  @brief �弳 ���� ����
	  @remarks �弳 ���� ���� �ɼ� : default ("../rsc/insult") 
	  */	
	char insult_dic[DRE_CONFIG_MAX_LEN];
	
	/**
	  @brief ö�� ���� ����
	  @remarks ö�� ���� ���� �ɼ� : default ("../rsc/speller") 
	  */	
	char speller_dic[DRE_CONFIG_MAX_LEN];
	
	/**
	  @brief ���� �ڿ� ���
	  @remarks ���� �ڿ� ��� �ɼ� : default ("../rsc") 
	  */	
	char spacer_path[DRE_CONFIG_MAX_LEN];

	/**
	  @brief ���¼� ����
	  @remarks ���¼� ���� �ɼ� : default ("../rsc") 
	  */	
	char morpheme_dic[DRE_CONFIG_MAX_LEN];
} reviser_config_t;

#define DEFAULT_EMOTICON_DIC	"../rsc/emoticon"
#define DEFAULT_REPEAT_DIC		"../rsc/repeat"
#define DEFAULT_INSULT_DIC		"../rsc/insult"
#define DEFAULT_SPELLER_DIC		"../rsc/speller"
#define DEFAULT_SPACER_PATH		"../rsc"
#define DEFAULT_MORPHEME_DIC	"../rsc/morpheme"



// message �ڷᱸ�� ����-----------------------------------------------------------------
/**
	@brief �ִ� �޽��� ����.
 */
#define		MAX_MSG_LEN		1024

#define		NUMBER_OF_TAGS	8		// �±��� ����

#define		INSULT			0		// �弳
#define		UNKNOWN			1		// ��

#define		JOY				2		// ���
#define		SAD				3		// ����
#define		SURPRISE		4		// ���
#define		COLD			5		// �ô�
#define		LOVE			6		// ���
#define		OTHERS			7		// ��Ÿ

/**
 @brief ��� �ڷᱸ��.
 ��ó���� ������ �����(���ڿ�)�� �����Ѵ�
 */
typedef struct	_Message
{
	/**
	  @brief �޽��� ����.
	  @remarks 
	  ����� �Է� ���ڿ�.
	  @remarks
	  ��) �� ���� �� �����ϰ� �;� ��������������
	  */
	char	orgstr[MAX_MSG_LEN];				// �޽��� ����
	/**
	  @brief �޽��� ������.
	  @remarks 
	  ��������/���� ��� ���ڿ�.
	  @remarks
	  ��) �ٽ� �����ϰ� �;� [���:������]
	  */
	char	revstr[MAX_MSG_LEN];				// �޽��� ������
	/**
	  @brief �±׸� �ɷ��� ������.
	  @remarks 
	  revstr���� �±׸� ������ ���ڿ�.
	  @remarks
	  ��) �ٽ� �����ϰ� �;� 
	  */
	char	fltstr[MAX_MSG_LEN];				// �޽��� ������
	/**
	  @brief �±� �ν� ���.
	  @remarks 
		@remarks 	
		@remarks	- exp[INSULT]	: �弳
		@remarks	- exp[UNKNOWN]	: ��
		@remarks	- exp[JOY]	: ��� 			
		@remarks	- exp[SAD]	: ����
		@remarks	- exp[SURPRISE] : ���
		@remarks	- exp[COLD]	: �ô�
		@remarks	- exp[LOVE]	: ���
		@remarks	- exp[OTHERS]	: ��Ÿ	  
		@remarks	�νİ�� : ������ �з��� ���� ���� ���� ��, �ǹ���(\\t)�� �����Ѵ�.
		
		��)
		@remarks exp[JOY]	= "������"
		@remarks exp[SAD] = "����\t����"
		@remarks exp[SAD] = "--" �������ٸ���
	  */
	char    exp[NUMBER_OF_TAGS][MAX_MSG_LEN];
} Message;

// �Լ� ����--------------------------------------------------------------------
/** 
 @brief ������ ���, ���� ��������� �ʱ�ȭ�Ѵ�.
 @param pStReviserConf : ������ ȯ�� ���� structure.
 @param slh : log �ڵ鷯.
 @return 0 : ����ó�� , -1 : ������ ó��
 @see reviser_config_t
 */
int reviser_init( reviser_config_t * pStReviserConf, kulog_handler_t * slh );

/**
 @brief input�� ���ڿ��� ��ó���ؼ� msg�� �����Ѵ�.
 @param pStReviserConf : ������ ȯ�� ���� structure.
 @param input ó�� ��� ���ڿ�(����� �Է� ���ڿ�)
 @param msg ó�� ��� ���� ����ü
 @return 0 : ����ó�� , -1 : ������ ó��
 @see reviser_config_t, Message
  */
int reviser_run( reviser_config_t * pStReviserConf, const char * input, Message * msg );

/**
 @brief ������ ���, ���� ����� �������Ѵ�..
 @param pStReviserConf : ������ ȯ�� ���� structure.
 @return 0 : ����ó��
 @see reviser_config_t
 */
int reviser_finish( reviser_config_t * pStReviserConf );

#endif
