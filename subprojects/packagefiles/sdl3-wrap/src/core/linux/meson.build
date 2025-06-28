sources += files(
  'SDL_dbus.c',
  'SDL_evdev.c',
  'SDL_evdev_capabilities.c',
  'SDL_evdev_kbd.c',
  'SDL_fcitx.c',
  'SDL_ibus.c',
  'SDL_ime.c',
  'SDL_system_theme.c',
  'SDL_threadprio.c',
)

opt_use_libudev = get_option('use_libudev')
if opt_use_libudev.allowed()
  has_libudev_header = cc.has_header('libudev.h')
  cdata.set('HAVE_LIBUDEV_H', has_libudev_header)
  if has_libudev_header
    libudev_dep = dependency(
      'libudev',
      required: false,
    )
    if libudev_dep.found()
      libudev_dep = libudev_dep.partial_dependency(
        compile_args: true,
        includes: true,
        sources: true,
      )
      cdata.set_quoted('SDL_UDEV_DYNAMIC', 'libudev.so')
      cdata.set('HAVE_LIBUDEV', true)
      sdl_deps += libudev_dep
    endif
    sources += files(
      'SDL_udev.c',
    )
  endif
endif

