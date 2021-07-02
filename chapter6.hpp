#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
int hmin = 0, smin = 0, vmin = 0;
int hmax = 179, smax = 255, vmax = 255;
void colorDetection() {
    std::string path = "resources/lambo.png";
    cv::Mat img = cv::imread(path);
    cv::Mat imgHSV, mask;
    cv::cvtColor(img, imgHSV, cv::COLOR_BGR2HSV);

    cv::namedWindow("Trackbars");
    cv::createTrackbar("Hue min", "Trackbars", &hmin, 179);
    cv::createTrackbar("Hue max", "Trackbars", &hmax, 179);
    cv::createTrackbar("Sat min", "Trackbars", &smin, 255);
    cv::createTrackbar("Sat max", "Trackbars", &smax, 255);
    cv::createTrackbar("val min", "Trackbars", &vmin, 255);
    cv::createTrackbar("val max", "Trackbars", &vmax, 255);

    while (true)
    {
        cv::Scalar lower(hmin, smin, vmin);
        cv::Scalar upper(hmax, smax, vmax);
        cv::inRange(imgHSV, lower, upper, mask);
        cv::imshow("src", img);
        cv::imshow("hsv", imgHSV);
        cv::imshow("mask", mask);
        cv::waitKey(5);
    }
    cv::destroyAllWindows();
}