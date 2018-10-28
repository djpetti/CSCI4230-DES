cc_library(
  name = "makeup_exam",
  srcs = glob(["*.cc"]),
  hdrs = glob(["*.h"]),
  deps = ["//numerical"],
)

cc_binary(
  name = "is_prime",
  srcs = ["is_prime.cc"],
  deps = [":makeup_exam"],
)
