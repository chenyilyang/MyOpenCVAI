#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

std::vector<cv::Point> allPoints;
const vector<vector<int>> painterColors{{0,126,163,179,206,235}};
const vector<Scalar> myColorValue{{0, 0, 255}};
std::vector<cv::Rect> detectObject(cv::Mat frameHSV) {
    //contours:
    //{{Point(x, y), Point(x, y)}, {...}, {...}}
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(frameHSV, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    // cv::drawContours(img, contours, -1, cv::Scalar(255, 0, 255), 2);
    std::vector<std::vector<cv::Point>> conPoly(contours.size());
    std::vector<cv::Rect> boundRect(contours.size());
    for (size_t i = 0; i < contours.size(); i++)
    {
        int area = cv::contourArea(contours[i]);
        std::cout << area << std::endl;
        if (area > 400)
        {
            float peri = cv::arcLength(contours[i], true);
            cv::approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
            // cv::drawContours(img, contours, i, cv::Scalar(255, 0, 255), 2);
            std::cout << "conPoly size = " << conPoly[i].size() << std::endl;
            boundRect[i] = cv::boundingRect(conPoly[i]);
            // cv::drawContours(img, conPoly, i, cv::Scalar(255, 0, 255), 2);
            // cv::putText(img, objectType, {boundRect[i].x, boundRect[i].y - 5}, cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 255), 1);
        }
    }
    return boundRect;
}
// int hmin = 0, smin = 0, vmin = 0;
// int hmax = 179, smax = 255, vmax = 255;
void detectPainterColor(cv::Mat frame) {
    cv::Mat frameHSV, mask;
    cv::namedWindow("DetectPianterColor", (640, 200));
    cv::createTrackbar("Hue Min", "DetectPianterColor", &hmin, 179);
    cv::createTrackbar("Hue Max", "DetectPianterColor", &hmax, 179);
    cv::createTrackbar("Sat Min", "DetectPianterColor", &smin, 255);
    cv::createTrackbar("Sat Max", "DetectPianterColor", &smax, 255);
    cv::createTrackbar("Val Min", "DetectPianterColor", &vmin, 255);
    cv::createTrackbar("Val Max", "DetectPianterColor", &vmax, 255);
    cv::cvtColor(frame, frameHSV, COLOR_BGR2HSV);
    cv::Scalar lower(hmin, smin, vmin);
    cv::Scalar upper(hmax, smax, vmax);
    std::cout << "{" << hmin << "," << smin << "," << vmin << "," << hmax << "," << smax << "," << vmax << "}" << std::endl;
    cv::inRange(frameHSV, lower, upper, mask);
    cv::imshow("painter mask", mask);
}
std::vector<std::vector<cv::Rect>> findObjectRect(cv::Mat frame) {
    cv::Mat frameHSV, mask;
    std::vector<std::vector<cv::Rect>> results;
    cv::cvtColor(frame, frameHSV, COLOR_BGR2HSV);
    for (size_t i = 0; i < painterColors.size(); i++)
    {  
        cv::Scalar lower(painterColors[i][0], painterColors[i][1], painterColors[i][2]);
        cv::Scalar upper(painterColors[i][3], painterColors[i][4], painterColors[i][5]);
        cv::inRange(frameHSV, lower, upper, mask);
        // cv::imshow(std::to_string(i), mask);
        std::vector<cv::Rect> result = detectObject(mask);
        results.push_back(result);
    }
    return results;
}
void drawObject(std::vector<std::vector<cv::Rect>> rects, cv::Mat frame) {
    for (size_t i = 0; i < rects.size(); i++)
    {
        for (size_t j = 0; j < rects[i].size(); j++)
        {
            // cv::rectangle(frame, objectArea[i][j], cv::Scalar(255, 0, 0), 2);
            allPoints.push_back(cv::Point(rects[i][j].x + rects[i][j].width / 2, rects[i][j].y + rects[i][j].height / 2));
        }
    }
    for (size_t k = 0; k < allPoints.size(); k++)
    {
        cv::circle(frame, cv::Point(allPoints[k].x, allPoints[k].y), 10, myColorValue[0], cv::FILLED);
    }
    
}
void virtualPainter() {
    cv::VideoCapture camera(0);
    cv::Mat frame;
    if (camera.isOpened())
    {
        while (true)
        {
            camera >> frame;
            // detectPainterColor(frame);
            std::vector<std::vector<cv::Rect>> objectArea = findObjectRect(frame);
            // for (size_t i = 0; i < objectArea.size(); i++)
            // {
            //     for (size_t j = 0; j < objectArea[i].size(); j++)
            //     {
            //         cv::rectangle(frame, objectArea[i][j], cv::Scalar(255, 0, 0), 2);
            //     }
            // }
            drawObject(objectArea, frame);
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