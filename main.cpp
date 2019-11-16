#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <stdio.h>
using namespace std;
using namespace cv;
void detectAndDisplay(Mat frame, String shape="circle");
String face_cascade_name = "haarcascades/haarcascade_frontalface_alt.xml";
CascadeClassifier face_cascade;
String window_name = "Capture - Face detection";
int main( void )
{
    //-- 1. Load the cascades
    if( !face_cascade.load( face_cascade_name ) ){
        printf("--(!)Error loading face cascade\n");
        return -1;
    };
    Mat frame;

    int mode=2; // 1-> Local image file Source ; 2-> Camera source
    if (mode==1) {
        // Read the image file
        Mat frame = imread("lena.tif");
        detectAndDisplay( frame, "rect" );
        char c = (char)waitKey(0);
        return 0;
    } else if (mode==2) {

        //-- 2. Read the video stream
        VideoCapture capture;
        capture.open(-1);
        if ( ! capture.isOpened() ) { printf("--(!)Error opening video capture\n"); return -1; }
        while ( capture.read(frame) )
        {
            if( frame.empty() )
            {
                printf(" --(!) No captured frame -- Break!");
                break;
            }
            //-- 3. Apply the classifier to the frame
            detectAndDisplay( frame, "circle" );
            char c = (char)waitKey(10);
            if( c == 27 ) { break; } // escape
        }
    }


    return 0;
}
void detectAndDisplay(Mat frame,String shape)
{
    std::vector<Rect> faces;
    Mat frame_gray;
    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );
    //-- Detect faces
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );
    for ( size_t i = 0; i < faces.size(); i++ )
    {
        if (shape.compare("circle")==0) {
            Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
            ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
        } else if (shape.compare("rect")==0) {
            Point pt1(faces[i].x, faces[i].y); // Display detected faces on main window - livestream from camera
            Point pt2((faces[i].x + faces[i].height), (faces[i].y + faces[i].width));
            rectangle(frame, pt1, pt2, Scalar(0, 255, 0), 2, 8, 0);
        }
        Mat faceROI = frame_gray( faces[i] );
    }
    //-- Show what you got
    imshow( window_name, frame );
}