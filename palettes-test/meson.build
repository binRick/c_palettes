palettes_test_srcs = [
  'palettes-test.c',
]
palettes_test_inc = [
 inc,
]
palettes_test_deps = [
  cargs_dep,
  c_string_buffer_dep,
  c_stringfn_dep,
  c_fsio_dep,
  timestamp_dep,
  bytes_dep,
  rgba_dep,
  djbhash_dep,
  palettes_dep,
  wildcardcmp_dep,
  ms_dep,
  parson_dep,
  libyaml_dep,
  generic_print_dep,
  c_timer_dep,
  ansi_rgb_utils_dep,
  ansi_utils_dep,
  ansi_codes_dep,
  dbg_dep,
]

if get_option('enable-binaries')
  palettes_test_exec = executable('palettes-test', palettes_test_srcs,
     dependencies: palettes_test_deps,
     include_directories: palettes_test_inc,
  )
  if get_option('enable-binary-tests')
    test('palettes-test', palettes_test_exec, args: ['--test'])
  endif
endif
