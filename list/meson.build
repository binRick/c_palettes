list_srcs = [
  'main.c',
]
list_deps = [
     ms_dep,
     bytes_dep,
     c_embed_dep,
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
     embedded_palettes_dep,
     palette_utils_dep,
]
if get_option('enable-binaries')
  list_exec = executable('list',
     list_srcs,
     dependencies: list_deps,
     include_directories: [
       '../submodules/meson_deps/submodules/c_string_buffer/include',
       inc,
     ],
  )
  if get_option('enable-binary-tests')
    test('list', list_exec, args: ['--test'])
  endif
endif
