#include <opencv2/opencv.hpp>
#include <iostream>

void plateDetection() {
    cv::CascadeClassifier plateCascade;
    plateCascade.load("resources/haarcascade_russian_plate_number.xml");
    if (plateCascade.empty()) {
        std::cerr << "XML file not loaded" << std::endl;
        return;
    }
    cv::VideoCapture camera(0);
    cv::Mat frame;
    std::vector<cv::Rect> plates;
    if (camera.isOpened()) {
        while (camera.read(frame)) {
            if (!frame.empty()) {
                cv::resize(frame, frame, cv::Size(), 0.5, 0.5);
                plateCascade.detectMultiScale(frame, plates, 1.1, 10);
            }
            for (size_t i = 0; i < plates.size(); i++) {
                cv::Mat imgCrop = frame(plates[i]);
                cv::imshow(std::to_string(i), imgCrop);
                cv::rectangle(frame, plates[i].tl(), plates[i].br(), cv::Scalar(0, 255, 0), 3);
            }
            cv::imshow("Plate frames", frame);
            if (cv::waitKey(30) == 27)
            {
                break;
            }
        }
    } else {
        std::cerr << "can not open camera 0" << std::endl;
    }
    cv::destroyAllWindows();
}