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

  //�α� �ڵ鷯 ���� �� ����
  //	fp : ��� �α� ���� ������
  //	level : �α� ������ 4 �ܰ�� �����Ǿ� �ִ�. (kulog.h ����)
  kulog_handler_t HanTag_log;
  HanTag_log.level = KULOG_LEVEL_DBG;
  HanTag_log.fp = stderr;
  
  // ���� �ɼ� ���� �� ����
  //	tagger : ���� ���θ� �����Ѵ�. (1: ����  0:�������� ����)
  //	tagger_path : ���ҽ� path�� �����Ѵ�.
  HanTag_config_t HanTagConf;
  HanTagConf.tagger = 1;
  strcpy(HanTagConf.tagger_path,"../../rsc");

  // ǰ�� �����⸦ ����, �ʱ�ȭ �Ѵ�.
  void *HanTag;
  if( HanTag_init( & HanTagConf, & HanTag_log, &HanTag ) <0 ) {
	fprintf(stderr, "ERROR :: cannot open HanTag.\n");
  	return -1;
  }

  // �����κ��� �� ���徿 �����Ͽ� ǰ�� ������ �Ѵ�.
  while (fgets(Sentence,10000,stdin)!=NULL) {

    // ǰ�縦 �����Ѵ�.
	num = HanTag_run( & HanTagConf , HanTag , Sentence, Wrd, Result );

	// ǰ�� ���� ����� ����� ǰ��� ��ȯ�Ѵ�.��
	for( i=0; i<num; i++ ) {
	Convert2Usertag( HanTag, Result[i][0] );
	}

	// ǰ�� ���� ����� ����Ѵ�.
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

  // �޸𸮿� �ε�� ���ҽ��� �����Ѵ�.
  HanTag_finish(HanTag);
	
  return 0;
}
