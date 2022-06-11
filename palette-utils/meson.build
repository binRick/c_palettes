palette_utils_srcs = [
  'palette-utils.c',
]
palette_utils_deps = [
  c_timer_dep,
  ms_dep,
  bytes_dep,
  c_embed_dep,
  generic_print_dep,
  #  debug_print_h_dep,
  djbhash_dep,
  c_string_buffer_dep,
  timestamp_dep,
  parson_dep,
  c_stringfn_dep,
  fs_dep,
  ansi_codes_dep,
  ansi_rgb_utils_dep,
  rgba_dep,
  cargs_dep,
  c_fsio_dep,
  #assertf_dep,
  embedded_palettes_dep,
]

palette_utils_lib = static_library('palette-utils',
  palette_utils_srcs,
  install: false,
  dependencies: palette_utils_deps,
  include_directories: [],
)


palette_utils_dep = declare_dependency(
  include_directories: [],
  link_with: palette_utils_lib,
)
