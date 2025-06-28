opt_use_camera = get_option('use_camera').disable_if(
  get_option('use_video').disabled(),
  error_message: 'use_camera requires use_video'
)
cdata.set('SDL_CAMERA_DISABLED', opt_use_camera.disabled())

sources += files('SDL_camera.c')
