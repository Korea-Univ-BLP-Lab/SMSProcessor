/*
 * reviser_test.cpp
 *
 * 전처리기 사용 방법을 보여주기 위한 파일
 * 
 * September. 2006
 * jhbyun@nlp.korea.ac.kr
 *
 */
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/reviserAPI.h"

void help();
void printMsg( FILE *outfile, Message msg );
void initmem( int flag, char orgMsg[MAX_MSG_LEN], Message *msg, char temp[MAX_MSG_LEN] );
int getOption( char * optFileName, reviser_config_t * pReviserConf, kulog_handler_t * );

#define 	OPTION_FILE_NAME "option.ini"
#define 	OPTION_MODULE_NUM	6
#define		OPTION_PATH_NUM		6

int main( int argc, char *argv[] )
{
	FILE	 *infile  = stdin;
	FILE	 *outfile = stdout;

	char    orgMsg[MAX_MSG_LEN];			// 원시메시지
	Message	msg;				// 메시지 처리 결과
	char	temp[MAX_MSG_LEN];

	clock_t bt, et, interval=0;
	int		count = 0;

	reviser_config_t	reviserConf;

  kulog_handler_t reviser_log;
  reviser_log.level = KULOG_LEVEL_DBG;
  reviser_log.fp = stderr;
	
	// 옵션을 입력 받는다.
	if(getOption( OPTION_FILE_NAME, &reviserConf, &reviser_log )==-1)
		return -1;

	bt = clock ();

	// 리소스를 읽어들이며 초기화한다.
	if ( reviser_init( &reviserConf, &reviser_log ) < 0 )
	{
		KULOG_ERR(&reviser_log, "ERROR :: cannot initiate reviser.\n");
		return -1;
	}

	et = clock ();
	fprintf(stderr, "Loading Time: %.2lfms\n", (double)(et-bt)/CLOCKS_PER_SEC*1000);

	// 메모리 초기화.
	initmem( 0, orgMsg, &msg, temp );

	KULOG_SVC(&reviser_log, "All resources were loaded!\n");

	// 메시지 하나씩 처리
    while( fgets( orgMsg, MAX_MSG_LEN, infile) != NULL)
    {
		bt = clock ();
    
    	// 전처리기 실행.
		reviser_run(  &reviserConf, orgMsg, &msg );
	
		et = clock ();
		interval+=et-bt;
		count++;

		// 결과 출력.
		printMsg( outfile, msg );

		// 초기화. 다음 문장을 입력 받을 준비.
		initmem( 1, orgMsg, &msg, temp );
    }

	fprintf(stderr, "\nProcessed Input: %d\n", count);
	fprintf(stderr, "Avg Processing Time: %.2lf(ms/input)\n", (double)interval/CLOCKS_PER_SEC/count*1000);

	// 마무리(처리단계): 단순오류 교정모듈, 띄어쓰기 교정모듈 마무리.
	reviser_finish( &reviserConf );

    return 0;
}

void printMsg( FILE *outfile, Message msg )
{
	fprintf(outfile, "%s\n", msg.revstr );	// 교정본만 출력

/*	출력 형식 변경
	fprintf(outfile, "원    본: %s\n", msg.orgstr );
	fprintf(outfile, "교 정 본: %s\n", msg.revstr );
	fprintf(outfile, "여 과 본: %s\n", msg.fltstr );
	if ( strlen(msg.exp[INSULT]  )>0 ) { fprintf(outfile, "\t\t욕 설 : %s\n", msg.exp[INSULT]   ); }
	if ( strlen(msg.exp[UNKNOWN] )>0 ) { fprintf(outfile, "\t\t모 름 : %s\n", msg.exp[UNKNOWN]  ); }
	if ( strlen(msg.exp[JOY]     )>0 ) { fprintf(outfile, "\t\t기 쁨 : %s\n", msg.exp[JOY]     ); }
	if ( strlen(msg.exp[SAD]     )>0 ) { fprintf(outfile, "\t\t슬 픔 : %s\n", msg.exp[SAD]     ); }
	if ( strlen(msg.exp[SURPRISE])>0 ) { fprintf(outfile, "\t\t놀 람 : %s\n", msg.exp[SURPRISE]); }
	if ( strlen(msg.exp[COLD]    )>0 ) { fprintf(outfile, "\t\t냉 담 : %s\n", msg.exp[COLD]    ); }
	if ( strlen(msg.exp[LOVE]    )>0 ) { fprintf(outfile, "\t\t사 랑 : %s\n", msg.exp[LOVE]    ); }
	if ( strlen(msg.exp[OTHERS]  )>0 ) { fprintf(outfile, "\t\t기 타 : %s\n", msg.exp[OTHERS]  ); }
	fprintf(outfile, "\n");//*/
	fflush(outfile);
}


int getOption( char * optFileName, reviser_config_t * pReviserConf, kulog_handler_t * reviser_log )
{
	char *optTags[OPTION_MODULE_NUM]={ "EMOTICON", "REMOVE_REPEAT", "INSULT", "SPELLER"
					, "COMBINE_PHONEME", "SPACER"};
	char *fileTags[OPTION_MODULE_NUM]={ "EMOTICON_DIC", "REPEAT_DIC", "INSULT_DIC", "SPELLER_DIC"
					  ,"SPACER_PATH","MORPHEME_DIC"};
	int i;
	
	FILE * fin;
	char str[MAX_MSG_LEN];
	char * p;
	
	pReviserConf->emoticon = 0;
	pReviserConf->repeat = 0;
	pReviserConf->insult = 0;
	pReviserConf->speller = 0;
	pReviserConf->phoneme = 0;
	pReviserConf->spacer = 0;
	
	if( ( fin = fopen(optFileName , "rt" ) ) == NULL )
	{
		KULOG_ERR(reviser_log, "Cannot Open %s file!\n",optFileName);
		return -1;
	}
	
	fprintf(stderr,"Loading Option file \"%s\"...",optFileName);
		
    while( fgets( str, MAX_MSG_LEN, fin) != NULL)
    {
    	if(*str=='\n' || *str=='#')	// 빈줄과 #로 시작하는 문장은 뛰어넘는다.
	   		continue;
 	
 		if(*str=='\t')	// Tab으로 시작하면 파일경로
   		for(i=0;i< OPTION_MODULE_NUM ;i++)
    	{
    		if(strstr(str,fileTags[i])!=NULL)	// 파일명 Tag가 있으면,
			{
				//strtok( ( p = strchr(str,'=' ) + 1 ), " #\t\n");
				char *tmp_buff;
				strtok_r( ( p = strchr(str,'=' ) + 1 ), " #\t\n", &tmp_buff );

				while(*p==' ')
					p++;
				switch(i)
				{
					case 0 : strcpy(pReviserConf->emoticon_dic,p);break;
					case 1 : strcpy(pReviserConf->repeat_dic,p);break;
					case 2 : strcpy(pReviserConf->insult_dic,p);break;
					case 3 : strcpy(pReviserConf->speller_dic,p);break;
					case 4 : strcpy(pReviserConf->spacer_path,p);break;
					case 5 : strcpy(pReviserConf->morpheme_dic,p);break;
				}
				break;
			}
		}
		else
		for(i=0;i<OPTION_MODULE_NUM;i++)
    	{
			if(strstr(str,optTags[i])==str)	// 옵션 Tag가 처음에 있으면,
			{
				char *tmp_buff;
				//p=strtok(strchr(str,'=')+1," #\t\n");
				p=strtok_r(strchr(str,'=')+1," #\t\n", &tmp_buff );
				if(strstr(p,"ON")!=NULL)
				switch(i)
				{
					case 0 : pReviserConf->emoticon=1;break;
					case 1 : pReviserConf->repeat=1;break;
					case 2 : pReviserConf->insult=1;break;
					case 3 : pReviserConf->speller=1;break;
					case 4 : pReviserConf->phoneme=1;break;
					case 5 : pReviserConf->spacer=1;break;
				}	
				break;
			}
		}
	}

	fclose (fin);
	
	fprintf(stderr,"Complete!\n\n");

	return 0;
}

void initmem( int flag, char orgMsg[MAX_MSG_LEN], Message *msg, char temp[MAX_MSG_LEN] )
{
//	int idx=0;

 	memset( orgMsg,'\0', sizeof(char)*MAX_MSG_LEN);
 	memset( temp,  '\0', sizeof(char)*MAX_MSG_LEN);

	msg->orgstr[0]='\0';
	msg->revstr[0]='\0';
	for(int   i=0;i<10;i++)
		msg->exp[i][0]='\0';
}

char *rev_tag[]={ "[욕설:","[신어]","[생략]","[모름:","[기쁨:","[슬픔:","[놀람:","[냉담:","[사랑:","[기타:","[공백]"};

void reviser_removeTag(char * str, char * temp)
{
	int len=strlen(str);
	int i,j,k;

	for(i=0,j=0;i<len;i++)
		if(str[i]!='[')
			temp[j++]=str[i];
		else
		{
			for(k=0;k<NUMBER_OF_TAGS;k++)
				if(strstr(str+i,rev_tag[k])==str+i)
					if(k==0 || k==3)
						for(i+=6;str[i]!=']';i++)
							;
					else if(rev_tag[k][5]==':')
						for(i+=6;str[i]!=']';)
							temp[j++]=str[i++];
					else
					{
						if(k==10)
							temp[j++]=' ';
						i+=5;
					}
		}
	temp[j]='\0';
}
