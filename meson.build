project('roaring_race', 'cpp')


# graphics things:
sfml_dep = [
  dependency('sfml-system'),
  dependency('sfml-graphics'),
  dependency('sfml-window'),
]

graphics_lib = library('graphics','graphics/sfml_window.cc', dependencies : sfml_dep)
graphics_dep = [sfml_dep , declare_dependency(link_with : graphics_lib)]
# ---

# physics things:
box2d_dep = dependency('box2d')

physics_lib = library('physics', 'physics/car.cc', dependencies : box2d_dep)
physics_dep = [box2d_dep, declare_dependency(link_with : physics_lib)]
# ---

# common:
common_lib = library('common', ['common/common_data.h', 'common/key_handler.h'], dependencies: sfml_dep)
common_dep = [sfml_dep, declare_dependency(link_with: common_lib)]
# ---

executable('sfml_window_test', 'sfml_window_test.cc', dependencies : graphics_dep)
executable('sfml_car_test', 'sfml_car_test.cc', dependencies : graphics_dep)

executable('physics_test', 'physics_test.cc', dependencies : physics_dep)
executable('sfml_events_test', 'sfml_event_test.cc', dependencies: sfml_dep)

executable('roaring_race', 'roaring_race.cc', dependencies: [graphics_dep, physics_dep, common_dep])