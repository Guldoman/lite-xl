opt_use_gpu = get_option('use_gpu')
cdata.set('SDL_GPU_DISABLED', opt_use_gpu.disabled())

if opt_use_gpu.allowed()
  if host_machine.system() == 'windows'
    subdir('d3d12')
    subdir('vulkan')
  elif host_machine.system() == 'darwin'
    subdir('metal')
    subdir('vulkan')
  else
    subdir('vulkan')
  endif
endif

sources += files('SDL_gpu.c')
