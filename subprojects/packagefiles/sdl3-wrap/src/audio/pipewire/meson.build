pipewire_dep = dependency('libpipewire-0.3', required: opt_use_audio_pipewire)
use_pipewire = opt_use_audio_pipewire.require(
  pipewire_dep.found(),
  error_message: 'use_audio_pipewire was enabled, but libpipewire-0.3 was not found'
)

cdata.set('SDL_AUDIO_DRIVER_PIPEWIRE', use_pipewire.allowed())
if use_pipewire.allowed()
  sdl_deps += pipewire_dep
  sources += files('SDL_pipewire.c')
endif
