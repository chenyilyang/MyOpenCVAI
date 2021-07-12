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
        "//conditions:default" : "@macos_opencv//:opencv",
    }),
)