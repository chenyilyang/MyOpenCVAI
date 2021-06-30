#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

void faceDetection() {
    std::string path = "resources/faces_detection_2.jpeg";
    cv::Mat img = cv::imread(path);
    cv::CascadeClassifier faceCascade;
    faceCascade.load("resources/haarcascade_frontalface_default.xml");
    if (faceCascade.empty())
    {
        std::cout << "XML file not loaded" << std::endl;
        return;
    }
    std::vector<cv::Rect> faces;
    faceCascade.detectMultiScale(img, faces, 1.1, 10);
    for (size_t i = 0; i < faces.size(); i++)
    {
        cv::rectangle(img, faces[i].tl(), faces[i].br(), cv::Scalar(255, 0, 255), 2);
    }
    cv::imshow("face detection", img);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
