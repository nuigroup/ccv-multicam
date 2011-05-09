#include <cassert>
#include <cstdio>
#include "ofxVideoWriter.h"

//
// Extra temp data bytes.
//
#define EXTRA_TEMP_SPACE 10000

ofxVideoWriter::ofxVideoWriter()
{
   m_pavifile		= NULL;
   m_pstream		= NULL;
   m_pstreamcompressed	= NULL;
   m_pTempBMPBuffer	= NULL;
   m_pTempBuffer	= NULL;
   m_pBitmapInfo        = NULL;

   m_iTimeIndex	     = 0;

   m_iCols     = 0;
   m_iRows     = 0;
   m_iBPP      = 0;
   m_iSize     = 0;
   m_iRowInc   = 0;
   m_frameRate = 0.0;

   m_bSizeLimited = false;
   m_iSplitFile = 0;   

   // Default compression options.
   m_compvars.fccHandler = mmioFOURCC( 'D', 'I', 'B', ' ' );
   m_compvars.dwFlags    = 0x0;
   m_compvars.lQ         = ICQUALITY_DEFAULT;
      
   m_compvars.lpState = NULL;

   m_liBytesWritten = 0;
}


ofxVideoWriter::~ofxVideoWriter()
{   
   close();

   if( m_pBitmapInfo != NULL )
   {
      free( m_pBitmapInfo );
      m_pBitmapInfo = NULL;
   }

   if( m_pTempBuffer != NULL )
   {
      delete [] m_pTempBuffer;
      m_pTempBuffer = NULL;
   }
   
   if( m_pTempBMPBuffer != NULL )
   {
      delete [] m_pTempBMPBuffer;
      m_pTempBMPBuffer = NULL;
   }

   if( m_compvars.lpState != NULL )
   {
      delete [] m_compvars.lpState;
      m_compvars.lpState = NULL;
   }
}


bool	 
ofxVideoWriter::openSizeLimitedAVI( 
		 const char*  pszFilename, 
		 int	      iCols, 
		 int	      iRows, 
		 int	      ibpp, 
		 double	      dFramerate )
{
   char szAVIFile[ _MAX_PATH ];

   m_bSizeLimited = true;
   m_iSplitFile = 0;

   sprintf(szAVIFile, "%s", pszFilename );

   // Remove any extensions.
   char * lastBackslash = strrchr( szAVIFile, '\\');
   if ( lastBackslash!=NULL){
      char* lastPeriod = strrchr( szAVIFile, '.' );
      if( lastPeriod != NULL && lastPeriod > lastBackslash)
      {
         *( lastPeriod ) = 0x0;
      }
   }
   else
   {
      char* lastPeriod = strrchr( szAVIFile, '.' );
      if( lastPeriod != NULL)
      {
         *( lastPeriod ) = 0x0;
      }
   }

   //
   // Keep the basic avi file name in m_szAVIDestFile.
   //
   sprintf(m_szAVIDestFile, "%s", szAVIFile );

   //
   // The avi file names will be in the form  of ***-0000.avi, ***-0001.avi,
   // ***-0002.avi...
   // The size of each avi file is limited by AVI_FILE_SPLIT_SIZE bytes
   //
   sprintf(szAVIFile, "%s-%04d.avi", m_szAVIDestFile, m_iSplitFile );
   return open( szAVIFile, iCols, iRows, ibpp, dFramerate );
}

bool	 
ofxVideoWriter::open( 
		 const char*  pszFilename, 
		 int	      iCols, 
		 int	      iRows, 
		 int	      ibpp, 
		 double	      dFramerate )
{
   //
   // Initialize the library.
   //
   ::AVIFileInit();

   if( !checkVFWVersion() )
   {
      return false;
   } 

   if( pszFilename == NULL || iRows <= 0 || iCols <= 0 || dFramerate <= 0.0 )
   {
      assert( false );
      return false;
   }

   if( ibpp != 24 && ibpp != 32 && ibpp != 8 && ibpp != 16)
   {
      assert( false );
      return false;
   }

   m_frameRate = dFramerate;
   m_iTimeIndex	= 0;
   m_liBytesWritten = 0;
   //
   // If these parameters change, set them and allocate the buffers
   //
   if ( m_iCols != iCols || m_iRows != iRows || m_iBPP  != ibpp ) 
   {
      //
      // Initialize members.
      //
      m_iCols     = iCols;
      m_iRows     = iRows;
      m_iBPP      = ibpp;
      m_iRowInc   = m_iCols * ( m_iBPP / 8 );
      m_iSize     = m_iRows * m_iRowInc;
      //
      // Allocate temp image buffers.
      //
      if( m_pTempBMPBuffer != NULL )
      {
         delete [] m_pTempBMPBuffer;
         m_pTempBMPBuffer = NULL;
      }
      m_pTempBMPBuffer  = new unsigned char[ m_iSize + EXTRA_TEMP_SPACE ];

      if( m_pTempBuffer != NULL )
      {
         delete [] m_pTempBuffer;
         m_pTempBuffer = NULL;
      }
      m_pTempBuffer = new unsigned char[ m_iSize ];
   }

   if ( m_pTempBMPBuffer == NULL || m_pTempBuffer == NULL ) 
      return false;

   //
   // If this function is called from openSizeLimitedAVI(), pszFilename 
   // is the file to open. m_szAVIDestFile should not be changed because
   // it is the base file name used in appendFrame() to generate the next 
   // split file name.
   //
   // If this function is called directly (not by openSizeLimitedAVI()),
   // the AVI file can not be splited and m_szAVIDestFile is not used. 
   // So we do not need to save the file name to m_szAVIDestFile.
   // 
#if 0  
   sprintf(m_szAVIDestFile, "%s", pszFilename );
   
   // Remove any extensions.
   char* pcExt = strstr( m_szAVIDestFile, "." );
   if( pcExt != NULL )
   {
      *( pcExt ) = 0x0;
   }
   
   // add an avi extension so that the file can be opened
   char szAVIFile[ _MAX_PATH ];
   sprintf(szAVIFile, "%s.avi", m_szAVIDestFile);
#endif
   
   //
   // Open the file.
   //
   HRESULT hr = ::AVIFileOpen( 
      &m_pavifile, pszFilename, OF_WRITE | OF_CREATE, NULL );
   
   if( hr != AVIERR_OK )
   {
      assert( false );
      return false;
   }   

   //
   // Create the bitmap info structure for our avi.
   //
   if ( m_iBPP == 8 )
   {
      // we'll need room for the colour palette.

      m_pBitmapInfo = 
	 (BITMAPINFO*)malloc( sizeof( BITMAPINFO ) + sizeof( RGBQUAD ) * 256 );

      //
      // assume that it's greyscale for now.
      //
      for( int i = 0; i < 256; i++ )
      {
      	 m_pBitmapInfo->bmiColors[i].rgbBlue     = (unsigned char)i;
      	 m_pBitmapInfo->bmiColors[i].rgbGreen    = (unsigned char)i;
      	 m_pBitmapInfo->bmiColors[i].rgbRed      = (unsigned char)i;
      	 m_pBitmapInfo->bmiColors[i].rgbReserved = (unsigned char)0;
      }

   }
   else
   {
      m_pBitmapInfo = (BITMAPINFO*)malloc( sizeof( BITMAPINFO ) );
   }

   ::memset( m_pBitmapInfo, 0x0, sizeof( BITMAPINFOHEADER ) );

   BITMAPINFOHEADER* pbmiHeader = &m_pBitmapInfo->bmiHeader;

   pbmiHeader->biBitCount        = (unsigned short)m_iBPP;
   pbmiHeader->biHeight	         = m_iRows,
   pbmiHeader->biWidth	         = m_iCols,
   pbmiHeader->biSizeImage       = m_iSize;
   pbmiHeader->biCompression     = BI_RGB;
   pbmiHeader->biXPelsPerMeter   = 100;
   pbmiHeader->biYPelsPerMeter   = 100;
   pbmiHeader->biPlanes	         = 1;
   pbmiHeader->biClrUsed	 = 0;
   pbmiHeader->biClrImportant    = 0;
   pbmiHeader->biSize            = sizeof( BITMAPINFOHEADER );   

   AVISTREAMINFO  streaminfo;
   ::memset( &streaminfo, 0x0, sizeof( AVISTREAMINFO ) );

   streaminfo.fccType		    = streamtypeVIDEO;
   streaminfo.fccHandler	    = m_compvars.fccHandler;
   streaminfo.dwScale		    = 100;
   streaminfo.dwRate		    = (int)( dFramerate * 100.0 );
   streaminfo.dwSuggestedBufferSize = m_pBitmapInfo->bmiHeader.biSizeImage;
   ::SetRect( &streaminfo.rcFrame, 0, 0, m_iCols, m_iRows );   
   streaminfo.dwQuality             = m_compvars.lQ;

   hr = ::AVIFileCreateStream( m_pavifile, &m_pstream, &streaminfo );
   if( hr != AVIERR_OK )
   {
      assert( false );
      return false;
   }

   //
   // Setup the compression options.
   //
   AVICOMPRESSOPTIONS compopts;

   if ( m_iBPP == 8 )
   {
	   compopts.fccHandler = mmioFOURCC( 'M', 'R', 'L', 'E' );
	   compopts.fccType    = streamtypeVIDEO;
	   compopts.dwQuality  = 10000; 
	   compopts.cbParms    = m_compvars.cbState;
	   compopts.lpParms    = m_compvars.lpState;
   } else {
	   compopts.fccHandler = m_compvars.fccHandler;
	   compopts.fccType    = streamtypeVIDEO;
	   compopts.dwQuality  = m_compvars.lQ; 
	   compopts.cbParms    = m_compvars.cbState;
	   compopts.lpParms    = m_compvars.lpState;
   }

   hr = ::AVIMakeCompressedStream( &m_pstreamcompressed, m_pstream, &compopts, NULL );
   if( hr != AVIERR_OK )
   {
      assert( false );
      return false;
   }   

   hr = ::AVIStreamSetFormat(      
      m_pstreamcompressed, 0,  m_pBitmapInfo, 
      m_pBitmapInfo->bmiHeader.biSize );

   if( hr != AVIERR_OK )
   {
      assert( false );      
      return false;
   }
   
   return true;
}


bool	 
ofxVideoWriter::open( 
                 const char*  pszFilename, 
                 int	      iCols, 
                 int	      iRows, 
                 int	      ibpp, 
                 int	      iFramerate )
{
   return open( pszFilename, iCols, iRows, ibpp, (double)iFramerate );
}


bool
ofxVideoWriter::open( char* pszFilename )
{
   //
   // Initialize the library.
   //
   ::AVIFileInit();

   if( pszFilename == NULL )
   {
      return false;
   }

   HRESULT hr = ::AVIFileOpen( &m_pavifile, pszFilename, OF_READ, NULL );
   if( hr != AVIERR_OK )
   {
      assert( false );
      return false;
   }


   hr = ::AVIFileGetStream( 
      m_pavifile, &m_pstreamcompressed, streamtypeVIDEO, 0 );
   if( hr != AVIERR_OK )
   {
      assert( false );
      return false;
   }

   BITMAPINFOHEADER  bi;
   LONG              lStreamSize;

   ::AVIStreamFormatSize( m_pstreamcompressed, 0, &lStreamSize); 
   assert( lStreamSize == sizeof( bi ) );
   
   lStreamSize = sizeof( bi ); 
   hr = ::AVIStreamReadFormat( m_pstreamcompressed, 0, &bi, &lStreamSize );
   assert( bi.biCompression == BI_RGB );
   
   AVISTREAMINFO    streaminfo; 
   hr = ::AVIStreamInfo( m_pstreamcompressed, &streaminfo, sizeof( streaminfo ) ); 
   if( hr != AVIERR_OK )
   {
      assert( false );
      return false;
   }

   assert( streaminfo.fccType == streamtypeVIDEO );

   m_iCols     = bi.biWidth;
   m_iRows     = bi.biHeight;
   m_iBPP      = bi.biBitCount;
   m_iRowInc   = m_iCols * ( m_iBPP / 8 );
   m_iSize     = m_iRows * m_iRowInc;

   m_iTimeIndex	     = 0;

   m_pTempBuffer = new unsigned char[ m_iSize ];


   return true;
}

long int
ofxVideoWriter::bytesWritten()
{
   return m_liBytesWritten;
   //return m_iTimeIndex * m_iSize;
}

bool	 
ofxVideoWriter::appendFrame( const unsigned char* pBuffer, bool bInvert )
{
   if( m_pavifile == NULL || m_pstream == NULL )
   {
      assert( false );
      return false;
   }

   const unsigned char* pWriteBuffer = pBuffer;

   if( bInvert )
   {
      unsigned char* pDest = m_pTempBuffer;
      for( int iRow = m_iRows - 1; iRow >= 0; iRow-- )
      {
         ::memcpy( pDest, &pBuffer[ iRow * m_iRowInc ], m_iRowInc );
         pDest += m_iRowInc;
      }

      pWriteBuffer = m_pTempBuffer;
   }

   LONG lSamplesWritten;
   LONG lBytesWritten;

   //
   // If the AVI file is opened with openSizeLimitedAVI(), split it if necessory.
   //
   if ( m_bSizeLimited )
   {
      //
      // Split the file if the total size is more than 'AVI_FILE_SPLIT_SIZE' bytes
      //
      if ( m_liBytesWritten  >= (AVI_FILE_SPLIT_SIZE - m_iSize) )
      {
         char szAVIFile[ _MAX_PATH ];
         close();
         m_iSplitFile++;
         sprintf(szAVIFile, "%s-%04d.avi", m_szAVIDestFile, m_iSplitFile );
         if( !open( szAVIFile, m_iCols, m_iRows, m_iBPP, m_frameRate ) )
            return false;
      }
   } 
  
   HRESULT hr = ::AVIStreamWrite(
      m_pstreamcompressed,
      m_iTimeIndex,
      1,
      (LPVOID)pWriteBuffer,
      m_iSize,
      AVIIF_KEYFRAME,
      &lSamplesWritten,
      &lBytesWritten  );

   m_liBytesWritten += lBytesWritten;

   m_iTimeIndex++;

   if( hr != AVIERR_OK )
   {
      errorToString( hr );
      close();
      assert( false );
      return false;
   }

   return true;
}


bool 
ofxVideoWriter::readNextFrame( unsigned char* pBuffer, bool bInvert )
{
   if( bInvert )
   {
      HRESULT hr = ::AVIStreamRead( 
         m_pstreamcompressed, m_iTimeIndex, 1, m_pTempBuffer, m_iSize, NULL, NULL );
      if( hr != AVIERR_OK )
      {
         return false;
      }

      unsigned char* pDest = m_pTempBuffer;
      for( int iRow = m_iRows - 1; iRow >= 0; iRow-- )
      {
         ::memcpy( &pBuffer[ iRow * m_iRowInc ], pDest, m_iRowInc );
         pDest += m_iRowInc;
      }
   }
   else
   {
      HRESULT hr = ::AVIStreamRead( 
         m_pstreamcompressed, m_iTimeIndex, 1, pBuffer, m_iSize, NULL, NULL );
      if( hr != AVIERR_OK )
      {
         return false;
      }
   }

   m_iTimeIndex++;

   return true;
}


bool	 
ofxVideoWriter::close()
{
   if( m_pstream != NULL )
   {
      ::AVIStreamClose( m_pstream );
      m_pstream = NULL;
   }

   if( m_pstreamcompressed != NULL )
   {
      ::AVIStreamClose( m_pstreamcompressed );
      m_pstreamcompressed = NULL;
   }

   if( m_pavifile != NULL )
   {
      ::AVIFileRelease( m_pavifile );
      m_pavifile = NULL;
   }
  
   //
   // Release the library.
   //
   ::AVIFileExit();   

   return true;
}


bool
ofxVideoWriter::appendBMP( const char* pszFilename )
{
   BITMAPFILEHEADER  header;

   FILE* file = ::fopen( pszFilename, "rb" );
   if( file == NULL )
   {
      // file not found.
      assert( false );
      return false;
   }

   //
   // Read and verify the header
   //
   ::fread( &header, sizeof( BITMAPFILEHEADER ), 1, file );

   if( header.bfType != ( (WORD)('M' << 8) | 'B') )
   {
      assert( false );
      return false;
   }

   //
   // Rewind the file
   //
   int result = ::fseek( file, 0, SEEK_SET );
   assert ( result == 0);

   //
   // Read in the entire file.
   //
   assert( header.bfSize <= ( (unsigned)m_iSize + EXTRA_TEMP_SPACE ));

   ::fread( m_pTempBMPBuffer, header.bfSize, 1, file );

   ::fclose( file );

   //
   // Send the data to the .avi.
   //
   if( m_pavifile == NULL || m_pstream == NULL )
   {
      assert( false );
      return false;
   }

   HRESULT hr = ::AVIStreamWrite(
      m_pstreamcompressed,
      m_iTimeIndex,
      1,
      m_pTempBMPBuffer + header.bfOffBits,
      m_iSize,
      AVIIF_KEYFRAME,
      NULL,
      NULL  );

   m_iTimeIndex++;

   if( hr != AVIERR_OK )
   {
      // close file?
      assert( false );
      return false;
   }

   return true;
}


bool 
ofxVideoWriter::checkVFWVersion()
{
   DWORD dwVer = ::VideoForWindowsVersion();

   if( dwVer < 0x010a )
   {
      assert( false );  // vfw version too old.
      return false;
   }

   return true;
}

int
ofxVideoWriter::enumerateCompressors( int  iRows, int iCols, int iBPP, 
                                  ICINFO* picinfo, int iNumICInfo )
{
   // If picinfo is NULL, then we are retreiving the number of
   // usable compressors at the current settings so we can allocate
   // enough memory.
   bool bpicinfoNull = picinfo == NULL;

   ICINFO* picinfoTemp = new ICINFO[ 50 ];

   HIC hic;
   
   BITMAPINFOHEADER bih; 
   
   // Initialize the bitmap structure. 
   bih.biSize          = sizeof( BITMAPINFOHEADER );
   bih.biPlanes        = 1;
   bih.biCompression   = BI_RGB;
   bih.biXPelsPerMeter = 100;
   bih.biYPelsPerMeter = 100;
   bih.biClrUsed       = 0;
   bih.biClrImportant  = 0;
   bih.biWidth         = iCols;
   bih.biHeight        = iRows;
   bih.biBitCount      = (unsigned short)iBPP;
   
   bih.biSizeImage = 
      bih.biWidth * bih.biHeight * ( bih.biBitCount / 8 );

   int  iNumCompressors = 0;
   bool bICInfoFull     = false;
   
   for( int i = 0; ICInfo( 0, i, &picinfoTemp[ i ] ) && !bICInfoFull; i++ ) 
   { 
      // Open the compressor so we can query it.
      hic = ICOpen( picinfoTemp[ i ].fccType, 
         picinfoTemp[ i ].fccHandler, ICMODE_QUERY ); 
      
      if( hic ) 
      { 
         // Skip this compressor if it can't handle the format. 
         if( ICCompressQuery( hic, &bih, 0 ) != ICERR_OK ) 
         { 
            ICClose( hic ); 
            continue; 
         } 
         
         // Find out the compressor info. 
         if( !bpicinfoNull )
         {
            ICGetInfo( hic, &picinfo[ iNumCompressors ], sizeof( ICINFO ) );
         }

         iNumCompressors++;

         if( !bpicinfoNull && (iNumCompressors == iNumICInfo) )
         {
            bICInfoFull = true;
         }
         
         // Close the compressor.
         ICClose( hic ); 
      } 
   } 

   delete [] picinfoTemp;

   return iNumCompressors;
}

void
ofxVideoWriter::setCompressor( COMPVARS* pcompvars )
{   
   if( pcompvars != NULL )
   {
      m_compvars = *pcompvars;
      m_compvars.lpState = new char[ pcompvars->cbState ];
      memcpy( m_compvars.lpState, pcompvars->lpState, pcompvars->cbState );
   }
}

void
ofxVideoWriter::errorToString( HRESULT hrErr )
{
   char* pszErr = new char[ 128 ];

   if( pszErr == NULL )
   {
      assert( false );
      return;
   }

   switch(hrErr) 
   {
   case AVIERR_UNSUPPORTED:  
      strcpy( pszErr, "The operation is not supported." ); 
      break;
   case AVIERR_BADFORMAT:		
      strcpy( pszErr, "The source image format is not acceptable." ); 
      break;
   case AVIERR_MEMORY:			
      strcpy( pszErr, "Not enough memory." ); 
      break;
   case AVIERR_INTERNAL:		
      strcpy( pszErr, "An internal error occurred." ); 
      break;
   case AVIERR_BADFLAGS:		
      strcpy( pszErr, "An invalid flag was specified." ); 
      break;
   case AVIERR_BADPARAM:		
      strcpy( pszErr, "An invalid parameter was specified." ); 
      break;
   case AVIERR_BADSIZE:			
      strcpy( pszErr, "An invalid size was specified." ); 
      break;
   case AVIERR_BADHANDLE:		
      strcpy( pszErr, "The handle is invalid." ); 
      break;
   case AVIERR_FILEREAD:		
      strcpy( pszErr, "Cannot read the file." ); 
      break;
   case AVIERR_FILEWRITE:		
      strcpy( pszErr, "Cannot write to the file." ); 
      break;
   case AVIERR_FILEOPEN:		
      strcpy( pszErr, "Cannot open the file." ); 
      break;
   case AVIERR_COMPRESSOR:		
      strcpy( pszErr, "Compressor error." ); 
      break;
   case AVIERR_NOCOMPRESSOR:		
      strcpy( pszErr, "No compressor." ); 
      break;
   case AVIERR_READONLY:		
      strcpy( pszErr, "File is read only." ); 
      break;
   case AVIERR_NODATA:		
      strcpy( pszErr, "No data." ); 
      break;
   case AVIERR_BUFFERTOOSMALL:		
      strcpy( pszErr, "Buffer to small." ); 
      break;
   case AVIERR_CANTCOMPRESS:		
      strcpy( pszErr, "Can't compress." ); 
      break;
   case AVIERR_USERABORT:			
      strcpy( pszErr, "The operation was aborted by the user." ); 
      break;
   case AVIERR_ERROR:			
      strcpy( pszErr, "An unknown error occurred (may be corrupt data)." ); 
      break;
   default:
      if( hrErr <= AVIERR_ERROR ) 
      {
         strcpy( pszErr,"A codec-specific error occurred." );
      }
      break;
   }

   delete [] pszErr;
}

