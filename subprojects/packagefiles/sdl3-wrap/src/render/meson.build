opt_use_render = get_option('use_render').disable_if(
  get_option('use_video').disabled(),
  error_message: 'use_render requires use_video'
)
cdata.set('SDL_RENDER_DISABLED', opt_use_render.disabled())

if opt_use_render.allowed()
  subdir('gpu')
  subdir('software')

  if host_machine.system() == 'windows'
    subdir('direct3d12')
    subdir('vulkan')
  elif host_machine.system() == 'darwin'
    subdir('metal')
    subdir('vulkan')
  else
    subdir('opengl')
    subdir('vulkan')
  endif
endif

sources += files(
  'SDL_d3dmath.c',
  'SDL_render.c',
  'SDL_render_unsupported.c',
  'SDL_yuv_sw.c',
)
