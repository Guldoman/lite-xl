opt_use_dialog = get_option('use_dialog')
cdata.set('SDL_DIALOG_DISABLED', opt_use_dialog.disabled())

if opt_use_dialog.allowed()
  if host_machine.system() == 'windows'
    subdir('windows')
  else
    subdir('unix')
  endif
endif

sources += files('SDL_dialog.c', 'SDL_dialog_utils.c')
