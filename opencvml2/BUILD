load("@rules_cc//cc:defs.bzl", "cc_binary")
# bazel build //opencvml1:opencvml1


# Note: this cannot just match "apple_platform_type": "macos" because that option
# defaults to "macos" even when building on Linux!
alias(
    name = "macos",
    actual = select({
        ":macos_i386": ":macos_i386",
        ":macos_x86_64": ":macos_x86_64",
        "//conditions:default": ":macos_i386",  # Arbitrarily chosen from above.
    }),
    visibility = ["//visibility:public"],
)

config_setting(
    name = "macos_i386",
    values = {
        "apple_platform_type": "macos",
        "cpu": "darwin",
    },
    visibility = ["//visibility:public"],
)

config_setting(
    name = "macos_x86_64",
    values = {
        "apple_platform_type": "macos",
        "cpu": "darwin_x86_64",
    },
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "opencvml2",
    srcs = ["opencvml2.cc",],
    deps = [
        "@//third_party:opencv",
    ],
)