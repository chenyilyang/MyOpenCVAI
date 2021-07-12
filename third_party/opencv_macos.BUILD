# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_library
cc_library(
    name = "opencv",
    srcs = glob([
        "lib/libopencv_core.dylib",
        "lib/libopencv_calib3d.dylib",
        "lib/libopencv_features2d.dylib",
        "lib/libopencv_highgui.dylib",
        "lib/libopencv_imgcodecs.dylib",
        "lib/libopencv_imgproc.dylib",
        "lib/libopencv_video.dylib",
        "lib/libopencv_videoio.dylib",
        "lib/libopencv_objdetect.dylib",
        "lib/libopencv_photo.dylib",
        "lib/libopencv_ml.dylib",
    ]),
    hdrs = glob(["include/opencv4/**/*"]),
    includes = ["include/opencv4"],
    linkstatic = 1,
    # alwayslink = 1,
    visibility = ["//visibility:public"],
)