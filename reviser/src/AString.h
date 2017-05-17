#ifndef __AString_H__
#define __AString_H__

#include <stdio.h>
typedef size_t positionType;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Title  : AString(Alphabet-String) Class.
// Author : º¯ÁõÇö(Byun Jeung Hyun) in KUNLP(Korea Univ. Natual Language Processing Lab.)
// Date   : 2006.7.12
/////////////////////////////////////////////////////////////////////////////////////////////////////////

class AString
{
public:
	AString();
	AString(const AString &);
	AString(const char *const);
	AString(const char);
	~AString();

	AString & operator= (const AString &);
	void operator+=(const AString &);
//	void operator+=(const char);
	AString operator+(const AString &);
//	AString operator+(const char ch);
	friend AString operator+(const AString&, const AString&);
//	friend AString operator+(const char ch, const AString&);
	bool operator==( const AString & rhs ) const;
	bool operator!=( const AString & rhs ) const { return !(*this==rhs); }

	char & operator[](positionType);
	char operator[](positionType p) const { return GetAt(p); };

	positionType GetLen() const { return itsLength; }
	const char * GetStr() const { return itsString; }
	char * GetStrP() const { return itsString; }
	char GetAt(positionType offset) const
		{ return (offset >= itsLength)? 0 : itsString[offset]; }

	bool SetAt(positionType , const char );

	bool IsEmpty() const { return !itsLength; }
	void Empty();

	AString Left( positionType nCount ) const
		{ return ( nCount >= itsLength )? *this : SubStr( 0, nCount-1 ); }
	AString Right( positionType nCount ) const
		{ return ( nCount >= itsLength )? *this : SubStr( itsLength-nCount, itsLength ); }
	AString Mid( positionType nFirst, positionType nCount = 0 ) const 
		{ return ( nCount )? SubStr( nFirst, nFirst+nCount-1 ) : SubStr( nFirst, itsLength ); }
	AString SubStr( positionType start, positionType end ) const;

	int Find( char , positionType pos = 0 ) const;
	int Find( AString , positionType pos = 0 )  const;
	AString Delete( positionType nIndex, positionType nCount = 1 ) const
		{ return ( nCount >= itsLength )? *this : Left( nIndex ) + Mid( nIndex+nCount ); }

protected :
	char * itsString;
	positionType itsLength;
};

#endif

