gnometest_sources = [
  'main.cpp'
]

gnometest_deps = [
  dependency(
    'gtk4',
    version: '>= 4.0.0',
    fallback: ['gtk4', 'gtk4'],
    default_options: [
      'build-demos=false',
      'build-testsuite=false',
      'build-tests=false',
      'build-examples=false',
      'media-gstreamer=disabled',
      'print-cups=disabled',
    ]
  ),
  dependency(
    'gtkmm-4.0',
    version: '>= 4.0.0',
    fallback: ['gtkmm4', 'gtkmm-4.0'],
    default_options: [
      'build-demos=false',
      'build-tests=false',
    ]
  ),
  dependency(
    'libsoup-3.0',
    version: '>= 3.0.0',
    fallback: ['libsoup-3.0', 'libsoup3'],
    default_options: [
      'docs=disabled',
      'tests=false',
    ]
  ),
]

executable('gnometest', gnometest_sources,
  dependencies: gnometest_deps,
  install: true,
  win_subsystem: 'windows'
)

