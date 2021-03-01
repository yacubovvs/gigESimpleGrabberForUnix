/*

This sample illustrates how to use the PylonDeviceGrabSingleFrame() convenience
method for grabbing images in a loop. PylonDeviceGrabSingleFrame() grabs one
single frame in single frame mode.

Grabbing in single frame acquisition mode is the easiest way to grab images. Note: in single frame
mode the maximum frame rate of the camera can't be achieved. The full frame
rate can be achieved by setting the camera to the continuous frame acquisition
mode and by grabbing in overlapped mode, i.e., image acquisition is done in parallel
with image processing. This is illustrated in the OverlappedGrab sample program.

*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include "bmp.h"
#include "bmp.c"

#include <pylonc/PylonC.h>

/* Simple error handling. */
#define CHECK( errc ) if ( GENAPI_E_OK != errc ) printErrorAndExit( errc )

/* This function can be used to wait for user input at the end of the sample program. */
void pressEnterToExit(void);

/* This method demonstrates how to retrieve the error message
   for the last failed function call. */
void printErrorAndExit( GENAPIC_RESULT errc );

/* Calculating the minimum and maximum gray value of an image buffer */
void getMinMax( const unsigned char* pImg, int32_t width, int32_t height,
               unsigned char* pMin, unsigned char* pMax);

void showUsageOnError(char *error){
	printf("\n");
	printf(error);
	printf("\n\n");
	printf("Usage:\n");
	printf("-s  Camera serial number\n");
	printf("-o  Path to output file\n");
	printf("-d  Inner package delay in ticks (default 50)\n");
	printf("-a  Attempts tо grab image (default 1)\n");
	printf("-p  Package size (default 1500)\n");
	printf("-e  Exposure time (default 35000)\n");
	printf("-t  Patch to sample file");
	//printf("-f  Image format [BMP|PNG|JPG|RAW|TIFF]\n");
	printf("\n");
}

int main(int argc, char* argv[])
{
    GENAPIC_RESULT          res;           /* Return value of pylon methods. */
    size_t                  numDevices;    /* Number of available devices. */
    PYLON_DEVICE_HANDLE     hDev;          /* Handle for the pylon device. */
    const int               numGrabs = 1; /* Number of images to grab. */
    size_t                  payloadSize=0;   /* Size of an image frame in bytes. */
    unsigned char*          imgBuf;        /* Buffer used for grabbing. */
    _Bool                    isAvail;
    int                     i;

    /* Before using any pylon methods, the pylon runtime must be initialized. */
    PylonInitialize();


    char *cameraSerialNumber 	= "";
    char *pathToFile 			= "/home/vitaly/Grabber/out.bmp";
    char *interPackegeDelay 	= "50";
    char *attemptsToGrab 		= "1";
    char *packageSize 			= "1500";
	char *exposureTime 			= "35000";
    char *imageFormat 			= "BMP";
    char *testImage 			= "";

    //"/home/vitaly/Grabber/image.bmp", "rb");
	//FILE* file_out  = fopen("/home/vitaly/Grabber/out.bmp", "wb");


    //printf( "App start!\n" );

    for(unsigned char i=1; i<argc-1; i++){

    	if (!strcmp(argv[i], "-s")){
    		cameraSerialNumber = argv[i+1];
    	}else if (!strcmp(argv[i], "-o")){
    		pathToFile = argv[i+1];
    	}else if (!strcmp(argv[i], "-d")){
    		interPackegeDelay = argv[i+1];
    	}else if (!strcmp(argv[i], "-a")){
    		attemptsToGrab = argv[i+1];
    	}else if (!strcmp(argv[i], "-p")){
    		packageSize = argv[i+1];
    	}else if (!strcmp(argv[i], "-e")){
    		exposureTime = argv[i+1];
    	}else if (!strcmp(argv[i], "-f")){
    		imageFormat = argv[i+1];
    	}else if (!strcmp(argv[i], "-t")){
    		testImage = argv[i+1];
    	}

    }

    if (!strcmp(cameraSerialNumber, "")){
    	showUsageOnError("Error: serial number is not set");
    }

    if (!strcmp(testImage, "")){
    	showUsageOnError("Path to sample image should not be empty\n(needed to catch bmp header from file)\nJust make a photo via Basler Pylon software and save it to disk like a bhmp image \n\n");
    }



    res = PylonEnumerateDevices( &numDevices );
    CHECK(res);
    if ( 0 == numDevices )
    {
        fprintf( stderr, "No devices found!\n" );
        /* Before exiting a program, PylonTerminate() should be called to release
           all pylon related resources. */
        PylonTerminate();
        pressEnterToExit();
        exit(EXIT_FAILURE);
    }

    bool deviceFound = false;
    for (uint di = 0; di < numDevices; di++) {

    	res = PylonCreateDeviceByIndex( di, &hDev );
		CHECK(res);

		res = PylonDeviceOpen( hDev, PYLONC_ACCESS_MODE_CONTROL | PYLONC_ACCESS_MODE_STREAM );
		CHECK(res);


			char buf[256];
			size_t siz = sizeof(buf);
			_Bool isReadable;

			isReadable = PylonDeviceFeatureIsReadable(hDev, "DeviceID");
			if ( isReadable )
			{
				res = PylonDeviceFeatureToString( hDev, "DeviceID", buf, &siz );
				CHECK(res);

				if (!strcmp(cameraSerialNumber, buf)){
					deviceFound = true;
					break;
				}
			}

    }

    int int_interPackegeDelay 		= atoi(interPackegeDelay);
    int int_packageSize				= atoi(packageSize);
    int int_exposureTime 			= atoi(exposureTime);
    int int_attemptsToGrab 			= atoi(attemptsToGrab);

    if(int_packageSize<96 || int_packageSize>9216){
    	showUsageOnError("Package size should be in [96..9216]\n\n");
    	return 0;
    }

    if(int_interPackegeDelay<0 || int_interPackegeDelay>1015){
		showUsageOnError("Inner packet delay should be in [0..1015]");
		return 0;
	}

    if(!deviceFound){
    	printf("Camera not found by serial number\n");
    	return 0;
    }

    /* Set the pixel format to Mono8 if available, where gray values will be output as 8 bit values for each pixel. */
    isAvail = PylonDeviceFeatureIsAvailable(hDev, "EnumEntry_PixelFormat_Mono8");
    if (isAvail)
    {
        res = PylonDeviceFeatureFromString(hDev, "PixelFormat", "Mono8" );
        CHECK(res);
    }

    /* Disable acquisition start trigger if available */
    isAvail = PylonDeviceFeatureIsAvailable( hDev, "EnumEntry_TriggerSelector_AcquisitionStart");
    if (isAvail)
    {
        res = PylonDeviceFeatureFromString( hDev, "TriggerSelector", "AcquisitionStart");
        CHECK(res);
        res = PylonDeviceFeatureFromString( hDev, "TriggerMode", "Off");
        CHECK(res);
    }

    /* Disable frame burst start trigger if available. */
    isAvail = PylonDeviceFeatureIsAvailable( hDev, "EnumEntry_TriggerSelector_FrameBurstStart");
    if (isAvail)
    {
        res = PylonDeviceFeatureFromString( hDev, "TriggerSelector", "FrameBurstStart");
        CHECK(res);
        res = PylonDeviceFeatureFromString( hDev, "TriggerMode", "Off");
        CHECK(res);
    }

    /* Disable frame start trigger if available */
    isAvail = PylonDeviceFeatureIsAvailable( hDev, "EnumEntry_TriggerSelector_FrameStart");
    if (isAvail)
    {
        res = PylonDeviceFeatureFromString( hDev, "TriggerSelector", "FrameStart");
        CHECK(res);
        res = PylonDeviceFeatureFromString( hDev, "TriggerMode", "Off");
        CHECK(res);
    }

    isAvail = PylonDeviceFeatureIsWritable(hDev, "GevSCPSPacketSize");
    if ( isAvail )
    {
        res = PylonDeviceSetIntegerFeature(hDev, "GevSCPSPacketSize", int_packageSize );
        CHECK(res);
    }


    isAvail = PylonDeviceFeatureIsWritable(hDev, "ExposureTimeAbs");
	if ( isAvail )
	{
		PylonDeviceFeatureFromString(hDev, "ExposureAuto", "Off");
		res = PylonDeviceSetFloatFeature(hDev, "ExposureTimeAbs", (float)int_exposureTime );
		CHECK(res);
	}

	isAvail = PylonDeviceFeatureIsWritable(hDev, "GevSCPD");
	if ( isAvail )
	{
		res = PylonDeviceSetIntegerFeature(hDev, "GevSCPD", int_interPackegeDelay);
		CHECK(res);
	}

    /* Determine the required size of the grab buffer. */
    
    {
        PYLON_STREAMGRABBER_HANDLE  hGrabber;
        /* Temporary create and open a stream grabber for the first channel. */
        res = PylonDeviceGetStreamGrabber( hDev, 0, &hGrabber );
        CHECK(res);
        res = PylonStreamGrabberOpen( hGrabber );
        CHECK(res);

        res = PylonStreamGrabberGetPayloadSize( hDev, hGrabber, &payloadSize );
        CHECK(res);

        res = PylonStreamGrabberClose( hGrabber );
        CHECK(res);
    }

    /* Allocate memory for grabbing. */
    imgBuf = (unsigned char*) malloc( payloadSize );
    if ( NULL == imgBuf )
    {
        fprintf( stderr, "Out of memory.\n" );
        PylonTerminate();
        pressEnterToExit();
        exit(EXIT_FAILURE);
    }

    /* Grab some images in a loop. */
    for ( i = 0; i < int_attemptsToGrab; ++i )
    {
        unsigned char min, max;
        PylonGrabResult_t grabResult;
        _Bool bufferReady;

        res = PylonDeviceGrabSingleFrame( hDev, 0, imgBuf, payloadSize,
            &grabResult, &bufferReady, 10000 );

        //PylonImagePersistenceOptions_t* pOptions);

        if ( GENAPI_E_OK == res && !bufferReady )
        {
            /* Timeout occurred. */
            printf("Frame %d: timeout\n", i+1);
        }
        CHECK(res);

        /* Check to see if the image was grabbed successfully. */
        if ( grabResult.Status == Grabbed )
        {
            /* Success. Perform image processing. */
            getMinMax( imgBuf, grabResult.SizeX, grabResult.SizeY, &min, &max );
            printf("Grabbed frame #%2d. Min. gray value = %3u, Max. gray value = %3u\n", i+1, min, max);

#ifdef GENAPIC_WIN_BUILD
            /* Display image */
            res = PylonImageWindowDisplayImageGrabResult(0, &grabResult);
            CHECK(res);
#endif


            /*
            FILE* file1     = fopen(testImage, "rb");

            */
            //FILE* file1     = fopen("/home/vitaly/Grabber/image.bmp", "rb");
            //FILE* file_out  = fopen("/home/vitaly/Grabber/out.bmp", "wb");
            FILE* file1     = fopen(testImage, "rb");
            FILE* file_out  = fopen(pathToFile, "wb");

            /*
            BITMAPFILEHEADER header1 = readHeader(file1);
			BITMAPINFOHEADER bmiHeader1 = readBmiHeader(file1);

			writeBmiHeader(header1, file_out);
			writeHeader(bmiHeader1, file_out);
			*/

			//imgBuf = (unsigned char*) malloc( payloadSize ); unsigned char*
			for(long i=0; i<1078; i++){
				putc(getc(file1),file_out);
			}
			/*
			for(long i=0; i<payloadSize; i++){
				//imgBuf[i] -= 64;
				//fwrite(imgBuf[i], 1, 1, file_out);
				write_u8(file_out, imgBuf[i]);
			}*/

			unsigned char t;

			//grabResult.SizeY
			for(int i_y=0; i_y<grabResult.SizeY; i_y++){
				for(int i=0; i<grabResult.SizeX/2; i++){
					t = imgBuf[grabResult.SizeX*i_y + i];                  // Swapping
					imgBuf[grabResult.SizeX*i_y + i] = imgBuf[grabResult.SizeX*i_y + grabResult.SizeX-i];
					imgBuf[grabResult.SizeX*i_y + grabResult.SizeX-i] = t;
				}
			}

			fwrite((unsigned char*)imgBuf, 1, payloadSize, file_out);

			fclose(file1);
			fclose(file_out);

			break;
        }
        else if ( grabResult.Status == Failed )
        {
            fprintf( stderr,  "Frame %d wasn't grabbed successfully.  Error code = 0x%08X\n",
                i+1, grabResult.ErrorCode );
        }
    }
    /* Clean up. Close and release the pylon device. */

    res = PylonDeviceClose( hDev );
    CHECK(res);
    res = PylonDestroyDevice ( hDev );
    CHECK(res);

    /* Free memory for grabbing. */
    free( imgBuf );

    //pressEnterToExit();

    /* Shut down the pylon runtime system. Don't call any pylon method after
       calling PylonTerminate(). */
    PylonTerminate();

    return EXIT_SUCCESS;
}

/* This function demonstrates how to retrieve the error message for the last failed
   function call. */
void printErrorAndExit( GENAPIC_RESULT errc )
{
    char *errMsg;
    size_t length;

    /* Retrieve the error message.
    ... Find out first how big the buffer must be, */
    GenApiGetLastErrorMessage( NULL, &length );
    errMsg = (char*) malloc( length );
    /* ... and retrieve the message. */
    GenApiGetLastErrorMessage( errMsg, &length );

    fprintf( stderr, "%s (%#08x).\n", errMsg, (unsigned int) errc);
    free( errMsg);

    /* Retrieve more details about the error.
    ... Find out first how big the buffer must be, */
    GenApiGetLastErrorDetail( NULL, &length );
    errMsg = (char*) malloc( length );
    /* ... and retrieve the message. */
    GenApiGetLastErrorDetail( errMsg, &length );

    fprintf( stderr, "%s\n", errMsg);
    free( errMsg);

    PylonTerminate();  /* Releases all pylon resources. */
    pressEnterToExit();

    exit(EXIT_FAILURE);
}



/* Simple "image processing" function returning the minimum and maximum gray
   value of an 8 bit gray value image. */
void getMinMax( const unsigned char* pImg, int32_t width, int32_t height,
               unsigned char* pMin, unsigned char* pMax)
{
    unsigned char min = 255;
    unsigned char max = 0;
    unsigned char val;
    const unsigned char *p;

    for ( p = pImg; p < pImg + width * height; p++ )
    {
        val = *p;
        if ( val > max )
           max = val;
        if ( val < min )
           min = val;
    }
    *pMin = min;
    *pMax = max;
}

/* This function can be used to wait for user input at the end of the sample program. */
void pressEnterToExit(void)
{
    fprintf( stderr, "\nPress enter to exit.\n");
    while( getchar() != '\n');
}

