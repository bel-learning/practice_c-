#include <iostream>
#include <stdexcept>
#include <cstring>
#include <vector>
#include <utility>

class CString {
	size_t length;
	size_t capacity;
	char * data;
public:
	CString ( const char * data = "" ) ;
	CString ( int ) ;
	CString ( const CString & other );
	CString ( CString && other ) ;
	~CString ( ) ;

	CString operator + ( const CString & str ) const;
	CString & operator = ( const CString & str );
	CString & operator = ( CString && str ) ;
	CString & operator += ( const CString & str );

	friend std::ostream & operator << ( std::ostream & os, const CString & str ) {
		return os << str.data;
	}
};

CString::CString ( const char * str )
	: length ( strlen ( str ) ), capacity ( length + 1 ), data ( new char [ capacity ] ) {
	memcpy ( data, str, length );
	data [ length ] = '\0';
}

CString::CString ( int num )
	: length ( 0 ), capacity ( 40 ), data ( new char [ capacity ] ) {
	snprintf ( data, sizeof (40), "%d", num );
	length = strlen ( data );
}

CString::CString ( const CString & other )
	: length ( other.length ), capacity ( length + 1 ), data ( new char [ capacity ] ) {
	memcpy ( data, other.data, length );
	data [ length ] = '\0';
}

CString::CString ( CString && other ) 
	: length ( other.length ), capacity ( other.capacity ), data ( other.data ) {
	other.data = nullptr;
}

CString & CString::operator = ( const CString & str ) {
	if ( this == & str )
		return *this;

	delete [] data;
	length = str.length;
	capacity = length + 1;
	data = new char [capacity];

	memcpy ( data, str.data, length );
	data [ length ] = '\0';

	return *this;
}

CString & CString::operator = ( CString && str )  {
	std::swap ( data, str.data );
	std::swap ( length, str.length );
	std::swap ( capacity, str.capacity );

	return *this;
}

CString::~CString ( )  {
	delete [] data;
	data = nullptr;
	length = 0;
	capacity = 0;
}

CString CString::operator + ( const CString & str ) const {
	CString copy ( * this );
	copy += str;
	return copy;
}

CString & CString::operator += ( const CString & str ) {
	if ( capacity < length + str.length + 1 ) {
		while ( capacity < length + str.length + 1 )
			capacity = capacity * 2 + 10;
		char * tmp = new char [ capacity ];
		memcpy ( tmp, data, length );
		delete [] data;
		data = tmp;
	}
	memcpy ( data + length, str.data, str.length );
	length += str.length;
	data [ length ] = '\0';
	return * this;
}

int main () {
	std::vector < CString > vec;

	CString str ( "Hello" );
	vec.push_back ( std::move ( str ) );

	str = "abc";
	CString str2 ( 10 );

	CString str3 ( str );
	str2 = str3;

	str2 += str2;

	std::cout << str2 << std::endl;
	return 0;
}

