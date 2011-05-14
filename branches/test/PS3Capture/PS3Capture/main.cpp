//! Main cpp file

#include "PS3.h"
#include <iostream>
#include <stdio.h>


int main( int argc, char** ) {
	int camCount = PS3::GetCameraCount();

	if ( camCount > 0 ) {
		printf( "%d PS3 camera%s found!\n", camCount, camCount > 1 ? "s" : "" );
	} else {
		printf( "No PS3 cameras found!\n" );
		return -1;
	}

	for ( int i = 0; i < camCount; i++ ) {
		GUID guid = PS3::GetGUID( i );
		std::cout << "GUID #" << (i+1) << ": "
			<< PS3::GUID2String( guid, '-', true ) << std::endl;
	}

	return 0;
}