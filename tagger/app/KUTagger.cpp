#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../include/KomaAPI.h"
#include "../../include/HanTagAPI.h"
#include "../../include/nlp.def"

int main(int argc, char *argv[])
{
  char Sentence[10000];
  char *Wrd[MaxNumWrd];
  char *Result[MaxNumWrd][MaxNumAmb];
  int i, num;

  //로그 핸들러 생성 및 설정
  //	fp : 출력 로그 파일 포인터
  //	level : 로깅 레벨은 4 단계로 구성되어 있다. (kulog.h 참조)
  kulog_handler_t HanTag_log;
  HanTag_log.level = KULOG_LEVEL_DBG;
  HanTag_log.fp = stderr;
  
  // 실행 옵션 생성 및 설정
  //	tagger : 실행 여부를 결정한다. (1: 실행  0:실행하지 않음)
  //	tagger_path : 리소스 path를 지정한다.
  HanTag_config_t HanTagConf;
  HanTagConf.tagger = 1;
  strcpy(HanTagConf.tagger_path,"../../rsc");

  // 품사 부착기를 생성, 초기화 한다.
  void *HanTag;
  if( HanTag_init( & HanTagConf, & HanTag_log, &HanTag ) <0 ) {
	fprintf(stderr, "ERROR :: cannot open HanTag.\n");
  	return -1;
  }

  // 문서로부터 한 문장씩 추출하여 품사 부착을 한다.
  while (fgets(Sentence,10000,stdin)!=NULL) {

    // 품사를 부착한다.
	num = HanTag_run( & HanTagConf , HanTag , Sentence, Wrd, Result );

	// 품사 부착 결과를 사용자 품사로 변환한다.ㅏ
	for( i=0; i<num; i++ ) {
	Convert2Usertag( HanTag, Result[i][0] );
	}

	// 품사 부착 결과를 출력한다.
	for (i=0; Result[i][0]; i++) {
	  fprintf(stdout,"%s\t", Wrd[i]);
      for (int j=0; Result[i][j]; j++) {
		if (j) fprintf(stdout, "^");
		fprintf(stdout, "%s", Result[i][j]);
	  }
	  fprintf(stdout,"\n"); 
	}
    fprintf(stdout,"\n");
  }

  // 메모리에 로드된 리소스를 해제한다.
  HanTag_finish(HanTag);
	
  return 0;
}
