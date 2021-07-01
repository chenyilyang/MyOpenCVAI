#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/types_c.h>
#include "gcode_converter.hpp"

void contours(cv::Mat &img) {
    cv::Mat contourImg(img.size(), img.type(), cv::Scalar(0));
    cv::cvtColor(img, img, CV_BGRA2GRAY);
    img = img < 128;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(img, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_NONE);
    cv::drawContours(contourImg, contours, -1, cv::Scalar::all(255), 1, 8);
    std::cout << cv::format(contourImg.row(0), cv::Formatter::FMT_NUMPY);
    std::cout << cv::format(contourImg.row(1), cv::Formatter::FMT_NUMPY);
    cv::namedWindow("Simple Demo", cv::WINDOW_AUTOSIZE);
    cv::imshow("Simple Demo", contourImg);
    cv::waitKey(0);
    cv::destroyAllWindows();
}

void genGCodes(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Expecting a image file to be passed to program" << std::endl;
        return;
    }
    cv::Mat img = cv::imread(argv[1]);
    
    if (img.empty()) {
        std::cout << "Not a valid image file" << std::endl;
        return;
    }
    cv::cvtColor(img, img, COLOR_BGR2HSV);
    // cv::resize(img, img, cv::Size(), 0.5, 0.5);
    ImageToGCode(img, 12, 400, 400, 1000, 2500, false, true);
    cv::namedWindow("GCode", cv::WINDOW_FREERATIO);
    cv::imshow("GCode", img);
    cv::waitKey(0);
    cv::destroyAllWindows();
}