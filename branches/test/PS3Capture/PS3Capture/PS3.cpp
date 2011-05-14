//! PS3.cpp

#include "PS3.h"

PS3::PS3(){

};

PS3::~PS3() {

};

int PS3::GetCameraCount() {
	return CLEyeGetCameraCount();
}

GUID PS3::GetGUID( int camId ) {
	return CLEyeGetCameraUUID( camId );
}

std::string PS3::GUID2String( GUID guid, char delimiter, bool uppercase ) {
	std::stringstream ss;
	if ( uppercase ) {
		ss << std::uppercase;
	} else {
		ss << std::nouppercase;
	}
	ss.fill( '0' );
	ss << std::hex;
	ss.width( 8 );
	ss << guid.Data1 << delimiter;
	ss.width( 4 );
	ss << guid.Data2 << delimiter;
	ss.width( 4 );
	ss << guid.Data3 << delimiter;
	ss.width( 2 );
	ss << (int)guid.Data4[0] << (int)guid.Data4[1] << delimiter
		<< (int)guid.Data4[2] << (int)guid.Data4[3] << (int)guid.Data4[4]
		<< (int)guid.Data4[5] << (int)guid.Data4[6] << (int)guid.Data4[7];

	return ss.str();
}