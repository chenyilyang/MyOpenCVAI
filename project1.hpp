#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

void virtualPainter() {
    cv::VideoCapture camera(0);
    cv::Mat frame;
    if (camera.isOpened())
    {
        while (true)
        {
            camera >> frame;
            cv::imshow("Virtual Painter", frame);
            if (cv::waitKey(30) == 27)
            {
                cv::destroyAllWindows();
                break;
            }
            
        }
    } else {
        std::cerr << "can not open camera 0" << std::endl;
    }
}