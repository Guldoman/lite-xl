#include "api.h"
#include "../renderer.h"
#include "../rencache.h"
#include "../renwindow.h"

extern RenWindow window_renderer;

// TODO: move this to a common place (this is also used in api/renderer.c)
static RenRect rect_to_grid(lua_Number x, lua_Number y, lua_Number w, lua_Number h) {
  int x1 = (int) (x + 0.5), y1 = (int) (y + 0.5);
  int x2 = (int) (x + w + 0.5), y2 = (int) (y + h + 0.5);
  return (RenRect) {x1, y1, x2 - x1, y2 - y1};
}

static RenColor parse_color(lua_State *L, int idx)
{
  RenColor color = {0, 0, 0, 0};
  if (lua_type(L, idx) == LUA_TTABLE)
  {
    if (luaL_len(L, idx) != 4)
    {
      luaL_error(L, "invalid color");
      return color;
    }
    lua_geti(L, idx, 1);
    lua_geti(L, idx, 2);
    lua_geti(L, idx, 3);
    lua_geti(L, idx, 4);
    color.r = luaL_checknumber(L, -4);
    color.g = luaL_checknumber(L, -3);
    color.b = luaL_checknumber(L, -2);
    color.a = luaL_checknumber(L, -1);
    lua_pop(L, 4);
  }
  else if (lua_type(L, idx) == LUA_TNUMBER)
  {
    uint32_t tmp_color = luaL_checkinteger(L, idx);
    color.r = (tmp_color & 0xFF000000) >> 24;
    color.g = (tmp_color & 0x00FF0000) >> 16;
    color.b = (tmp_color & 0x0000FF00) >> 8;
    color.a = (tmp_color & 0x000000FF);
  }
  else
    luaL_error(L, "invalid color");
  return color;
}

static RenRect parse_rect(lua_State *L, int idx)
{
  RenRect rr;
  luaL_checktype(L, idx, LUA_TTABLE);
  lua_getfield(L, idx, "x");
  lua_getfield(L, idx, "y");
  lua_getfield(L, idx, "w");
  lua_getfield(L, idx, "h");
  lua_Number x, y, w, h;
  x = luaL_optnumber(L, -4, 0);
  y = luaL_optnumber(L, -3, 0);
  w = luaL_optnumber(L, -2, 0);
  h = luaL_optnumber(L, -1, 0);
  rr = rect_to_grid(x, y, w, h);
  lua_pop(L, 4);
  return rr;
}

// TODO: move this to a common place (this is also used in api/renderer.c)
static bool font_retrieve(lua_State* L, RenFont** fonts, int idx) {
  memset(fonts, 0, sizeof(RenFont*)*FONT_FALLBACK_MAX);
  if (lua_type(L, idx) != LUA_TTABLE) {
    fonts[0] = *(RenFont**)luaL_checkudata(L, idx, API_TYPE_FONT);
    return false;
  }
  int i = 0;
  do {
    lua_rawgeti(L, idx, i+1);
    fonts[i] = !lua_isnil(L, -1) ? *(RenFont**)luaL_checkudata(L, -1, API_TYPE_FONT) : NULL;
    lua_pop(L, 1);
  } while (fonts[i] && i++ < FONT_FALLBACK_MAX);
  return true;
}

static int f_create_texture(lua_State *L) {
  int w = luaL_checkinteger(L, 1);
  int h = luaL_checkinteger(L, 2);
  if (w <= 0 || h <= 0)
    return luaL_error(L, "invalid size");

  int surface_scale = renwin_surface_scale(&window_renderer);
  w *= surface_scale;
  h *= surface_scale;

  RenColor color = luaL_opt(L, parse_color, 3, ((RenColor){0, 0, 0, 0}));

  SDL_Surface *s = SDL_CreateRGBSurface(0, w, h, 32,
    0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
  if (s == NULL)
    return luaL_error(L, "unable to create texture: %s", SDL_GetError());
  s->userdata = NULL;
  SDL_FillRect(s, NULL, SDL_MapRGBA(s->format, color.r, color.g, color.b, color.a));
  RenSurface *rs = lua_newuserdata(L, sizeof(RenSurface));
  rs->s = s;
  rs->last_change = 0;
  rs->area = (RenRect){0, 0, w, h};
  luaL_setmetatable(L, API_TYPE_TEXTURE);
  return 1;
}

static int f_texture_set_pixels(lua_State *L) {
  RenSurface *rs = luaL_checkudata(L, 1, API_TYPE_TEXTURE);
  SDL_Surface *s = rs->s;
  RenRect rr;
  rs->last_change++;
  luaL_checktype(L, 2, LUA_TTABLE);
  luaL_checktype(L, 3, LUA_TTABLE);
  rr = parse_rect(L, 3);

  int size = luaL_len(L, 2);
  if (size != rr.width * rr.height)
    return luaL_error(L, "mismatching pixel data and sizes, got %d pixels, needed %d",
                      size, rr.width * rr.height);

  RenColor rc;
  int c = 1;
  for (int j = rr.y; j < rr.y + rr.height; j++)
  {
    uint8_t *pixels = (uint8_t *)s->pixels + j * s->pitch + rr.x * s->format->BytesPerPixel;
    for (int i = rr.x; i < rr.x + rr.width; i++)
    {
      lua_geti(L, 2, c++);
      rc = parse_color(L, -1);
      *(uint32_t *)pixels = SDL_MapRGBA(s->format, rc.r, rc.g, rc.b, rc.a);
      pixels += s->format->BytesPerPixel;
      lua_pop(L, 1);
    }
  }
  return 0;
}

static int f_texture_draw_texture(lua_State *L) {
  RenSurface *dst = luaL_checkudata(L, 1, API_TYPE_TEXTURE);
  dst->last_change++;
  RenSurface *src = luaL_checkudata(L, 2, API_TYPE_TEXTURE);
  RenRect dst_rect = luaL_opt(L, parse_rect, 3, dst->area);
  RenRect src_rect = luaL_opt(L, parse_rect, 4, src->area);
  bool blend = luaL_opt(L, lua_toboolean, 5, true);

  ren_draw_surface(src->s, src_rect, dst->s, dst_rect, blend);
  return 0;
}

static int f_texture_draw_rect(lua_State *L) {
  RenSurface *rs = luaL_checkudata(L, 1, API_TYPE_TEXTURE);
  rs->last_change++;
  
  RenRect rr = luaL_opt(L, parse_rect, 2, rs->area);
  RenColor color = parse_color(L, 3);
  bool blend = luaL_opt(L, lua_toboolean, 4, true);

  ren_draw_rect(rs->s, rr, color, blend);
  return 0;
}

// CATCH: subpixel rendering is relative to the surface,
// not the final position on screen
static int f_texture_draw_text(lua_State *L) {
  RenSurface *rs = luaL_checkudata(L, 1, API_TYPE_TEXTURE);
  rs->last_change++;
  RenFont* fonts[FONT_FALLBACK_MAX];
  font_retrieve(L, fonts, 2);
  const char *text = luaL_checkstring(L, 3);
  float x = luaL_checknumber(L, 4);
  int y = luaL_checknumber(L, 5);
  RenColor color = luaL_opt(L, parse_color, 6, ((RenColor){255, 255, 255, 255}));

  x = ren_draw_text(rs->s, fonts, text, x, y, color);
  lua_pushnumber(L, x);
  return 1;
}

static int f_texture_copy(lua_State *L) {
  RenSurface *src = luaL_checkudata(L, 1, API_TYPE_TEXTURE);
  int w = src->area.width;
  int h = src->area.height;
  SDL_Surface *s = SDL_CreateRGBSurface(0, w, h, 32,
    0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
  s->userdata = NULL;

  RenSurface *rs = lua_newuserdata(L, sizeof(RenSurface));
  rs->s = s;
  rs->last_change = 0;
  rs->area = src->area;
  ren_draw_surface(src->s, src->area, rs->s, rs->area, false);
  luaL_setmetatable(L, API_TYPE_TEXTURE);
  return 1;
}

static int f_texture_get_size(lua_State *L) {
  RenSurface *rs = luaL_checkudata(L, 1, API_TYPE_TEXTURE);
  lua_pushinteger(L, rs->area.width);
  lua_pushinteger(L, rs->area.height);
  return 2;
}

static int f_texture_gc(lua_State *L){
  RenSurface *rs = luaL_checkudata(L, 1, API_TYPE_TEXTURE);
  SDL_Surface *s = rs->s;
  s->refcount--;
  // We need the refcount because this could still be queued for drawing
  if (s->refcount == 0)
  {
    SDL_FreeSurface(s);
  }
  return 0;
}

static const luaL_Reg lib[] = {
  { "create_texture",        f_create_texture             },
  { NULL, NULL }
};

static const luaL_Reg textureLib[] = {
  { "__gc",                  f_texture_gc                 },
  { "set_pixels",            f_texture_set_pixels         },
  { "draw_texture",          f_texture_draw_texture       },
  { "draw_rect",             f_texture_draw_rect          },
  { "draw_text",             f_texture_draw_text          },
  { "copy",                  f_texture_copy               },
  { "get_size",              f_texture_get_size           },
  { NULL, NULL }
};

int luaopen_texture(lua_State *L) {
  luaL_newlib(L, lib);
  luaL_newmetatable(L, API_TYPE_TEXTURE);
  luaL_setfuncs(L, textureLib, 0);
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
  lua_setfield(L, -2, "texture");
  return 1;
}
