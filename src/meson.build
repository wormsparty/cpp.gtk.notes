gnometest_sources = [
  'main.cpp'
]

gnometest_deps = [
  dependency(
    'gtk4',
    version: '>= 4.0.0',
    fallback: ['gtk', 'gtk4'],
    default_options: [
      'introspection=disabled',
      'build-demos=false',
      'build-testsuite=false',
      'build-tests=false',
      'build-examples=false',
      'media-gstreamer=disabled',
      'print-cups=disabled',
    ]
  ),
  dependency(
    'libsoup-3.0',
    version: '>= 3.0.0',
    fallback: ['libsoup-3.0', 'libsoup3'],
    default_options: [
      'introspection=disabled',
      'vapi=disabled',
      'docs=disabled',
      'tests=false',
      'autobahn=disabled',
      'pkcs11_tests=disabled',
    ]
  ),
  dependency('glib-2.0'),
]

executable('gnometest', gnometest_sources,
  dependencies: gnometest_deps,
  install: true,
  win_subsystem: 'windows'
)

