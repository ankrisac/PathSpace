project(
  'PathSpace', 'cpp',
  version : '0.1',
  default_options : [
    'cpp_std=c++20',
    'warning_level=3'
  ]
)

executable(
  'main', 
  sources: [
    'src/main.cpp',
    'src/gfx.cpp',
    'src/canvas.cpp',
  ],
  dependencies: [
    dependency('glfw3'),
    dependency('epoxy'),
    dependency('glm'),
    dependency('fmt')
  ],
  cpp_args: [
    '-Wno-unused-parameter',
    '-Wno-volatile'
  ]
)
