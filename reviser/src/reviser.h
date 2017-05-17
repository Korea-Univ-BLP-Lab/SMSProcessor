#ifndef __REVISER_H__
#define __REVISER_H__

#include "FST.h"
#include "han.h"
#include "HString.h"

#define		MAX_STR_LEN		1024

/**
 @brief �Է� ���ڿ� �ȿ� ����� �ѱ� ���ҵ��� �����Ѵ�.
 @param input ó�� ��� ���ڿ�(����� �Է� ���ڿ�)
 @param output ó�� ��� ���ڿ��� ����� ��ġ
 @return ����
 @remarks	��) �� �� �� �� => ����
  */
void combinePhoneme( const AString & input , char * output);

/**
 @brief �Է� ���ڿ� �ȿ� �ݺ����� �����Ѵ�.
 @param input ó�� ��� ���ڿ�(����� �Է� ���ڿ�)
 @param output ó�� ��� ���ڿ��� ����� ��ġ
 @return ����
 @remarks	��) ���������������� => ������ , ���̹��̹��� => ����
  */
void RemoveRep( const HString & input , char * output);

/**
 @brief �Է� ���ڿ� �ȿ� ������ġ FST Ű ��� FST value(fst_info)�� ġȯ�Ͽ� output�� �ִ´�.
 @param fst FST key data
 @param fst_info FST value data
 @param input ó�� ��� ���ڿ�(����� �Է� ���ڿ�)
 @param output ó�� ��� ���ڿ��� ����� ��ġ
 @param msg ó�� ��� ���� ����ü
 @return ����
 @remarks	��) �� ���� ����? => �� ��� ����?  ( ���� : ���� => ��� )
  */
void ApplyRule( const HString & input , char * output, int o1, int o2, int o3, int o4);
//void ApplyRule( FST * fst, char **fst_info , const HString & input , char * output);

/**
 @brief �Է� ���ڿ� �ȿ� ���Ұ� �ִ��� �˷��ش�.
 @param input ó�� ��� ���ڿ�(����� �Է� ���ڿ�)
 @return true - �������� �޽��� , false - ���ǹ��� �޽����
 @remarks	��) �����Ӥ��� => false
  */
bool HaveHanPhoneme( const char *input );

/**
 @brief �±� ���� ���Ҹ� �����Ѵ�.
 @param input ó�� ��� ���ڿ�(����� �Է� ���ڿ�)
 @return ����.
 @remarks	��) �Ԥ��� [�ô�:�ѤѤ���]��	=> �Գ� [�ô�:�ѤѤ���]
  */
void RemoveHanPhoneme(char *str);
//--------------------------------���� �Լ�----------------------------------------
/**
 @brief �Է� ���ڿ��� ���� ��ġ�ϴ� 
  */
int LongestMatchHash( const AString & str, HString & key, char * value, int o1, int o2, int o3, int o4 );

/**
 @brief ������ �ڵ带 �����Ͽ� �ϼ��� �ѱ��� �����. ������ ���� ����.
 @param first �ʼ�
 @param second �߼�
 @param third ����(���� ����)
 @return ������� �ѱ� 1���� ���ڿ�
  */
AString makeHan( const byte first, const byte second, const byte third = 1);

#endif
