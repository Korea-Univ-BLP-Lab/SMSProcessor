#ifndef __MorphAPI__
#define __MorphAPI__

#include "nlp.def"

// Koma ������ ���� ���� �� ���� �б�
// ��ȯ�� : 1 - ����, 0 - ����
int LoadKomaEngine(const char *MainEntryFilename, const char *MainDataFilename,
					const char *UserEntryFilename, const char *UserDataFilename,
                    const char *ConnectionTableFilename,
                    const char *TagFilename, const char *TagOutFilename );

// ���� �� ������ ���� �޸� ����
void EndKomaEngine(void);

/* CodeType   -   1: KS5601 code, 0: TRIGEM code */

// Koma class�� instance�� �Ҵ� �ޱ�
// ��ȯ�� : NULL - ����, ��Ÿ - ����
void *CreateKoma(int CodeType);

// Koma class�� instance�� ����
void FreeKoma(void *Koma);

// �̹� �Ҵ� ���� Koma�� instance�� �̿��Ͽ� �� ������ ���¼� �м� ����
// ��ȯ�� : 1 - ����, 0 - ����
int DoKomaOneWord(void *Koma, char *Eojeol, char Result[MaxNumAmb][BuffSize]);

// �̹� �Ҵ� ���� Koma�� instance�� �̿��Ͽ� �� ������ ���¼� �м� ����
// ��ȯ�� : ���峻 ���� ����
// ���� : �� �Լ��� �°ſ��� ����ϱ� ���� ����
int DoKomaOneSentence(void *Koma, char *Sentence, void *Result);

// ���¼� �м� ��� ���ٿ� �Լ�

// ���¼� �м� ��� ����
short GetNumResult(void *Koma);

// wai��° �м� ����� wcol��° ���¼��� �ε��� ��������
short GetPair(void *Koma, short wai, short wcol);

// pi��° �ε����� �ش��ϴ� ���¼��� ���ڿ�
const char *GetMorph(void *Koma, short pi);

// pi��° �ε����� �ش��ϴ� ���¼��� ǰ��
const char *GetTag(void *Koma, short pi);

#endif
