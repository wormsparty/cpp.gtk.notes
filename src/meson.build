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
]

executable('gnometest', gnometest_sources,
  dependencies: gnometest_deps,
  install: true,
  win_subsystem: 'windows'
)

