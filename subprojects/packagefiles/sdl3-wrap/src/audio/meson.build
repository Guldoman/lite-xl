opt_use_audio = get_option('use_audio')
cdata.set('SDL_AUDIO_DISABLED', opt_use_audio.disabled())

opt_use_audio_pipewire = get_option('use_audio_pipewire').require(
  opt_use_audio.allowed(),
  error_message: 'use_audio_pipewire requires use_audio'
)

if opt_use_audio.allowed()
  if host_machine.system() == 'windows'
    cdata.set('SDL_AUDIO_DRIVER_DSOUND', 1)
    #  cdata.set('SDL_AUDIO_DRIVER_WASAPI', 1)
    subdir('directsound')
    #  subdir('wasapi')
  elif host_machine.system() == 'darwin'
    subdir('coreaudio')
  else
    if opt_use_audio_pipewire.allowed()
      subdir('pipewire')
    endif
  endif
endif

sources += files(
  'SDL_audio.c',
  'SDL_audiocvt.c',
  'SDL_audiodev.c',
  'SDL_audioqueue.c',
  'SDL_audioresample.c',
  'SDL_audiotypecvt.c',
  'SDL_mixer.c',
  'SDL_wave.c',
)
