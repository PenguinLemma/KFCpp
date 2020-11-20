load(
    "//config:copts/cpp_flags.bzl",
    "KFCPP_CPP_COMPILER_FLAGS"
)

cc_library(
    name = "main",
    srcs = ["catch-main.cpp"],
    copts = KFCPP_CPP_COMPILER_FLAGS,
    deps = ["@com_github_catchorg_catch2//:catch2"],
    testonly = True,
    visibility = ["//visibility:public"],
)

cc_library(
    name = "approx_periodic",
    hdrs = ["approx_periodic.hpp"],
    copts = KFCPP_CPP_COMPILER_FLAGS,
    deps = [
        "@com_github_catchorg_catch2//:catch2",
        "//test/utils/floating_point",
    ],
    testonly = True,
    visibility = ["//visibility:public"],
)

