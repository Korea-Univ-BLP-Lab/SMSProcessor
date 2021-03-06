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

//-------------鋼差姥 薦暗 乞汲:RemoveRep()--------------------------------------------------------------
#define Remain 3	// 護 鯵 害移 兜聖 依昔走.
#define MaxRepeatLength	6	// 3鯵 戚馬稽 鋼差鞠澗 姥庚聖 薦暗廃陥.
void RemoveRep( const HString & input , char * output)
{
	HString result;

#ifdef whereis
puts("Rs");
#endif

	// 廃 越切税 庚舌伸戚 室 腰 戚雌 鋼差 吉陥檎 室 腰幻 害移客
	// 脊径 : 馬戚 せせせせせせせ 設走鎧?
	// 窒径 : 馬戚 せせせ 設走鎧?
	result += input.Left(Remain);
	for( unsigned i=Remain; i<input.GetNum(); i++ )
	if( input.GetNth( i ) != input.GetNth( i-1 ) ||
		input.GetNth( i ) != input.GetNth( i-2 ) ||
		input.GetNth( i ) != input.GetNth( i-3 ) )
	result += input.GetNth( i );			
		
	// 砧 越切 戚雌税 庚切伸戚 砧 腰 戚雌 鋼差鞠澗 依聖 薦暗
	// 脊径 : "戚薦 郊戚郊戚郊戚醤
	// 窒径 : 戚薦 郊戚醤
	// 戚研 是背 脊径聖 室 採歳生稽 蟹堪
	// 脊径 : "戚薦 " + "郊戚" + "郊戚郊戚醤"
	// pre : 戚薦
	// str1 : 郊戚
	// str2 : 郊戚郊戚醤

	// 獣拙 是帖亜 及稽 戚疑敗
	for( unsigned j=0; j<result.GetNum(); j++ )	{//獣拙

		// 獣拙 是帖 戚穿 舛左研 煽舌
		HString pre = result.Left( j );

		// str1聖 掩戚紺稽 持失
		HString str1 = result.GetNth(j);
		for( unsigned len=1; len <= MaxRepeatLength ; len++ )	
		{
			str1 += result.GetNth(j+len );			

			HString str2 = result.Mid( j+len+1 );

			// str2拭辞 str1戚 坦製拭 是帖廃陥檎 肢薦
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

//----------製社 繕杯 乞汲:combinePhoneme()-----------------------------------------------------------------------------------
//char phonemeJ[]="ぁ あ ぃ い ぅ う ぇ え ぉ お か が き ぎ く ぐ け げ こ ご さ ざ し じ す ず せ ぜ そ ぞ";
int firstP[] =   { 2, 3, 0, 4, 0, 0, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 8, 9, 10,0, 11,12,13,14,15,16,17,18,19,20};
int thirdP[] =   { 2, 3, 4, 5, 6, 7, 8, 0, 9, 10,11,12,13,14,15,16,17,19,0, 20,21,22,23,24,0, 25,26,27,28,29};

//char phonemeM[]="た だ ち ぢ っ つ づ て で と ど な に ぬ ね の は ば ぱ ひ び";
int secondP[] =  { 3, 4, 5, 6, 7, 10,11,12,13,14,15,18,19,20,21,22,23,26,27,28,29};

inline byte ChosungCode(byte t)	// 廃越 戚乞銅嬬 及 1byte研 郊伝生稽 繕杯莫 段失 坪球稽 郊菓陥. 段失戚 焼艦檎 0聖 軒渡廃陥.
{
	return (t>=HanPhonemeLocJ && t<HanPhonemeLocM) ? firstP[t-HanPhonemeLocJ] : 0;
}
inline byte JungsungCode(byte t)	// 廃越 戚乞銅嬬 及 1byte研 郊伝生稽 繕杯莫 掻失 坪球稽 郊菓陥. 掻失戚 焼艦檎 0聖 軒渡廃陥.
{
	return (t>=HanPhonemeLocM && t<HanPhonemeLocM+21) ? secondP[t-HanPhonemeLocM] : 0;
}
inline byte JongsungCode(byte t)	// 廃越 戚乞銅嬬 及 1byte研 郊伝生稽 繕杯莫 曽失 坪球稽 郊菓陥. 曽失戚 焼艦檎 0聖 軒渡廃陥.
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
	int state = 0;	// {0: 段失 蒸製 , 1:段失 , 2:段失,掻失 , 3: 段失,掻失,曽失 }
	int i,blanks=0;

	AString result;

#ifdef whereis
puts("Cs");
#endif

	if(!HaveHanPhoneme( input.GetStr() ))	// 製社 蒸生檎 益企稽 窒径.
	{
		strcpy(output,input.GetStr());
		return;
	}

	for(i=0;i<(int)input.GetLen();i++)
	if(!IsHanPhoneme(input[i]))
	{
		if(input[i]==' ')	// 製社 紫戚 因拷 肢薦.
		{
			if(state==0)
				result+=' ';
			else
				blanks++;
		}
		else	// 快医生檎
		{
			switch(state)
			{
			case 1:	// 段失猿走 条壱 快医生檎
				result+=(char)HanPhonemeLoc;
				result+=ch1;
				break;
			case 2:	// 掻失猿走 条壱 快医生檎
				result+=makeHan(code1,code2);
				break;
			case 3:	// 曽失猿走 条壱 快医生檎
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
					if(code3==1)	// 閤徴 蒸澗 廃越戚檎
					{
						code1=code1>>10;	// 切製 乞製 脅嬢 煽舌.
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
			if(code1)	// 段失戚檎,
			{
				state = 1;
				blanks = 0;
			}
			else	// 焼艦檎,
			{
				result+=(char)HanPhonemeLoc;
				result+=ch1;
			}
			break;
		case 1:
			ch2=(unsigned char)input[++i];
			code2=JungsungCode(ch2);
			if(code2)	// 掻失戚檎,
			{
				state = 2;
				blanks = 0;
			}
			else	// 焼艦檎,
			{
				result+=(char)HanPhonemeLoc;
				result+=ch1;
				if(blanks)result+=' ';

				i-=2;	// 段失昔走 仙溌昔.
				state=0;
			}
			break;
		case 2:
			ch3=(unsigned char)input[++i];
			code3=JongsungCode(ch3);
			if(code3)	// 曽失戚檎,
			{
				state = 3;
				blanks = 0;
			}
			else	// 焼艦檎
			{
				result+=makeHan(code1,code2);
				if(blanks)result+=' ';

				i-=2;	// 段失昔走 仙溌昔
				state=0;
			}
			break;
		case 3:
			ch4=(unsigned char)input[++i];
			code4=JungsungCode(ch4);
			if(code4)	// 掻失戚檎,
			{
				result+=makeHan(code1,code2);
				ch1=ch3;
				ch2=ch4;
				code1=ChosungCode(ch3);
				code2=code4;
				state=2;
				blanks = 0;
			}
			else	// 焼艦檎
			{
				result+=makeHan(code1,code2,code3);
				if(blanks)result+=' ';
		
				i-=2;	// 段失昔走 仙溌昔
				state=0;
			}
			break;
		}
	}

	switch(state)
	{
	case 1:	//段失猿走 条壱 快医生檎
		result+=(char)HanPhonemeLoc;
		result+=ch1;
		break;
	case 2:	//掻失猿走 条壱 快医生檎
		result+=makeHan(code1,code2);
		break;
	case 3:	//曽失猿走 条壱 快医生檎
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

//------------置舌析帖 乞汲:LongestMatchHash()-------------------------------------------------------------------------------------------
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
		
		// 蒋 庚呼 蓄窒
		p = temp;
		while(*p!='\t')
			p++;
		*p='\0';
		previous = temp;
		pNum = previous.GetNum();

		// 嘘舛 庚切伸 蓄窒
		remember=++p;
		while(*p!='\t')
			p++;
		*p='\0';
		right = remember;
		rNum = right.GetNum();

		// 及庚呼 蓄窒
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

//				for(k=0,countBlank=0;((int)k)<last;k++)	//嬢箭 室奄
//					if( source[k][0]==' ')countBlank++;

/*				if(hash==3)		// 戚乞銅嬬税 井酔, 及拭 奄硲亀 匂敗獣轍陥. (2007.2.11 痕)
			{
				for(k=last+1;k<(int)source.GetNum()-1;k++)
*/		// 左嫌--- 庚呼 蒸澗 戚乞銅嬬 慎狽 溌昔.					
				
		}
		else if(i>=last)result+=source[i];
	}
	
	// 蒋楕 $ 薦暗
	for(k=0;k<result.GetLen();k++)
	if(result.GetAt(k)!='$')
		break;
	result=result.Mid(k);
	
	// 及楕 $ 薦暗
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
		
//---------------------------奄展 乞汲----------------------------------------------------------
void RemoveHanPhoneme(char *str)
{
	char temp[MAX_STR_LEN];
	int i=0,j=0,lastHanFlag=0;

	for(i=0,j=0;str[i]!='\0';i++)
	{
		// 廃越戚檎,
		if(IsHan(str[i]))
		{
			//煽舌馬壱 妊獣.
			temp[j++]=str[i++];
			temp[j++]=str[i];
			lastHanFlag = 1;
		}
		else
		{
			// 製社戚壱 蒋拭 廃越戚 赤醸生檎,
			if(IsHanPhoneme(str[i]) && lastHanFlag)
			{
				int k=0;
				// 酔識 切社級聖 乞砧 差紫背 兜壱,
				for(;IsHanPhoneme(str[i+k]);k+=2)
				{	
					temp[j+k]	= str[i+k];
					temp[j+k+1]	= str[i+k+1];
				}
				i+=k;
					
				// 及亜 廃越戚檎,
				if(IsHan(str[i]))
				{
					// 差紫廃 依聖 巷獣馬壱 廃越幻 歯稽 差紫. 益軒壱 妊獣.
					temp[j++]=str[i++];
					temp[j++]=str[i];
					lastHanFlag = 1;
					continue;
				}
				// 及亜 廃越戚 焼艦檎,
				else
				{
					// 切社 差紫研 政走.
					j+=k;
				}
			}
			// 殿益澗 巷繕闇 差紫.
			if(str[i]=='[')
			{
				while(str[i]!=']')
					temp[j++]=str[i++];
				temp[j++]=str[i];
			}
			else
			// 益 須税 移酔澗 益企稽 差紫.
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

