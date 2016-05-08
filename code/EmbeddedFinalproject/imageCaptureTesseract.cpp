#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <tesseract/baseapi.h>
#include <tesseract/strngs.h>

#include <cstring>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;


//thresholding variables
int threshold_value = 145;
int threshold_type = 0;
int threshold_ImageValue = 100;
int threshold_done = 0;
int const max_value = 255;
int const max_type = 4;
int const max_BINARY_value = 255;
//Image Threshold Value
int ImageThresh = 0;
//thresholding sliders for setup/testing
const char* window2_name = "Threshold";
const char* trackbar_ImageValue = "Image Threshold Value";
const char* trackbar_Done = "0 = not done 1 = done";
const char* trackbar_value = "Value";
//end thresholding sliders
Mat src, src_gray, dst;
IplImage* iplImg2;


// Function Headers
int detectAndDisplay(Mat frame);//Perform Face detection
int DetectFace(void);//returns if face detected
void Threshold( int, void* ); //threshold function header
void findOptimalThreshold(void);//find optimal threshold level for current lighting
STRING GetText(char*); // tesseract function header

// Global variables for Facial Detection
// Copy this file from opencv/data/haarscascades to target folder
string face_cascade_name = "lbpcascade_frontalface.xml";
CascadeClassifier face_cascade;
string window_name = "Capture - Face detection";
int filenumber; // Number of file to be saved
string filename;
int faceDetected = 0;

int main( int argc, const char** argv )
{

    CvCapture* capture = 0;
    Mat frame, frameCopy, image;
    //capture from camera 0 on device
    capture = cvCaptureFromCAM( 0);
    if( !capture )
    {
        cout << "No camera detected" << endl;
    }
    cvNamedWindow( "result", CV_WINDOW_AUTOSIZE );
    //If stream is open run program
    if( capture )
    {
    	for(;;)
   	{
		faceDetected = 0;
		//Take Picture from camera stream
		IplImage* iplImg = cvQueryFrame( capture );
		frame = iplImg;
   		if( frame.empty() )
        	break;
		//Flip image if reversed
    		if( iplImg->origin == IPL_ORIGIN_TL )
			frame.copyTo( frameCopy );
    		else
    		flip( frame, frameCopy, 0 );
		//display result image
   		cvShowImage( "result", iplImg );
		//save Image
		int p[3];
    		p[0] = CV_IMWRITE_JPEG_QUALITY;
    		p[1] = 100;
    		p[2] = 0;
		cvSaveImage("croptest2.png",iplImg,p);
       		cvSaveImage("CaptureTest.png",iplImg,p);
    		if( waitKey( 10 ) >= 0 )
			break;

		//Check if a face is present
		faceDetected = DetectFace();
		//if a face is detected, inform user
    		if(faceDetected)
			{
	   			cout << "Face Detected" << endl;
			}
		//Perform Tesseract on image "ThresholdTest.png"
    		else
        	{
			//find optimal threshold level for current lighting
			findOptimalThreshold();	
			STRING ImageText = GetText("ThresholdTest.png");
			std::cout << "Image Text: " << ImageText.string() << std::endl;
			}
  			waitKey(0);
			//Acqure new frame from stream
			cvReleaseCapture( &capture );
			capture = cvCaptureFromCAM( 0);
		}
    return 0;
    }
}

int DetectFace(void)
{
//Detect Face
// Load the cascade
if (!face_cascade.load(face_cascade_name))
	{
	printf("--(!)Error loading\n");
      	return (-1);
	}
// Read the image file
Mat frame = imread("CaptureTest.png");
// Apply the classifier to the frame
if (!frame.empty())
	{
	faceDetected = detectAndDisplay(frame);
	}
else
	{
	printf(" --(!) No captured frame -- Break!");
	}
return faceDetected;
}

// Function detectAndDisplay
int detectAndDisplay(Mat frame)
{
    std::vector<Rect> faces;
    Mat frame_gray;
    Mat crop;
    Mat res;
    Mat gray;
    string text;
    stringstream sstm;

    cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

    // Detect faces
    face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

    // Set Region of Interest
    cv::Rect roi_b;
    cv::Rect roi_c;

    size_t ic = 0; // ic is index of current element
    int ac = 0; // ac is area of current element

    size_t ib = 0; // ib is index of biggest element
    int ab = 0; // ab is area of biggest element

    for (ic = 0; ic < faces.size(); ic++) // Iterate through all current elements (detected faces)

    {
        roi_c.x = faces[ic].x;
        roi_c.y = faces[ic].y;
        roi_c.width = (faces[ic].width);
        roi_c.height = (faces[ic].height);

        ac = roi_c.width * roi_c.height; // Get the area of current element (detected face)

        roi_b.x = faces[ib].x;
        roi_b.y = faces[ib].y;
        roi_b.width = (faces[ib].width);
        roi_b.height = (faces[ib].height);

        ab = roi_b.width * roi_b.height; // Get the area of biggest element, at beginning it is same as "current" element

        if (ac > ab)
        {
            ib = ic;
            roi_b.x = faces[ib].x;
            roi_b.y = faces[ib].y;
            roi_b.width = (faces[ib].width);
            roi_b.height = (faces[ib].height);
        }

        crop = frame(roi_b);
        resize(crop, res, Size(128, 128), 0, 0, INTER_LINEAR); // This will be needed later while saving images
        cvtColor(crop, gray, CV_BGR2GRAY); // Convert cropped image to Grayscale

        Point pt1(faces[ic].x, faces[ic].y); // Display detected faces on main window - live stream from camera
        Point pt2((faces[ic].x + faces[ic].height), (faces[ic].y + faces[ic].width));
        rectangle(frame, pt1, pt2, Scalar(0, 255, 0), 2, 8, 0);
    }
    if(!crop.empty())
	return 1;
    else
	return 0;
}


void Threshold( int, void* )
{

 	threshold( src_gray, dst, threshold_value, max_BINARY_value,threshold_type );
  	imshow( window2_name, dst );
	ImageThresh = threshold_ImageValue;
	if( waitKey( 10 ) >= 0 )
	{
		cvDestroyWindow(window2_name );
		
	}
}

void findOptimalThreshold(void)
{
	//thresholding
	cout << "beginning threshold ..." << endl;
	//Make image grey
	src = imread("croptest2.png", 1 );
	//src = cropImage;
	cvtColor( src, src_gray, CV_BGR2GRAY);
	//threshold function
    /// Create a window for sliders
	namedWindow( window2_name, CV_WINDOW_AUTOSIZE );
	//Create Trackbar to set ID Threshold image level
	createTrackbar( trackbar_ImageValue,
		          window2_name, &threshold_ImageValue,
 			        max_value, Threshold );
	//Create Trackbar to set threshold done
	createTrackbar( trackbar_Done,
		          window2_name, &threshold_done,
         			        1, Threshold );

	createTrackbar( trackbar_value,
	         	   window2_name, &threshold_value,
		         	  max_value, Threshold );
	Threshold( 0, 0 );
	imwrite("ThresholdTest.png",dst);
}

STRING GetText(char* filename)
{
	const char* lang = "eng";
    const char* imageText;
	//Initialize tesseract API
    tesseract::TessBaseAPI tess;
    tess.Init(NULL, lang, tesseract::OEM_DEFAULT);
    tess.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
	//open file to perform OCR on
    FILE* fin = fopen(filename, "rb");
    if (fin == NULL)
    {
        std::cout << "Cannot open " << filename << std::endl;
    }
    fclose(fin);
    STRING text;

 	//process image and get text
    tess.ProcessPages(filename, NULL, 0, &text);
    imageText = text.string();

    return text;

}
