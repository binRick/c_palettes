parser_srcs = [
  'parser.c',
]
parser_deps = [
     cargs_dep,
     parson_shared_dep,
     c_fsio_dep,
     palette_utils_dep,
     db_dep,
     c_string_buffer_dep,
]
if get_option('enable-binaries')
  parser_exec = executable('parser',
     [
      parser_srcs,
     ],
     dependencies: parser_deps,
     include_directories: [
      inc,
      '../',
     ],
  )
  if get_option('enable-binary-tests')
    test('parser Test', parser_exec, args: ['--test'])
  endif
endif
