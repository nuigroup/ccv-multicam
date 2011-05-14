//! Main cpp file
// Update: 2011-05-14	by Yishi Guo
//         Add the multi-thread capture and show function
// --------------------------------------------------------

#include "PS3.h"
#include <iostream>
#include <stdio.h>

PS3** cams = {NULL};

void PrintHelp() {
	printf( "Usage:\n"
		"\tESC - Exit the program\n"
		"\t...\n" );
}

int main( int argc, char** ) {
	int camCount = PS3::GetCameraCount();

	if ( camCount > 0 ) {
		printf( "%d PS3 camera%s found!\n", camCount, camCount > 1 ? "s" : "" );
	} else {
		printf( "No PS3 cameras found!\n" );
		return -1;
	}

	//! Allocate the space
	cams = new PS3*[camCount];

	for ( int i = 0; i < camCount; i++ ) {
		GUID guid = PS3::GetGUID( i );
		std::cout << "GUID #" << (i+1) << ": "
			<< PS3::GUID2String( guid, '-', true ) << std::endl;

		cams[i] = new PS3();
		cams[i]->SetCamera( guid, CLEYE_COLOR_PROCESSED, CLEYE_VGA, 30 );
		if ( cams[i]->StartCamera() ) {
			printf( "#%d: started!\n", i );
		} else {
			printf( "#%d: could not started!\n", i );
		}
	}

	PrintHelp();

	int key;
	while ( key = cvWaitKey(0) != 0x1b )	{
		//switch ( key ) {
		//	case ' ':	break;
		//}
	}

	for ( int i = 0; i < camCount; ++i ) {
		printf( "Stopping capture on camera %d\n", i );
		cams[i]->StopCamera();
		delete cams[i];
	}

	return 0;
}