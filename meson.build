project('c_palettes', ['c'], default_options: ['warning_level=0','c_std=gnu99'])
cc = meson.get_compiler('c')
meson_deps_dir = join_paths(meson.current_source_dir(),'submodules/meson_deps')
pwd = meson.current_source_dir()
submodule_srcs = []
inc = [
  include_directories(
   './submodules/meson_deps',
   './submodules/meson_deps/submodules',
   './submodules/meson_deps/vendor',
   './submodules/meson_deps/vendor/sqlite',
  ),  
  include_directories(
    'submodules/c_ansi',
    './',
  ),
]
#############################################################################
subdir('submodules/meson_deps/meson/args')
subdir('submodules/meson_deps/meson/inc')
#//***********************************************************/
subdir('submodules/meson_deps/submodules/c_ansi/ansi-codes')
subdir('submodules/meson_deps/meson/deps/incbin')
subdir('submodules/meson_deps/meson/deps/wildcardcmp')
subdir('submodules/meson_deps/meson/deps/cargs')
subdir('submodules/meson_deps/meson/deps/libyaml')
subdir('submodules/meson_deps/meson/deps/b64')
subdir('submodules/meson_deps/meson/deps/parson')
subdir('submodules/meson_deps/meson/deps/c_string_buffer')
subdir('submodules/meson_deps/meson/deps/c_stringfn')
subdir('submodules/meson_deps/meson/deps/debug-memory')
subdir('submodules/meson_deps/meson/deps/strdup')
subdir('submodules/meson_deps/meson/deps/tempdir')
subdir('submodules/meson_deps/meson/deps/c_fsio')
subdir('submodules/meson_deps/meson/deps/asprintf')
subdir('submodules/meson_deps/meson/deps/generic_print')
subdir('submodules/meson_deps/meson/deps/timestamp')
subdir('submodules/meson_deps/meson/deps/ms')
subdir('submodules/meson_deps/meson/deps/c_timer')
subdir('submodules/meson_deps/meson/deps/djbhash')
subdir('submodules/meson_deps/meson/deps/dbg')
subdir('submodules/meson_deps/meson/deps/rgba')
subdir('submodules/meson_deps/meson/deps/fs')
subdir('submodules/meson_deps/meson/deps/bytes')
subdir('submodules/meson_deps/meson/deps/termpaint')
#//////////////////////////////////////////////////////////////////////
subdir('submodules/meson_deps/submodules/c_ansi/ansi-rgb-utils')
subdir('submodules/meson_deps/submodules/c_ansi/ansi-utils')
subdir('submodules/meson_deps/submodules/c_ansi/term-utils')
#//////////////////////////////////////////////////////////////////////
#subdir('meson/deps/embedded_palettes')
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
