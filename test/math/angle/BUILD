load(
    "//config:copts/cpp_flags.bzl",
    "KFCPP_CPP_COMPILER_FLAGS"
)

cc_test(
    name = "angle",
    srcs = ["angle.test.cpp"],
    copts = KFCPP_CPP_COMPILER_FLAGS,
    deps = [
        "@com_github_catchorg_catch2//:catch2",
        "//test/utils/catch:main",
        "//test/utils/catch:approx_periodic",
        "//src/math/angle",
    ],
    testonly = True,
    visibility = ["//visibility:public"],
)
