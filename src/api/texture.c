#include "api.h"
#include "../renderer.h"
#include "../rencache.h"
#include "../renwindow.h"

extern RenWindow window_renderer;

static uint32_t parse_color(lua_State *L, int idx)
{
  uint32_t color = 0;
  if (lua_type(L, idx) == LUA_TTABLE)
  {
    if (luaL_len(L, idx) != 4)
    {
      return luaL_error(L, "invalid color");
    }
    for (int i = 1; i <= 4; i++)
    {
      lua_geti(L, idx, i);
      color <<= 8;
      color += luaL_checknumber(L, -1);
      lua_pop(L, 1);
    }
  }
  else if (lua_type(L, idx) == LUA_TNUMBER)
  {
    color = luaL_checkinteger(L, idx);
  }
  return color;
}

static int f_create_texture(lua_State *L) {
  int w = luaL_checkinteger(L, 1);
  int h = luaL_checkinteger(L, 2);
  if (w <= 0 || h <= 0)
    return luaL_error(L, "invalid size");

  int surface_scale = renwin_surface_scale(&window_renderer);
  w *= surface_scale;
  h *= surface_scale;

  uint32_t color = luaL_opt(L, parse_color, 3, 0);

  SDL_Surface *s = SDL_CreateRGBSurface(0, w, h, 32,
    0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
  if (s == NULL)
    return luaL_error(L, "unable to create texture: %s", SDL_GetError());
  s->userdata = NULL;
  SDL_FillRect(s, NULL, color);
  RenSurface *rs = lua_newuserdata(L, sizeof(RenSurface));
  rs->s = s;
  rs->last_change = 0;
  rs->area = (RenRect){0, 0, w, h};
  luaL_setmetatable(L, API_TYPE_TEXTURE);
  return 1;
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
  { "create_texture",         f_create_texture         },
  { NULL,                 NULL                 }
};

static const luaL_Reg textureLib[] = {
  { "__gc",               f_texture_gc                 },
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
