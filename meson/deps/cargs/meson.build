cargs_lib = library('cargs', 
'../../../submodules/cargs/src/cargs.c',
install: false, dependencies: [
  ], 
  include_directories: [
    '../../../submodules/cargs',
    '../../../submodules/cargs/src',
  ]
)
cargs_dep = declare_dependency(include_directories: [
'../../../submodules/cargs/src',
  ], link_with: cargs_lib)
#deps += b64_dep
