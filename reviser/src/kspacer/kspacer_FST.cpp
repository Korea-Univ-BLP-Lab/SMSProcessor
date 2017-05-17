#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "spacer_FST.h"
#include "getopt.h"

#include "definitions.h"
#include "global_option.h"

Model_Type model_type;

/*****************************************************************************/
int main (int argc, char *argv[]) {
  fprintf( stderr, "FST Version (2006/09/15)" );
  clock_t ct1, ct2;

  char input_sentence[4096];    /* 입력 문장 */
  char new_sentence2[4096];      /* 띄어쓰기 교정된 문장 */

  int i;

  PROB_MAP_FST ws_transition_prob2; /* 전이 확률 */
  PROB_MAP_FST syllable_prob2; /* 어휘 확률 */

  ct1 = clock ();               /* 시작 시간 */

  g = init_globals(NULL);  /* 메모리 할당 및 초기화 */
  g->cmd = strdup(get_basename(argv[0], NULL)); /* 순 명령어 이름만 찾기 위해 */

    /* 옵션 알아내기 */
  int inputstart = get_options(g, argc, argv);

  if (argc < inputstart + 2) {
    usage();
    return 0;
  }

  /*************************************************************************/
  fprintf(stderr, "\n%s %s\n", Description, Version); /* 프로그램 정보 출력 */

  /* 초기화 */
  fprintf(stderr, "Initializing kspacer..\n");
  if (!spacer_open_FST(argv[inputstart], argv[inputstart+1], ws_transition_prob2, syllable_prob2)) {
    return 0;
  }
  fprintf(stderr, "Initializing tagger\t[done]\n");


  /*****************************************************************************/
  /* 입력 */
  if (argc == inputstart + 2) {              /* stdin 입력 -> stdout 출력 */

	/* input_sentence에 입력되는 값은 전혀 띄어쓰지 않은 문장이어야 함 */
    //while (fscanf(stdin, "%s", input_sentence) != EOF) {       /* 어절 단위 */
    while (fgets(input_sentence,1024,stdin) ) {       /* 어절 단위 */
      
  	  if( strlen( input_sentence ) > 512 )	{
  	  	
		  fprintf( stderr, "Buffer Full\n" );	
		  continue;
	  }
	  input_sentence[ strlen(input_sentence)-1 ] = '\0';
	  //--------------------------------------------
	  
      /* 띄어쓰기 교정 */
      statistical_spacer_FST(ws_transition_prob2, syllable_prob2, input_sentence, new_sentence2);  /* 결과는 new_sentence에 저장 */

      /* 결과 출력 */
      fprintf(stdout, "%s\n", new_sentence2);
      fflush(stdout);

    } /* end of while */
  }

  /* 화일 입력 -> 화일 출력 */
  else if (argc > inputstart + 2) {
    char outfilename[80];       /* 출력 화일 이름 */
    FILE *infp, *outfp;         /* 입력 화일 포인터, 출력 화일 포인터 */

    for (i = inputstart+2; i < argc; i++) {    /* 모든 화일에 대해 */
      char *ptr;

      if ((infp = fopen (argv[i], "rt")) == NULL) {
        fprintf(stderr, "file open error : %s\n", argv[i]);
        return 0;
      }

      /* 출력 화일명 */
      strcpy(outfilename, argv[i]);
      ptr = strrchr(outfilename, '.');

      sprintf(ptr, ".ans");

      /* 출력 화일 열기 */
      if ((outfp = fopen(outfilename, "wt")) == NULL) {
        fprintf(stderr, "file open error : %s\n", outfilename);
        return 0;
      }

      /* 현재 상태(처리중인 화일) 보여주기 */
      fprintf(stderr, "\t%s -> %s\n", argv[i], outfilename);

      /* 자동 띄어쓰기*/
      //while (fscanf(infp, "%s", input_sentence) != EOF) {  /* 어절 단위 (실제로는 모두 붙여쓴 문장이어야 함) */
      while ( fgets( input_sentence, 1024, infp) ) {  /* 어절 단위 (실제로는 모두 붙여쓴 문장이어야 함) */
      	
	  	if( strlen( input_sentence ) > 512 )	{
	  	   
	  	   // 남은 라인을 계속 읽음.
	  	   while( strlen( input_sentence ) == 1023 )	{
	  	   	  fprintf (outfp, "%s", input_sentence);
      	      if( !fgets( input_sentence, 1024, infp) )	{
      	      	 break;
      	      }
      	   }
      	   
      	   fprintf( stderr, "Buffer Full\n" );	
	  	   fprintf (outfp, "%s", input_sentence);
	  	   continue;
	  	}	  
	  	input_sentence[ strlen(input_sentence)-1 ] = '\0';
	  	//--------------------------------------------
	  
	
        statistical_spacer_FST(ws_transition_prob2, syllable_prob2, input_sentence, new_sentence2);  /* 결과는 new_sentence에 저장 */

      	/* 결과 출력 */
      	fprintf(outfp, "%s\n", new_sentence2);
      	fflush(outfp);
      
    
      }

      fclose (infp);
      fclose (outfp);

    } /* end of for 모든 화일에 대해 */
  } /* end of else */
  
  /*****************************************************************************/
  free_globals(g); /* 전역 변수에 대한 메모리 해제 및 화일 닫기 */

  ct2 = clock ();               /* 마침 시간 */
  fprintf (stderr, "Total time = %.2lf(sec)\n", (double) (ct2 - ct1) / CLOCKS_PER_SEC);

  return 1;
}
