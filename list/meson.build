list_srcs = [
  'main.c',
  'rgb2ansi.c',
]
list_deps = [
     ms_dep,
     bytes_dep,
     c_embed_dep,
     generic_print_dep,
     debug_print_h_dep,
     djbhash_dep,
     c_string_buffer_dep,
     timestamp_dep,
     parson_shared_dep,
     cstringfn_dep,
     fs_dep,
     ansi_codes_dep,
     ansi_rgb_utils_dep,
     rgba_dep, 
     #     svpng_dep, spng_dep,
     cargs_dep,
     c_timer_dep,
     c_fsio_dep,
     embedded_palettes_dep,
]
if get_option('enable-binaries')
  list_exec = executable('list',
     [
      list_srcs,
      files(submodule_srcs),
     ],
     dependencies: list_deps,
     include_directories: [
       '../submodules/c_string_buffer/include',
       inc,
     ],
  )
  if get_option('enable-binary-tests')
    test('list', list_exec, args: ['--test'])
  endif
endif
