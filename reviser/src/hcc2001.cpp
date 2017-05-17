/* Filename	: hcc2001.c */
/* Version	: 2001 */
/* Discription	: library for hangul code converting (KS-TG) */
/* Programmer	: Sang-Zoo Lee (Dept. of Computer Science, Korea University) */
/* Date		: 2 July 1998 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* About code systems used here:
    KS �ڵ� : KS �ϼ��� 5601-1987 �ڵ�
    TG �ڵ� : TriGem ������ �ڵ�
    H3 �ڵ� : 4����Ʈ Han3 �ڵ�
		8f ??(�ʼ�) ??(�߼�) ??(����)
    K8 �ڵ� : �ϼ��� �����ڸ� �̿��Ͽ� �ѱ��� ǥ���� 8����Ʈ KS-8 �ڵ� 
		a4d4 a4??(�ʼ�) a4??(�߼�) a4??(����) */

#define TG_CODE 1
#define KS_CODE 0

#define SINGLENUM   51  //  �ѱ� ������ �� 
#define SIZE_EXTKS  (sizeof(word)*8822)
#define SIZE_KS     (sizeof(word)*2350)

typedef unsigned char	byte;
typedef unsigned int	word;
typedef unsigned short	sword;

typedef struct  {   //  �ѱ� �ڵ� ����
    unsigned lasc :5;
    unsigned midc :5;
    unsigned firc :5;
    unsigned dummy:1;
}   hcode;

#include "hcc2001_codetable.h"

word KSSM2KS(unsigned code);      //  ���� -> �ϼ�
word KS2KSSM(unsigned code);      //  �ϼ� -> ����
word KSSM2ExtKS(unsigned code);   //  ���� -> Ȯ��ϼ�
word ExtKS2KSSM(unsigned code);   //  Ȯ��ϼ� -> ����

word get_ks (byte ch);
word get_tg (byte ch);
void CodeConv (word wc , char *out , short outCode);
word convKS (word wc);
word word_2h3 (word wc);
word word_tg2ks (word wc);

FILE *outfile;

/* KS �ڵ� ���ڿ��� TG �ڵ� ���ڿ��� ��ȯ�Ѵ�.
    ks : KS ���ڿ� 
    tg : TriGem ���ڿ� */
void ks2tg (byte *ks , byte *tg)
{    
	/* ���� */
    byte ch;		

    /* 4����Ʈ �ڵ� ���� */
    word wc = 0;	

    /* ��ȯ�� �ڵ� ����� */
    char temp [10];
    
    *tg = (byte)NULL;

	while (ch = *ks++) 
    {
        wc = get_ks (ch);

        /* wc�� 0�̸� KS �ڵ尡 �ϼ����� �ʾ����� ��Ÿ�� */
		if (wc != 0) 
		{
			/* wc�� KS �ڵ尡 ����Ǿ� �����Ƿ�, TG �ڵ�� ��ȯ �� ���ڿ��� ÷�� */
			CodeConv (wc , temp , TG_CODE);
			strcat ((char *)tg , (char *)temp);
			wc = 0;
		}
    }
}

/* TG �ڵ� ���ڿ��� KS �ڵ� ���ڿ��� ��ȯ�Ѵ�.
    tg : TriGem ���ڿ�
    ks : KS ���ڿ� */
void tg2ks (byte *tg , byte *ks)
{
    /* ���� */
    byte ch;		

    /* 4����Ʈ �ڵ� ���� */
    word wc = 0;	

    /* ��ȯ�� �ڵ� ����� */
    char temp [10];

    *ks = (byte)NULL;

    while (ch = *tg++) 
    {
        wc = get_tg (ch);
        
        /* wc�� 0�̸� TG �ڵ尡 �ϼ����� �ʾ����� ��Ÿ�� */
		if (wc != 0) 
		{
			/* wc�� TG �ڵ尡 ����Ǿ� �����Ƿ�, KS �ڵ�� ��ȯ ���ڿ��� ÷�� */
			CodeConv (wc , temp , KS_CODE);
			strcat ((char *)ks , (char *)temp);
			wc = 0;
		}
    }	
}

/* KS �ڵ� �Է� ��Ʈ���� TG �ڵ� ��� ��Ʈ������ ��ȯ�Ѵ�.
    ks_in : KS �ڵ� �Է� ��Ʈ�� 
    tg_out : TG �ڵ� ��� ��Ʈ�� */
void stream_ks2tg (FILE *ks_in , FILE *tg_out)
{
    /* ���� */
    byte ch;		

    /* 4����Ʈ �ڵ� ���� */
    word wc = 0;	

    /* ��ȯ�� �ڵ� ����� */
    char temp [10];
    
    while ((ch = getc (ks_in)) != (byte)EOF)
    {
        wc = get_ks (ch);

        /* wc�� 0�̸� KS �ڵ尡 �ϼ����� �ʾ����� ��Ÿ�� */
	if (wc != 0) 
	{
	    /* wc�� KS �ڵ尡 ����Ǿ� �����Ƿ�, TG �ڵ�� ��ȯ �� ��� */
	    CodeConv (wc , temp , TG_CODE);
	    fprintf (tg_out, "%s", temp);
	    wc = 0;
	}
    }
}

/* TG �ڵ� �Է� ��Ʈ���� KS �ڵ� ��� ��Ʈ������ ��ȯ�Ѵ�.
    tg_in : TG �ڵ� �Է� ��Ʈ�� 
    ks_out : KS �ڵ� ��� ��Ʈ�� */
void stream_tg2ks (FILE *tg_in , FILE *ks_out)
{
    /* ���� */
    byte ch;		

    /* 4����Ʈ �ڵ� ���� */
    word wc = 0;	

    /* ��ȯ�� �ڵ� ����� */
    char temp [10];

    while ((ch = getc (tg_in)) != (byte)EOF)
    {
	wc = get_tg (ch);

        /* wc�� 0�̸� TG �ڵ尡 �ϼ����� �ʾ����� ��Ÿ�� */
	if (wc != 0) 
	{
	    /* wc�� TG �ڵ尡 ����Ǿ� �����Ƿ�, KS �ڵ�� ��ȯ �� ��� */
	    CodeConv (wc , temp , KS_CODE);
	    fprintf (ks_out, "%s", temp);
	    wc = 0;
	}
    }
}

/* 4����Ʈ KS �ڵ� �Ǵ� Han3 �ڵ� ����.
    �ϼ����� ������ 0�� ��ȯ.
    �ϼ��Ǹ� �ϼ��� �ڵ带 ��ȯ. */
word get_ks (byte ch)
{
    //ù��° ����Ʈ�� a4������ ��Ÿ��
	static short flag=0;
	
	/* 2����Ʈ �ѱ� �ڵ��� ù��° ����Ʈ */
    static word first = 0;    

	/* Han3 �ڵ带 ����� ���� �߰� ��� */
	static word all = 0;
        
    /* KS8 �ڵ忡�� ������ a4�� �о������� ��Ÿ���� flag */
    static word a4 = 0;
	
	/* 4����Ʈ KS �Ǵ� Han3 �ڵ� */
    word tmp;		       

    /* ���ڰ��� 0x80���� ������ ... */
    if (ch < 0x80) 
    {				
		/* ASCII : first == 0, ch ��ȯ */
		first = 0;
		return ch;
    }

    /* ���ڰ��� 0x80���� ũ�ų� ������ ... */
    /* first�� 0�̸� ... */
    if (first == 0) 
    {				
		/* KS �ڵ��� ù ����Ʈ: first == ch, 0 ��ȯ */
		first = ch;
		return 0;
    } 
    /* first�� 0xa4�̸� ... */
    else if (first == 0xa4) 
    {
		/* ���ڰ��� 0xd4�̸� */
		if (ch == 0xd4) 
		{			
			/* KS8 �ڵ��� ù �� ����Ʈ (FILL): first == 0xd4, 0 ��ȯ*/
			first = 0xd4;
			
			//���ڵ尡 ���۵Ǿ����� ��Ÿ��
			flag = 1;			
			return 0;
 		} 
		/* ���ڰ��� 0xd4�� �ƴϸ� */
		else 
		{			
			/* KS�ڵ��� �� �ڸ� (Separate Jamo):  first == 0, �ڵ� ��ȯ */
			first = 0;
			return (0x8ffba400L) + ch;
		}
    } 
    /* first�� 0x8f�̰ų� 0xd4�̸� ... */
    else if (first==0xd4&&flag) 
    {				
		/* KS8 */	
		/* a4�� 1�̸� ... */
		if (a4==1) 
		{
			/* KS8 �ڵ��� �׹�° ����Ʈ:
			all�� ���ڰ��� �߰�, a4 == 0, first == 0x8f */
			all = (all << 8) + ch;
			a4 = 0;
		} 

		/* a4�� 1�� �ƴϸ� ...
			��, first�� 0xd4�̸� */
		/* ���ڰ��� 0xa4�̸� ... */
		else if (ch == 0xa4) 
		{
			/* KS8 �ڵ��� ����° ����Ʈ: a4 == 1, first == 0xd4 */
			a4 = 1;flag=0;
		}

		/* all�� 0xffff���� �۰ų� ������ ... */
		if (all <= 0xffff) 
		{
			/* Han3 �Ǵ� KS8�� �Ϸ���� �����Ƿ� 0�� ��ȯ */
			return 0;
		}

		/* all�� 0xffff���� ũ�� ... */
		else 
		{
			/* Han3 �Ǵ� KS8�� �Ϸ�ǹǷ� 
			all == 0, first == 0, �ڵ带 �ϼ��� �� ��ȯ */
			tmp = (first<<24) + all;
			all = 0;
			first = 0;
			flag = 0;
			return tmp;
		}
    }     
    else 
    {
		/* �Ϲ����� KS �ڵ�: �ڵ�ϼ� �� ��ȯ, first == 0 */
		tmp = (0x8ffb0000L) + (first<<8) + ch;
		first = 0;
		return tmp;
    }
}

/* 4����Ʈ TG �ڵ� ����.
    �ϼ����� ������ 0�� ��ȯ.
    �ϼ��Ǹ� �ϼ��� �ڵ带 ��ȯ. */
word get_tg (byte ch)
{
    /* ������ ù��° ����Ʈ */
    static word first = 0; 

    /* 4����Ʈ TG �ڵ� */
    word tmp;

    /* first�� 0�̰� ... */
    if (first == 0) 
    {
		/* ���ڰ��� 0x80���� ������ ... */
		if (ch < 0x80) 
		{
			/* ASC: ���ڰ� ��ȯ */
			return ch;
		}
		/* ���ڰ��� 0x80���� ũ�ų� ������ ... */
		/* TG ù ����Ʈ: first == ch, 0 ��ȯ */
		first = ch;
		return 0;
    }
    
    /* first�� 0�� �ƴϸ� ... */
    /* TG �ι�° ����Ʈ: first == 0, ������ �ڵ带 �ϼ��ϰ� ��ȯ */
    tmp = (first << 8) + ch;
    first = 0;
    return tmp;
}

/* Compare two codes for binary search. 
    Return positive value if the former is greater than the latter. 
    Return zero if the former is eqaul to the latter. 
    Return negative value if the former is less than the latter. */
int codecmp (const void *p, const void *q)
{
    word *p1 = (word *)p;
    word *q1 = (word *)q;
    return (*p1 - *q1);
}

/* 4����Ʈ �ڵ� ���ڸ� ��� �ڵ� ���·� ���ڿ��� ����.
   wc : 4����Ʈ �ڵ� ����
   out : ��� ���ڿ�
   outCode : ��� �ڵ� ���� */
void CodeConv (word wc , char *out , short outCode)
{
	if (wc>>8 == 0) 
    {
		/* �ƽ�Ű �ڵ带 ��� ���ڿ��� ���� */
		*out++ = (char) wc;
		*out = (char)NULL;
		return;
    }		
    
	/* ��� �ڵ尡 TG code�̸� �ڵ� ��ȯ �Լ��� convTG �ڵ� ��ȯ �Լ� ���� */
    if (outCode == TG_CODE) wc = ExtKS2KSSM(wc);    
    /* ��� �ڵ尡 TG code�� �ƴϸ� �ڵ� ��ȯ �Լ��� convKS �ڵ� ��ȯ �Լ� ���� */
    else wc = KSSM2ExtKS(wc);

    if (wc>>16 == 0 ||  wc>>16 == 0x8ffb) 
    { 
		/* wc>>16�� 0�̸� TG �ڵ�, 0x8ffb�̸� KS �ڵ�� ��� ���ڿ��� ���� */
		*out++ = (char) (wc>>8) & 0xff; /* ù��° ����Ʈ */
		*out++ = (char) wc & 0xff; /* �ι�° ����Ʈ */
    } 
    else 
    {	
		*out++ = (char) (0xa4 & 0xff); 
		*out++ = (char) (wc & 0xff);     
    }

    *out = (char)NULL;
}

word KSSM2ExtKS(word Code)
{
  word newCode;
  word index;
  word *p;
  byte high, low;

  /* ��ȯ�� KS �ڵ� */
  word check;
  /* TG �ڵ� */
  word tg;	    
  /* �ʼ� */
  word ch;	    
  /* �߼� */
  word ju;
  /* ���� */
  word jo;  
  
  newCode = KSSM2KS(Code);
  if (newCode == 0) {
	if ((p = (word *)bsearch(&Code, ExtKSTable,
		   SIZE_EXTKS / sizeof(word),
		   sizeof(word), codecmp)) != NULL)
	  index = (word) (p - ExtKSTable);
	else 
	{
		if (Code>>16 == 0) tg = Code;
		else {
			if (Code>>24 != 0x8f) Code = word_2h3 (Code);

			/* H3 �ڵ��� �ڼҰ��� ���ؼ� TG �ڵ�� ��ȯ */
			ch = CON [ ((Code>>16) & 0xff) - 0xa1 ] [1]; 
			ju = VOW [ ((Code>>8) & 0xff) - 0xa1 ] [1];
			jo = CON [ (Code&0xff) - 0xa1 ] [2];

			/* tg ������ TG �ڵ� ���� */
			tg = (ch & 0xfc00) + (ju & 0x03e0) + (jo & 0x1f);
		}

		/* TG �ڵ带 KS �ڵ�� ��ȯ */
    
		/* KS �ڵ忡 ���� ���ڰ� �ִ� ���, KS �ڵ� ��ȯ */
		if ((check = word_tg2ks (tg)) >> 16 == 0x8ffb) return check;
    
		/* KS �ڵ忡 ���� ���ڰ� ���� ���, KS8 �ڵ�� ��ȯ */		

		/* TG �ڵ带 H3 �ڵ�� ��ȯ */
		if (Code>>16 == 0) Code = word_2h3 (Code);

		/* H3 �ڵ带 K8 �ڵ�� ��ȯ�ϱ� ���� �ڼҰ� �и� */
		ch = CON [ ((Code>>16) & 0xff) - 0xa1 ] [3]; 
		ju = VOW [ ((Code>>8) & 0xff) - 0xa1 ] [2];
		jo = CON [ (Code & 0xff) - 0xa1 ] [3];
    
		/* K8 �ڵ�� ��ȯ */
		if (ch < 0x100 || ju < 0x100 || jo < 0x100) 
		{
			return 0;
		}

		/* K8 �ڵ� ��ȯ */
		return (0xd4L << 24) + ((ch & 0xff) << 16) + ((ju & 0xff) << 8) +(jo & 0xff);
	}

	if (index < 5696) {
	  high = (index / 178) + 0x81;
	  index %= 178;
	  if (index < 26) low = 0x41 + index;
	  else if (index < 52) low = 0x61 + index - 26;
	  else low = 0x81 + index - 52;
	}
	else if (index < 8804) {
	  index -= 5696;
	  high = (index / 84) + 0xA1;
	  index %= 84;
	  if (index < 26) low = 0x41 + index;
	  else if (index < 52) low = 0x61 + index - 26;
	  else low = 0x81 + index - 52;
	}
	else {
	  high = 0xC6;
	  low = (index - 8804) % 18 + 0x41;
	}
	newCode = (((word) high) << 8) + low;
  }
  return newCode;
}
//---------------------------------------------------------------------------

word ExtKS2KSSM(word Code)
{
  word newCode;
  byte high, low;
  word index=0;

  newCode = KS2KSSM(Code);
  if (newCode == 0) {
    high = (byte) (Code >> 8);
    low = (byte) Code;
    if (high >= 0x81 && high <= 0xA0) {
      index = (high - 0x81) * 178;
      if (low >= 0x41 && low <= 0x5A)
        index += (low - 0x41);
      else if (low >= 0x61 && low <= 0x7A)
        index += (low - 0x61 + 26);
      else index += (low - 0x81 + 52);
    }
    else if (high >= 0xA1 && high <= 0xC5) {
      index = (high - 0xA1) * 84 + 5696;
      if (low >= 0x41 && low <= 0x5A)
        index += (low - 0x41);
      else if (low >= 0x61 && low <= 0x7A)
        index += (low - 0x61 + 26);
      else index += (low - 0x81 + 52);
    }
    else if (high == 0xC6 && low >= 0x41 && low <= 0x52)
      index = (low - 0x41 + 8804);
    if(index >= 8822) return 0;
    newCode = ExtKSTable[index];
  }
  return  newCode;
}
//---------------------------------------------------------------------------

word KSSM2KS(word code)
{
  byte high, low;
  byte temp;
  int  index;
  word *p;

  high = (byte)(code >> 8);
  low = (byte)code;

  //  �ѱ� ��ȯ
  if ((p = (word *)bsearch(&code, KSTable, 2350,
         sizeof(word), codecmp)) != NULL)
  {
    index = (int)(p - KSTable);
    return((((index / 94)+0xb0)<<8)+(index % 94) + 0xa1);
  }
  //  Ư�� ���� ��ȯ1
  else if (high >= 0xd9 && high <= 0xde) {
    if (low < 0x30 || (low > 0x7e && low < 0x91)) return(0);
    if (low >= 0xa1) temp = (high - 0xd9) * 2 + 0xa1 + 1;
    else {
      temp = (high - 0xd9) * 2 + 0xa1;
      if (low <= 0x7e) low += 0x70;
      else low += (0x70 - 18);
    }
    return((temp << 8) + low);
  }
  //  Ư�� ���� ��ȯ2
  else if (high == 0xd4) {
    if (low < 0x80) return(0);
    if (low <= 0xdd) {
      temp = 0xad;
      low += 0x21;
    }
    else {
      temp = 0xae;
      low -= 0x3d;
    }
    return((temp << 8) + low);
  }
  //  ���� ��ȯ
  else if (high >= 0xe0 && high <= 0xf9) {
    if (low < 0x30 || (low > 0x7e && low < 0x91)) return  0;
    if (low >= 0xa1) temp = (high - 0xe0) * 2 + 0xca + 1;
    else {
      temp = (high - 0xe0) * 2 + 0xca;
      if (low <= 0x7e) low += 0x70;
      else low += (0x70 - 18);
    }
    return((temp << 8) + low);
  }

  for (index = 0;index < SINGLENUM;index++)
    if (SingleTable[index] == code) return(0xa4a1 + index);

  return(0);
}
//---------------------------------------------------------------------------

word KS2KSSM(word code)
{
  byte high, low;
  byte temp;
  int  index;
  int  mod, rem;

  high = (byte)(code >> 8);
  low = (byte)code;

  //  �ѱ� ��ȯ
  if (high >= 0xb0 && high <= 0xc8) {
    if (low < 0xa1 || low > 0xfe) return(0);
    index = (high - 0xb0) * 94 + low - 0xa1;
    return(KSTable[index]);
  }
  //  Ư�� ���� ��ȯ1
  else if (high >= 0xa1 && high <= 0xac) {
    if (low < 0xa1 || low > 0xfe) return  0;
    mod = (high - 0xa1) >> 1;
    rem = (high - 0xa1) & 0x01;
    if (rem) temp = low;
    else {
      temp = low - 0x70;
      if (temp > 0x7e) temp += 18;
    }
    return(((mod + 0xd9) << 8) + temp);
  }
  //  Ư�� ���� ��ȯ2
  else if (high == 0xad) {
    if (low < 0xa1 || low > 0xfe) return  0;
    temp = low - 0x21;
    return((0xd4 << 8) + temp);
  }
  //  Ư�� ���� ��ȯ3
  else if (high == 0xae) {
    if (low < 0xa1 || low > 0xc1) return  0;
    temp = low + 0x3d;
    return((0xd4 << 8) + temp);
  }
  //  �ѱ� ��ȯ
/*  else if (high >= 0xb0 && high <= 0xc8) {
    if (low < 0xa1 || low > 0xfe) return(0);
    index = (high - 0xb0) * 94 + low - 0xa1;
    return(KSTable[index]);
  }
  */
  //  ���� ��ȯ
  else if (high >= 0xca && high <= 0xfd) {
    if (low < 0xa1 || low > 0xfe) return(0);
    mod = (high - 0xca) >> 1;
    rem = (high - 0xca) & 0x01;
    if (rem) temp = low;
    else {
      temp = low - 0x70;
      if (temp > 0x7e) temp += 18;
    }
    return(((mod + 0xe0) << 8) + temp);
  }

  index = ((code >> 8) - 0xb0) * 94 + (code & 0x00ff) - 0xa1;
  if( index < 0 || index >= 2350 )  return  0;
  return(KSTable[index]);
}

/* 4����Ʈ KS �ڵ�� ��ȯ */
word convKS (word wc)
{
    /* ��ȯ�� KS �ڵ� */
    word check;

    /* TG �ڵ� */
    word tg;	
    
    /* �ʼ� */
    word ch;	
    
    /* �߼� */
    word ju;

    /* ���� */
    word jo;

	word temp;
    
    /* KS : ���� �ڵ尡 KS �ڵ��̹Ƿ� ��ȯ���� ���� */
    if (wc>>16 == 0x8ffb) return wc;
    
    /* TG �ڵ带 tg ������ ���� */
    if (wc>>16 == 0) tg = wc;
    
    else 
    {
	/* K8 �ڵ带 H3 �ڵ�� ��ȯ */
	if (wc>>24 != 0x8f) wc = word_2h3 (wc);

	/* H3 �ڵ��� �ڼҰ��� ���ؼ� TG �ڵ�� ��ȯ */
	ch = CON [ ((wc>>16) & 0xff) - 0xa1 ] [1]; 
	ju = VOW [ ((wc>>8) & 0xff) - 0xa1 ] [1];
	jo = CON [ (wc&0xff) - 0xa1 ] [2];

	/* tg ������ TG �ڵ� ���� */
	tg = (ch & 0xfc00) + (ju & 0x03e0) + (jo & 0x1f);
    }

    /* TG �ڵ带 KS �ڵ�� ��ȯ */
    
    /* KS �ڵ忡 ���� ���ڰ� �ִ� ���, KS �ڵ� ��ȯ */
    if ((check = word_tg2ks (tg)) >> 16 == 0x8ffb) return check;
    
    /* KS �ڵ忡 ���� ���ڰ� ���� ���, KS8 �ڵ�� ��ȯ */
	temp = wc;

    /* TG �ڵ带 H3 �ڵ�� ��ȯ */
    if (wc>>16 == 0) wc = word_2h3 (wc);

    /* H3 �ڵ带 K8 �ڵ�� ��ȯ�ϱ� ���� �ڼҰ� �и� */
    ch = CON [ ((wc>>16) & 0xff) - 0xa1 ] [3]; 
    ju = VOW [ ((wc>>8) & 0xff) - 0xa1 ] [2];
    jo = CON [ (wc & 0xff) - 0xa1 ] [3];
    
    /* K8 �ڵ�� ��ȯ */
    if (ch < 0x100 || ju < 0x100 || jo < 0x100) 
    {
		return KSSM2ExtKS(temp);
    }

    /* K8 �ڵ� ��ȯ */
    return (0xd4L << 24) + ((ch & 0xff) << 16) + ((ju & 0xff) << 8) +(jo & 0xff);
}

/* 4����Ʈ TG �Ǵ� K8 �ڵ带 4����Ʈ H3 �ڵ�� ��ȯ */
word word_2h3 (word wc)
{
    /* �ʼ� */
    unsigned short ch=0;		

    /* �߼� */
    unsigned short ju=0;		

    /* ���� */
    unsigned short jo=0;

    if (wc>>16 == 0) 
    {	
	/* 4����Ʈ TG �ڵ带 H3 �ڵ��� �ڼҰ����� ��ȯ */
	for (ch = NoCON-1; ch > 0; ch--) 
	{
	    if (CON [ch] [1] == (wc & 0x7c00) + 0x8041) break;
	}
	for (ju = NoVOW-1; ju > 0; ju--)
	{
	    if (VOW [ju] [1] == (wc & 0x03e0) + 0x8401) break;
	}
	for (jo = NoCON-1; jo > 0; jo--)
	{
	    if (CON [jo] [2] == (wc & 0x001f) + 0x8440) break;
	}
    } 

    else if (wc>>24 == 0xd4) 
    { 
	/* 4����Ʈ KS8 �ڵ带 H3 �ڵ��� �ڼҰ����� ��ȯ */
	for (ch = NoCON-1; ch > 0; ch--) 
	{
	    if (CON [ch] [3] == 0xa400 + ((wc>>16) & 0xff)) break;
	}
	for (ju = NoVOW-1; ju > 0; ju--)
	{
	    if (VOW[ ju] [2] == 0xa400 + ((wc>>8) & 0xff)) break;
	}
	for (jo = NoCON-1; jo > 0; jo--)
	{
	    if (CON [jo] [3] == 0xa400 + (wc & 0xff)) break;
	}
    }

    /* H3 �ڵ� ���� */
    return 0x8f000000L + ((long) CON [ch] [0] << 16) + (VOW [ju] [0] << 8) + CON [jo] [0];
}

/* 4����Ʈ TG �ڵ带 4����Ʈ KS �ڵ�� ��ȯ */
word word_tg2ks (word wc)
{
    /* TG: high, low byte, short word */
    byte high, low;
    word swc;

    /* temporary byte */
    byte temp;

    /* KS Table index */
    int  index;

    /* pointer to a item in KS Table */
    word *p;

    high = (byte)(wc >> 8);
    low = (byte)wc;
    swc = (word)wc;

    /* �ѱ� ��ȯ */
    if ((p = (word *)bsearch(&swc, KSTable, 2350, sizeof(word), codecmp)) != NULL)
    {
	index = (word) (p - KSTable);
	return (0x8ffb0000L + (((index / 94)+0xb0)<<8)+(index % 94) + 0xa1);
    }

    /* Ư�� ���� ��ȯ1 */
    else if (high >= 0xd9 && high <= 0xde) 
    {
	if (low < 0x30 || (low > 0x7e && low < 0x91)) return (0xff000000L);
	if (low >= 0xa1) temp = (high - 0xd9) * 2 + 0xa1 + 1;
	else 
	{
	    temp = (high - 0xd9) * 2 + 0xa1;
	    if (low <= 0x7e) low += 0x70;
	    else low += (0x70 - 18);
	}
	return (0x8ffb0000L + (temp << 8) + low);
    }

    /* Ư�� ���� ��ȯ2 */
    else if (high == 0xd4) 
    {
	if (low < 0x80) return (0xff000000L);
	if (low <= 0xdd) 
	{
	    temp = 0xad;
	    low += 0x21;
	}
	else 
	{
	    temp = 0xae;
	    low -= 0x3d;
	}
	return (0x8ffb0000L + (temp << 8) + low);
    }

    /* ���� ��ȯ */
    else if (high >= 0xe0 && high <= 0xf9) 
    {
	if (low < 0x30 || (low > 0x7e && low < 0x91)) return (0xff000000L);
	if (low >= 0xa1) temp = (high - 0xe0) * 2 + 0xca + 1;
	else 
	{
	    temp = (high - 0xe0) * 2 + 0xca;
	    if (low <= 0x7e) low += 0x70;
	    else low += (0x70 - 18);
	}
	return (0x8ffb0000L + (temp << 8) + low);
    }

    /* �ѱ� ���� ��ȯ */
    for (index = 0; index < SINGLENUM; index++)
    {
	if (SingleTable[index] == swc) 
	    return (0x8ffb0000L + 0xa4a1 + index);
    }

    return (0xff000000L);
}
