#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* About code systems used here:
    KS 코드 : KS 완성형 5601-1987 코드
    TG 코드 : TriGem 조합형 코드
    H3 코드 : 4바이트 Han3 코드
		8f ??(초성) ??(중성) ??(종성)
    K8 코드 : 완성형 낱글자를 이용하여 한글을 표현한 8바이트 KS-8 코드 
		a4d4 a4??(초성) a4??(중성) a4??(종성) */

#define TG_CODE 1
#define KS_CODE 0

#define SINGLENUM   51  //  한글 낱자의 수 
#define SIZE_EXTKS  (sizeof(word)*8822)
#define SIZE_KS     (sizeof(word)*2350)

typedef unsigned char	byte;
typedef unsigned int	word;
typedef unsigned short	sword;

typedef struct  {   //  한글 코드 형태
    unsigned lasc :5;
    unsigned midc :5;
    unsigned firc :5;
    unsigned dummy:1;
}   hcode;

#include "hcc98_codetable.h"

word KSSM2KS(unsigned code);      //  조합 -> 완성
word KS2KSSM(unsigned code);      //  완성 -> 조합
word KSSM2ExtKS(unsigned code);   //  조합 -> 확장완성
word ExtKS2KSSM(unsigned code);   //  확장완성 -> 조합

word get_ks (byte ch);
word get_tg (byte ch);
void CodeConv (word wc , char *out , short outCode);
word convKS (word wc);
word word_2h3 (word wc);
word word_tg2ks (word wc);

/* KS 코드 문자열을 TG 코드 문자열로 변환한다.
    ks : KS 문자열 
    tg : TriGem 문자열 */

//#include "hcc98_codetable.h"

word KSSM2KS(unsigned code);      //  조합 -> 완성
word KS2KSSM(unsigned code);      //  완성 -> 조합
word KSSM2ExtKS(unsigned code);   //  조합 -> 확장완성
word ExtKS2KSSM(unsigned code);   //  확장완성 -> 조합

word get_ks (byte ch);
word get_tg (byte ch);
void CodeConv (word wc , char *out , short outCode);
word convKS (word wc);
word word_2h3 (word wc);
word word_tg2ks (word wc);

FILE *outfile;

/* KS 코드 문자열을 TG 코드 문자열로 변환한다.
    ks : KS 문자열 
    tg : TriGem 문자열 */
void ks2tg (byte *ks , byte *tg)
{    
	/* 문자 */
    byte ch;		

    /* 4바이트 코드 문자 */
    word wc = 0;	

    /* 변환된 코드 저장소 */
    char temp [10];
    
    *tg = (byte)NULL;

	while (ch = *ks++) 
    {
        wc = get_ks (ch);

        /* wc가 0이면 KS 코드가 완성되지 않았음을 나타냄 */
		if (wc != 0) 
		{
			/* wc에 KS 코드가 저장되어 있으므로, TG 코드로 변환 후 문자열에 첨가 */
			CodeConv (wc , temp , TG_CODE);
			strcat ((char *)tg , (char *)temp);
			wc = 0;
		}
    }
}

/* TG 코드 문자열을 KS 코드 문자열로 변환한다.
    tg : TriGem 문자열
    ks : KS 문자열 */
void tg2ks (byte *tg , byte *ks)
{
    /* 문자 */
    byte ch;		

    /* 4바이트 코드 문자 */
    word wc = 0;	

    /* 변환된 코드 저장소 */
    char temp [10];

    *ks = (byte)NULL;

    while (ch = *tg++) 
    {
        wc = get_tg (ch);
        
        /* wc가 0이면 TG 코드가 완성되지 않았음을 나타냄 */
		if (wc != 0) 
		{
			/* wc에 TG 코드가 저장되어 있으므로, KS 코드로 변환 문자열에 첨가 */
			CodeConv (wc , temp , KS_CODE);
			strcat ((char *)ks , (char *)temp);
			wc = 0;
		}
    }	
}

/* KS 코드 입력 스트림을 TG 코드 출력 스트림으로 변환한다.
    ks_in : KS 코드 입력 스트림 
    tg_out : TG 코드 출력 스트림 */
void stream_ks2tg (FILE *ks_in , FILE *tg_out)
{
    /* 문자 */
    byte ch;		

    /* 4바이트 코드 문자 */
    word wc = 0;	

    /* 변환된 코드 저장소 */
    char temp [10];
    
    while ((ch = getc (ks_in)) != (byte)EOF)
    {
        wc = get_ks (ch);

        /* wc가 0이면 KS 코드가 완성되지 않았음을 나타냄 */
	if (wc != 0) 
	{
	    /* wc에 KS 코드가 저장되어 있으므로, TG 코드로 변환 후 출력 */
	    CodeConv (wc , temp , TG_CODE);
	    fprintf (tg_out, "%s", temp);
	    wc = 0;
	}
    }
}

/* TG 코드 입력 스트림을 KS 코드 출력 스트림으로 변환한다.
    tg_in : TG 코드 입력 스트림 
    ks_out : KS 코드 출력 스트림 */
void stream_tg2ks (FILE *tg_in , FILE *ks_out)
{
    /* 문자 */
    byte ch;		

    /* 4바이트 코드 문자 */
    word wc = 0;	

    /* 변환된 코드 저장소 */
    char temp [10];

    while ((ch = getc (tg_in)) != (byte)EOF)
    {
	wc = get_tg (ch);

        /* wc가 0이면 TG 코드가 완성되지 않았음을 나타냄 */
	if (wc != 0) 
	{
	    /* wc에 TG 코드가 저장되어 있으므로, KS 코드로 변환 후 출력 */
	    CodeConv (wc , temp , KS_CODE);
	    fprintf (ks_out, "%s", temp);
	    wc = 0;
	}
    }
}

/* 4바이트 KS 코드 또는 Han3 코드 생성.
    완성되지 않으면 0을 반환.
    완성되면 완성된 코드를 반환. */
word get_ks (byte ch)
{
    //첫번째 바이트가 a4인지를 나타냄
	static short flag=0;
	
	/* 2바이트 한글 코드의 첫번째 바이트 */
    static word first = 0;    

	/* Han3 코드를 만들기 위한 중간 결과 */
	static word all = 0;
        
    /* KS8 코드에서 낱글자 a4를 읽었는지를 나타내는 flag */
    static word a4 = 0;
	
	/* 4바이트 KS 또는 Han3 코드 */
    word tmp;		       

    /* 문자값이 0x80보다 작으면 ... */
    if (ch < 0x80) 
    {				
		/* ASCII : first == 0, ch 반환 */
		first = 0;
		return ch;
    }

    /* 문자값이 0x80보다 크거나 같으면 ... */
    /* first가 0이면 ... */
    if (first == 0) 
    {				
		/* KS 코드의 첫 바이트: first == ch, 0 반환 */
		first = ch;
		return 0;
    } 
    /* first가 0xa4이면 ... */
    else if (first == 0xa4) 
    {
		/* 문자값이 0xd4이면 */
		if (ch == 0xd4) 
		{			
			/* KS8 코드의 첫 두 바이트 (FILL): first == 0xd4, 0 반환*/
			first = 0xd4;
			
			//필코드가 시작되었음을 나타냄
			flag = 1;			
			return 0;
 		} 
		/* 문자값이 0xd4가 아니면 */
		else 
		{			
			/* KS코드의 각 자모 (Separate Jamo):  first == 0, 코드 반환 */
			first = 0;
			return (0x8ffba400L) + ch;
		}
    } 
    /* first가 0x8f이거나 0xd4이면 ... */
    else if (first==0xd4&&flag) 
    {				
		/* KS8 */	
		/* a4가 1이면 ... */
		if (a4==1) 
		{
			/* KS8 코드의 네번째 바이트:
			all에 문자값을 추가, a4 == 0, first == 0x8f */
			all = (all << 8) + ch;
			a4 = 0;
		} 

		/* a4가 1이 아니면 ...
			즉, first가 0xd4이면 */
		/* 문자값이 0xa4이면 ... */
		else if (ch == 0xa4) 
		{
			/* KS8 코드의 세번째 바이트: a4 == 1, first == 0xd4 */
			a4 = 1;flag=0;
		}

		/* all이 0xffff보다 작거나 같으면 ... */
		if (all <= 0xffff) 
		{
			/* Han3 또는 KS8이 완료되지 않으므로 0을 반환 */
			return 0;
		}

		/* all이 0xffff보다 크면 ... */
		else 
		{
			/* Han3 또는 KS8이 완료되므로 
			all == 0, first == 0, 코드를 완성한 후 반환 */
			tmp = (first<<24) + all;
			all = 0;
			first = 0;
			flag = 0;
			return tmp;
		}
    }     
    else 
    {
		/* 일반적인 KS 코드: 코드완성 후 반환, first == 0 */
		tmp = (0x8ffb0000L) + (first<<8) + ch;
		first = 0;
		return tmp;
    }
}

/* 4바이트 TG 코드 생성.
    완성되지 않으면 0을 반환.
    완성되면 완성된 코드를 반환. */
word get_tg (byte ch)
{
    /* 조합형 첫번째 바이트 */
    static word first = 0; 

    /* 4바이트 TG 코드 */
    word tmp;

    /* first가 0이고 ... */
    if (first == 0) 
    {
		/* 문자값이 0x80보다 작으면 ... */
		if (ch < 0x80) 
		{
			/* ASC: 문자값 반환 */
			return ch;
		}
		/* 문자값이 0x80보다 크거나 같으면 ... */
		/* TG 첫 바이트: first == ch, 0 반환 */
		first = ch;
		return 0;
    }
    
    /* first가 0이 아니면 ... */
    /* TG 두번째 바이트: first == 0, 조합형 코드를 완성하고 반환 */
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

/* 4바이트 코드 문자를 출력 코드 형태로 문자열에 저장.
   wc : 4바이트 코드 문자
   out : 출력 문자열
   outCode : 출력 코드 형태 */
void CodeConv (word wc , char *out , short outCode)
{
	if (wc>>8 == 0) 
    {
		/* 아스키 코드를 출력 문자열에 저장 */
		*out++ = (char) wc;
		*out = (char)NULL;
		return;
    }		
    
	/* 출력 코드가 TG code이면 코드 변환 함수로 convTG 코드 변환 함수 설정 */
    if (outCode == TG_CODE) wc = ExtKS2KSSM(wc);    
    /* 출력 코드가 TG code가 아니면 코드 변환 함수로 convKS 코드 변환 함수 설정 */
    else wc = KSSM2ExtKS(wc);

    if (wc>>16 == 0 ||  wc>>16 == 0x8ffb) 
    { 
		/* wc>>16이 0이면 TG 코드, 0x8ffb이면 KS 코드로 출력 문자열에 저장 */
		*out++ = (char) (wc>>8) & 0xff; /* 첫번째 바이트 */
		*out++ = (char) wc & 0xff; /* 두번째 바이트 */
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

  /* 변환된 KS 코드 */
  word check;
  /* TG 코드 */
  word tg;	    
  /* 초성 */
  word ch;	    
  /* 중성 */
  word ju;
  /* 종성 */
  word jo;  
  
  newCode = KSSM2KS(Code);
  if (newCode == 0) {
	if ((p = (word *)bsearch(&Code, ExtKSTable,
		   SIZE_EXTKS / sizeof(word),
		   sizeof(word), codecmp)) != NULL)
	  index = p - ExtKSTable;
	else 
	{
		if (Code>>16 == 0) tg = Code;
		else {
			if (Code>>24 != 0x8f) Code = word_2h3 (Code);

			/* H3 코드의 자소값을 구해서 TG 코드로 변환 */
			ch = CON [ ((Code>>16) & 0xff) - 0xa1 ] [1]; 
			ju = VOW [ ((Code>>8) & 0xff) - 0xa1 ] [1];
			jo = CON [ (Code&0xff) - 0xa1 ] [2];

			/* tg 변수에 TG 코드 설정 */
			tg = (ch & 0xfc00) + (ju & 0x03e0) + (jo & 0x1f);
		}

		/* TG 코드를 KS 코드로 변환 */
    
		/* KS 코드에 대응 문자가 있는 경우, KS 코드 반환 */
		if ((check = word_tg2ks (tg)) >> 16 == 0x8ffb) return check;
    
		/* KS 코드에 대응 문자가 없는 경우, KS8 코드로 변환 */		

		/* TG 코드를 H3 코드로 변환 */
		if (Code>>16 == 0) Code = word_2h3 (Code);

		/* H3 코드를 K8 코드로 변환하기 위한 자소값 분리 */
		ch = CON [ ((Code>>16) & 0xff) - 0xa1 ] [3]; 
		ju = VOW [ ((Code>>8) & 0xff) - 0xa1 ] [2];
		jo = CON [ (Code & 0xff) - 0xa1 ] [3];
    
		/* K8 코드로 변환 */
		if (ch < 0x100 || ju < 0x100 || jo < 0x100) 
		{
			return 0;
		}

		/* K8 코드 반환 */
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

  //  한글 변환
  if ((p = (word *)bsearch(&code, KSTable, 2350,
         sizeof(word), codecmp)) != NULL)
  {
    index = (int)(p - KSTable);
    return((((index / 94)+0xb0)<<8)+(index % 94) + 0xa1);
  }
  //  특수 문자 변환1
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
  //  특수 문자 변환2
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
  //  한자 변환
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

  //  한글 변환
  if (high >= 0xb0 && high <= 0xc8) {
    if (low < 0xa1 || low > 0xfe) return(0);
    index = (high - 0xb0) * 94 + low - 0xa1;
    return(KSTable[index]);
  }
  //  특수 문자 변환1
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
  //  특수 문자 변환2
  else if (high == 0xad) {
    if (low < 0xa1 || low > 0xfe) return  0;
    temp = low - 0x21;
    return((0xd4 << 8) + temp);
  }
  //  특수 문자 변환3
  else if (high == 0xae) {
    if (low < 0xa1 || low > 0xc1) return  0;
    temp = low + 0x3d;
    return((0xd4 << 8) + temp);
  }
  //  한글 변환
/*  else if (high >= 0xb0 && high <= 0xc8) {
    if (low < 0xa1 || low > 0xfe) return(0);
    index = (high - 0xb0) * 94 + low - 0xa1;
    return(KSTable[index]);
  }
  */
  //  한자 변환
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

/* 4바이트 KS 코드로 변환 */
word convKS (word wc)
{
    /* 변환된 KS 코드 */
    word check;

    /* TG 코드 */
    word tg;	
    
    /* 초성 */
    word ch;	
    
    /* 중성 */
    word ju;

    /* 종성 */
    word jo;

	word temp;
    
    /* KS : 현재 코드가 KS 코드이므로 변환하지 않음 */
    if (wc>>16 == 0x8ffb) return wc;
    
    /* TG 코드를 tg 변수에 설정 */
    if (wc>>16 == 0) tg = wc;
    
    else 
    {
	/* K8 코드를 H3 코드로 변환 */
	if (wc>>24 != 0x8f) wc = word_2h3 (wc);

	/* H3 코드의 자소값을 구해서 TG 코드로 변환 */
	ch = CON [ ((wc>>16) & 0xff) - 0xa1 ] [1]; 
	ju = VOW [ ((wc>>8) & 0xff) - 0xa1 ] [1];
	jo = CON [ (wc&0xff) - 0xa1 ] [2];

	/* tg 변수에 TG 코드 설정 */
	tg = (ch & 0xfc00) + (ju & 0x03e0) + (jo & 0x1f);
    }

    /* TG 코드를 KS 코드로 변환 */
    
    /* KS 코드에 대응 문자가 있는 경우, KS 코드 반환 */
    if ((check = word_tg2ks (tg)) >> 16 == 0x8ffb) return check;
    
    /* KS 코드에 대응 문자가 없는 경우, KS8 코드로 변환 */
	temp = wc;

    /* TG 코드를 H3 코드로 변환 */
    if (wc>>16 == 0) wc = word_2h3 (wc);

    /* H3 코드를 K8 코드로 변환하기 위한 자소값 분리 */
    ch = CON [ ((wc>>16) & 0xff) - 0xa1 ] [3]; 
    ju = VOW [ ((wc>>8) & 0xff) - 0xa1 ] [2];
    jo = CON [ (wc & 0xff) - 0xa1 ] [3];
    
    /* K8 코드로 변환 */
    if (ch < 0x100 || ju < 0x100 || jo < 0x100) 
    {
		return KSSM2ExtKS(temp);
    }

    /* K8 코드 반환 */
    return (0xd4L << 24) + ((ch & 0xff) << 16) + ((ju & 0xff) << 8) +(jo & 0xff);
}

/* 4바이트 TG 또는 K8 코드를 4바이트 H3 코드로 변환 */
word word_2h3 (word wc)
{
    /* 초성 */
    unsigned short ch=0;		

    /* 중성 */
    unsigned short ju=0;		

    /* 종성 */
    unsigned short jo=0;

    if (wc>>16 == 0) 
    {	
	/* 4바이트 TG 코드를 H3 코드의 자소값으로 변환 */
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
	/* 4바이트 KS8 코드를 H3 코드의 자소값으로 변환 */
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

    /* H3 코드 생성 */
    return 0x8f000000L + ((long) CON [ch] [0] << 16) + (VOW [ju] [0] << 8) + CON [jo] [0];
}

/* 4바이트 TG 코드를 4바이트 KS 코드로 변환 */
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

    /* 한글 변환 */
    if ((p = (word *)bsearch(&swc, KSTable, 2350, sizeof(word), codecmp)) != NULL)
    {
	index = p - KSTable;
	return (0x8ffb0000L + (((index / 94)+0xb0)<<8)+(index % 94) + 0xa1);
    }

    /* 특수 문자 변환1 */
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

    /* 특수 문자 변환2 */
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

    /* 한자 변환 */
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

    /* 한글 낱자 변환 */
    for (index = 0; index < SINGLENUM; index++)
    {
	if (SingleTable[index] == swc) 
	    return (0x8ffb0000L + 0xa4a1 + index);
    }

    return (0xff000000L);
}

