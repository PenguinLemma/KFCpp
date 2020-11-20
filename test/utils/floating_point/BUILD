load(
    "//config:copts/cpp_flags.bzl",
    "KFCPP_CPP_COMPILER_FLAGS"
)

cc_library(
    name = "floating_point",
    hdrs = ["floating_point.hpp"],
    copts = KFCPP_CPP_COMPILER_FLAGS,
    deps = [],
    testonly = True, # perhaps should be promoted to utility, and not only-test utility
    visibility = ["//visibility:public"],
)
