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
  './submodules/c_string_buffer/include',
  './submodules',
)
#//***********************************************************/
subdir('submodules/c_ansi/ansi-codes')
subdir('submodules/c_ansi/ansi-rgb-utils')
subdir('submodules/c_ansi/ansi-utils')
subdir('meson/deps')
subdir('meson/deps/embedded_palettes')
cargs_dep = dependency('cargs',fallback:['cargs', 'cargs_dep'])
termpaint_dep = dependency('termpaint', required: true)
spng_dep = dependency('libspng',fallback:['libspng', 'spng_dep'])
#c_colors_dep = dependency('c_colors',fallback:['c_colors', 'c_colors_dep'])
parson_shared_dep = dependency('parson',fallback:['parson', 'parson_shared_dep'])
subdir('list')
subdir('menu')
