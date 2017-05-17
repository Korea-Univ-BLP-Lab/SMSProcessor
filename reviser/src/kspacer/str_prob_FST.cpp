#include "str_prob_FST.h"
#include "kulog_kspacer.h"

CStrProbFST::CStrProbFST()
{
	 fst = NULL;
	 prob = NULL;
}

CStrProbFST::~CStrProbFST()
{
	free(fst);
	if( prob )	delete prob;
}

bool CStrProbFST::Load( char arg_fn_prefix[] )	
{
	sprintf( fst_fn, "%s.FST", arg_fn_prefix );
	fst=LoadFST( fst_fn );
	if( fst == NULL )	{
		KSPACER_LOG_ERR(kulog_reviser, "FST open error... %s\n", fst_fn );
		return false;
	}
	
	sprintf( prob_fn, "%s.dat", arg_fn_prefix );
	if( !LoadProb() )	{
		KSPACER_LOG_ERR(kulog_reviser, "LoadProb error... %s\n", prob_fn );
		return false;
	}
	
  	return true;
}
  
  
bool CStrProbFST::LoadProb()
{
//	char fn[MaxStringLength];
	FILE *fp = fopen( prob_fn, "r" );
	if( !fp )	{
		KSPACER_LOG_ERR(kulog_reviser, "file open error... %s\n", prob_fn );
		return false;
	}
		
	size_t total_line = 0;
	char buf[MaxStringLength];
	while( fgets( buf, MaxStringLength, fp ) )	{
		total_line++;
	}
	
	if( prob )	delete prob;
	prob = new double[total_line];
	fseek( fp, 0, SEEK_SET );
	total_line = 0;
	while( fgets( buf, MaxStringLength, fp ) )	{
		prob[total_line] = atof( buf );
		total_line++;
	}
		
	KSPACER_LOG_DBG(kulog_reviser, "Prob : %d\n", total_line );	
	fclose( fp );
	return true;
}

double CStrProbFST::GetProb( char *str1 )
{
	if( !fst )	return -1.0;
	if( !prob )	return -1.0;
	
	//KSPACER_LOG_DBG(kulog_reviser, "%s %d\n", str1, FSTMaxMatch );
	int IndexList[FSTMaxMatch];
	

	int hash = String2Hash( fst, str1, IndexList );
	if( hash >= 0 )	{
		return prob[hash];
	}
	else	{
		return -1.0;
	}
	
}
	
	
// EOF
