parser_srcs = [
  'parser.c',
]
parser_deps = [
     c_string_buffer_dep,
     c_fsio_dep,c_stringfn_dep,
     cargs_dep,
     parson_dep,
     db_dep,
     palette_utils_dep,
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
    test('parser', parser_exec, args: ['--test'])
    #'-c','100','-C','etc/colornames.bestof.csv','-J','etc/colornames.test.json','-D','etc/colornames.test.sqlite','-m','json'])
  endif
endif
