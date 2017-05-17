#ifndef __HString_H__
#define __HString_H__

#include "AString.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Title  : HString(HanGeul-String) Class.
// Author : ������(Byun Jeung Hyun) in KUNLP(Korea Univ. Natual Language Processing Lab.)
// Date   : 2006.7.12
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#define HanPhonemeLoc 0xa4
#define HanPhonemeLocJ 0xa1
#define HanPhonemeLocM 0xbf

static bool Is2byteCharacter(char ch) { return ch<0; } 

#define LocRate 10	//��ġ ���� ����  - �� : 10 -> 10���ڴ� �� ���� ��ġ�� ���.

class HString : public AString	// AString�� ���. �׷��� offset�� 2byte���� ó����.
{
public :
	HString();
	HString(const HString &);
	HString(const AString &);
	HString(const char * const);
	HString(const char);
	~HString();

	HString & operator= (const HString &);
	void operator+=(const HString&);
	void operator+=(const char);
	AString operator[](positionType offset) const { return GetNth(offset); };

	positionType GetNum() const { return itsNumber; }
	int GetLoc(positionType p) const;	// private?
	AString GetNth(positionType offset) const
		{ return (offset>=itsNumber)? AString("") : SubStr(offset,offset).GetStr(); }

// ���� �Լ��鿡�� ���̴� ��ġ�� n��° ���ڸ� ��Ÿ����.
	HString Left( positionType nCount ) const
		{ return ( nCount == 0 )? HString("") : SubStr( 0, nCount-1 ); }
	HString Right( positionType nCount ) const
		{ return ( nCount >= itsNumber )? *this : SubStr( itsNumber - nCount, itsNumber-1 ); }
	HString Mid( positionType nFirst, positionType nCount = 0 ) const 
		{ return ( nCount )? SubStr( nFirst, nFirst+nCount-1 ) : SubStr( nFirst, itsNumber-1 ); }
	HString SubStr( positionType start, positionType end ) const;

	int Find( char , positionType pos = 0 ) const;
	int Find( HString , positionType pos = 0 ) const;
	HString Delete( positionType nIndex, positionType nCount = 1 ) const
		{ return ( nIndex >= itsNumber )? Mid(0) : HString(Left( nIndex ) + Mid( nIndex+nCount )); }

private :
	void fillLoc();

	positionType itsNumber;	// ������ ���� ( 2byte ���� ���� )
	positionType * itsLocation;	// ���� ��ġ���� �迭
};

#endif


