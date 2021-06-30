#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>

void warpImage() {
    cv::Mat imgWarp;
    std::string path = "resources/cards.jpg";
    cv::Mat img = cv::imread(path);
    cv::Point2f src[4] = {{526, 142}, {771, 190}, {400, 400}, {680, 460}};
    cv::Point2f dst[4] = {{0.0f, 0.0f}, {250.0f, 0.0f}, {0.0f, 350.f}, {250.f, 350.f}};
    cv::Mat warpMatrix = cv::getPerspectiveTransform(src, dst);
    cv::warpPerspective(img, imgWarp, warpMatrix, cv::Point(250, 350));
    for (size_t i = 0; i < 4; i++)
    {
        cv::circle(img, src[i], 10, cv::Scalar(0, 0, 255), cv::FILLED);
    }
    cv::line(img, src[0], src[1], cv::Scalar(0, 0, 255), 3);
    cv::line(img, src[1], src[3], cv::Scalar(0, 0, 255), 3);
    cv::line(img, src[3], src[2], cv::Scalar(0, 0, 255), 3);
    cv::line(img, src[2], src[0], cv::Scalar(0, 0, 255), 3);
    cv::imshow("Image", img);
    cv::imshow("warpImage", imgWarp);
    cv::waitKey(0);
    cv::destroyAllWindows();
}