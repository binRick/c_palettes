palettes_srcs = [
  'palettes.c',
  'tn.c',
]
palettes_deps = [
    parson_dep,
    djbhash_dep,
    incbin_dep,c_stringfn_dep,c_string_buffer_dep,
    #   spng_dep,
    #hex_png_pixel_utils_dep,
]
palettes_lib = library('palettes',
    palettes_srcs,
    dependencies: palettes_deps,
    include_directories: [
       inc,
    ],
)
palettes_dep = declare_dependency(
  include_directories: [inc],
  link_with: palettes_lib,
)

