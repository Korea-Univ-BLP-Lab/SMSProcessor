#include <string.h>
#include "AString.h"

AString::AString()
{
	itsString = new char[1];
	itsString[0] = '\0';
	itsLength = 0;
}

AString::AString (const AString & rhs)
{
	itsLength=rhs.GetLen();
	itsString = new char[itsLength+1];
	
	strcpy(itsString,rhs.GetStr());
}

AString::AString(const char * const chStr)
{
	itsLength = (positionType)strlen(chStr);
	itsString = new char[itsLength+1];

	strcpy(itsString,chStr);
}

AString::AString(const char ch)
{
	itsString = new char[2];
	itsString[0] = ch;
	itsString[1] = '\0';
	itsLength=1;
}

AString::~AString ()
{
	delete [] itsString;
	itsLength = 0;
}

AString& AString::operator=(const AString & rhs)
{
	if (this == &rhs)
		return *this;

	itsLength=rhs.GetLen();
	delete [] itsString;
	itsString = new char[itsLength+1];

	strcpy(itsString,rhs.GetStr());
	
	return *this;
}

bool AString::operator==( const AString & rhs ) const
{
	if( strcmp( itsString, rhs.GetStr() ) == 0 )	return true;
	return false;	
}

void AString::operator+=(const AString& rhs)
{
	itsLength+=rhs.GetLen();

	char *temp = new char[itsLength+1];

	strcpy(temp,itsString);
	strcat(temp,rhs.GetStr());

	delete [] itsString;
	itsString = temp;
}

//void AString::operator+=(const char ch)
//{
//	char * temp = new char[itsLength+2];
//
//	strcpy(temp,itsString);
//
//	temp[itsLength] = ch;
//	temp[itsLength+1] = '\0';
//
//	delete [] itsString;
//	itsString = temp;
//	itsLength++;
//}

AString AString::operator+(const AString& rhs)
{
	AString temp(itsString);

	temp+=rhs;

	return temp;
}

//AString AString::operator+(const char ch)
//{
//	AString temp(itsString);
//
//	temp+=ch;
//
//	return temp;
//}

AString operator+(const AString& lhs, const AString& rhs)
{
	AString temp(lhs.GetStr());

	temp+=rhs.GetStr();

	return temp;
}

//AString operator+(const char ch, const AString& rhs)
//{
//	AString temp;
//
//	temp+=ch;
//	temp+=rhs.GetStr();
//
//	return temp;
//}

char & AString::operator[](positionType offset)
{
	if (offset > itsLength)
		return itsString[itsLength-1];	// 마지막 글자.
	else
		return itsString[offset];
}

bool AString::SetAt(positionType offset, const char ch)
{
	if (offset < itsLength)
	{
		itsString[offset] = ch;
		return true;
	}
	else
		return false;
}

void AString::Empty()
{
	char * temp = new char[1];
	*temp = '\0';

	delete[] itsString;
	itsString = temp;
	itsLength = 0;
}

AString AString::SubStr( positionType start, positionType end ) const
{
	if( start > end || start > itsLength)
		return "";

	int SubStrLen = (end >= itsLength)? (int)itsLength-(int)start : (int)end-(int)start+1;
	char *temp = new char[SubStrLen+1];

	strncpy(temp,&itsString[start],SubStrLen);
	temp[SubStrLen] = '\0';	

	AString ret( temp );
	delete[] temp;

	return ret;
}

int AString::Find( char ch, positionType pos ) const
{
	char *ptr;

	if( pos >= itsLength )
		return -1;

	if((ptr=strchr(&itsString[pos],ch)) == NULL)
		return -1;
	else
		return (int) (ptr-itsString);
}

int AString::Find( AString str, positionType pos ) const
{
	char * ptr;

	if( pos >= itsLength )
		return -1;

	if( (ptr=strstr(&itsString[pos],str.GetStr())) == NULL)
		return -1;
	else
		return (int) (ptr-itsString);
}

