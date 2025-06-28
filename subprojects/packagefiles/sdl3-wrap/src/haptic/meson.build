opt_use_haptic = get_option('use_haptic')
cdata.set('SDL_HAPTIC_DISABLED', opt_use_haptic.disabled())

if opt_use_haptic.allowed()
  if host_machine.system() == 'windows'
    cdata.set('SDL_HAPTIC_DINPUT', 1)
    subdir('windows')
  elif host_machine.system() == 'darwin'
    cdata.set('SDL_HAPTIC_IOKIT', 1)
    subdir('darwin')
  else
    cdata.set('SDL_HAPTIC_LINUX', 1)
    subdir('linux')
  endif
else
  # TODO Can be enabled by SDL_HAPTIC_DUMMY too
  subdir('dummy')
endif

sources += files('SDL_haptic.c')
