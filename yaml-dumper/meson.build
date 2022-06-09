yaml_dumper_srcs = [
  'yaml-dumper.c',
]
yaml_dumper_deps = [
     djbhash_dep,
     libyaml_dep,
]
yaml_dumper_exec = executable('yaml-dumper',
     yaml_dumper_srcs,
     dependencies: yaml_dumper_deps,
     include_directories: []
  )
if get_option('enable-binary-tests')
    test('yaml-dumper', yaml_dumper_exec, args: ['../etc/test-theme.yaml'])
endif
