/* Filename	: hcc.h */
/* Version	: 02 (2002) */
/* Discription	: external header of hcc.c */
/* Programmer	: Sang-Zoo Lee (Dept. of Computer Science, Korea University) */
/* Date		: 2 July 1998 */
/* Modifier	: Won-Ho Ryu, Do-Gil Lee */
/* Date		: 19 October 2001 - Ȯ�� �ϼ��� ��ȯ �ڵ� �߰� */
/* Modifier	: Sang-Zoo Lee */
/* Date		: 19 January 2002 - �ڵ� ���� �� �ϼ��� ���� ���� ��� �Լ� �߰� */
/* Modifier	: Sang-Zoo Lee */
/* Date		: 06 February 2002 - �ϼ��� ���� 2-�׷� ��� �Լ� �߰� */

#ifndef __HCC_H__
#define __HCC_H__

#include "stdio.h"

typedef unsigned char	byte;

/* KS �ڵ� ���ڿ��� TG �ڵ� ���ڿ��� ��ȯ�Ѵ�.
    ks : KS ���ڿ� 
    tg : TriGem ���ڿ� */
extern void ks2tg (byte *ks , byte *tg);

/* TG �ڵ� ���ڿ��� KS �ڵ� ���ڿ��� ��ȯ�Ѵ�.
    tg : TriGem ���ڿ�
    ks : KS ���ڿ� */
extern void tg2ks (byte *tg , byte *ks);

/* KS �ڵ� �Է� ��Ʈ���� TG �ڵ� ��� ��Ʈ������ ��ȯ�Ѵ�.
    ks_in : KS �ڵ� �Է� ��Ʈ�� 
    tg_out : TG �ڵ� ��� ��Ʈ�� */
extern void stream_ks2tg (FILE *ks_in , FILE *tg_out);

/* TG �ڵ� �Է� ��Ʈ���� KS �ڵ� ��� ��Ʈ������ ��ȯ�Ѵ�.
    tg_in : TG �ڵ� �Է� ��Ʈ�� 
    ks_out : KS �ڵ� ��� ��Ʈ�� */
extern void stream_tg2ks (FILE *tg_in , FILE *ks_out);

/* ��� �ϼ��� ���� ������ �ϼ��� �ڵ�� ��� */
extern void show_all_KS_set_in_KS_CODE (FILE* outfp);

/* ��� �ϼ��� ���� ������ ������ �ڵ�� ��� */
extern void show_all_KS_set_in_TG_CODE (FILE* outfp);

/* ��� �ϼ��� ���� 2-�׷��� �ϼ��� �ڵ�� ��� */
extern void show_all_syllable_2_gram_in_KS_CODE (FILE* outfp);

/* ��� �ϼ��� ���� 2-�׷��� ������ �ڵ�� ��� */
extern void show_all_syllable_2_gram_in_TG_CODE (FILE* outfp);

#endif
