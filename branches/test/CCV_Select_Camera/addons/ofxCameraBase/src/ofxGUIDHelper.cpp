#include "ofxGUIDHelper.h"


template<class T>
T HexToInt(const std::string &str)
{
  if(str.size()==0)return 0;
  std::istringstream iss(str);
  T result=0;
  iss>>std::hex>>result;
  return result;
}

template<class T>
std::string ToHex(const T &value)
{
  std::ostringstream oss;
  oss.setf(std::ios_base::uppercase);
  oss<<std::hex<<value;
  return oss.str();
}

int StringToInt(const std::string &str)
{
  if(str.size()==0)return 0;
  std::istringstream iss(str);
  int result=0;
  iss>>result;
  return result;
}

std::string IntToString(const int &value)
{
  std::ostringstream oss;
  oss.setf(std::ios_base::uppercase);
  oss<<value;
  return oss.str();
}


//I'm not writing shitty code -> I'm creating refactoring opportunities

//std::string GUIDToString(const GUID &guid)
//{
//	unsigned char* data = (unsigned char*)(&guid.Data1);
//	std::string part1 = "";
//	for (int i=0;i<4;i++)
//	{
//		part1 = (((*data < 16)?"0":"") + ToHex<unsigned short>((unsigned short)*data)) + part1;
//		data++;
//	}
//	std::string part2 = "";
//	data = (unsigned char*)(&guid.Data2);
//	for (int i=0;i<2;i++)
//	{
//		part2 = (((*data < 16)?"0":"") + ToHex<unsigned short>((unsigned short)*data)) + part2;
//		data++;
//	}
//	std::string part3 = "";
//	data = (unsigned char*)(&guid.Data3);
//	for (int i=0;i<2;i++)
//	{
//		part3 = (((*data < 16)?"0":"") + ToHex<unsigned short>((unsigned short)*data)) + part3;
//		data++;
//	}
//	std::string part4 = "";
//	data = (unsigned char*)(&guid.Data4);
//	for (int i=0;i<2;i++)
//	{
//		part4 += (((*data < 16)?"0":"") + ToHex<unsigned short>((unsigned short)*data));
//		data++;
//	}
//	std::string part5 = "";
//	for (int i=0;i<6;i++)
//	{
//		part5 += (((*data < 16)?"0":"") + ToHex<unsigned short>((unsigned short)*data));
//		data++;
//	}
//	return '{'+part1 + '-' + part2 + '-' + part3 +'-' + part4 + '-' + part5 + '}';
//}

//GUID StringToGUID(const std::string &str)
//{
//	GUID result;
//	memset((void*)&result,0,sizeof(result));
//	if ( str.length()!=38)
//		return result;
//	result.Data1 = HexToInt<unsigned long>(str.substr(1,8));
//	result.Data2 = HexToInt<unsigned short>(str.substr(10,4));
//	result.Data3 = HexToInt<unsigned short>(str.substr(15,4));
//	unsigned char* dataPtr = (unsigned char*)&result.Data4;
//	*(dataPtr++) = (unsigned char)HexToInt<unsigned short>(str.substr(20,2));
//	*(dataPtr++) = (unsigned char)HexToInt<unsigned short>(str.substr(22,2));
//	*(dataPtr++) = (unsigned char)HexToInt<unsigned short>(str.substr(25,2));
//	*(dataPtr++) = (unsigned char)HexToInt<unsigned short>(str.substr(27,2));
//	*(dataPtr++) = (unsigned char)HexToInt<unsigned short>(str.substr(29,2));
//	*(dataPtr++) = (unsigned char)HexToInt<unsigned short>(str.substr(31,2));
//	*(dataPtr++) = (unsigned char)HexToInt<unsigned short>(str.substr(33,2));
//	*(dataPtr++) = (unsigned char)HexToInt<unsigned short>(str.substr(35,2));
//	return result;
//}

std::string GUIDToString( const GUID& guid, char delimiter, bool uppercase ) {
	std::stringstream ss;
	if ( uppercase ) {
		ss << std::uppercase;
	} else {
		ss << std::nouppercase;
	}

	//! Get the hex format GUID string
	/*! Like: XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXXXX 
	*/
	ss.fill( '0' );	//! Fill the space with "0"
	ss << std::hex;	//! Output as Hexadecimal

	//! 8 Bytes	|	Data1
	ss.width( 8 );	//! Set the width to 8
	ss << guid.Data1 << delimiter;

	//! 4 Bytes	|	Data2
	ss.width( 4 );	ss << guid.Data2 << delimiter;

	//! 4 Bytes	|	Data3
	ss.width( 4 );	ss << guid.Data3 << delimiter;

	//! 4 Bytes	|	Initial two bytes from Data4
	ss.width( 2 );	ss << (int)guid.Data4[0];
	ss.width( 2 );	ss << (int)guid.Data4[1] << delimiter;

	//! 12 Bytes|	Remaining six bytes from Data4
	ss.width( 2 );	ss << (int)guid.Data4[2];
	ss.width( 2 );	ss << (int)guid.Data4[3];
	ss.width( 2 );	ss << (int)guid.Data4[4];
	ss.width( 2 );	ss << (int)guid.Data4[5];
	ss.width( 2 );	ss << (int)guid.Data4[6];
	ss.width( 2 );	ss << (int)guid.Data4[7];

	return ss.str();
}

GUID StringToGUID( const std::string &str, char delimiter, bool uppercase ) {
	// TODO
	GUID guid;
	int Data4[8];
	std::string formatStr;

	if ( uppercase ) {
		// TODO use the delimiter rather then the "-"
		formatStr = "%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X";
	} else {
		formatStr = "%08lx-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x";
	}
	sscanf( str.c_str(), formatStr.c_str(),
		&guid.Data1, &guid.Data2, &guid.Data3, &Data4[0], &Data4[1],
		&Data4[2], &Data4[3], &Data4[4], &Data4[5], &Data4[6], &Data4[7] );

	// Ref:
	//		http://hi.baidu.com/yapn/blog/item/a7acb40117e9810e738da5d1.html
	for ( int i = 0; i < 8; ++i ) {
		guid.Data4[i] = (unsigned char)Data4[i];
	}

	return guid;
}

bool EqualGUID( const GUID &guid1, const GUID &guid2 ) {
	if (
		guid1.Data1 == guid2.Data1 &&
		guid1.Data2 == guid2.Data2 &&
		guid1.Data3 == guid2.Data3 &&
		guid1.Data4[0] == guid2.Data4[0] &&
		guid1.Data4[1] == guid2.Data4[1] &&
		guid1.Data4[2] == guid2.Data4[2] &&
		guid1.Data4[3] == guid2.Data4[3] &&
		guid1.Data4[4] == guid2.Data4[4] &&
		guid1.Data4[5] == guid2.Data4[5] &&
		guid1.Data4[6] == guid2.Data4[6] &&
		guid1.Data4[7] == guid2.Data4[7]
	) {
		return true;
	}

	return false;
}