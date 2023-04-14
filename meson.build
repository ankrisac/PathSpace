project(
  'PathSpace', 'cpp',
  version : '0.1',
  default_options : [
    'warning_level=3'
  ]
)

executable(
  'main', 
  sources: [
    'src/main.cpp',
    'src/canvas.cpp'
  ],
  dependencies: [
    dependency('glfw3'),
    dependency('epoxy'),
    dependency('glm'),
    dependency('fmt')
  ],
  cpp_args: [
    '-Wno-unused-parameter',
  ]
)
