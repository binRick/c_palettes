palettes_srcs = [
  'palettes.c',
  #  'palette-utils.c',
]
palettes_deps = [
     embedded_palettes_dep,
     djbhash_dep,
     palette_utils_dep,
]
palettes_exec = executable('palettes',
    palettes_srcs,
    dependencies: palettes_deps,
    include_directories: [
       inc,
    ],
  )
if get_option('enable-binary-tests')
    test('palettes', palettes_exec, args: ['--test'])
endif
