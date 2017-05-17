#include <stdio.h>
#include <stdlib.h>

//#include "bt.h"
//#include "env.h"
#include "spacer_FST.h"
#include "definitions.h"
#include "report.h"
#include "probability_io_FST.h"
/*****************************************************************************/

Model_Type model_type;


/*****************************************************************************/
int spacer_open_FST(char *trans_file, char *syllable_prob, PROB_MAP_FST &ws_transition_prob2, PROB_MAP_FST &lexical_prob2) {
  
  
   // for ÀüÀÌ È®·ü
   if( !ws_transition_prob2.Load( trans_file ) )	{
   	return 0;
   }


  // for ¾îÈÖ È®·ü
  if( !lexical_prob2.Load( syllable_prob ) )	{
   	return 0;
  }



  return 1;
}

