cc_library(
  name = "analysis",
  srcs = glob(["*.cc"], exclude=["*_main.cc"]),
  hdrs = glob(["*.h"]),
  deps = ["//des:des", "//des/common:common"],
)

cc_binary(
  name = "diff_analysis",
  srcs = ["diff_analysis_main.cc"],
  deps = [":analysis"],
)
