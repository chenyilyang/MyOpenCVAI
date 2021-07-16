package(default_visibility = ["//visibility:public"])

alias(
    name = "opencv",
    actual = select({
        "//conditions:default": ":opencv_binary",
    }),
    visibility = ["//visibility:public"],
)
alias(
    name = "opencv_binary",
    actual = select({
	"//mediapipe:macos" : "@macos_opencv//:opencv",
        "//conditions:default" : "@linux_opencv//:opencv",
    }),
)

cc_library(
    name = "libffmpeg",
    visibility = ["//visibility:public"],
    deps = select({
        "//mediapipe:android_x86": [],
        "//mediapipe:android_x86_64": [],
        "//mediapipe:android_armeabi": [],
        "//mediapipe:android_arm": [],
        "//mediapipe:android_arm64": [],
        "//mediapipe:ios": [],
        "//mediapipe:macos": [
            "@macos_ffmpeg//:libffmpeg",
        ],
        "//conditions:default": [
            "@linux_ffmpeg//:libffmpeg",
        ],
    }),
)
cc_library(
    name = "glog",
    visibility = ["//visibility:public"],
    deps = select({
        "//mediapipe:macos": [
            "@com_github_glog_glog//:glog",
        ],
        "//conditions:default": [
            "@com_github_glog_glog//:glog",
        ],
    }),
)
