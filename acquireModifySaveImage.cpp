//
// initialize camera
// assign pointer to camera image buffer to Mat data type
// call imwrite to save raw image
// call imshow to display raw image
// call blur to apply smoothing filter to image
// call imshow to display filtered image
// save blurred image in separate file
//

#include <iostream>
#include <string.h>
#include <cstdio>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <uEye.h>

using namespace std;
using namespace cv;

// camera configuration parameters
const int img_width = 2048;
const int img_height = 1536;
int img_bpp = 24;
const HIDS hCam = 1;
int masterGain = 1;
int redGain = 4;
int greenGain = 1;
int blueGain = 6;
double exposure = 98.0;
void *pMemVoid = NULL;      // pointer to where the image is stored
char *imgMem = NULL;
int memId;
double disable = 0.0;

void configureCamera(HIDS hCam, char *imgMem, int memId)
{
    if (is_InitCamera(&hCam, NULL) != IS_SUCCESS){
        cout << "could not get camera handle..." << endl;
    }

    // memory initialization
    is_AllocImageMem(hCam, img_width, img_height, img_bpp, &imgMem, &memId);

    // set memory active
    is_SetImageMem(hCam, imgMem, memId);

    // set display mode
    is_SetDisplayMode(hCam, IS_SET_DM_DIB);

    // disable auto parameters
    is_SetAutoParameter(hCam, IS_SET_ENABLE_AUTO_WHITEBALANCE, &disable, 0);
    is_SetAutoParameter(hCam, IS_SET_ENABLE_AUTO_FRAMERATE, &disable, 0);
    is_SetAutoParameter(hCam, IS_SET_ENABLE_AUTO_SHUTTER, &disable, 0);
    is_SetAutoParameter(hCam, IS_SET_ENABLE_AUTO_SENSOR_GAIN, &disable, 0);
    is_SetAutoParameter(hCam, IS_SET_ENABLE_AUTO_GAIN, &disable, 0);
    is_SetAutoParameter(hCam, IS_SET_ENABLE_AUTO_SENSOR_WHITEBALANCE, &disable, 0);
    is_SetAutoParameter(hCam, IS_SET_ENABLE_AUTO_SENSOR_SHUTTER, &disable, 0);

    // set the gain values for manual white balance
    is_SetHardwareGain(hCam, masterGain, redGain, greenGain, blueGain);

    // disable IR color correction
    double factors = 0.0;
    is_SetColorCorrection(hCam, IS_CCOR_DISABLE, &factors);

    // enable auto blacklevel
    uint8_t nMode = IS_AUTO_BLACKLEVEL_OFF;
    is_Blacklevel(hCam, IS_BLACKLEVEL_CMD_SET_MODE, (void*)&nMode , sizeof(nMode ));

    // set auto black level offset
    uint8_t nOffset = 127;
    is_Blacklevel(hCam, IS_BLACKLEVEL_CMD_SET_OFFSET, (void*)&nOffset, sizeof(nOffset));

}

int main (void)
{
	cout << "initializing camera..." << endl;

//
    // configure cameras
    //
    configureCamera(hCam, imgMem, memId);
  
    // adjust the gain values for to balance right and left cameras
    is_SetHardwareGain(hCam, masterGain, redGain, greenGain, blueGain);

    //
    // set the exposure time in msec
    //
    is_Exposure(hCam, IS_EXPOSURE_CMD_SET_EXPOSURE,
                        (void*)&exposure, sizeof(exposure));
    
    //
    // create the image matrices and display windows
    //
    Mat rawImgMat = Mat(Size(img_width, img_height), CV_8UC3);
	Mat resizeRawImgMat = Mat(rawImgMat);
	const string& rawImgWinName("Raw Image");
    namedWindow(rawImgWinName, WINDOW_NORMAL | WINDOW_AUTOSIZE);
	Mat blurImgMat = Mat(Size(img_width, img_height), CV_8UC3);
	Mat resizeBlurImgMat = Mat(blurImgMat);
	const string& blurImgWinName("Blur Image");
    namedWindow(blurImgWinName, WINDOW_NORMAL | WINDOW_AUTOSIZE);

	//
    // link camera image buffer to OpenCV matrix
    //
    is_GetImageMem(hCam, &pMemVoid);    		// get pointer to the image buffer
    rawImgMat.data = (uchar *)pMemVoid;   		// assign the pointer to Matrix
    
    // start acquisition loop
    uchar waitRet = -1;
    while(1) {
        if (is_FreezeVideo(hCam, IS_WAIT) == IS_SUCCESS){
			//
			// shrink and dsiplay the raw image
			//
			resize(rawImgMat, resizeRawImgMat, Size(), .25, .25, CV_INTER_AREA);
            imshow(rawImgWinName, resizeRawImgMat);

			//
			// create the blurred image, shrink it and display it
			//
			blur(rawImgMat, blurImgMat, Size(9, 9));
			resize(blurImgMat, resizeBlurImgMat, Size(), .25, .25, CV_INTER_AREA);
			imshow(blurImgWinName, resizeBlurImgMat);

			//
            // buld up jpeg image data and write to file
            //
            vector<int> compression_params;
            compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
            compression_params.push_back(95);
            try {
            	imwrite("/home/ubuntu/Desktop/raw_image.jpg", resizeRawImgMat, compression_params);
            }
            catch (Exception& ex) {
                fprintf(stderr, "Exception converting image to JPG format: %s\n", ex.what());
                return 1;
            }
        	
        	//
        	// check if escape key pressed to terminate...
        	//
        	waitRet = waitKey(1);
        	if (waitRet == 27) {
            	break;
        	}
		}
    }
    destroyAllWindows();
    is_FreeImageMem(hCam, imgMem, memId);
    is_ExitCamera(hCam);

    return 0;
} 
