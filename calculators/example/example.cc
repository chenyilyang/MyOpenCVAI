#include "mediapipe/framework/calculator_framework.h"

namespace mediapipe {
    class ExampleCalculator:public CalculatorBase {
        ExampleCalculator() {};
        ~ExampleCalculator() {};
        static ::mediapipe::Status GetContract(CalculatorContract * cc) {
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Open(CalculatorContract * cc) {
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Process(CalculatorContract * cc) {
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Close(CalculatorContract * cc) {
            return ::mediapipe::OkStatus();
        }
    };
    REGISTER_CALCULATOR(ExampleCalculator);
}