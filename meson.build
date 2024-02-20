project('roaring_race', 'cpp')



sfml_dep = [
  dependency('sfml-system'),
  dependency('sfml-graphics'),
  dependency('sfml-window'),
]

graphics_lib = library('roaring_race_graphics','graphics/window.cc', dependencies : sfml_dep)

graphics_dep = declare_dependency(dependencies : sfml_dep, link_with :graphics_lib)

executable('sfml_window_test', 'sfml_window_test.cc', dependencies : graphics_dep)