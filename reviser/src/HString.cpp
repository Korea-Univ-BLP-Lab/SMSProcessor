#include <string.h>
#include "HString.h"

HString::HString():
	AString(),
	itsNumber (0),
	itsLocation (NULL)
{
}

HString::HString(const HString & rhs):
	AString(rhs.GetStr())
	{
		fillLoc();
	}

HString::HString(const AString & rhs):
	AString(rhs)
	{
		fillLoc();
	}

HString::HString(const char * const str):
	AString(str)
	{
		fillLoc();
	}

HString::HString(const char ch):
	AString(ch)
	{
		fillLoc();
	}

HString::~HString()
{
	itsNumber = 0;
	delete [] itsLocation;
}

HString& HString::operator=(const HString & rhs)
{
	if (this == &rhs)
		return *this;

	itsLength=rhs.GetLen();
	delete [] itsString;
	itsString = new char[itsLength+1];

	strcpy(itsString,rhs.GetStr());

	delete[] itsLocation;

//	fillLoc();	//비효율적.
	itsNumber=rhs.GetNum();
	itsLocation = new positionType[itsNumber/LocRate+1];
	for(size_t i=0;i<itsNumber/LocRate+1;i++)
		itsLocation[i]=rhs.GetLoc(i*LocRate);

	return *this;
}

void HString::operator+=(const HString& rhs)
{
	itsLength+=rhs.GetLen();

	char *temp = new char[itsLength+1];

	strcpy(temp,itsString);
	strcat(temp,rhs.GetStr());

	delete [] itsString;
	itsString = temp;

	delete[] itsLocation;
	fillLoc();
}

void HString::operator+=(const char ch)
{
	char * temp = new char[itsLength+2];

	strcpy(temp,itsString);
	temp[itsLength++] = ch;
	temp[itsLength] = '\0';

	delete [] itsString;
	itsString = temp;

// 꼭 위치를 추가 해야하는 상황에서만 추가.
	if( itsNumber % LocRate == 0 )
	{
		delete[] itsLocation;
		fillLoc();
	}
	else
		itsNumber++;
}

// LocRate(e.g. 10개) 단위로 저장된 위치부터 원하는 위치를 찾아준다.
int HString::GetLoc(positionType p) const	
{
	if(p>=itsNumber)
		return -1;

	positionType start = itsLocation[p / LocRate];
	
	for(int count=0,i=0;count<LocRate;count++,i++)
	{
		if( ((int)p) % LocRate == count )
			return (int)(start+i);
		if(Is2byteCharacter(itsString[start+i]))
			i++;
	}
	return -1;
}

HString HString::SubStr( positionType start, positionType end ) const
{
	if( start > end || !itsNumber || start > itsNumber )
		return "";

	start = GetLoc(start);
	end = ( end < itsNumber )? GetLoc(end) : GetLoc(itsNumber-1);
	if(Is2byteCharacter(itsString[end]))
		end++;

	positionType SubStrLen = end-start+1;

	char *temp = new char[SubStrLen+1];

	strncpy(temp,&itsString[start],SubStrLen);
	temp[SubStrLen] = '\0';	

	HString ret( temp );
	delete[] temp;

	return ret;
}

int HString::Find( char ch, positionType pos ) const
{
	char *ptr;

	if( pos >= itsNumber || Is2byteCharacter(ch) || !itsNumber)
		return -1;

	if((ptr=strchr(&itsString[GetLoc(pos)],ch)) == NULL)
		return -1;

	positionType n = ptr-itsString;

	for(positionType i=pos;i<itsNumber;i++)
	if(GetLoc(i)==(int)n)
		return (int)i;
	return -1;
}

int HString::Find( HString str, positionType pos ) const
{
	char * ptr;

	if( pos >= itsNumber || !itsNumber)
		return -1;

	if( (ptr=strstr(&itsString[GetLoc(pos)],str.GetStr())) == NULL)
		return -1;

	positionType n = ptr-itsString;

	for(positionType i=pos;i<itsNumber;i++)
	if(GetLoc(i)==(int)n)
		return (int)i;
	return -1;
}

// LocRate(e.g. 10개) 마다 위치를 한번씩 저장한다. 문자열 처음부터 끝까지 선형 접근한다.
void HString::fillLoc()
{
	itsLocation = new positionType[itsLength/LocRate+1];
	
	itsNumber=0;
	for(positionType count=0,i=0;i<itsLength;i++,itsNumber++)
	{
		if(itsNumber%LocRate==0)
			itsLocation[count++]=i;

		if(Is2byteCharacter(itsString[i]))
			i++;
	}
}

