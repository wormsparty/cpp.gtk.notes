gnometest_sources = [
  'main.cpp'
]

gnometest_deps = [
  dependency('gtk4'),
  dependency('libsoup-3.0'),
]

executable('gnometest', gnometest_sources,
  dependencies: gnometest_deps,
  install: true,
  win_subsystem: 'windows'
)

