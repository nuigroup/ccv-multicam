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

std::string GUIDToString(const GUID &guid)
{
	unsigned char* data = (unsigned char*)(&guid.Data1);
	std::string part1 = "";
	for (int i=0;i<4;i++)
	{
		part1 = (((*data < 16)?"0":"") + ToHex<unsigned short>((unsigned short)*data)) + part1;
		data++;
	}
	std::string part2 = "";
	data = (unsigned char*)(&guid.Data2);
	for (int i=0;i<2;i++)
	{
		part2 = (((*data < 16)?"0":"") + ToHex<unsigned short>((unsigned short)*data)) + part2;
		data++;
	}
	std::string part3 = "";
	data = (unsigned char*)(&guid.Data3);
	for (int i=0;i<2;i++)
	{
		part3 = (((*data < 16)?"0":"") + ToHex<unsigned short>((unsigned short)*data)) + part3;
		data++;
	}
	std::string part4 = "";
	data = (unsigned char*)(&guid.Data4);
	for (int i=0;i<2;i++)
	{
		part4 += (((*data < 16)?"0":"") + ToHex<unsigned short>((unsigned short)*data));
		data++;
	}
	std::string part5 = "";
	for (int i=0;i<6;i++)
	{
		part5 += (((*data < 16)?"0":"") + ToHex<unsigned short>((unsigned short)*data));
		data++;
	}
	return '{'+part1 + '-' + part2 + '-' + part3 +'-' + part4 + '-' + part5 + '}';
}

GUID StringToGUID(const std::string &str)
{
	GUID result;
	memset((void*)&result,0,sizeof(result));
	if ( str.length()!=38)
		return result;
	result.Data1 = HexToInt<unsigned long>(str.substr(1,8));
	result.Data2 = HexToInt<unsigned short>(str.substr(10,4));
	result.Data3 = HexToInt<unsigned short>(str.substr(15,4));
	unsigned char* dataPtr = (unsigned char*)&result.Data4;
	*(dataPtr++) = (unsigned char)HexToInt<unsigned short>(str.substr(20,2));
	*(dataPtr++) = (unsigned char)HexToInt<unsigned short>(str.substr(22,2));
	*(dataPtr++) = (unsigned char)HexToInt<unsigned short>(str.substr(25,2));
	*(dataPtr++) = (unsigned char)HexToInt<unsigned short>(str.substr(27,2));
	*(dataPtr++) = (unsigned char)HexToInt<unsigned short>(str.substr(29,2));
	*(dataPtr++) = (unsigned char)HexToInt<unsigned short>(str.substr(31,2));
	*(dataPtr++) = (unsigned char)HexToInt<unsigned short>(str.substr(33,2));
	*(dataPtr++) = (unsigned char)HexToInt<unsigned short>(str.substr(35,2));
	return result;
}