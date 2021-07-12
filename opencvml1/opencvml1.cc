#include <opencv2/opencv.hpp>

#define OPENCV_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#define OPENCV_VERSION_CODE OPENCV_VERSION(CV_MAJOR_VERSION, CV_MINOR_VERSION, CV_SUBMINOR_VERSION)

void testRotatedRect() {
    cv::Mat background(300, 400, CV_8UC3, cv::Scalar(255, 255, 255));
    cv::Point2f center(200, 150);
    cv::Size2f size(180, 120);
    float angle = 30.0f;
    cv::RotatedRect rRect(center, size, angle);
    std::cout << "the center of rRect is:" << rRect.center << std::endl;
    std::cout << "the size of rRect is:" << rRect.size << std::endl;
    std::cout << "the angle of rRect is:" << rRect.angle << std::endl;
    cv::Point2f vertices[4];
    rRect.points(vertices);
    for (size_t i = 0; i < 4; i++) {
        cv::line(background, vertices[i], vertices[(i + 1) % 4], cv::Scalar(255, 0, 255), 2);
    }
    cv::Rect boundingRect = rRect.boundingRect2f();
    cv::rectangle(background, boundingRect, cv::Scalar(0, 255, 0));
    cv::imshow("Rotated rectangle", background);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
void testMatCalc() {
    cv::Mat m0 = cv::Mat::eye(3, 3, CV_8UC1);
    cv::Mat m1 = cv::Mat::ones(3, 3, CV_8UC1);
    cv::Mat m2 = (cv::Mat_<u_int8_t>(cv::Size(3, 3)) << 1, 3, 0, 0, 0, 1, 0, 2, 0);

    std::cout << "m0 = " << std::endl << m0 << std::endl << std::endl;
    std::cout << "m1 = " << std::endl << m1 << std::endl << std::endl;
    std::cout << "m2 = " << std::endl << m2 << std::endl << std::endl;

    std::cout << "-m2 = " << std::endl << -m2 << std::endl << std::endl;
    std::cout << "m1 + m2 = " << std::endl << m1 + m2 << std::endl << std::endl;
    std::cout << "m0 * 128 = " << std::endl << m0 * 128 << std::endl << std::endl;
    std::cout << "128 * m0 = " << std::endl << 128 * m0 << std::endl << std::endl;
    // std::cout << "m0 * m1 = " << std::endl << m0 * m1 << std::endl << std::endl;
    std::cout << "m0 .* m1 = " << std::endl << m0.mul(m1) << std::endl << std::endl;
    std::cout << "m1 .* m2 = " << std::endl << m1.mul(m2) << std::endl << std::endl;
    std::cout << "m2.t() " << std::endl << m2.t() << std::endl << std::endl;
}

void testMat() {
    cv::Mat_<cv::Vec4b> img(300, 300, cv::Vec4b(0, 200, 0, 255));
    for (size_t i = 0; i < img.cols; i++) {
        img(i, i) = cv::Vec4b(255, 255, 255, 255);
    }
    cv::imshow("vec to mat", img);
    //Mat to STL vector
    cv::Mat m1 = (cv::Mat_<double>(3, 3) << 1,2,3,4,5,6,7,8,9);
    std::cout << "m1 = " << std::endl << m1 << std::endl;
    std::vector<double> v1;
    m1 = m1.reshape(0, 1);//变形为一行的行列式
    #if OPENCV_VERSION_CODE < OPENCV_VERSION(2,3,0)
        m1.copyTo<double>(v1);
    #else
        m1.copyTo(v1);
    #endif
    std::cout << "v1 = " << std::endl;
    std::cout << "[";
    for (std::vector<double>::iterator it = v1.begin(); it != v1.end(); ++it) {
        std::cout << *it << ",";
    }
    std::cout << "]" << std::endl;
    cv::waitKey(0);
    cv::destroyAllWindows();
}
int main() {
    testMat();
    return 0;
}