#include "stdio.h"
#include "str_prob.h"


	
int main()
{
	CStrProb prob;
	char *name = "hello";
	prob.AddProb( name, 50 );
	printf( "%f\n", prob.GetProb("hello") );



	
	return 0;
}
