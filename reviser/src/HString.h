#ifndef __HString_H__
#define __HString_H__

#include "AString.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Title  : HString(HanGeul-String) Class.
// Author : 변증현(Byun Jeung Hyun) in KUNLP(Korea Univ. Natual Language Processing Lab.)
// Date   : 2006.7.12
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#define HanPhonemeLoc 0xa4
#define HanPhonemeLocJ 0xa1
#define HanPhonemeLocM 0xbf

static bool Is2byteCharacter(char ch) { return ch<0; } 

#define LocRate 10	//위치 저장 비율  - 예 : 10 -> 10문자당 한 번씩 위치를 기억.

class HString : public AString	// AString을 상속. 그러나 offset이 2byte문자 처리됨.
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

// 이후 함수들에서 쓰이는 위치는 n번째 문자를 나타낸다.
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

	positionType itsNumber;	// 문자의 개수 ( 2byte 문자 포함 )
	positionType * itsLocation;	// 문자 위치정보 배열
};

#endif


