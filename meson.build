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
    'src/main.cpp'
  ],
  dependencies: [
    dependency('glfw3'),
    dependency('epoxy'),
    dependency('glm')
  ],
  link_args: '-Wno-unused-arguments'
)
