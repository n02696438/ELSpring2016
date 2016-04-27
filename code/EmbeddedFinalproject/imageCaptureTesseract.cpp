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
int const max_value = 255;
int const max_type = 4;
int const max_BINARY_value = 255;
Mat src, src_gray, dst;
IplImage* iplImg2;

//FaceDetection
// Function Headers
int detectAndDisplay(Mat frame);

// Global variables
// Copy this file from opencv/data/haarscascades to target folder
string face_cascade_name = "lbpcascade_frontalface.xml";
CascadeClassifier face_cascade;
string window_name = "Capture - Face detection";
int filenumber; // Number of file to be saved
string filename;
int faceDetected = 0;

int main( int argc, const char** argv )
{
    //Tesseract Variables
    const char* lang = "eng";
    const char* filename = "ThresholdTest.png";
    const char* imageText;
	//Tesseract Init
	tesseract::TessBaseAPI tess;
    tess.Init(NULL, lang, tesseract::OEM_DEFAULT);
    tess.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);

    CvCapture* capture = 0;
    Mat frame, frameCopy, image;

    capture = cvCaptureFromCAM( 0);//1 - CHANGED TO 0, Forgot webcam);//CV_CAP_ANY ); //0=default, -1=any camera, 	1..99=your camera
    if( !capture )
    {
        cout << "No camera detected" << endl;
    }
	
	//capture.set(CV_CAP_PROP_FRAME_WIDTH, 1920); // valueX = your wanted width 
	//capture.set(CV_CAP_PROP_FRAME_HEIGHT, 1080); // valueY = your wanted heigth
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 1920);
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 1080);

    cvNamedWindow( "result", CV_WINDOW_AUTOSIZE );

    if( capture )
    {
        cout << "In capture ..." << endl;
    	for(;;)
   		{
			faceDetected = 0;
			IplImage* iplImg = cvQueryFrame( capture );
			frame = iplImg;

   			if( frame.empty() )
        	break;
    		if( iplImg->origin == IPL_ORIGIN_TL )
			frame.copyTo( frameCopy );
    		else
    		flip( frame, frameCopy, 0 );

   			cvShowImage( "result", iplImg );
//End picture being taken

//compression test
			int p[3];
    		p[0] = CV_IMWRITE_JPEG_QUALITY;
    		p[1] = 100;
    		p[2] = 0;
//end compression
///DETECTING EDGES AND CROPPING
   			cvSaveImage("croptesting.png",iplImg,p);
    		//Mat img = frame;
    		Mat img = imread("croptesting.png", CV_LOAD_IMAGE_GRAYSCALE);

    		Mat threshold_output;
    		/// Detect edges using Threshold 
    		threshold( img, threshold_output, 140, 255, THRESH_BINARY);

    		//imshow( "threshold_output", threshold_output );

    		vector<vector<Point> > contours;
    		vector<Vec4i> hierarchy;
    		/// Find contours
    		findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    		/// Approximate contours to polygons + get bounding rects
    		vector<vector<Point> > contours_poly( contours.size() );
    		Rect boundRect;

	    	double maxArea = 0.0;
    		for( int i = 0; i < contours.size(); i++ )
    		{ 
        		double area = contourArea(contours[i]);
        		if(area > maxArea) 
				{
            		maxArea = area;
            		approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
            		boundRect = boundingRect( Mat(contours_poly[i]) );
        		}
    		}
    		Mat cropImage = img(boundRect);
    		//imshow("cropImage", cropImage);
    		imwrite("croptest2.png",cropImage);
///END DETECT EDGES AND CROPPING

			//thresholding
			//cout << "beginning threshold ..." << endl;
			//Make image grey
			src = imread("croptest2.png", 1 );
			//src = cropImage;
			cvtColor( src, src_gray, CV_BGR2GRAY);
			//threshold function
			threshold( src_gray, dst, threshold_value, max_BINARY_value,threshold_type );
			//cout << "threshold done, saving threshold image ..." << endl;
			imwrite("ThresholdTest.png",dst);
			//Mat roi = dst( Rect(810,622,370,300) );
			//imwrite("croptest.png",roi);

			//saving picture
			//cout << "Picture saving ..." << endl;
       		cvSaveImage("CaptureTest.png",iplImg,p);
			//cout << "Picture saved ..." << endl;
    		if( waitKey( 10 ) >= 0 )
				break;

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
       			//break;
    		}
    		if(faceDetected)
			{
	   			cout << "Face Detected" << endl;
			}
    		else
        	{
				//Perform Tesseract on image "ThresholdTest.png"
				FILE* fin = fopen(filename, "rb");
   				 if (fin == NULL)
    				{
			      	  std::cout << "Cannot open " << filename << std::endl;
			      	  return -1;
			   		}
		    	fclose(fin);

		    	STRING text;
 
				tess.ProcessPages(filename, NULL, 0, &text);
   		    	imageText = text.string();

    			if(strcmp(imageText, "") == 0)
					std::cout << "No text" << std::endl;
		   		else
					std::cout << "image text: " << imageText << std::endl;
		    	//std::cout << text.string() << std::endl;
			}
  			waitKey(0);
			cvReleaseCapture( &capture );
			capture = cvCaptureFromCAM( 0);
		}
	
    //cvReleaseCapture( &capture );
    //cvDestroyWindow( "result" );
    return 0;
    }
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

        // Form a filename
        //filename = "";
        //stringstream ssfn;
        //ssfn << filenumber << ".png";
        //filename = ssfn.str();
        //filenumber++;

        //imwrite(filename, gray);

        Point pt1(faces[ic].x, faces[ic].y); // Display detected faces on main window - live stream from camera
        Point pt2((faces[ic].x + faces[ic].height), (faces[ic].y + faces[ic].width));
        rectangle(frame, pt1, pt2, Scalar(0, 255, 0), 2, 8, 0);
    }
    if(!crop.empty())
	return 1;
    else
	return 0;
    // Show image
    /*sstm << "Crop area size: " << roi_b.width << "x" << roi_b.height << " Filename: " << filename;
    text = sstm.str();

    putText(frame, text, cvPoint(30, 30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0, 0, 255), 1, CV_AA);
    imshow("original", frame);

    if (!crop.empty())
    {
        imshow("detected", crop);
    }
    else
        destroyWindow("detected");*/
}
