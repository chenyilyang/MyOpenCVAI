#include "gcode.hpp"
#include "chapter1.hpp"
#include "chapter5.hpp"
#include "chapter6.hpp"
#include "chapter7.hpp"
#include "chapter8.hpp"
#include "project1.hpp"
#include "project2.hpp"
#include "project3.hpp"

#include "opencvml1.hpp"
#include "opencvml2.hpp"

using std::cout;
using std::endl;

int main() {
    
    // genGCodes();
    // warpImage();
    // colorDetection();
    // shapeDetection();
    // faceDetection();
    // virtualPainter();
    // scanPaper();
    plateDetection();

    /**
     * opencvML
     * */
    // testRotatedRect();
    // testMatCalc();
    // testMat();
    // testImageTransform();
    // testHistgram();
    return 0;
}
