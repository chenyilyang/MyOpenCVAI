#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

cv::Mat preProcessing(cv::Mat img) {
    cv::Mat imgGray, imgBlur, imgDil, imgErode, imgCanny;
    cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(imgGray, imgBlur, cv::Size(3, 3), 3, 0);
    cv::Canny(imgBlur, imgCanny, 25, 75);
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::dilate(imgCanny, imgDil, kernel);
    // cv::erode(imgDil, imgErode, kernel);
    return imgDil;
}
std::vector<cv::Point> getBiggestContours(cv::Mat preImg, cv::Mat originalImg) {
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierachy;
    cv::findContours(preImg, contours, hierachy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    std::vector<std::vector<cv::Point>> conPoly(contours.size());
    std::vector<cv::Rect> boundRect(contours.size());
    std::vector<cv::Point> biggestPoints;
    std::vector<std::vector<cv::Point>> biggestConPoly;
    int maxArea;
    int maxAreaIndex;
    for (size_t i = 0; i < contours.size(); i++)
    {
        int area = cv::contourArea(contours[i]);
        std::cout << area << std::endl;
        std::string objectType;
        if (area > 1000) {
            float peri = cv::arcLength(contours[i], true);
            cv::approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
            if (area > maxArea && conPoly[i].size() == 4) {
                biggestPoints = { conPoly[i][0], conPoly[i][1], conPoly[i][2], conPoly[i][3] };
                maxArea = area;
            }
        }
    }
    if (!biggestPoints.empty()) {
        biggestConPoly.push_back(biggestPoints);
        // cv::drawContours(originalImg, biggestConPoly, -1, cv::Scalar(255, 0, 255), 2);
    }
    return biggestPoints;
}
void drawPoints(cv::Mat imgOriginal, std::vector<cv::Point> points, cv::Scalar color) {
    for (size_t i = 0; i < points.size(); i++)
    {
        cv::circle(imgOriginal, points[i], 10, color, cv::FILLED);
        cv::putText(imgOriginal, std::to_string(i), points[i], cv::FONT_HERSHEY_PLAIN, 4, color, 4);
    }
}
std::vector<cv::Point> reorder(std::vector<cv::Point> points) {
    std::vector<cv::Point> newPoints;
    std::vector<int> sumPoints, subPoints;
    //reorder{68, 430,750, 325}
    //(23, 45)    68
    //(400, 30)   430
    //(350, 400)  750
    //(25, 300)   325
    for (size_t i = 0; i < 4; i++)
    {
        sumPoints.push_back(points[i].x + points[i].y);
        subPoints.push_back(points[i].x - points[i].y);
    }
    newPoints.push_back(points[std::min_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]);//0
    newPoints.push_back(points[std::max_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]);//1
    newPoints.push_back(points[std::min_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]);//2
    newPoints.push_back(points[std::max_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]);//3
    return newPoints;
}
cv::Mat getWarpImage(cv::Mat orginalImg, std::vector<cv::Point> docPoints, float width, float height) {
    cv::Mat warpImg;
    cv::Point2f src[4] = { docPoints[0], docPoints[1], docPoints[2], docPoints[3] };
    cv::Point2f dst[4] = { {0.0f, 0.0f}, {width, 0.0f}, {0.0f, height}, {width, height} };
    cv::Mat matrix = cv::getPerspectiveTransform(src, dst);
    cv::warpPerspective(orginalImg, warpImg, matrix, cv::Point(width, height));
    return warpImg;
}
void cropImage(cv::Mat img, int width, int height) {
    cv::Rect roi(5, 5, width - (2*5), height - (2* 5));
    img = img(roi);
}
void scanPaper() {
    std::string path = "resources/paper.jpg";
    cv::Mat imgOriginal = cv::imread(path);
    // cv::resize(imgOriginal, imgOriginal, cv::Size(), 0.5, 0.5);
    cv::Mat preProImg = preProcessing(imgOriginal);
    cv::imshow("preProImg", preProImg);
    //Get Contours - Biggest
    std::vector<cv::Point> biggestPoints = getBiggestContours(preProImg, imgOriginal);
    // drawPoints(imgOriginal, biggestPoints, cv::Scalar(0, 0, 255));
    biggestPoints = reorder(biggestPoints);
    // drawPoints(imgOriginal, biggestPoints, cv::Scalar(0, 255, 0));
    cv::Mat imgwarp = getWarpImage(imgOriginal, biggestPoints, 400, 600);
    cropImage(imgwarp, 400, 600);
    cv::imshow("Original", imgOriginal);
    cv::imshow("warpImage", imgwarp);
    cv::waitKey(0);
    cv::destroyAllWindows();
}