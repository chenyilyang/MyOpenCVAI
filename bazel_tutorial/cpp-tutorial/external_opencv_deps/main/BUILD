load("@rules_cc//cc:defs.bzl", "cc_binary")
#note: if you meet some error like: can not find image etc.
#You have to run "bazel build //main:hello-mat --cxxopt='-std=c++17' --linkopt=-Wl,-rpath,/Users/mac/OpenCV/opencv_env/lib"
#with in link options which name is rpath
cc_binary(
    name = "hello-mat",
    srcs = ["hello-mat.cc", "test_face_detection.hpp"],
    deps = [
        "@opencv//:opencv"
    ],
)