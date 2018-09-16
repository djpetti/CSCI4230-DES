cc_library(
  name = "transfer",
  srcs = glob(["*.cc"], exclude=["*_main.cc"]),
  hdrs = glob(["*.h"]),
  deps = ["//des:des"],
)

cc_binary(
  name = "server",
  srcs = ["server_main.cc"],
  deps = [":transfer"],
  visibility = ["//visibility:public"],
)

cc_binary(
  name = "client",
  srcs = ["client_main.cc"],
  deps = [":transfer"],
  visibility = ["//visibility:public"],
)
