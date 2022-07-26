local core = require "core"
local common = require "core.common"
local config = require "core.config"
local style = require "core.style"
local Object = require "core.object"

---Scrollbar
---@class core.scrollbar : core.object
local Scrollbar = Object:extend()

---@param direction "v" | "h" @Vertical or Horizontal
---@param alignment "s" | "e" @Start or End (left to right, top to bottom)
function Scrollbar:new(direction, alignment)
  ---Position information of the owner
  self.rect = {
    x = 0, y = 0, w = 0, h = 0,
    ---Scrollable size
    scrollable = 0
  }
  ---@type integer @Position in percent [0-1]
  self.percent = 0
  ---@type boolean @Scrollbar dragging status
  self.dragging = false
  ---@type integer @Private. Used to offset the start of the drag from the top of the thumb
  self.drag_start_offset = 0
  ---What is currently being hovered. `thumb` implies` track`
  self.hovering = { track = false, thumb = false }
  ---@type "v" | "h"@Vertical or Horizontal
  self.direction = direction or "v"
  ---@type "s" | "e" @Start or End (left to right, top to bottom)
  self.alignment = alignment or "e"
  ---@type integer @Private. Used to keep track of animations
  self.expand_percent = 0
end


function Scrollbar:get_thumb_rect()
  local sz = self.rect.scrollable
  if sz <= self.rect.h or sz == math.huge then
    return 0, 0, 0, 0
  end
  local h = math.max(20, self.rect.h * self.rect.h / sz)
  local width = style.scrollbar_size
  width = width + (style.expanded_scrollbar_size - style.scrollbar_size) * self.expand_percent
  return
    self.rect.x + self.rect.w - width,
    self.rect.y + self.percent * self.rect.scrollable * (self.rect.h - h) / (sz - self.rect.h),
    width,
    h
end


function Scrollbar:get_track_rect()
  local sz = self.rect.scrollable
  if sz <= self.rect.h or sz == math.huge then
    return 0, 0, 0, 0
  end
  local width = style.scrollbar_size
  width = width + (style.expanded_scrollbar_size - style.scrollbar_size) * self.expand_percent
  return
    self.rect.x + self.rect.w - width,
    self.rect.y,
    width,
    self.rect.h
end


function Scrollbar:overlaps(x, y)
  local sx, sy, sw, sh = self:get_thumb_rect()
  local result
  if x >= sx - style.scrollbar_size * 3 and x < sx + sw and y > sy and y <= sy + sh then
    result = "thumb"
  else
    sx, sy, sw, sh = self:get_track_rect()
    if x >= sx - style.scrollbar_size * 3 and x < sx + sw and y > sy and y <= sy + sh then
      result = "track"
    end
  end
  return result
end


function Scrollbar:on_mouse_pressed(button, x, y, clicks)
  local overlaps = self:overlaps(x, y)
  if overlaps then
    local _, ty, _, th = self:get_thumb_rect()
    if overlaps == "thumb" then
      self.dragging = true
      self.drag_start_offset = ty - y
      return true
    elseif overlaps == "track" then
      return (y - self.rect.y - th / 2) / self.rect.h
    end
  end
end


function Scrollbar:on_mouse_released(button, x, y)
  self.dragging = false
end


function Scrollbar:on_mouse_moved(x, y, dx, dy)
  if self.dragging then
    return common.clamp((y - self.rect.y + self.drag_start_offset) / self.rect.h, 0, 1)
  end
  local overlaps = self:overlaps(x, y)
  self.hovering.thumb = overlaps == "thumb"
  self.hovering.track = self.hovering.thumb or overlaps == "track"
  return self.hovering.track or self.hovering.thumb
end


function Scrollbar:on_mouse_left()
  self.hovering = { track = false, thumb = false }
end


function Scrollbar:set_size(x, y, w, h, scrollable)
  self.rect.x, self.rect.y, self.rect.w, self.rect.h = x, y, w, h
  self.rect.scrollable = scrollable
end


function Scrollbar:set_percent(percent)
  self.percent = percent
end


function Scrollbar:update()
  -- TODO: move the animation code to its own class
  local dest = (self.hovering.track or self.dragging) and 1 or 0
  local diff = math.abs(self.expand_percent - dest)
  if not config.transitions or diff < 0.05 or config.disabled_transitions["scroll"] then
    self.expand_percent = dest
  else
    local rate = 0.3
    if config.fps ~= 60 or config.animation_rate ~= 1 then
      local dt = 60 / config.fps
      rate = 1 - common.clamp(1 - rate, 1e-8, 1 - 1e-8)^(config.animation_rate * dt)
    end
    self.expand_percent = common.lerp(self.expand_percent, dest, rate)
  end
  if diff > 1e-8 then
    core.redraw = true
  end
end


function Scrollbar:draw_track()
  if not (self.hovering.track or self.dragging)
     and self.expand_percent == 0 then
    return
  end
  local color = { table.unpack(style.scrollbar_track) }
  color[4] = color[4] * self.expand_percent
  local x, y, w, h = self:get_track_rect()
  renderer.draw_rect(x, y, w, h, color)
end


function Scrollbar:draw_thumb()
  local highlight = self.hovering.thumb or self.dragging
  local color = highlight and style.scrollbar2 or style.scrollbar
  local x, y, w, h = self:get_thumb_rect()
  renderer.draw_rect(x, y, w, h, color)
end


function Scrollbar:draw()
  self:draw_track()
  self:draw_thumb()
end


return Scrollbar