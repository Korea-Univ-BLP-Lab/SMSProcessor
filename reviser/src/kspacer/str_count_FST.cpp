#include "str_count_BTree.h"

CStrCountBTree::CStrCountBTree()
{
	btree.BtreeProlog();
	int field_size[2]={sizeof(int), 0}; 
	btree.BtreeUse("tmp_btree_file", 2, field_size, 0.5);
}

CStrCountBTree::~CStrCountBTree()
{
	btree.BtreeFree();
	btree.BtreeEpilog();
}


void CStrCountBTree::AddStr( char *str1 )
{
	char buf[128];
	sprintf( buf, "%s", str1 );
	wordtype *x; 
	if((x=(wordtype *)btree.BtreeSearch((void *)buf)) != NULL) {
		x->count++; 
	}else {
		wordtype y; 
		y.count=1; 
		strcpy(y.key, buf); 
		btree.BtreeInsert(&y); 
	}	
}

int CStrCountBTree::GetCount( char *str1 )
{
	char buf[128];
	sprintf( buf, "%s", str1 );
	wordtype *x; 
	
	if((x=(wordtype *)btree.BtreeSearch(buf))!=NULL) {
		return x->count; 
	}
	
	return 0;
}
	
void CStrCountBTree::SaveToFile( char *file_name )
{
	FILE *fp = fopen(file_name, "wt");
	if (fp == NULL)
	{
		fprintf(stderr, "file open error... %s\n", file_name);
		return;
	}
	
	wordtype *x;
	char buffer[256]; 

	buffer[0]=0; 
	if(btree.BtreeSearchNext(buffer, &x)) {
		do{
			if ( x->count > 0 )
				fprintf(fp,"%s\t%d\n", x->key, x->count); 
		}while(btree.BtreeSearchNext(x->key, &x)); 
	}
	fclose(fp);
	
}

void CStrCountBTree::LoadFromFile( char *file_name )
{
	FILE *fp = fopen( file_name, "r" );
	if( !fp )	{
		fprintf( stderr, "file open error... %s\n", file_name );
		return;
	}
	
	char buf[256];
	char buf2[256];
	while( fgets( buf, 256, fp ) )	{
		int len = strlen( buf );
		if( len >= 255 )	break;
		
		int pos;
		for( pos = len-1; pos >= 0; pos-- )	{
			if( buf[pos] == '\t' )	break;
		}
		
		wordtype x;
		x.count = atoi( &(buf[pos+1]) );
		buf[pos] = '\0';
		sprintf( x.key, "%s", buf );		
		btree.BtreeInsert( (void*)&x );
	}
	
	fclose( fp );
}
	


