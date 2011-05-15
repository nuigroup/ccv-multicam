//! Main cpp file
// Update: 2011-05-14	by Yishi Guo
//         Add the multi-thread capture and show function
// --------------------------------------------------------

#include "PS3.h"
#include <iostream>
#include <stdio.h>

PS3** cams = {NULL};
PS3* pCam = NULL;
int camIndex;
bool hFlip = false;
bool vFlip = false;
bool bShow = false;

//! Print help information
void PrintHelp( int camCount = 1 ) {
	printf( "Help:\n" );
	//! First print "camera selected" section
	for ( int i = 0; i < camCount; ++i ) {
		printf(
		"        %d - Select camera %d\n",
			i, i );
	}

	//! Second print "control" section
	printf(
		"    [ESC] - Exit the program\n"
		"  [SPACE] - Stop display capture image[developing]\n"
		"        h - Horizontal flip\n"
		"        v - Vertical flip\n"
		);
}

//! Select the camera by key pressed
bool SelectCamera( char key, int maxCamCount ) {
	int index = key - 0x30;	//! Convert the value from char to int, '0' = 0x30
	if ( index < maxCamCount ) {
		pCam = cams[index];	//! Set the current camera point
		camIndex = index;	//! Set the global value
		return true;
	}

	return false;	//! Input incorrect
}

int main( int argc, char** ) {
	printf(
		"------------------------------------\n"
		"PS3 Multi-cam capture test project\n"
		"Creator: Yishi Guo\n"
		"------------------------------------\n"
		);

	camIndex = -1;
	int camCount = PS3::GetCameraCount();

	if ( camCount > 0 ) {
		printf( "%d PS3 camera%s found!\n", camCount, camCount > 1 ? "s" : "" );
	} else {
		printf( "No PS3 cameras found!\n" );
		return -1;
	}

	//! Allocate the space
	cams = new PS3*[camCount];

	//! Create and set every PS3 cameras
	for ( int i = 0; i < camCount; i++ ) {
		GUID guid = PS3::GetGUID( i );
		std::cout << "GUID #" << (i+1) << ": "
			<< PS3::GUID2String( guid, '-', true ) << std::endl;

		cams[i] = new PS3();
		//! Set the values for camera
		cams[i]->SetCamera(
			guid,	//!< Camera with this guid you want to create
			CLEYE_COLOR_PROCESSED, //!< Color mode
			CLEYE_VGA,	//!< Resolution of camera VGA(640x480) or QVGA(320x240)
			3	//!< Framerate
			);
		//! Show window to display the capture images
		cams[i]->SetShow( true );

		//cams[i]->SetDebugMode( true );
		//cams[i]->ClearFrameCount();

		//printf( "#%d frame count: %d\n", i, cams[i]->GetFrameCount() );

		if ( cams[i]->StartCamera() ) {
			printf( "#%d: started!\n", i );
		} else {
			printf( "#%d: could not started!\n", i );
		}
	}

	//! print help info
	PrintHelp( camCount );

	int key = 0;

	//! Debug
	//while ( true ) {
	//	Sleep( 1000 );
	//	printf( "%d\n", ++key );
	//	for( int i = 0; i < camCount; ++i ) {
	//		printf( "#%d frame count: %d\n", i, cams[i]->GetFrameCount() );
	//	}
	//}

	while ( ( key = cvWaitKey(0) ) != 0x1b /* ESC key: 27 or 0x1b in hex*/ )	{

		//! Print DEBUG information
		printf( "// DEBUG:\n// \tKey: %d [Hex: %x]\n", key, key );
		for( int i = 0; i < camCount; ++i ) {
			printf( "// \t#%d frame count: %d\n", i, cams[i]->GetFrameCount() );
		}

		//! Select the camera to control
		if ( key >= '0' && key <= '9' ) {
			if ( SelectCamera( key, camCount ) ) {
				printf( "Select Camera #%d\n", camIndex );
			}			
			continue;
		}

		if ( pCam != NULL ) {	//! camera must be selected
			switch ( key ) {
				//! Horizontal flip
				case 'h':	case 'H':
					hFlip = hFlip ? false : true;
					if ( pCam->SetHFlip( hFlip ) ) {
						printf( "Camera #%d Horizontal flip set: %s\n", camIndex, hFlip ? "true" : "false" );
					} else {
						printf( "Camera #%d could not set H Flip\n" );
					}
					break;
				//! Vertical flip
				case 'v':	case 'V':
					vFlip = vFlip ? false : true;
					if ( pCam->SetVFlip( vFlip ) ) {
						printf( "Camera #%d Vertical flip set: %s\n", camIndex, vFlip ? "true" : "flase" );
					} else {
						printf( "Camera #%d could not set V Flip\n" );
					}
					break;
				//! Show or hide the window
				case ' ':	//! Should be fixed, developing in this moment
					bShow = bShow ? false : true;
					if ( pCam->ShowWindow( bShow ) ) {
						printf( "%s camera #%d capture image\n", bShow ? "Show" : "Hide", camIndex );
					} else {
						printf( "Could not %s camera #%d capture image\n", bShow ? "show" : "hide", camIndex );
					}
					break;
				default:
					break;
			}
		}
	}

	for ( int i = 0; i < camCount; ++i ) {
		printf( "Stopping capture on camera %d\n", i );
		//! Stop the camera
		cams[i]->StopCamera();
		delete cams[i];
	}

	return 0;
}