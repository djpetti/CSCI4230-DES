cc_library(
  name = "key_exchange",
  srcs = glob(["*.cc"], exclude=["*_main.cc"]),
  hdrs = glob(["*.h"]),
  deps = ["//des", "//transfer/common"],
)

cc_binary(
  name = "key_server",
  srcs = ["key_server_main.cc"],
  deps = [":key_exchange"],
)

cc_binary(
  name = "kdc_client_node",
  srcs = ["client_node_main.cc"],
  deps = [":key_exchange"],
)
