#include "stdio.h"
#include "str_prob_FST.h"


	
int main()
{
	CStrProbFST prob;
	prob.Load( "/home/Korea/module/motdg_WordSpacer/tmp/tr.seoul4" );
	
	char str[1024] = "0¤Ð1±â->0";
	double p = prob.GetProb( str );
	if( p < -0.5 )	{
			fprintf( stderr, "%s : not found\n", str );
	}
	else	{
		fprintf( stderr, "%s : %1.12e\n", str, p );
	}
	
	return 0;
}
