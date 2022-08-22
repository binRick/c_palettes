project('c_palettes', ['c'], default_options: ['warning_level=0','c_std=gnu99','default_library=shared'], 
  subproject_dir: 'submodules/c_deps/subprojects',
  version: '0.0.1',
)
cc = meson.get_compiler('c')
c_deps_dir = join_paths(meson.current_source_dir(),'submodules/c_deps')
pwd = meson.current_source_dir()
submodule_srcs = []
inc = [
  include_directories(
        'palette-utils',
        'embeds',
        '.',
  ),
  include_directories(
        'submodules/c_deps/submodules/list/src',
        'submodules/c_deps/submodules/debug_print_h/include',
        'submodules/c_deps/submodules/debug_print_h',
        'submodules/c_deps/etc',
        'submodules/c_deps',
  ),  
  include_directories(
        'submodules/c_deps/submodules/c_ansi',
        'submodules/c_deps/submodules',
        ),
  include_directories(
        'submodules/c_deps/submodules/progressbar/include',
        'submodules/c_deps/submodules/progressbar/lib',
        'submodules/c_deps/submodules/progressbar/include/progressbar',
  ),
]
#############################################################################
subdir('submodules')
#//////////////////////////////////////////////////////////////////////
subdir('yaml-dumper')
subdir('db')
subdir('palette-utils')
#//////////////////////////////////////
subdir('palettes')
subdir('palettes-test')
#//////////////////////////////////////
subdir('menu')
subdir('parser')
subdir('palette-names')
#//////////////////////////////////////////////////////////////////////
