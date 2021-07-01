#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>

void getContours(cv::Mat imgDil, cv::Mat img) {
    //contours:
    //{{Point(x, y), Point(x, y)}, {...}, {...}}
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(imgDil, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    // cv::drawContours(img, contours, -1, cv::Scalar(255, 0, 255), 2);
    cv::Mat mask = cv::Mat::zeros(img.size(), CV_8UC1);
    for (size_t i = 0; i < contours.size(); i++)
    {
        int area = cv::contourArea(contours[i]);
        std::cout << area << std::endl;
        std::vector<std::vector<cv::Point>> conPoly(contours.size());
        std::vector<cv::Rect> boundRect(contours.size());
        std::string objectType;
        if (area > 1000)
        {
            float peri = cv::arcLength(contours[i], true);
            cv::approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
            // cv::drawContours(img, contours, i, cv::Scalar(255, 0, 255), 2);
            std::cout << "conPoly size = " << conPoly[i].size() << std::endl;
            boundRect[i] = cv::boundingRect(conPoly[i]);
            int objCorner = (int)conPoly[i].size();
            if (objCorner == 3)
            {
                objectType = "Triangle";
            }
            if (objCorner == 4)
            {
                float aspRatio = (float)boundRect[i].width / (float)boundRect[i].height;
                if (aspRatio > 0.95 && aspRatio < 1.05)
                {
                    objectType = "Square";
                } else {
                    objectType = "Rectangle";
                }                
            }
            if (objCorner > 4)
            {
                objectType = "Circle";
            }
            cv::drawContours(img, conPoly, i, cv::Scalar(255, 0, 255), 2);
            cv::rectangle(img, boundRect[i].tl(), boundRect[i].br(), cv::Scalar(255, 0, 0), 5);
            cv::putText(img, objectType, {boundRect[i].x, boundRect[i].y - 5}, cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 255), 1);
        } else {
            float peri = cv::arcLength(contours[i], true);
            cv::approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
            boundRect[i] = cv::boundingRect(conPoly[i]);
            cv::rectangle(mask, boundRect[i], cv::Scalar(255), -1);
            cv::inpaint(img, mask, img, 3, cv::INPAINT_TELEA);
        }
    }
}
void shapeDetection() {
    std::string path = "resources/shapes.png";
    cv::Mat img = cv::imread(path);
    cv::Mat imgGray, imgBlur, imgCanny, imgDil;
    cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(imgGray, imgBlur, cv::Size(3, 3), 3, 0);
    cv::Canny(imgBlur, imgCanny, 25, 75);
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::dilate(imgCanny, imgDil, kernel);
    getContours(imgDil, img);

    cv::imshow("src", img);
    cv::imshow("gray", imgGray);
    cv::imshow("blur", imgBlur);
    cv::imshow("canny", imgCanny);
    cv::imshow("dil", imgDil);

    cv::waitKey(0);
    cv::destroyAllWindows();
}
