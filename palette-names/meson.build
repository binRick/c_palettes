palette_names_srcs = [
  'palette-names.c',
]
palette_names_deps = [
  #c_string_buffer_dep,
  #c_stringfn_dep,
  #  embedded_palettes_dep,
  #djbhash_dep,
  palette_utils_dep,
]
palette_names_exec = executable('palette-names',
    palette_names_srcs,
    dependencies: palette_names_deps,
    include_directories: [
       inc,
    ],
  )
if get_option('enable-binary-tests')
    test('palette-names', palette_names_exec, args: ['--test'])
endif
