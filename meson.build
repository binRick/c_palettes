project('c_palettes', ['c'], default_options: ['warning_level=0','c_std=gnu99'], 
  subproject_dir: 'submodules/meson_deps/subprojects',
  version: '0.0.1',
)
cc = meson.get_compiler('c')
meson_deps_dir = join_paths(meson.current_source_dir(),'submodules/meson_deps')
pwd = meson.current_source_dir()
submodule_srcs = []
inc = [
  include_directories(
        'palette-utils',
        'embeds',
        '.',
  ),
  include_directories(
        'submodules/meson_deps/submodules/list/src',
        'submodules/meson_deps/submodules/debug_print_h/include',
        'submodules/meson_deps/submodules/debug_print_h',
        'submodules/meson_deps/etc',
        'submodules/meson_deps',
  ),  
  include_directories(
        'submodules/meson_deps/submodules/c_ansi',
        'submodules/meson_deps/submodules',
        ),
  include_directories(
        'submodules/meson_deps/submodules/progressbar/include',
        'submodules/meson_deps/submodules/progressbar/lib',
        'submodules/meson_deps/submodules/progressbar/include/progressbar',
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
