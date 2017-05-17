#include <string.h>
#include "reviser.h"
#include "kulog_reviser.h"

//#define whereis
static bool IsHan(char ch)
{
	return (unsigned char)ch >= 0xb0 && (unsigned char)ch <= 0xc8;
}

static bool IsHanPhoneme(char ch)
{
	return (unsigned char)ch == HanPhonemeLoc;
}

//-------------반복구 제거 모듈:RemoveRep()--------------------------------------------------------------
#define Remain 3	// 몇 개 남겨 놓을 것인지.
#define MaxRepeatLength	6	// 3개 이하로 반복되는 구문을 제거한다.
void RemoveRep( const HString & input , char * output)
{
	HString result;

#ifdef whereis
puts("Rs");
#endif

	// 한 글자의 문장열이 세 번 이상 반복 된다면 세 번만 남겨둠
	// 입력 : 하이 ㅋㅋㅋㅋㅋㅋㅋ 잘지내?
	// 출력 : 하이 ㅋㅋㅋ 잘지내?
	result += input.Left(Remain);
	for( unsigned i=Remain; i<input.GetNum(); i++ )
	if( input.GetNth( i ) != input.GetNth( i-1 ) ||
		input.GetNth( i ) != input.GetNth( i-2 ) ||
		input.GetNth( i ) != input.GetNth( i-3 ) )
	result += input.GetNth( i );			
		
	// 두 글자 이상의 문자열이 두 번 이상 반복되는 것을 제거
	// 입력 : "이제 바이바이바이야
	// 출력 : 이제 바이야
	// 이를 위해 입력을 세 부분으로 나눔
	// 입력 : "이제 " + "바이" + "바이바이야"
	// pre : 이제
	// str1 : 바이
	// str2 : 바이바이야

	// 시작 위치가 뒤로 이동함
	for( unsigned j=0; j<result.GetNum(); j++ )	{//시작

		// 시작 위치 이전 정보를 저장
		HString pre = result.Left( j );

		// str1을 길이별로 생성
		HString str1 = result.GetNth(j);
		for( unsigned len=1; len <= MaxRepeatLength ; len++ )	
		{
			str1 += result.GetNth(j+len );			

			HString str2 = result.Mid( j+len+1 );

			// str2에서 str1이 처음에 위치한다면 삭제
			bool change = false;
			while( str2.Find( str1.GetStr() ) == 0 )	{
				str2=str2.Mid(str1.GetNum() );
				change = true;
				if( str2.IsEmpty() )	{
					break;					
				}
			}
			if( str2 == str1 )	{
				str2.Empty();
				change=true;
			}

			if( change == true )
				result = pre+str1+str2;
		}
	}
	
	strcpy(output,result.GetStr());

#ifdef whereis
puts("Re");
#endif

	return;
}

//----------음소 조합 모듈:combinePhoneme()-----------------------------------------------------------------------------------
//char phonemeJ[]="ㄱ ㄲ ㄳ ㄴ ㄵ ㄶ ㄷ ㄸ ㄹ ㄺ ㄻ ㄼ ㄽ ㄾ ㄿ ㅀ ㅁ ㅂ ㅃ ㅄ ㅅ ㅆ ㅇ ㅈ ㅉ ㅊ ㅋ ㅌ ㅍ ㅎ";
int firstP[] =   { 2, 3, 0, 4, 0, 0, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 8, 9, 10,0, 11,12,13,14,15,16,17,18,19,20};
int thirdP[] =   { 2, 3, 4, 5, 6, 7, 8, 0, 9, 10,11,12,13,14,15,16,17,19,0, 20,21,22,23,24,0, 25,26,27,28,29};

//char phonemeM[]="ㅏ ㅐ ㅑ ㅒ ㅓ ㅔ ㅕ ㅖ ㅗ ㅘ ㅙ ㅚ ㅛ ㅜ ㅝ ㅞ ㅟ ㅠ ㅡ ㅢ ㅣ";
int secondP[] =  { 3, 4, 5, 6, 7, 10,11,12,13,14,15,18,19,20,21,22,23,26,27,28,29};

inline byte ChosungCode(byte t)	// 한글 이모티콘 뒤 1byte를 바탕으로 조합형 초성 코드로 바꾼다. 초성이 아니면 0을 리턴한다.
{
	return (t>=HanPhonemeLocJ && t<HanPhonemeLocM) ? firstP[t-HanPhonemeLocJ] : 0;
}
inline byte JungsungCode(byte t)	// 한글 이모티콘 뒤 1byte를 바탕으로 조합형 중성 코드로 바꾼다. 중성이 아니면 0을 리턴한다.
{
	return (t>=HanPhonemeLocM && t<HanPhonemeLocM+21) ? secondP[t-HanPhonemeLocM] : 0;
}
inline byte JongsungCode(byte t)	// 한글 이모티콘 뒤 1byte를 바탕으로 조합형 종성 코드로 바꾼다. 종성이 아니면 0을 리턴한다.
{
	return (t>=HanPhonemeLocJ && t<HanPhonemeLocM) ? thirdP[t-HanPhonemeLocJ] : 0;
}

AString makeHan( const byte first, const byte second, const byte third )
{
	byte t[3];
	char s[3];

	t[0]=0x80+(first<<2)+(second>>3);
	t[1]=(second<<5)%256+third;
	t[2]='\0';
	tg2ks(t ,(byte *)s);

	return AString(s);
}

//#define OPT_ADD_JONGSUNG

void combinePhoneme( const AString & input , char * output)
{
	unsigned char ch1='\0',ch2='\0',ch3='\0',ch4='\0';
	char code1='\0',code2='\0',code3='\0',code4='\0';
	int state = 0;	// {0: 초성 없음 , 1:초성 , 2:초성,중성 , 3: 초성,중성,종성 }
	int i,blanks=0;

	AString result;

#ifdef whereis
puts("Cs");
#endif

	if(!HaveHanPhoneme( input.GetStr() ))	// 음소 없으면 그대로 출력.
	{
		strcpy(output,input.GetStr());
		return;
	}

	for(i=0;i<(int)input.GetLen();i++)
	if(!IsHanPhoneme(input[i]))
	{
		if(input[i]==' ')	// 음소 사이 공백 삭제.
		{
			if(state==0)
				result+=' ';
			else
				blanks++;
		}
		else	// 끊겼으면
		{
			switch(state)
			{
			case 1:	// 초성까지 얻고 끊겼으면
				result+=(char)HanPhonemeLoc;
				result+=ch1;
				break;
			case 2:	// 중성까지 얻고 끊겼으면
				result+=makeHan(code1,code2);
				break;
			case 3:	// 종성까지 얻고 끊겼으면
				result+=makeHan(code1,code2,code3);
				break;
			}

			if(blanks)result+=' ';
			
			state = 0;
			blanks = 0;
			if(Is2byteCharacter(input[i]))
			{
#ifdef	OPT_ADD_JONGSUNG
				if(IsHan(input[i]))
				{
					GetGrapheme((short*)&code1, (short*)&code2, (short*)&code3, (unsigned char*)input.Mid(i,2).GetStrP());
					if(code3==1)	// 받침 없는 한글이면
					{
						code1=code1>>10;	// 자음 모음 떼어 저장.
						code2=code2>>5;
						state=2;
					}
					else
						result+=input.Mid(i,2);
				}
				else
#endif
					result+=input.Mid(i,2);
				i++;
			}
			else				
				result+=input[i];
		}
	}
	else
	{
		switch(state)
		{
		case 0:
			ch1=(unsigned char)input[++i];				
			code1=ChosungCode(ch1);
			if(code1)	// 초성이면,
			{
				state = 1;
				blanks = 0;
			}
			else	// 아니면,
			{
				result+=(char)HanPhonemeLoc;
				result+=ch1;
			}
			break;
		case 1:
			ch2=(unsigned char)input[++i];
			code2=JungsungCode(ch2);
			if(code2)	// 중성이면,
			{
				state = 2;
				blanks = 0;
			}
			else	// 아니면,
			{
				result+=(char)HanPhonemeLoc;
				result+=ch1;
				if(blanks)result+=' ';

				i-=2;	// 초성인지 재확인.
				state=0;
			}
			break;
		case 2:
			ch3=(unsigned char)input[++i];
			code3=JongsungCode(ch3);
			if(code3)	// 종성이면,
			{
				state = 3;
				blanks = 0;
			}
			else	// 아니면
			{
				result+=makeHan(code1,code2);
				if(blanks)result+=' ';

				i-=2;	// 초성인지 재확인
				state=0;
			}
			break;
		case 3:
			ch4=(unsigned char)input[++i];
			code4=JungsungCode(ch4);
			if(code4)	// 중성이면,
			{
				result+=makeHan(code1,code2);
				ch1=ch3;
				ch2=ch4;
				code1=ChosungCode(ch3);
				code2=code4;
				state=2;
				blanks = 0;
			}
			else	// 아니면
			{
				result+=makeHan(code1,code2,code3);
				if(blanks)result+=' ';
		
				i-=2;	// 초성인지 재확인
				state=0;
			}
			break;
		}
	}

	switch(state)
	{
	case 1:	//초성까지 얻고 끊겼으면
		result+=(char)HanPhonemeLoc;
		result+=ch1;
		break;
	case 2:	//중성까지 얻고 끊겼으면
		result+=makeHan(code1,code2);
		break;
	case 3:	//종성까지 얻고 끊겼으면
		result+=makeHan(code1,code2,code3);
		break;
	}

#ifdef whereis
puts("Ce");
#endif
	strcpy(output,result.GetStr());
	
	return;
}

extern FST   *emoticon_fst;
extern char  **emoticon_info;
extern int   *emoticon_freq;

extern FST   *repeat_fst;
extern char  **repeat_info;
extern int   *repeat_freq;

extern FST   *insult_fst;
extern char  **insult_info;
extern int   *insult_freq;

extern FST   *speller_fst;
extern char  **speller_info;
extern int   *speller_freq;

//------------최장일치 모듈:LongestMatchHash()-------------------------------------------------------------------------------------------
int LongestMatchHash( const AString & str, HString & key, char * value, int o1, int o2, int o3, int o4 )
{
	int i=0,hash,remember=-1;
	int pMatchNumber;
	AString temp;
#ifdef whereis
puts("Ls");
#endif
	
	if(str.IsEmpty()) return -1;
	
	do
	{
		if(Is2byteCharacter(str[i]))
		{
			temp+=str.Mid(i,2);
			i+=2;
		}
		else
		{
			temp+=str[i];
			i++;
		}
		
		if(o1 && (hash=String2Hash( emoticon_fst, temp.GetStrP(), &pMatchNumber))>=0)
		{
			remember=1;
			key=_Hash2String( emoticon_fst, hash, value );
			strcpy ( value , emoticon_info[hash] );
		}			
		
		if(o2 && (hash=String2Hash( repeat_fst, temp.GetStrP(), &pMatchNumber))>=0)
		{
			remember=2;
			key=_Hash2String( repeat_fst, hash, value );
			strcpy ( value , repeat_info[hash] );
		}
		
		if(o3 && (hash=String2Hash( insult_fst, temp.GetStrP(), &pMatchNumber))>=0)
		{
			remember=3;
			key=_Hash2String( insult_fst, hash, value );
			strcpy ( value , insult_info[hash] );
		}
		
		if(o4 && (hash=String2Hash( speller_fst, temp.GetStrP(), &pMatchNumber))>=0)
		{
			remember=4;
			key=_Hash2String( speller_fst, hash, value );
			strcpy ( value , speller_info[hash] );
		}
			
	}while(str[i]!='\0');
#ifdef whereis
puts("Le");
#endif
	return remember;
}

#define A_DEBUG	

void ApplyRule( const HString & input , char * output, int o1, int o2, int o3, int o4)
{
	int i,last,hash;
	char temp[MAX_STR_LEN]="";
	char *p;
	char *remember;

	HString source,result;

	HString wrong,previous,right,next;
	int pNum,wNum,nNum,rNum;

#ifdef whereis
puts("As");
#endif

unsigned int k=0;

	source="$$$"+input+"$$$";

	for(i=0,last=-1;i<(int)source.GetNum()-1;i++)
	{
#ifdef A_DEBUG	
//REVISER_LOG_DBG(kulog_reviser,"a%s\n",source.Mid(i).GetStr());
#endif

		if( (hash=LongestMatchHash( source.Mid(i), wrong, temp, o1, o2, o3, o4 )) == -1 )
		{
			if(i>=last)
				result+=source[i];
			continue;
		}

#ifdef A_DEBUG	
//REVISER_LOG_DBG(kulog_reviser,"b%s\n",temp);
#endif
		
		// 앞 문맥 추출
		p = temp;
		while(*p!='\t')
			p++;
		*p='\0';
		previous = temp;
		pNum = previous.GetNum();

		// 교정 문자열 추출
		remember=++p;
		while(*p!='\t')
			p++;
		*p='\0';
		right = remember;
		rNum = right.GetNum();

		// 뒤문맥 추출
		remember=++p;
		while(*p!='\t' && *p!='\0')
			p++;
		*p='\0';
		next = remember;
		nNum = next.GetNum();

		wNum=wrong.GetNum()-pNum-nNum;

#ifdef A_DEBUG	
REVISER_LOG_DBG(kulog_reviser,"at last:%d\ti:%d\tp:%d\tw:%d\tn:%d\tr:%d\n",last,i,pNum,wNum,nNum,rNum);
REVISER_LOG_DBG(kulog_reviser,"w:%s\tp:%s\tr:%s\tn:%s\n",wrong.GetStr(),previous.GetStr(),right.GetStr(),next.GetStr());
#endif

		if(last<=i+pNum && last<i+pNum+wNum)
		{
			result+=previous.Right(i+pNum-last);
			result+=right;
			last=i+pNum+wNum;
#ifdef A_DEBUG	
REVISER_LOG_DBG(kulog_reviser,"result:%s\n",result.GetStr());
#endif
			if(i>=last)result+=source[i];

//				for(k=0,countBlank=0;((int)k)<last;k++)	//어절 세기
//					if( source[k][0]==' ')countBlank++;

/*				if(hash==3)		// 이모티콘의 경우, 뒤에 기호도 포함시킨다. (2007.2.11 변)
			{
				for(k=last+1;k<(int)source.GetNum()-1;k++)
*/		// 보류--- 문맥 없는 이모티콘 영향 확인.					
				
		}
		else if(i>=last)result+=source[i];
	}
	
	// 앞쪽 $ 제거
	for(k=0;k<result.GetLen();k++)
	if(result.GetAt(k)!='$')
		break;
	result=result.Mid(k);
	
	// 뒤쪽 $ 제거
	for(k=result.GetLen()-1;k>0;k--)
	if(result.GetAt(k)!='$')
		break;
	result=result.AString::Left(k+1);

	strcpy(output,result.GetStr());

#ifdef whereis
puts("Ae");
#endif

	return;
}	
		
//---------------------------기타 모듈----------------------------------------------------------
void RemoveHanPhoneme(char *str)
{
	char temp[MAX_STR_LEN];
	int i=0,j=0,lastHanFlag=0;

	for(i=0,j=0;str[i]!='\0';i++)
	{
		// 한글이면,
		if(IsHan(str[i]))
		{
			//저장하고 표시.
			temp[j++]=str[i++];
			temp[j++]=str[i];
			lastHanFlag = 1;
		}
		else
		{
			// 음소이고 앞에 한글이 있었으면,
			if(IsHanPhoneme(str[i]) && lastHanFlag)
			{
				int k=0;
				// 우선 자소들을 모두 복사해 놓고,
				for(;IsHanPhoneme(str[i+k]);k+=2)
				{	
					temp[j+k]	= str[i+k];
					temp[j+k+1]	= str[i+k+1];
				}
				i+=k;
					
				// 뒤가 한글이면,
				if(IsHan(str[i]))
				{
					// 복사한 것을 무시하고 한글만 새로 복사. 그리고 표시.
					temp[j++]=str[i++];
					temp[j++]=str[i];
					lastHanFlag = 1;
					continue;
				}
				// 뒤가 한글이 아니면,
				else
				{
					// 자소 복사를 유지.
					j+=k;
				}
			}
			// 태그는 무조건 복사.
			if(str[i]=='[')
			{
				while(str[i]!=']')
					temp[j++]=str[i++];
				temp[j++]=str[i];
			}
			else
			// 그 외의 겨우는 그대로 복사.
			{
				if(Is2byteCharacter(str[i]))
					temp[j++]=str[i++];
				temp[j++]=str[i];
			}
			lastHanFlag = 0;
		}
	}
	
	temp[j]='\0';
	strcpy(str,temp);
	
	return;
}

bool HaveHanPhoneme(const char *str)
{
	int length = strlen(str);
	for(int i=0;i<length;i++)
	if(Is2byteCharacter(str[i]))
		if(IsHanPhoneme(str[i]))
			return true;
		else
			i++;
	return false;
}

void printCode(unsigned char * str)
{
	short choseong,jungseong,jongseong;

	if(str[0]=='\0')return;
	GetGrapheme(&choseong, &jungseong, &jongseong, str);
	printf(" %s : %2d %2d %2d - 0x%2x %2x\n",str,choseong>>10, jungseong>>5, jongseong,*str,*(str+1));
	return;
}

