#include "kspacer/spacer_FST.h"
#include "reviser.h"
#include "GetFST.h"
#include "reviserAPI.h"

#include "kulog_reviser.h"
kulog_handler_t * kulog_reviser;

FST   *emoticon_fst;
char  **emoticon_info;
int   *emoticon_freq;

FST   *repeat_fst;
char  **repeat_info;
int   *repeat_freq;

FST   *insult_fst;
char  **insult_info;
int   *insult_freq;

FST   *speller_fst;
char  **speller_info;
int   *speller_freq;

FST   *morpheme_fst;
char  **morpheme_info;
int   *morpheme_freq;


PROB_MAP_FST ws_trns_prob; /* 전이 확률 */
PROB_MAP_FST ws_syl_prob; /* 어휘 확률 */

void reviser_formMsg( const char *orgmsg, const char *revmsg, Message * msg );
void reviser_convertKeyword( char *revmsg );
bool reviser_Have2ByteChar(const char *str);


int reviser_init( reviser_config_t * pStReviserConf, kulog_handler_t * slh )
{
	char filename_FST[256];
	char filename_info[256];
	char filename_freq[256];
			
	kulog_reviser = slh;	// log 핸들러 준비.

//	if (time(NULL) > 1172674799) { REVISER_LOG_ERR(kulog_reviser, "Error : Expired, http://nlp.korea.ac.kr\n"); return -1; }
				
	// Default 입력
	if(pStReviserConf->emoticon_dic == NULL || *(pStReviserConf->emoticon_dic) == '\0')
		strcpy( pStReviserConf->emoticon_dic, DEFAULT_EMOTICON_DIC);
	if(pStReviserConf->repeat_dic == NULL 	|| *(pStReviserConf->repeat_dic) == '\0')
		strcpy( pStReviserConf->repeat_dic 	, DEFAULT_REPEAT_DIC);
	if(pStReviserConf->insult_dic == NULL 	|| *(pStReviserConf->insult_dic) == '\0')
		strcpy( pStReviserConf->insult_dic 	, DEFAULT_INSULT_DIC );
	if(pStReviserConf->speller_dic == NULL 	|| *(pStReviserConf->speller_dic) == '\0')
		strcpy( pStReviserConf->speller_dic , DEFAULT_SPELLER_DIC);
	if(pStReviserConf->spacer_path == NULL 	|| *(pStReviserConf->spacer_path) == '\0')
		strcpy( pStReviserConf->spacer_path , DEFAULT_SPACER_PATH);
	if(pStReviserConf->morpheme_dic == NULL || *(pStReviserConf->morpheme_dic) == '\0')
		strcpy( pStReviserConf->morpheme_dic , DEFAULT_MORPHEME_DIC);


	if(pStReviserConf->emoticon)
	{
		sprintf( filename_FST,  "%s%s", pStReviserConf->emoticon_dic, ".FST"  );
		sprintf( filename_info, "%s%s", pStReviserConf->emoticon_dic, ".info" );
//		sprintf( filename_freq, "%s%s", pStReviserConf->emoticon_dic, ".freq" );
		
		if(!FST_open(filename_FST, filename_info, filename_freq, &emoticon_fst, &emoticon_info, &emoticon_freq))
		{
			REVISER_LOG_DBG(kulog_reviser, "ERROR :: cannot open emoticon FST file.\n");
			return -1;
		}
		else
			REVISER_LOG_SVC(kulog_reviser, "Emoticon_Dic Ready.\n" );
	}

	if(pStReviserConf->repeat)
	{
		sprintf( filename_FST,  "%s%s", pStReviserConf->repeat_dic, ".FST"  );
		sprintf( filename_info, "%s%s", pStReviserConf->repeat_dic, ".info" );
		sprintf( filename_freq, "%s%s", pStReviserConf->repeat_dic, ".freq" );
		
		if(!FST_open(filename_FST, filename_info, filename_freq, &repeat_fst, &repeat_info, &repeat_freq))
		{
			REVISER_LOG_ERR(kulog_reviser, "ERROR :: cannot open repeat FST file.\n");
			return -1;
		}
		else
			REVISER_LOG_SVC(kulog_reviser, "Repeat_Dic Ready.\n" );
	}

	if(pStReviserConf->insult)
	{
		sprintf( filename_FST,  "%s%s", pStReviserConf->insult_dic, ".FST"  );
		sprintf( filename_info, "%s%s", pStReviserConf->insult_dic, ".info" );
		sprintf( filename_freq, "%s%s", pStReviserConf->insult_dic, ".freq" );
		
		if(!FST_open(filename_FST, filename_info, filename_freq, &insult_fst, &insult_info, &insult_freq))
		{
			REVISER_LOG_ERR(kulog_reviser, "ERROR :: cannot open insult FST file.\n");
			return -1;
		}
		else
			REVISER_LOG_SVC(kulog_reviser, "Insult_Dic Ready.\n" );
	}

	if(pStReviserConf->speller)
	{
		sprintf( filename_FST,  "%s%s", pStReviserConf->speller_dic, ".FST"  );
		sprintf( filename_info, "%s%s", pStReviserConf->speller_dic, ".info" );
		sprintf( filename_freq, "%s%s", pStReviserConf->speller_dic, ".freq" );
		
		if(!FST_open(filename_FST, filename_info, filename_freq, &speller_fst, &speller_info, &speller_freq))
		{
			REVISER_LOG_ERR(kulog_reviser, "ERROR :: cannot open speller FST file.\n");
			return -1;
		}
		else
			REVISER_LOG_SVC(kulog_reviser, "Speller_Dic Ready.\n" );

	}

	if(pStReviserConf->spacer)
	{
		char tr_prefix[256],le_prefix[256];
		sprintf(tr_prefix, "%s/tr",pStReviserConf->spacer_path );
		sprintf(le_prefix, "%s/le",pStReviserConf->spacer_path );
		
		if ( !spacer_open_FST(tr_prefix, le_prefix, ws_trns_prob, ws_syl_prob) )
		{
			REVISER_LOG_ERR(kulog_reviser, "ERROR :: Spacer init error\n" );
			return -1; 
		}
		else
			REVISER_LOG_SVC(kulog_reviser, "Spacer Ready.\n" );
	}


//	if(pStReviserConf->morpheme)
	{
		sprintf( filename_FST,  "%s%s", pStReviserConf->morpheme_dic, ".FST"  );
		sprintf( filename_info, "%s%s", pStReviserConf->morpheme_dic, ".info" );
		sprintf( filename_freq, "%s%s", pStReviserConf->morpheme_dic, ".freq" );
		
		if(!FST_open(filename_FST, filename_info, filename_freq, &morpheme_fst, &morpheme_info, &morpheme_freq))
		{
			REVISER_LOG_ERR(kulog_reviser, "ERROR :: cannot open morpheme FST file.\n");
			return -1; 
		}
		else
			REVISER_LOG_SVC(kulog_reviser, "Morpheme_Dic Ready.\n" );
	}

	return 0;
}
	
int reviser_run( reviser_config_t * pStReviserConf, const char * input, Message * msg )
{
	char temp[MAX_MSG_LEN];
	char result[MAX_MSG_LEN];
	char raw[MAX_MSG_LEN];
	int i=0,pMatchNumber=0;

//	if (time(NULL) > 1172674799) { REVISER_LOG_ERR(kulog_reviser, "Error : Expired, http://nlp.korea.ac.kr\n"); return -1; }

	// 문장 끝 불필요한 \n, \t, 공백 제거.
	strcpy( result, input );
	for(i=strlen(result)-1;i>=0;i--)
	if(result[i]==' ' || result[i]=='\n' || result[i]=='\t')
		;
	else
		break;
	result[i+1]='\0';
	strcpy( raw, result );	
	
	REVISER_LOG_SVC(kulog_reviser, "0.Revising         :%s\n", result );

	// 반복 제거
	if(pStReviserConf->repeat)
	{
		strcpy( temp, result );
		RemoveRep( temp, result );
	}
	REVISER_LOG_SVC(kulog_reviser, "1.Removed repeat   :%s\n", result );

// 메세지 가운데 알파벳 부분은 빼고 띄어쓰기 실행
	int r=0,p=0,length=0,remember=0;
	char inp[MAX_MSG_LEN]="";
	char part[MAX_MSG_LEN]="";
	strcpy( inp, result );
	strcpy( result, part );

	length=strlen(input);
//REVISER_LOG_DBG(kulog_reviser,"l%d\ti%s\tp%s\tr%s\n",length,inp,part,result);

	for(i=0;i<length;i++)
	if( (inp[i]>='A' && inp[i]<='Z') || (inp[i]>='a' && inp[i]<='z'))
	{
		part[p]='\0';
	
		// 음소 조합
		if(pStReviserConf->phoneme)
		{
			strcpy( temp, part );
			combinePhoneme( temp, part );
		}
		REVISER_LOG_SVC(kulog_reviser, "2.Combined phoneme :%s\n", part );
	
		//형태소 사전에 있으면 그대로 출력
		if( String2Hash( morpheme_fst, part, &pMatchNumber) != -1 )
		{
			REVISER_LOG_SVC(kulog_reviser, "Morpheme detected  :%s\n", part );
		}
		else if(pStReviserConf->spacer) // 띄어쓰기 교정
		{	
			REVISER_LOG_SVC(kulog_reviser, "Space Start ", part );
			int a=0,b=0;
		
			//공백 제거
			for(;part[a]!='\0';a++)
				if(part[a]!=' ')
					temp[b++]=part[a];
			temp[b]='\0';

	
			//띄어쓰기 실행
			statistical_spacer_FST(ws_trns_prob, ws_syl_prob, temp, part);
		}
	
		strcat(result,part);
		r+=strlen(part);
		part[0]='\0';
		p=0;

		if(remember==' ')
			result[r++]=' ';

		while( (inp[i]>='A' && inp[i]<='Z') || (inp[i]>='a' && inp[i]<='z') || inp[i]==' ' )
			result[r++]=inp[i++];
		i--;
		
		result[r]='\0';
	}
	else
	{
		remember=inp[i];

		if(Is2byteCharacter(inp[i]))
			part[p++]=inp[i++];	
		part[p++]=inp[i];	
	}
	
	part[p]='\0';
	if(strlen(part)>0)
	{
		// 음소 조합
		if(pStReviserConf->phoneme)
		{
			strcpy( temp, part );
			combinePhoneme( temp, part );
		}
		REVISER_LOG_SVC(kulog_reviser, "2.Combined phoneme :%s\n", part );
	
		//형태소 사전에 있으면 그대로 출력
		if( String2Hash( morpheme_fst, part, &pMatchNumber) != -1 )
		{
			REVISER_LOG_SVC(kulog_reviser, "Morpheme detected  :%s\n", part );
		}
		else if(pStReviserConf->spacer) // 띄어쓰기 교정
		{	
			REVISER_LOG_SVC(kulog_reviser, "Space Start ", part );
			int a=0,b=0;
		
			//공백 제거
			for(;part[a]!='\0';a++)
				if(part[a]!=' ')
					temp[b++]=part[a];
			temp[b]='\0';

			//띄어쓰기 실행
			statistical_spacer_FST(ws_trns_prob, ws_syl_prob, temp, part);
		}
	}
	strcat(result,part);

	REVISER_LOG_SVC(kulog_reviser, "3.Spaced           :%s\n", result );

	// 키워드 변경 [:] => (;)
	reviser_convertKeyword( result );
	REVISER_LOG_SVC(kulog_reviser, "4.Converted        :%s\n", result );

	// 철자 교정
	strcpy( temp, result );
	ApplyRule( temp, result
	, pStReviserConf->emoticon, pStReviserConf->repeat
	, pStReviserConf->insult, pStReviserConf->speller );

	REVISER_LOG_SVC(kulog_reviser, "5.Spell revised    :%s\n", result );

	// 음절 사이 음소제거
	RemoveHanPhoneme(result);

	// 태그 추출
	reviser_formMsg( raw, result, msg );	
	REVISER_LOG_SVC(kulog_reviser, "6.Filtered         :%s\n", msg->fltstr );

	// 비어있으면 무의미
	if( msg->revstr[0]=='\n' )
		return -1;
	else
		return 0;
}	
	
int reviser_finish( reviser_config_t * pStReviserConf )
{
	REVISER_LOG_SVC(kulog_reviser, "Reviser finished.\n" );
	
	if(pStReviserConf->emoticon)
		FST_close(emoticon_fst, emoticon_info, emoticon_freq);

	if(pStReviserConf->repeat)
		FST_close(repeat_fst, repeat_info, repeat_freq);

	if(pStReviserConf->insult)
		FST_close(insult_fst, insult_info, insult_freq);

	if(pStReviserConf->speller)
		FST_close(speller_fst, speller_info, speller_freq);

//	if(pStReviserConf->morpheme)
		FST_close(morpheme_fst, morpheme_info, morpheme_freq);

	return 0;
}

//--------------------------------------------------------내부 함수------------------------
void reviser_formMsg( const char *orgmsg, const char *revmsg, Message * msg )
{
	char	temp[MAX_MSG_LEN];
	char	*type=NULL;
	int		i=0;
	int 	in=0,out=0,length=0;

	strcpy( msg->orgstr, orgmsg );
	strcpy( msg->revstr, revmsg );
	length=strlen(revmsg);
	memset(	msg->fltstr,'\0',sizeof(char)*MAX_MSG_LEN);


//for(i=0;i<7;i++)
REVISER_LOG_DBG(kulog_reviser,"r%s\n",msg->exp[JOY]);

	for ( in=0; (in<length)&&(revmsg[in]!='\0'); in++ )
	{
		// 의성어, 이모티콘은 따로 저장
		if ( revmsg[in]=='[' ) 
		{ 
			if ( !strncmp(&revmsg[in+1],"욕설",4) ) { type=msg->exp[INSULT]; } 
			else if ( !strncmp(&revmsg[in+1],"모름",4) ) { type=msg->exp[UNKNOWN]; } 
			else if ( !strncmp(&revmsg[in+1],"기쁨",4) ) { type=msg->exp[JOY]; } 
			else if ( !strncmp(&revmsg[in+1],"슬픔",4) ) { type=msg->exp[SAD]; } 
			else if ( !strncmp(&revmsg[in+1],"놀람",4) ) { type=msg->exp[SURPRISE]; } 
			else if ( !strncmp(&revmsg[in+1],"냉담",4) ) { type=msg->exp[COLD]; } 
			else if ( !strncmp(&revmsg[in+1],"사랑",4) ) { type=msg->exp[LOVE]; } 
			else if ( !strncmp(&revmsg[in+1],"기타",4) ) { type=msg->exp[OTHERS]; } 
			else 
			{
				msg->fltstr[out++]='['; 
				continue;
			}

			memset(temp,'\0',sizeof(char)*MAX_MSG_LEN);
			
			for ( i=0, in=in+6; revmsg[in]!=']' && revmsg[in]!=' ' && revmsg[in]!='\t'
												&& revmsg[in]!='\0' ; in++ )
			{
				temp[i++]=revmsg[in];

				//욕설은 두 곳에 모두 저장
				if(type==msg->exp[INSULT])
					msg->fltstr[out++]=revmsg[in]; 
			}
			temp[i]='\0';
			
			if ( type[0]!='\0' )
				strcat(type,"\t");
			strcat(type,temp);

			if( revmsg[in]!=']' )
				msg->fltstr[out++]=revmsg[in]; 

REVISER_LOG_DBG(kulog_reviser,"%d\tt%s\n",in,type);
		}
		// 여과본에 저장
		else
		{ 
			if ( (revmsg[in]==' ')&&((out==0)||(msg->fltstr[out-1]==' ')) ) { continue; }
			msg->fltstr[out++]=revmsg[in]; 
		}
	}
	msg->fltstr[out]='\0';
REVISER_LOG_DBG(kulog_reviser,"f%s\n",msg->fltstr);
	
	// 뒤쪽의 빈 칸이나 물결 제거.
	for ( out=strlen(msg->fltstr)-1; (msg->fltstr[out]==' ')||(msg->fltstr[out]=='~'); out-- ) 
	{ 
		if ( out<=0 ) { break; } 
		msg->fltstr[out]='\0'; 
	}
}

void reviser_convertKeyword( char *revmsg )
{
	// keyword변환: "[:] => (;)" 태그관련 기호 변환, "$ => %"(문자열 끝기호 변환)
	for ( int i=0; (i<MAX_MSG_LEN)&&(revmsg[i]!='\0'); i++ ) 
	{ 
		if (revmsg[i]=='[') { revmsg[i]='('; } 
		else if (revmsg[i]==']') { revmsg[i]=')'; } 
//		else if (revmsg[i]==':') { revmsg[i]=';'; } 
		else if (revmsg[i]=='$') { revmsg[i]='#'; } 
	}

	return;
//	if ( (i>0)&&(revmsg[i]=='\0') ) { revmsg[i]='$';  revmsg[i+1]='\0'; } 
}

bool reviser_Have2ByteChar(const char *str)
{
	int length = strlen(str);
// 11월 23일 박소영수정
//	for(unsigned int i=0;i<length;i++)
	for(int i=0;i<length;i++)
	if(Is2byteCharacter(str[i]))
		return true;
	return false;
}

bool reviser_HaveAlphabetr(const char *str)
{
	int length = strlen(str);
// 11월 23일 박소영수정
//	for(unsigned int i=0;i<length;i++)
	for(int i=0;i<length;i++)
	if( (str[i]>='A' && str[i]<='Z') || (str[i]>='a' && str[i]<='z'))
		return true;
	return false;
}
