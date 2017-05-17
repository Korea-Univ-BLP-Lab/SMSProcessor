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

  char input_sentence[4096];    /* �Է� ���� */
  char new_sentence2[4096];      /* ���� ������ ���� */

  int i;

  PROB_MAP_FST ws_transition_prob2; /* ���� Ȯ�� */
  PROB_MAP_FST syllable_prob2; /* ���� Ȯ�� */

  ct1 = clock ();               /* ���� �ð� */

  g = init_globals(NULL);  /* �޸� �Ҵ� �� �ʱ�ȭ */
  g->cmd = strdup(get_basename(argv[0], NULL)); /* �� ��ɾ� �̸��� ã�� ���� */

    /* �ɼ� �˾Ƴ��� */
  int inputstart = get_options(g, argc, argv);

  if (argc < inputstart + 2) {
    usage();
    return 0;
  }

  /*************************************************************************/
  fprintf(stderr, "\n%s %s\n", Description, Version); /* ���α׷� ���� ��� */

  /* �ʱ�ȭ */
  fprintf(stderr, "Initializing kspacer..\n");
  if (!spacer_open_FST(argv[inputstart], argv[inputstart+1], ws_transition_prob2, syllable_prob2)) {
    return 0;
  }
  fprintf(stderr, "Initializing tagger\t[done]\n");


  /*****************************************************************************/
  /* �Է� */
  if (argc == inputstart + 2) {              /* stdin �Է� -> stdout ��� */

	/* input_sentence�� �ԷµǴ� ���� ���� ���� ���� �����̾�� �� */
    //while (fscanf(stdin, "%s", input_sentence) != EOF) {       /* ���� ���� */
    while (fgets(input_sentence,1024,stdin) ) {       /* ���� ���� */
      
  	  if( strlen( input_sentence ) > 512 )	{
  	  	
		  fprintf( stderr, "Buffer Full\n" );	
		  continue;
	  }
	  input_sentence[ strlen(input_sentence)-1 ] = '\0';
	  //--------------------------------------------
	  
      /* ���� ���� */
      statistical_spacer_FST(ws_transition_prob2, syllable_prob2, input_sentence, new_sentence2);  /* ����� new_sentence�� ���� */

      /* ��� ��� */
      fprintf(stdout, "%s\n", new_sentence2);
      fflush(stdout);

    } /* end of while */
  }

  /* ȭ�� �Է� -> ȭ�� ��� */
  else if (argc > inputstart + 2) {
    char outfilename[80];       /* ��� ȭ�� �̸� */
    FILE *infp, *outfp;         /* �Է� ȭ�� ������, ��� ȭ�� ������ */

    for (i = inputstart+2; i < argc; i++) {    /* ��� ȭ�Ͽ� ���� */
      char *ptr;

      if ((infp = fopen (argv[i], "rt")) == NULL) {
        fprintf(stderr, "file open error : %s\n", argv[i]);
        return 0;
      }

      /* ��� ȭ�ϸ� */
      strcpy(outfilename, argv[i]);
      ptr = strrchr(outfilename, '.');

      sprintf(ptr, ".ans");

      /* ��� ȭ�� ���� */
      if ((outfp = fopen(outfilename, "wt")) == NULL) {
        fprintf(stderr, "file open error : %s\n", outfilename);
        return 0;
      }

      /* ���� ����(ó������ ȭ��) �����ֱ� */
      fprintf(stderr, "\t%s -> %s\n", argv[i], outfilename);

      /* �ڵ� ����*/
      //while (fscanf(infp, "%s", input_sentence) != EOF) {  /* ���� ���� (�����δ� ��� �ٿ��� �����̾�� ��) */
      while ( fgets( input_sentence, 1024, infp) ) {  /* ���� ���� (�����δ� ��� �ٿ��� �����̾�� ��) */
      	
	  	if( strlen( input_sentence ) > 512 )	{
	  	   
	  	   // ���� ������ ��� ����.
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
	  
	
        statistical_spacer_FST(ws_transition_prob2, syllable_prob2, input_sentence, new_sentence2);  /* ����� new_sentence�� ���� */

      	/* ��� ��� */
      	fprintf(outfp, "%s\n", new_sentence2);
      	fflush(outfp);
      
    
      }

      fclose (infp);
      fclose (outfp);

    } /* end of for ��� ȭ�Ͽ� ���� */
  } /* end of else */
  
  /*****************************************************************************/
  free_globals(g); /* ���� ������ ���� �޸� ���� �� ȭ�� �ݱ� */

  ct2 = clock ();               /* ��ħ �ð� */
  fprintf (stderr, "Total time = %.2lf(sec)\n", (double) (ct2 - ct1) / CLOCKS_PER_SEC);

  return 1;
}
