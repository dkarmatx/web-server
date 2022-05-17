project('web-server', 'cpp',
    default_options : ['cpp_std=c++17']
)

include_dir     = ['src/']
cpp_args        = ['-Wall', '-Werror', '-Wextra']

srcs = [
    'src/http/url.h', 'src/http/url.cc',

    'src/util/expected.h',
    'src/util/string.h', 'src/util/string.cc',
    'src/util/charset.h', 'src/util/charset.cc',

    'src/util/iterator/enumerate.h',
]

test_srcs = [
    'src/tests/http-url-set-scheme.cc',
    'src/tests/http-url-set-opaque.cc',
    'src/tests/http-url-set-userinfo.cc',
    'src/tests/http-url-set-path.cc',
    'src/tests/http-url-set-query.cc',
]


executable('web-server', srcs + 'src/main.cc',
    include_directories: include_dir,
    cpp_args: cpp_args
)

gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_dep')
gtest_main_dep = gtest_proj.get_variable('gtest_main_dep')

executable('web-server-test', srcs + test_srcs,
    include_directories: include_dir,
    cpp_args: cpp_args,
    dependencies: [ gtest_dep, gtest_main_dep ]
)
