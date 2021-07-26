#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

void testImageTransform() {
    Mat image = imread("resources/lemu.jpg");
    //1尺度变换
    Mat resize_image, resize_image_M;
    resize(image, resize_image, Size(0, 0), 0.5, 0.5, 1);
    Mat warp_matrix_resize;//声明尺度变换M矩阵
    warp_matrix_resize = Mat::eye(2, 3, CV_32F);
    float cx = 1.5f, cy = 0.5f;
    warp_matrix_resize.at<float>(0, 0) = cx;
    warp_matrix_resize.at<float>(1, 1) = cy;
    Size dsize = image.size();
    dsize.width *= cx;
    dsize.height *= cy;
    cout << "M resize = " << endl << warp_matrix_resize << endl << endl;
    warpAffine(image, resize_image_M, warp_matrix_resize, dsize, INTER_LINEAR, 1);
    imshow("original", image);
    imshow("resize", resize_image);
    imshow("M resize", resize_image_M);
    
    //平移变换
    Mat temp_image = resize_image.clone();
    Mat translation_image;
    Mat warp_matrix_translation;
    warp_matrix_translation = Mat::eye(2, 3, CV_32F);
    float tx = 40, ty = 20;
    warp_matrix_translation.at<float>(0, 2) = tx;
    warp_matrix_translation.at<float>(1, 2) = ty;
    cout << "M translation = " << warp_matrix_translation << endl << endl;
    int top = 0, bottom = ty;
    int left = 0, right = tx;
    copyMakeBorder(temp_image, temp_image, top, bottom, left, right, BORDER_CONSTANT, Scalar(200));
    imshow("translation border image", temp_image);
    warpAffine(temp_image, translation_image, warp_matrix_translation, temp_image.size(), INTER_LINEAR, BORDER_TRANSPARENT);
    imshow("translation image", translation_image);

    //旋转变换
    Mat rotate_image;
    temp_image = resize_image.clone();
    double angle = 45;
    int border = 0.207 * temp_image.rows;
    copyMakeBorder(temp_image, temp_image, border, border, border, border, BORDER_CONSTANT, Scalar(200));
    Point2f center(temp_image.cols / 2, temp_image.rows / 2);
    Mat warp_rotate_matrix = getRotationMatrix2D(center, angle, 1.0f);
    cout << "M Rotate matrix = " << warp_rotate_matrix << endl << endl;
    warpAffine(temp_image, rotate_image, warp_rotate_matrix, temp_image.size(), INTER_LINEAR, BORDER_REPLICATE);
    imshow("rotate border image", temp_image);
    imshow("rotate image", rotate_image);

    //剪切变换
    Mat shear_vertical_image, shear_horizontal_image;
    temp_image = resize_image.clone();
    Mat temp_image_vertical, temp_image_horizontal;
    border = 40;
    copyMakeBorder(temp_image, temp_image_vertical, border, border, 10, 4 * border, BORDER_CONSTANT, Scalar(200));
    copyMakeBorder(temp_image, temp_image_horizontal, 10, 3 * border, border, border, BORDER_CONSTANT, Scalar(200));
    Mat warp_matrix_shear_vertical = Mat::eye(2, 3, CV_32F);
    Mat warp_matrix_shear_horizontal = Mat::eye(2, 3, CV_32F);
    float sv = 0.3, sh = 0.3;
    warp_matrix_shear_vertical.at<float>(0, 1) = sh;// 1 0 0
                                                    //sh 1 0
                                                    //0  0 1 (optional)
    warp_matrix_shear_horizontal.at<float>(1, 0) = sv;//1 sv 0
                                                      //0 1 0
                                                      //0  0 1 (optional)
    warpAffine(temp_image_vertical, shear_vertical_image, warp_matrix_shear_vertical, temp_image_vertical.size(), INTER_LINEAR, BORDER_REPLICATE);
    warpAffine(temp_image_horizontal, shear_horizontal_image, warp_matrix_shear_horizontal, temp_image_horizontal.size(), INTER_LINEAR, BORDER_REPLICATE);
    imshow("shear vertical", shear_vertical_image);
    imshow("shear horizontal", shear_horizontal_image);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
void drawHist(string windowName, Mat &srcImg) {
    //需要计算图像的哪个通道（RGB空间需要确定计算R空间，G空间，B空间）
    const int channels[1] = { 0 };
    //直方图的数目
    int histSize[] = { 256 };
    //单个纬度的直方图数值的取值范围
    float singleRanges[] = { 0, 256 };
    //确定每个纬度的取值范围，即横坐标的最大个数
    const float *ranges[] = { singleRanges };
    //输出的直方图，采用MatND类型
    MatND dstHist;
    calcHist(&srcImg, 1, channels, Mat(), dstHist, 1, histSize, ranges, true, false);
    //create a white Mat
    Mat drawImage(Size(256, 256), CV_8UC3, Scalar::all(255));
    //先用minMaxLoc函数计算得到直方图后的像素的最大个数
    double maxValue;
    minMaxLoc(dstHist, 0, &maxValue, 0, 0);
    for (size_t i = 0; i < 256; i++)
    {
        int value = cvRound(256 * 0.9 * (dstHist.at<float>(i) / maxValue));
        //第i个bin的高度
        line(drawImage, Point(i, drawImage.rows - 1), Point(i, drawImage.rows - 1 - value), Scalar(255, 0, 0));//绘制柱状图
    }
    imshow(windowName, drawImage);
}
int main() {
    Mat lemu = imread("resources/lemu.jpg", 0);
    //Mat dstImg;
    //直方图均衡化
    //equalizeHist(lemu, dstImg);
    //保存均衡化的结果
    //imwrite("resources/lemu_equalizeHist.jpg", dstImg);
    //计算并显示直方图
    //drawHist("Histogram", lemu);
    //drawHist("Equallize Histogram", dstImg);

    imshow("lemu", lemu);
    //imshow("equalizeHist_lemu", dstImg);

    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}
