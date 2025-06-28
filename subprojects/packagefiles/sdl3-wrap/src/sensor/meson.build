opt_use_sensor = get_option('use_sensor')
cdata.set('SDL_SENSOR_DISABLED', opt_use_sensor.disabled())

if opt_use_sensor.allowed()
  if host_machine.system() == 'windows'
    subdir('windows')
  else
    # TODO Can be enabled by SDL_SENSOR_DUMMY too
    subdir('dummy')
  endif
endif

sources += files('SDL_sensor.c')
