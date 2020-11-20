load(
    "//config:copts/cpp_flags.bzl",
    "KFCPP_CPP_COMPILER_FLAGS"
)

cc_library(
    name = "angle",
    hdrs = [
        "angle.hpp",
    ],
    copts = KFCPP_CPP_COMPILER_FLAGS,
    visibility = [
        "//test:__subpackages__",
    ],
)
