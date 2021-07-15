# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_library
cc_library(
    name = "opencv",
    srcs = glob([
        "lib/libopencv_core.so",
        "lib/libopencv_calib3d.so",
        "lib/libopencv_features2d.so",
        "lib/libopencv_highgui.so",
        "lib/libopencv_imgcodecs.so",
        "lib/libopencv_imgproc.so",
        "lib/libopencv_video.so",
        "lib/libopencv_videoio.so",
        "lib/libopencv_objdetect.so",
        "lib/libopencv_photo.so",
        "lib/libopencv_ml.so",
    ]),
    hdrs = glob(["include/opencv4/**/*"]),
    includes = ["include/opencv4"],
    linkstatic = 1,
    # alwayslink = 1,
    visibility = ["//visibility:public"],
)