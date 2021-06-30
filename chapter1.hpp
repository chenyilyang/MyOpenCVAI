#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void readVideo() {
    string path = "resources/test_video.mp4";
    VideoCapture cap(path);
    Mat img;
    cap.read(img);
    while(!img.empty()) {
        imshow("Image", img);
        cap.read(img);
        waitKey(30);
    }
    waitKey(20);
    destroyAllWindows();
}