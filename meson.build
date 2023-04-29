project(
  'PathSpace', 'cpp',
  version : '0.1',
  default_options : [
    'cpp_std=c++20',
    'warning_level=3',
    'optimization=3'
  ]
)

cc = meson.get_compiler('cpp')


executable(
  'main', 
  sources: [
    'src/main.cpp',

    'src/graphics/backend.cpp',
    'src/graphics/canvas.cpp',
    'src/graphics/frame.cpp',
    'src/graphics/window.cpp',

    'src/shapes/sphere.cpp',
    'src/shapes/plane.cpp',
    'src/shapes/disc.cpp',
    'src/shapes/diamond.cpp',
    'src/shapes/shapelist.cpp'
  ],
  dependencies: [
    cc.find_library('m', required: false),
    dependency('glfw3'),
    dependency('epoxy'),
    dependency('pcg_cpp')
  ],
  cpp_args: [
    '-Wno-unused-parameter',
    '-Wno-unused-variable',
    '-Wno-volatile',
    '-fmax-errors=2'
  ]
)
