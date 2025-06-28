opt_use_video = get_option('use_video')
cdata.set('SDL_VIDEO_DISABLED', opt_use_video.disabled())

# Required by SDL_yuv.c
subdir('yuv2rgb')

if opt_use_video.allowed()
  if host_machine.system() == 'windows'
    cdata.set('SDL_VIDEO_DRIVER_WINDOWS', 1)
    subdir('windows')
  elif host_machine.system() == 'darwin'
    subdir('cocoa')
  else
    opt_use_video_x11 = get_option('use_video_x11')
    if opt_use_video_x11.allowed()
      subdir('x11')
    else
      cdata.set('SDL_VIDEO_DRIVER_X11', false)
    endif

    opt_use_video_wayland = get_option('use_video_wayland')
    if opt_use_video_wayland.allowed()
      subdir('wayland')
    else
      cdata.set('SDL_VIDEO_DRIVER_WAYLAND', false)
    endif
  endif
endif

sources += files(
  'SDL_RLEaccel.c',
  'SDL_blit.c',
  'SDL_blit_0.c',
  'SDL_blit_1.c',
  'SDL_blit_A.c',
  'SDL_blit_N.c',
  'SDL_blit_auto.c',
  'SDL_blit_copy.c',
  'SDL_blit_slow.c',
  'SDL_bmp.c',
  'SDL_clipboard.c',
  'SDL_egl.c',
  'SDL_fillrect.c',
  'SDL_pixels.c',
  'SDL_rect.c',
  'SDL_stb.c',
  'SDL_stretch.c',
  'SDL_surface.c',
  'SDL_video.c',
  'SDL_video_unsupported.c',
  'SDL_vulkan_utils.c',
  'SDL_yuv.c',
)
