x11_dep = dependency(
  'x11',
  required: opt_use_video_x11,
)
xext_dep = dependency(
  'xext',
  required: opt_use_video_x11,
)

# According to sdlchecks.cmake xext_dep is not needed, but I'm not sure...
has_x11_minimum = x11_dep.found() and xext_dep.found() and cc.has_header('X11/extensions/Xext.h')

cdata.set('SDL_VIDEO_DRIVER_X11', has_x11_minimum)
if not has_x11_minimum
  subdir_done()
endif

opt_use_video_x11_shared = get_option('use_video_x11_shared')
if opt_use_video_x11_shared.allowed()
  x11_dep = x11_dep.partial_dependency(
    compile_args: true,
    includes: true,
    sources: true,
  )
  cdata.set_quoted('SDL_VIDEO_DRIVER_X11_DYNAMIC', 'libX11.so')

  xext_dep = xext_dep.partial_dependency(
    compile_args: true,
    includes: true,
    sources: true,
  )
  cdata.set_quoted('SDL_VIDEO_DRIVER_X11_DYNAMIC_XEXT', 'libXext.so')
endif
sdl_deps += [x11_dep, xext_dep]

xlib_has_genericevent = cc.compiles('''
#include <X11/Xlib.h>
int main(int argc, char **argv) {
  Display *display;
  XEvent event;
  XGenericEventCookie *cookie = &event.xcookie;
  XNextEvent(display, &event);
  XGetEventData(display, cookie);
  XFreeEventData(display, cookie);
  return 0;
}
''')
cdata.set('SDL_VIDEO_DRIVER_X11_SUPPORTS_GENERIC_EVENTS', xlib_has_genericevent)

# For some reason
# xlib_has_xkblookupkeysym = cc.has_function('XkbLookupKeySym', prefix: ['#include <X11/Xlib.h>', '#include <X11/XKBlib.h>'])
# doesn't seem to work
xlib_has_xkblookupkeysym = (cc.has_header_symbol('X11/Xlib.h', 'XkbLookupKeySym')
                            or cc.has_header_symbol('X11/XKBlib.h', 'XkbLookupKeySym'))
cdata.set('SDL_VIDEO_DRIVER_X11_HAS_XKBLOOKUPKEYSYM', xlib_has_xkblookupkeysym)

opt_use_video_x11_xcursor = get_option('use_video_x11_xcursor')
xcursor_dep = dependency(
  'xcursor',
  required: opt_use_video_x11_xcursor,
)
has_x11_xcursor = xcursor_dep.found()
if has_x11_xcursor
  has_x11_xcursor = cc.has_header('X11/Xcursor/Xcursor.h', required: opt_use_video_x11_xcursor)
endif
cdata.set('SDL_VIDEO_DRIVER_X11_XCURSOR', has_x11_xcursor)
if has_x11_xcursor
  if opt_use_video_x11_shared.allowed()
    xcursor_dep = xcursor_dep.partial_dependency(
      compile_args: true,
      includes: true,
      sources: true,
    )
    cdata.set_quoted('SDL_VIDEO_DRIVER_X11_DYNAMIC_XCURSOR', 'libXcursor.so')
  endif
  sdl_deps += xcursor_dep
endif

opt_use_video_x11_xdbe = get_option('use_video_x11_xdbe')
cdata.set('SDL_VIDEO_DRIVER_X11_XDBE', cc.has_header('X11/extensions/Xdbe.h', required: opt_use_video_x11_xdbe))

opt_use_video_x11_xinput = get_option('use_video_x11_xinput')
xi_dep = dependency(
  'xi',
  required: opt_use_video_x11_xinput,
)
has_x11_xinput = xi_dep.found() and cc.has_header('X11/extensions/XInput2.h', required: opt_use_video_x11_xinput)
cdata.set('SDL_VIDEO_DRIVER_X11_XINPUT2', has_x11_xinput)
if has_x11_xinput
  if opt_use_video_x11_shared.allowed()
    xi_dep = xi_dep.partial_dependency(
      compile_args: true,
      includes: true,
      sources: true,
    )
    cdata.set_quoted('SDL_VIDEO_DRIVER_X11_DYNAMIC_XINPUT2', 'libXi.so')
  endif
  sdl_deps += xi_dep
endif

xi_has_multitouch = cc.compiles('''
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/extensions/XInput2.h>
int event_type = XI_TouchBegin;
XITouchClassInfo *t;
Status XIAllowTouchEvents(Display *a,int b,unsigned int c,Window d,int f) {
  return (Status)0;
}
int main(int argc, char **argv) { return 0; }
''')
cdata.set('SDL_VIDEO_DRIVER_X11_XINPUT2_SUPPORTS_MULTITOUCH', xi_has_multitouch)

opt_use_video_x11_xfixes = get_option('use_video_x11_xfixes')
xfixes_dep = dependency(
  'xfixes',
  required: opt_use_video_x11_xfixes,
)
has_x11_xfixes = (
  xfixes_dep.found()
  and cc.has_header('X11/extensions/Xfixes.h', required: opt_use_video_x11_xfixes)
  and cc.compiles('''
  #include <X11/Xlib.h>
  #include <X11/Xproto.h>
  #include <X11/extensions/XInput2.h>
  #include <X11/extensions/Xfixes.h>
  BarrierEventID b;
  int main(int argc, char **argv) { return 0; }
  ''')
)
cdata.set('SDL_VIDEO_DRIVER_X11_XFIXES', has_x11_xfixes)
if has_x11_xfixes
  if opt_use_video_x11_shared.allowed()
    xfixes_dep = xfixes_dep.partial_dependency(
      compile_args: true,
      includes: true,
      sources: true,
    )
    cdata.set_quoted('SDL_VIDEO_DRIVER_X11_DYNAMIC_XFIXES', 'libXfixes.so')
  endif
  sdl_deps += xfixes_dep
endif

opt_use_video_x11_xsync = get_option('use_video_x11_xsync')
has_x11_xsync_header = cc.has_header('X11/extensions/sync.h', required: opt_use_video_x11_xsync)
cdata.set('SDL_VIDEO_DRIVER_X11_XSYNC', has_x11_xsync_header)

use_video_x11_xrandr = get_option('use_video_x11_xrandr')
xrandr_dep = dependency(
  'xrandr',
  required: use_video_x11_xrandr,
)
has_x11_xrandr = xrandr_dep.found() and cc.has_header('X11/extensions/Xrandr.h', required: use_video_x11_xrandr)
cdata.set('SDL_VIDEO_DRIVER_X11_XRANDR', has_x11_xrandr)
if has_x11_xrandr
  if opt_use_video_x11_shared.allowed()
    xrandr_dep = xrandr_dep.partial_dependency(
      compile_args: true,
      includes: true,
      sources: true,
    )
    cdata.set_quoted('SDL_VIDEO_DRIVER_X11_DYNAMIC_XRANDR', 'libXrandr.so')
  endif
  sdl_deps += xrandr_dep
endif

use_video_x11_xscrnsaver = get_option('use_video_x11_xscrnsaver')
xscrnsaver_dep = dependency(
  'xscrnsaver',
  required: use_video_x11_xscrnsaver,
)
has_x11_xscrnsaver = xscrnsaver_dep.found() and cc.has_header('X11/extensions/scrnsaver.h', required: use_video_x11_xscrnsaver)
cdata.set('SDL_VIDEO_DRIVER_X11_XSCRNSAVER', has_x11_xscrnsaver)
if has_x11_xscrnsaver
  if opt_use_video_x11_shared.allowed()
    xscrnsaver_dep = xscrnsaver_dep.partial_dependency(
      compile_args: true,
      includes: true,
      sources: true,
    )
    cdata.set_quoted('SDL_VIDEO_DRIVER_X11_DYNAMIC_XSS', 'libXss.so')
  endif
  sdl_deps += xscrnsaver_dep
endif

use_video_x11_xshape = get_option('use_video_x11_xshape')
cdata.set('SDL_VIDEO_DRIVER_X11_XSHAPE', cc.has_header('X11/extensions/shape.h', required: use_video_x11_xshape))

sources += files(
  'SDL_x11clipboard.c',
  'SDL_x11dyn.c',
  'SDL_x11events.c',
  'SDL_x11framebuffer.c',
  'SDL_x11keyboard.c',
  'SDL_x11messagebox.c',
  'SDL_x11modes.c',
  'SDL_x11mouse.c',
  'SDL_x11opengl.c',
  'SDL_x11opengles.c',
  'SDL_x11pen.c',
  'SDL_x11settings.c',
  'SDL_x11shape.c',
  'SDL_x11touch.c',
  'SDL_x11video.c',
  'SDL_x11vulkan.c',
  'SDL_x11window.c',
  'SDL_x11xfixes.c',
  'SDL_x11xinput2.c',
  'SDL_x11xsync.c',
  'edid-parse.c',
  'xsettings-client.c',
)
