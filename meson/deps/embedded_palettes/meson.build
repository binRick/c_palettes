embedded_palettes_lib = library('embedded_palettes', 
  #//pwd+'/embeds/tbl1.c',
  install: false, 
  dependencies: [
  ], 
  include_directories: [
    inc,
  ]
)
embedded_palettes_dep = declare_dependency(include_directories: 
  [
    inc,
  ], 
  link_with: embedded_palettes_lib,
)
