if not wayland_scanner.found()
  subdir_done()
endif

wayland_client_dep = dependency(
  'wayland-client',
  required: opt_use_video_wayland,
)
wayland_egl_dep = dependency(
  'wayland-egl',
  required: opt_use_video_wayland,
)
wayland_cursor_dep = dependency(
  'wayland-cursor',
  required: opt_use_video_wayland,
)
xkbcommon_dep = dependency(
  'xkbcommon',
  required: opt_use_video_wayland,
)

has_wayland_minimum = (
  wayland_client_dep.found()
  and wayland_egl_dep.found()
  and wayland_cursor_dep.found()
  and xkbcommon_dep.found()
)

cdata.set('SDL_VIDEO_DRIVER_WAYLAND', has_wayland_minimum)
if not has_wayland_minimum
  subdir_done()
endif

opt_use_video_wayland_shared = get_option('use_video_wayland_shared')
if opt_use_video_wayland_shared.allowed()
  wayland_client_dep = wayland_client_dep.partial_dependency(
    compile_args: true,
    includes: true,
    sources: true,
  )
  cdata.set_quoted('SDL_VIDEO_DRIVER_WAYLAND_DYNAMIC', 'libwayland-client.so')

  wayland_egl_dep = wayland_egl_dep.partial_dependency(
    compile_args: true,
    includes: true,
    sources: true,
  )
  cdata.set_quoted('SDL_VIDEO_DRIVER_WAYLAND_DYNAMIC_EGL', 'libwayland-egl.so')

  wayland_cursor_dep = wayland_cursor_dep.partial_dependency(
    compile_args: true,
    includes: true,
    sources: true,
  )
  cdata.set_quoted('SDL_VIDEO_DRIVER_WAYLAND_DYNAMIC_CURSOR', 'libwayland-cursor.so')

  xkbcommon_dep = xkbcommon_dep.partial_dependency(
    compile_args: true,
    includes: true,
    sources: true,
  )
  cdata.set_quoted('SDL_VIDEO_DRIVER_WAYLAND_DYNAMIC_XKBCOMMON', 'libxkbcommon.so')
endif
sdl_deps += [wayland_client_dep, wayland_egl_dep, wayland_cursor_dep, xkbcommon_dep]

opt_use_video_wayland_libdecor = get_option('use_video_wayland_libdecor')
opt_use_video_wayland_libdecor_shared = get_option('use_video_wayland_libdecor_shared')
wayland_libdecor_dep = dependency(
  'libdecor-0',
  required: opt_use_video_wayland_libdecor,
)
if wayland_libdecor_dep.found()
  cdata.set('HAVE_WAYLAND_LIBDECOR', true)
  cdata.set('HAVE_LIBDECOR_H', 1)
  libdecor_version_arr = wayland_libdecor_dep.version().split('.')
  if libdecor_version_arr.length() != 3
    libdecor_version_arr = ['0', '1', '0']
  endif
  cdata.set('SDL_LIBDECOR_VERSION_MAJOR', libdecor_version_arr[0].to_int())
  cdata.set('SDL_LIBDECOR_VERSION_MINOR', libdecor_version_arr[1].to_int())
  cdata.set('SDL_LIBDECOR_VERSION_PATCH', libdecor_version_arr[2].to_int())
  if opt_use_video_wayland_libdecor_shared.allowed()
    wayland_libdecor_dep = wayland_libdecor_dep.partial_dependency(
      compile_args: true,
      includes: true,
      sources: true,
    )
    cdata.set_quoted('SDL_VIDEO_DRIVER_WAYLAND_DYNAMIC_LIBDECOR', 'libdecor-0.so')
  endif
  sdl_deps += wayland_libdecor_dep
endif

sources += files(
  'SDL_waylandclipboard.c',
  'SDL_waylandcolor.c',
  'SDL_waylanddatamanager.c',
  'SDL_waylanddyn.c',
  'SDL_waylandevents.c',
  'SDL_waylandkeyboard.c',
  'SDL_waylandmessagebox.c',
  'SDL_waylandmouse.c',
  'SDL_waylandopengles.c',
  'SDL_waylandshmbuffer.c',
  'SDL_waylandvideo.c',
  'SDL_waylandvulkan.c',
  'SDL_waylandwindow.c',
)
