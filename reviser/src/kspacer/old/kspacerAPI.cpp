#include <stdio.h>
#include <stdlib.h>

//#include "bt.h"
//#include "env.h"
#include "kspacerAPI.h"
#include "definitions.h"
#include "report.h"
#include "probability_io_FST.h"
#include "kulog_kspacer.h"
/*****************************************************************************/

Model_Type model_type;

extern
int statistical_spacer_FST(PROB_MAP_FST &ws_transition_prob2, PROB_MAP_FST &syllable_prob2, 
                       char *input_sentence, char *new_sentence);

//PROB_MAP_FST ws_trns_prob; /* ÀüÀÌ È®·ü */
//PROB_MAP_FST ws_syl_prob; /* ¾îÈÖ È®·ü */

/*****************************************************************************/
int kspacer_init(kulog_handler_t * kspacer_log, char *trans_file, char *syllable_prob)
{
	kulog_reviser = kspacer_log;
	
   // for ÀüÀÌ È®·ü
   if( !ws_trns_prob.Load( trans_file ) )	{
   	return -1;
   }

  // for ¾îÈÖ È®·ü
  if( !ws_syl_prob.Load( syllable_prob ) )	{
   	return -1;
  }

  return 0;
}

int  kspacer_run (char *input_sentence, char *new_sentence)
{
	statistical_spacer_FST(ws_trns_prob,ws_syl_prob,input_sentence,new_sentence);
	return 0;
}
