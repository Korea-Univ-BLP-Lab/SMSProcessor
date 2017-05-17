#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../../include/KomaAPI.h"
#include "../../include/HanTagAPI.h"
#include "../../include/nlp.def"
#define MAX_SEN_LEN	4096

int main(int argc, char *argv[])
{
	char Sentence[MAX_SEN_LEN]; // 입력 문장
	char *Wrd[MaxNumWrd];	// 원 어절
	char *Result[MaxNumWrd][MaxNumAmb];	// 분석 결과
	void *HanTag;

	int	 count = 0;	 // 문장수
	clock_t bt, et, interval=0;

	// 품사 부착기 설정
	HanTag_config_t HanTagConf;

	// 로그 관리자
	kulog_handler_t HanTag_log;
	HanTag_log.level=3;
	HanTag_log.fp=stderr;
	HanTagConf.tagger = 1;
	strcpy(HanTagConf.tagger_path,"../../rsc");

	// 초기화
	bt=clock();
	if( HanTag_init( & HanTagConf, & HanTag_log, &HanTag ) <0 )
	{
		KULOG_ERR( &HanTag_log, "[ERROR] cannot open HanTag.\n" );
		return -1;
	}
	et = clock ();
	KULOG_DBG( &HanTag_log,  "Loading Time: %.2lfms\n", (double)(et-bt)/CLOCKS_PER_SEC*1000);

	// 입력문 로드
	 bt = clock ();
	 while (fgets(Sentence, MAX_SEN_LEN, stdin)!=NULL)
	 {
		 // 품사 부착 호출
		 int num = HanTag_run( & HanTagConf , HanTag , Sentence, Wrd, Result );

		 // 해당하는 사용자 정의 품사 변환
		for( int i=0; i<num; i++ )	
		{
			Convert2Usertag( HanTag, Result[i][0] );
		}

		// 분석 결과 출력
		for (int i=0; Result[i][0]; i++)
		{
			KULOG_DBG( &HanTag_log,  "%s\t%s\n", Wrd[i], Result[i][0]);
		}
		KULOG_DBG( &HanTag_log,  "\n" );
		count++;
	}

	et = clock ();
	interval+=et-bt;
	KULOG_DBG( &HanTag_log, "\nProcessed Input: %d\n", count);
	KULOG_DBG( &HanTag_log, "Avg Processing Time: %.2lf(ms/input)\n", (double)interval/CLOCKS_PER_SEC/count*1000);
	
	// 종료
	HanTag_finish(HanTag);
	return 0;
}
