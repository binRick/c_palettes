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
inc = include_directories(
  './',
  '../',
  '../../',
  '../../../',
  'db/',
  'embeds/',
  'submodules/',
  'submodules/c_ansi',
)
#//***********************************************************/
cargs_dep = dependency('cargs',fallback:['cargs', 'cargs_dep'])
termpaint_dep = dependency('termpaint', required: true)
parson_shared_dep = dependency('parson',fallback:['parson', 'parson_shared_dep'])
#//////////////////////////////////////////////////////////////////////
subdir('submodules/c_ansi/ansi-codes')
subdir('submodules/c_ansi/ansi-rgb-utils')
subdir('submodules/c_ansi/ansi-utils')
subdir('submodules/c_ansi/term-utils')
subdir('submodules/c_ansi/term-termpaint')
#//////////////////////////////////////////////////////////////////////
subdir('meson/deps')
subdir('meson/deps/embedded_palettes')
subdir('meson/deps/libyaml')
subdir('yaml-dumper')
subdir('db')
subdir('palette-utils')
subdir('palettes')
subdir('menu')
subdir('parser')
