# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_library
cc_library(
    name = "libffmpeg",
    srcs = glob(
        [
            "lib/x86_64-linux-gnu/libav*.so",
        ]
    ),
    hdrs = glob(
        ["include/x86_64-linux-gnu/libav*/*.h"],
    ),
    include = ["include"],
    linkopts = [
        "-lavcodec",
        "-lavformat",
        "-lavutil",
    ],
    linkstatic = 1,
    visibility = ["//visibility:public"],
)