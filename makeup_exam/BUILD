cc_library(
  name = "makeup_exam",
  srcs = glob(["*.cc"],
              exclude=["is_prime.cc", "factor.cc"]),
  hdrs = glob(["*.h"]),
  deps = ["//numerical"],
)

cc_binary(
  name = "is_prime",
  srcs = ["is_prime.cc"],
  deps = [":makeup_exam"],
)

cc_binary(
  name = "factor",
  srcs = ["factor.cc"],
  deps = [":makeup_exam"],
)
