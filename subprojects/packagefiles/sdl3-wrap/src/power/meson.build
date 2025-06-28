opt_use_power = get_option('use_power')
cdata.set('SDL_POWER_DISABLED', opt_use_power.disabled())

if opt_use_power.allowed()
  if host_machine.system() == 'windows'
    subdir('windows')
  elif host_machine.system() == 'darwin'
    subdir('macos')
  else
    subdir('linux')
  endif
endif

sources += files('SDL_power.c')
