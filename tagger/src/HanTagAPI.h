/** ���¼� �м� �� ǰ�� ������ ��� �⺻ API ��� ����
 	@file HanTagAPI.h
 */
#ifndef __HANTAGAPI__
#define __HANTAGAPI__

#include "nlp.def"
///////////////////////////////////////////////////////////////
#include "kulog.h"

/**
 @brief ���¼� �м��� ȯ�� ���� �ڷᱸ��.
 */
typedef struct _HanTag_config_t
{
	/**
	  @brief �°� ���� ����.
	  @remarks 
	  on(1) /off(0)
	  */
	int tagger;
	/**
	  @brief �°� ���ҽ� ���.
	  @remarks 
	  ��) ../tagger/rsc
	  */
	char tagger_path[DRE_CONFIG_MAX_LEN];
} HanTag_config_t;

#define DEFAULT_TAGGER_PATH		"../rsc/tagger"
/** 
 @brief ���¼� �м��� �� ǰ�� ���� ����� �ʱ�ȭ
 @param pStHanTagConf : ���¼� �м��� �� ǰ�� ������ ȯ�� ���� ����
 @param HanTag_log : ���¼� �м��� �� ǰ�� ������ �α� �ڵ鷯
 @param HanTag : ���¼� �м� �� ǰ�� ������
 @return 0 : ����ó�� , -1 : ������ ó��
 @see HanTag_config_t
 */
int HanTag_init( HanTag_config_t * pStHanTagConf, kulog_handler_t * HanTag_log, void ** HanTag );

/**
 @brief �� ������ �Է¹޾� ���¼� �м� �� ǰ�� ������ ����
 @param pStHanTagConf : ���¼� �м��� �� ǰ�� ������ ȯ�� ���� ����
 @param HanTag : ���¼� �м��� ǰ���±��� ���� ��ü
 @param Sentence : ���¼� �м� �Ǵ� ǰ�� �±��� ������ ������ ���� ��) Sentence="���� ����"
 @param Wrd[] : ������ ���������� ���ʷ� ���� ��) Wrd[0]="����", Wrd[1]="����"
 @param Result[] : �� ������ ���� ���¼� �м� ����� ���� ��) Result[0]="��/NP+��/JX", Result[1]="��/VA+��/EF"
 @return ���¼� �м��� ������ ������ ���� ��ȯ
 @see HanTag_config_t
 */ 
int HanTag_run( HanTag_config_t * pStHanTagConf, void *HanTag, char *Sentence, char *Wrd[MaxNumWrd], char *Result[MaxNumWrd][MaxNumAmb] );


/**
 @brief ����� ���� ǰ��� ��ȯ
**/
bool Convert2Usertag ( void *HanTag,  char Result[MaxNumAmb] );

/**
 @brief ���¼� �м��� �� ǰ�� ������ ����� ������
 @param HanTag : ���¼� �м��� ǰ�� �±��� ���� ��ü
 @return 0 : ����ó�� , -1 : ������ ó��
 */
int HanTag_finish(void * HanTag);

///////////////////////////////////////////////////////////////

#ifndef __TaggingMethod__
#define __TaggingMethod__
typedef enum {
  NO_TAGGING,           // ���¼� �м���
  PATH_BASED_TAGGING,   // ��� ��� �±�
  STATE_BASED_TAGGING   // ���� ��� �±�
} TaggingMethod;
#endif

// ���Ͽ��� �� ������ �о���̱�
int LoadSentenceFromStream(FILE *f, char *Sentence, int MaxLength);

// ��Ʈ��(*pString)���� �� ������ �о���̱�
// ������ ���� �� *pString�� ���� ���� ������ ���� ��ġ�� �Ű���
int LoadSentenceFromString(char **pString, char *Sentence, int MaxLength);

// HanTag ������ ���� ���� �� ���� �б�
// ��ȯ�� : 1 - ����, 0 - ����
int LoadHanTagEngine(const char *ProbEntryFilename, const char *ProbDataFilename, 
								const char *PosEntryFilename, const char *PosDataFilename,
								const char *ExistEntryFilename, const char *ExistDataFilename
);

// ���� �� ������ ���� �޸� ����
void EndHanTagEngine(void);

// HanTag class�� instance�� �Ҵ� �ޱ�
// ��ȯ�� : NULL - ����, ��Ÿ - ����
void *CreateHanTag(void);

// HanTag class�� instance�� ����
void FreeHanTag(void *HanTag);

// �̹� �Ҵ� ���� HanTag�� instance�� �̿��Ͽ� �� ������ ���¼� �м� �� �±�
// ��ȯ�� : ���峻 ������ ����
int DoKomaAndHanTag(void *HanTag, TaggingMethod Method, char *Sentence, char *Wrd[MaxNumWrd], char *Result[MaxNumWrd][MaxNumAmb], char bSEJONG='T');

// �̹� �Ҵ� ���� HanTag�� instance�� �̿��Ͽ� �� ������ ���¼� �м� �� �±�
// ��ȯ�� : ���峻 ������ ����
int DoHanTag(void *HanTag, TaggingMethod Method, char *Cnd[MaxNumWrd][MaxNumAmb], char *Result[MaxNumWrd][MaxNumAmb]);

#endif
