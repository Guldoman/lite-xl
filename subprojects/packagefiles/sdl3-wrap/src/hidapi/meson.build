opt_use_hidapi = get_option('use_hidapi')
cdata.set('SDL_HIDAPI_DISABLED', opt_use_hidapi.disabled())

if opt_use_hidapi.allowed()
  if host_machine.system() == 'windows'
    subdir('windows')
  elif host_machine.subsystem() == 'macos'
    #  subdir('mac')
  else
    subdir('linux')
  endif
endif

sources += files('SDL_hidapi.c')
