project('c_palettes', ['c'],
  default_options: [
   'warning_level=0',
   'c_std=gnu99',
  ]
)
bins = [
  'm1',
]
config = configuration_data()
version = run_command('git', 'describe', '--tags')
config.set('version', version.stdout().strip())
add_project_arguments('-D_GNU_SOURCE', language : 'c')
add_project_arguments('-DDBG_H_DEF_ONCE', language : 'c')
add_project_arguments('-DDBG_H_NO_WARNING', language : 'c')
add_project_arguments('-DASSERTF_DEF_ONCE', language : 'c')
add_project_arguments('-DLOG_USE_COLOR', language : 'c')
add_project_arguments('-DLOG_LEVEL=TRACE', language : 'c')
add_project_arguments('-Wno-pragma-once-outside-header', language : 'c')
add_project_arguments('-Wno-incompatible-pointer-types', language : 'c')
add_project_arguments('-Wno-pointer-sign', language : 'c')
add_project_arguments('-Wno-unused-value', language : 'c')
add_project_arguments('-Wno-gnu-alignof-expression', language : 'c')
add_project_arguments('-Wno-varargs', language : 'c')
add_project_arguments('-Wno-return-type', language : 'c')
#//***********************************************************/
deps = []
bin_srcs = []
submodule_srcs = []
cc = meson.get_compiler('c')
#//***********************************************************/
sh = find_program('sh', required : true)
pwd_bin = find_program('pwd', required : true)
pwd = run_command(pwd_bin, check: true).stdout().strip()
inc = include_directories(
  './',
  #'db/',
  #  'embeds/',
  'submodules/',
  'submodules/c_ansi',
  'submodules/meson_deps',
  'submodules/meson_deps/submodules',
  'submodules/meson_deps/submodules/c_fsio/include',
  'submodules/meson_deps/submodules/c_string_buffer/include',
  'submodules/meson_deps/submodules/c_stringfn/include',
  'meson/deps/ms',
  #'submodules/c_fsio/include',
  #'submodules/c_string_buffer/include',
  #'submodules/c_stringfn/include',
  #'submodules/c_embed',
  #  'submodules/debug_print_h',
  #'submodules/generic-print',
  #'submodules/c_timer',
  #'submodules/b64.c',
)
#//***********************************************************/
#//////////////////////////////////////////////////////////////////////
subdir('submodules/meson_deps/meson/deps/cargs')
subdir('submodules/meson_deps/meson/deps/libyaml')
subdir('submodules/meson_deps/meson/deps/b64')
subdir('submodules/meson_deps/meson/deps/termpaint')
subdir('submodules/meson_deps/meson/deps/parson')
subdir('submodules/meson_deps/meson/deps/c_string_buffer')
subdir('submodules/meson_deps/meson/deps/c_stringfn')
subdir('submodules/meson_deps/meson/deps/c_fsio')
subdir('submodules/meson_deps/meson/deps/asprintf')
subdir('submodules/meson_deps/meson/deps/generic_print')
#subdir('submodules/meson_deps/meson/deps/debug_print_h')
subdir('submodules/meson_deps/meson/deps/c_timer')
subdir('submodules/meson_deps/meson/deps/timestamp')
subdir('submodules/meson_deps/meson/deps/c_embed')
subdir('submodules/meson_deps/meson/deps/djbhash')
subdir('submodules/meson_deps/meson/deps/dbg')
subdir('submodules/meson_deps/meson/deps/rgba')
#subdir('submodules/meson_deps/meson/deps/occurrences')
subdir('submodules/meson_deps/meson/deps/fs')
subdir('submodules/meson_deps/meson/deps/ms')
subdir('submodules/meson_deps/meson/deps/bytes')
subdir('submodules/c_ansi/ansi-codes')
subdir('submodules/c_ansi/ansi-rgb-utils')
subdir('submodules/c_ansi/ansi-utils')
subdir('submodules/c_ansi/term-utils')
subdir('submodules/c_ansi/term-termpaint')
subdir('meson/deps/embedded_palettes')
#//////////////////////////////////////////////////////////////////////
#subdir('meson/deps')
#//////////////////////////////////////////////////////////
subdir('yaml-dumper')
subdir('db')
subdir('palette-utils')
subdir('palettes')
subdir('menu')
subdir('parser')
#subdir('palette-names')
