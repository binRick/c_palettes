palettes_srcs = [
  'palettes.c',
  'tn.c',
]
palettes_deps = [
    parson_dep,
    djbhash_dep,
    incbin_dep,
    #   spng_dep,
    #hex_png_pixel_utils_dep,
]
palettes_lib = static_library('palettes',
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

