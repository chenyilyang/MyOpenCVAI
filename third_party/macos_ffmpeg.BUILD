# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_library
cc_library(
    name = "libffmpeg",
    srcs = glob(
        [
            "lib/libav*.dylib",
        ]
    ),
    hdrs = glob(
        ["include/libav*/*.h"],
    ),
    include = ["include/"],
    linkopts = [
        "-lavcodec",
        "-lavformat",
        "-lavutil",
    ],
    linkstatic = 1,
    visibility = ["//visibility:public"],
)