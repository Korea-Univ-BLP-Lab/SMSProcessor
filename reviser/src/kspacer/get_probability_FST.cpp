#include "definitions.h"


/*****************************************************************************/
double get_probability4_FST(PROB_MAP_FST &probs, char *num, char *t1, char *t2, char *t3, char *t4) {

	char prev[256];
	double p;
	double ret;
	
	//fprintf( stdout, "FS:%s%s%s%s->%s\n", t1, t2, t3, t4, num );	fflush(stdout);
	sprintf( prev, "%s%s%s%s->%s", t1,  t2,  t3,  t4, num );
	//printf( "[%s]\t%f\n", prev, ret );
	p = probs.GetProb( prev );
	if( p > 0.0 )	{
		ret = p;
	}
	else	{
		sprintf( prev, "%s%s%s->%s", t2, t3, t4, num );
		//printf( "[%s]\t%f\n", prev, ret );
		p = probs.GetProb( prev );
		if( p > 0.0 )	{
			ret = p;
		}
		else	{
			sprintf( prev, "%s%s->%s", t3, t4, num );
			//printf( "[%s]\t%f\n", prev, ret );
			p = probs.GetProb( prev );
			if( p > 0.0 )	{
				ret = p;
			}
			else	{
				sprintf( prev, "%s->%s", t4, num );
				//printf( "[%s]\t%f\n", prev, ret );
				p = probs.GetProb( prev );
				if( p >= 0.0 )	{
					ret = p;
				}
				else	{
					sprintf( prev, "%s", num );
					//printf( "[%s]\t%f\n", prev, ret );
					p = probs.GetProb( prev );
					if( p >= 0.0 )	{
						ret = p;
					}
					else	{
						ret = ALMOST_ZERO;
					}
				}
			}
		}
	}
	
	
	//fprintf( stdout, "FS[%s]\t%f\n\n", prev, ret );	fflush( stdout );
	return ret;
		
}
