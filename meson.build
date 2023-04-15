project(
  'PathSpace', 'cpp',
  version : '0.1',
  default_options : [
    'cpp_std=c++20',
    'warning_level=3'
  ]
)

cc = meson.get_compiler('c')

executable(
  'main', 
  sources: [
    'src/main.cpp',
    'src/gfx.cpp',
    'src/canvas.cpp',
    'src/frame.cpp',

    'src/shapes/plane.cpp',
    'src/shapes/sphere.cpp',
  ],
  dependencies: [
    cc.find_library('m', required: false),
    dependency('glfw3'),
    dependency('epoxy'),
    dependency('glm'),
    dependency('fmt'),
  ],
  cpp_args: [
    '-Wno-unused-parameter',
    '-Wno-volatile'
  ]
)
