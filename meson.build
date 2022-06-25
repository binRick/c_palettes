project('c_palettes', ['c'], default_options: ['warning_level=0','c_std=gnu99'], 
  subproject_dir: 'submodules/meson_deps/subprojects',
)
cc = meson.get_compiler('c')
meson_deps_dir = join_paths(meson.current_source_dir(),'submodules/meson_deps')
pwd = meson.current_source_dir()
submodule_srcs = []
inc = [
  include_directories(
   './submodules/meson_deps','./submodules/meson_deps/submodules','./submodules/meson_deps/vendor',
   './submodules/meson_deps/vendor/sqlite',
  ),  
  include_directories(
    'submodules/c_ansi','./',
  ),
]
#############################################################################
subdir('submodules')
#//////////////////////////////////////////////////////////////////////
#subdir('meson/deps/embedded_palettes')
#//////////////////////////////////////////////////////////////////////
subdir('yaml-dumper')
subdir('db')
subdir('palette-utils')
#//////////////////////////////////////
subdir('palettes')
#subdir('palettes-test')
#//////////////////////////////////////
subdir('menu')
#subdir('parser')
subdir('palette-names')
#//////////////////////////////////////////////////////////////////////
