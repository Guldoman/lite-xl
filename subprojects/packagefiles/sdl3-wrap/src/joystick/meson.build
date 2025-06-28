opt_use_joystick = get_option('use_joystick')
cdata.set('SDL_JOYSTICK_DISABLED', opt_use_joystick.disabled())

if opt_use_joystick.allowed()
  if host_machine.system() == 'windows'
    cdata.set('SDL_JOYSTICK_DINPUT', 1)
    cdata.set('SDL_JOYSTICK_XINPUT', 1)
    subdir('hidapi')
    subdir('windows')
  elif host_machine.system() == 'darwin'
    cdata.set('SDL_JOYSTICK_IOKIT', 1)
    cdata.set('SDL_JOYSTICK_MFI', 1)
    subdir('apple')
    subdir('darwin')
  else
    cdata.set('SDL_JOYSTICK_LINUX', 1)
    subdir('linux')
  endif
else
  # TODO Can be enabled by SDL_JOYSTICK_DUMMY too
  subdir('dummy')
endif

# FIXME: there is a bug in 3.2.10 that requires virtual joysticks to be enabled
# Issue: https://github.com/libsdl-org/SDL/issues/12599
# Fix: https://github.com/libsdl-org/SDL/commit/fbdb6379781f2874860fe3079f452ec1efbca1ac
subdir('virtual')
cdata.set('SDL_JOYSTICK_VIRTUAL', 1)

sources += files(
  'SDL_gamepad.c',
  'SDL_joystick.c',
  'SDL_steam_virtual_gamepad.c',
  'controller_type.c',
)
