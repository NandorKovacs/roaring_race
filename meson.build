project('roaring_race', 'cpp')


# graphics things:
sfml_dep = [
  dependency('sfml-system'),
  dependency('sfml-graphics'),
  dependency('sfml-window'),
]

#raylib_proj = subproject('raylib')
#raylib_dep = raylib_proj.get_variable('raylib_dep')
raylib_dep = dependency('raylib', fallback: ['raylib', 'raylib_dep'])

sfml_graphics_lib = library('sfml_graphics','graphics/sfml_window.cc', dependencies : sfml_dep)
sfml_graphics_dep = [sfml_dep , declare_dependency(link_with : sfml_graphics_lib)]

raylib_graphics_lib = library('raylib_graphics','graphics/raylib_window.cc', dependencies : raylib_dep)
raylib_graphics_dep = [raylib_dep , declare_dependency(link_with : raylib_graphics_lib)]

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

executable('sfml_window_test', 'sfml_window_test.cc', dependencies : sfml_graphics_dep)
executable('sfml_car_test', 'sfml_car_test.cc', dependencies : sfml_graphics_dep)

executable('physics_test', 'physics_test.cc', dependencies : physics_dep)
executable('sfml_events_test', 'sfml_event_test.cc', dependencies: sfml_dep)

executable('roaring_race', 'roaring_race.cc', dependencies: [raylib_graphics_dep, physics_dep, common_dep])
executable('raylib_car_test', 'raylib_car_test.cc', dependencies: [raylib_graphics_dep, common_dep])