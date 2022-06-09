embedded_palettes_lib = library('embedded_palettes', 
  '../../../embeds/tbl1.c',
  install: false, 
  dependencies: [
    c_timer_dep,
  ], 
  include_directories: [
    inc,
    '../../../',
    '../../../embeds',
  ]
)
embedded_palettes_dep = declare_dependency(include_directories: 
  [
    inc,
    '../../../embeds',
  ], 
  link_with: embedded_palettes_lib,
)
