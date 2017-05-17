/* Filename	: hcc98.h */
/* Version	: 98 */
/* Discription	: external header of hcc.c */
/* Programmer	: Sang-Zoo Lee (Dept. of Computer Science, Korea University) */
/* Date		: 2 July 1998 */

#ifndef __HCC98_H__
#define __HCC98_H__

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

#endif
