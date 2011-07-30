#ifndef _OFX_GUID_HELPER_
#define _OFX_GUID_HELPER_

#include <sstream>
#include <string>
#include <windows.h>


//Convert an hex string to a number
template<class T> T HexToInt(const std::string &str);
//Convert a number to an hex string
template<class T> std::string ToHex(const T &value);
int StringToInt(const std::string &str);
std::string IntToString(const int &value);
//Convert a GUID to string
//std::string GUIDToString(const GUID &guid);
std::string GUIDToString( const GUID& guid, char delimiter = '-', bool uppercase = true );
//Convert a string to GUID
//GUID StringToGUID(const std::string &str);
GUID StringToGUID( const std::string &str, char delimiter = '-', bool uppercase = true );

bool EqualGUID( const GUID &guid1, const GUID &guid2 );
#endif//_OFX_GUID_HELPER_