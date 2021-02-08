-----------------------------------------------------------------------------------------------------
-- Big LuaPlayer: tilemap˜ module v1.00 2013 (c) M4MSOFT
-----------------------------------------------------------------------------------------------------

-- load the table_ext module.
local table_ext = require "table_ext"

-- Declare the 'point' module.
local modname = "tilemap"
local modv = "1.00"
local modlv = 2
module (modname, package.seeall)

_PACKAGE = nil
_M = nil
_NAME = nil
_VERSION = modv

-- Declare moduletable and metatable.
local tblmod = package.loaded[modname]
local tblmeta = {__index = tblmod}
-----------------------------------------------------------------------------------------------------
--(info) metatable fields:
-- add, sub, mul, div, mod, pow, unm, tostring, concat,
-- len, eq, le, lt, append, index, newindex, call, gc, mode.
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--Create module settings.
display = {x = 0, y = 0, w = 480, h = 272}
view = {x = 0, y = 0}
tileSize = {w = 32, h = 32}
rangeOffset = {x = 0, y = 0}
color = GL.clr.white:clone()
fonts = {}
buffer = {
	update = false, draw = false, tex = false,
	x = 0, y = 0, w = 480, h = 272,
	rx = 1, ry = 1, rw = 0, rh = 0,
	tx = 0, ty = 0, tw = 0, th = 0
}
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func new: Build a new tilemap object.
--  @param [_t]: The object table.
-- @returns
--   @param _r: as tilemap object.
function new(_t)
	-- build the object.
	local _t = setmetatable ({} or _t, tblmeta)
	-- Set the data.
	_t["w"] = 0
	_t["h"] = 0
	-- data {fnt, string.char(0), ...}
	_t["data"] = {}
	-- Return the object.
	return _t
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func load: load a tilemap object from a file.
--  @param path: The file to load.
-- @returns
--   @param _r: as tilemap object.
function load(path)
	-- load the file.
	local _t = table_ext.load(path)
	-- Return the object.
	return setmetatable (_t, tblmeta)
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func save: Save the data to a file.
--  @param self: as tilemap object.
--  @param path: The file name as string.
-- @returns
--   @param nil
function save(self, path)
	-- Check the arguments.
	if type(self) ~= "table" then error("bad argument #1 'self' to '"..modname..".save' (tilemap expected, got "..type(self)..")", modlv) end
	-- Save the tilemap.
	table_ext.save(self, path)
	-- Dispose arguments.
	path = nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func clone: Clone the object.
--  @param self: as tilemap object.
-- @returns
--   @param tbl: as tilemap object.
function clone(self)
	-- Check the arguments.
	if type(self) ~= "table" then error("bad argument #1 'self' to '"..modname..".clone' (tilemap expected, got "..type(self)..")", modlv) end
	-- Copy the data and return the object.
	return table_ext.clone(self)
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func free: free the object
--  @param self: as tilemap object.
-- @returns
--   @param nil
function free(self)
	-- Check the arguments.
	if type(self) ~= "table" then error("bad argument #1 'self' to '"..modname..".free' (tilemap expected, got "..type(self)..")", modlv) end
	-- Free the data.
	self = table_ext.free(self)
	return nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func update: update the tilemap.
--  @param self: as tilemap object.
--  @param x: The x location of the tilemap object.
--  @param y: The y location of the tilemap object.
-- @returns
--   @param nil
local TileWidth, TileHeight = 0,0
local PositionX, PositionY = 0,0
local BaseX, BaseY = 0,0
local RenderX,RenderY,RenderWidth,RenderHeight = 1,1,0,0
local displayX,displayY,displayWidth,displayHeight = 0,0,0,0
function update(self, x, y)
	-- Update the Display size.
	displayX = display["x"]
	displayY = display["y"]
	displayWidth = display["w"]
	displayHeight = display["h"]
	-- Update the Tile Size.
	TileWidth = tileSize["w"]
	TileHeight = tileSize["h"]
	-- Update the Tile Position.
	PositionX = math.floor(x)
	PositionY = math.floor(y)
	view["x"] = PositionX
	view["y"] = PositionY
	-- Update the x and y offset positions.
	BaseX = math.fmod(PositionX,TileWidth)
	BaseY = math.fmod(PositionY,TileHeight)
	-- Update the tile render size
	RenderWidth = self["w"]
	RenderHeight = self["h"]
	x = math.ceil((displayWidth-BaseX)/TileWidth) - 1
	y = math.ceil((displayHeight-BaseY)/TileHeight) - 1
	RenderX = (BaseX-PositionX)/TileWidth + 1
	RenderY = (BaseY-PositionY)/TileHeight + 1
	RenderWidth = RenderX + x
	RenderHeight = RenderY + y
	-- Correct the value location
	if RenderX < 1 then
		BaseX = BaseX+(1-RenderX)*TileWidth
		RenderX = 1
	end
	if RenderY < 1 then
		BaseY = BaseY+(1-RenderY)*TileHeight
		RenderY = 1
	end
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func render: render the tilemap.
--  @param self: as tilemap object.
-- @returns
--   @param nil
function render(self)
	-- Get the data table.
	local data = self["data"]

	-- set the draw data
	local dx,dy = 0,0

	-- Reset the Range. (Need to check more tiles if some tile are bigger then the TL2D.TileSize)
	local RenderWidth = RenderWidth + rangeOffset["x"]
	local RenderHeight = RenderHeight + rangeOffset["y"]
	if RenderWidth > self["w"] then RenderWidth = self["w"] end
	if RenderHeight > self["h"] then RenderHeight = self["h"] end

	-- render the tilemap.
	local tile = false
	dy = displayY+BaseY
	for ny = RenderY, RenderHeight do
		dx = displayX+BaseX
		for nx = RenderX, RenderWidth do
			tile = data[nx] and data[nx][ny] or false
			if tile then
				for n = 1, rawlen(tile), 2 do
					fonts[tile[n]]:print(dx, dy, tile[n+1], color)
				end
			end
			dx = dx+TileWidth
		end
		dy = dy+TileHeight
	end

end
-----------------------------------------------------------------------------------------------------


-----------------------------------------------------------------------------------------------------
-- @func setBuffer: Activate the vram buffer.
--  @param w: the width of the vram buffer.
--  @param h: the height of the vram buffer.
-- @returns
--   @param nil
function setBuffer(w, h, psm)
	-- Dispose the vram buffer.
	resetBuffer()
	-- Create the new vram buffer.
	buffer.w = w
	buffer.h = h
	buffer.tex = Texture.new(buffer.w, buffer.h, true, psm or GL.PSM_8888) or error("Out of memory: Can't create the buffer.", modlv)
	buffer.tex:clear(GL.clr.white)
	--buffer["tex"]:swizzle()
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func resetBuffer: Dispose the vram buffer.
--  @param self: as tilemap object.
-- @returns
--   @param nil
function resetBuffer()
	-- Move the Texture to the Ram memory.
	if buffer.tex then buffer.tex:toRam() end
	-- Reset the buffer data.
	buffer = {
		update = false, draw = false, tex = false,
		x = 0, y = 0, w = 0, h = 0, rx = 0, ry = 0, rw = 0, rh = 0,
		tx = 0, ty = 0, tw = 0, th = 0
	}
	-- Collect Garbage.
	collectgarbage("collect")
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func updateBuffer: Update the vram buffer.
--  @param self: as tilemap object.
-- @returns
--   @param nil
local function updateBuffer()
	if not buffer["update"] then return end
	buffer["tx"] = BaseX == 0 and 0 or 1
	buffer["ty"] = BaseY == 0 and 0 or 1
	buffer["tw"] = BaseX+(RenderWidth-RenderX+1)*TileWidth <= displayWidth and 0 or -1
	buffer["th"] = BaseY+(RenderHeight-RenderY+1)*TileHeight <= displayHeight and 0 or -1
	buffer["rx"] = RenderX + buffer["tx"]
	buffer["ry"] = RenderY + buffer["ty"]
	buffer["rw"] = RenderWidth + buffer["tw"]
	buffer["rh"] = RenderHeight + buffer["th"]
	buffer["x"] = PositionX
	buffer["y"] = PositionY
	-- Render the buffer.
	GL.Finish()
	--buffer["tex"]:blit(0, 0, GL.drawBuffer, displayX, displayY, displayWidth, displayHeight, false)
	if not GL.SetDrawTarget(buffer["tex"]) then error("Failed to set texture as target.", modlv) end
	GL.BeginRects(GL.drawBuffer)
	GL.SetTexAlpha(false)
	GL.SetTexLinear(false)
	GL.AddTex(0, 0, displayWidth, displayHeight, displayX, displayY, displayWidth, displayHeight)
	--GL.AddTex(0, 0)
	GL.End()
	if not GL.ResetDrawTarget() then error("Failed to set screen as target.", modlv) end
	buffer["draw"] = true
	buffer["update"] = false
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func renderBuffer: render the tilemap with the current active buffer.
--  @param self: as tilemap object.
-- @returns
--   @param nil
local function renderRange(self, x, y, rx, ry, rw, rh)
	-- render the tilemap.
	local tile = false
	-- Get the data table.
	local data = self["data"]
	-- set the draw data
	local dx,dy = 0,0
	dy = y
	for ny = ry, rh do
		dx = x
		for nx = rx, rw do
			tile = data[nx] and data[nx][ny] or false
			if tile then
				for n = 1, rawlen(tile), 2 do
					fonts[tile[n]]:print(dx, dy, tile[n+1], color)
				end
			end
			dx = dx+TileWidth
		end
		dy = dy+TileHeight
	end
end
function renderBuffer(self)

	-- Reset the Range. (Need to check more tiles if some tile are bigger then the TL2D.TileSize)
	local RenderWidth = RenderWidth + rangeOffset["x"]
	local RenderHeight = RenderHeight + rangeOffset["y"]
	if RenderWidth > self["w"] then RenderWidth = self["w"] end
	if RenderHeight > self["h"] then RenderHeight = self["h"] end

	-- Draw the buffer.
	if buffer["draw"] then
		GL.BeginRects(buffer["tex"])
		GL.SetTexAlpha(false)
		GL.SetTexLinear(false)
		GL.AddPoint(displayX+PositionX-buffer["x"], displayY+PositionY-buffer["y"])
		GL.End()
	end

	-- Draw the Remaining base Tiles.
	if buffer["rw"] == 0 or buffer["rh"] == 0 then
		self:render(self)
		buffer["update"] = true
	else
		if buffer["y"] ~= PositionY then
			if buffer["ry"] > RenderY then
				renderRange(self,
					displayX+BaseX,
					displayY+BaseY,
					RenderX, RenderY,
					RenderWidth, buffer["ry"]
				)
				--buffer["ty"] = BaseY == 0 and 0 or 1
				--buffer["ry"] = RenderY + buffer["ty"]
				buffer["update"] = true
			end
			if buffer["rh"] < RenderHeight then
				renderRange(self,
					displayX+BaseX,
					displayY+BaseY+(buffer["rh"]-buffer["th"]-RenderY)*TileHeight,
					RenderX, buffer["rh"] - buffer["th"],
					RenderWidth, RenderHeight
				)
				--buffer["th"] = BaseY+(RenderHeight-RenderY+1)*TileHeight <= displayHeight and 0 or -1
				--buffer["rh"] = RenderHeight + buffer["th"]
				buffer["update"] = true
			end
		end
		if buffer["x"] ~= PositionX then
			if buffer["rx"] > RenderX then
				renderRange(self,
					displayX+BaseX,
					displayY+BaseY+(buffer["ry"]-buffer["ty"]-RenderY)*TileHeight,
					RenderX, buffer["ry"]-buffer["ty"],
					buffer["rx"], buffer["rh"] - buffer["th"]
				)
				--buffer["tx"] = BaseX == 0 and 0 or 1
				--buffer["rx"] = RenderX + buffer["tx"]
				buffer["update"] = true
			end
			if buffer["rw"] < RenderWidth then
				renderRange(self,
					displayX+BaseX+(buffer["rw"]-RenderX)*TileWidth,
					displayY+BaseY+(buffer["ry"]-buffer["ty"]-RenderY)*TileHeight,
					buffer["rw"], buffer["ry"]-buffer["ty"],
					RenderWidth, buffer["rh"] - buffer["th"]
				)
				--buffer["tw"] = BaseX+(RenderWidth-RenderX+1)*TileWidth <= displayWidth and 0 or -1
				--buffer["rw"] = RenderWidth + buffer["tw"]
				buffer["update"] = true
			end
		end
	end

	-- Update the draw buffer.
	updateBuffer()

end
-----------------------------------------------------------------------------------------------------


-----------------------------------------------------------------------------------------------------
-- Define the module metamethods.
-----------------------------------------------------------------------------------------------------


-----------------------------------------------------------------------------------------------------
-- @func dispose: Remove the module.
--  @param : takes no arguments.
-- @returns
--   @param nil
function dispose()
	-- Dispose the vram buffer.
	resetBuffer()
	-- Dispose the moduletable and metatable.
	for i,v in pairs(tblmod) do
		tblmod[i]=nil
	end
	tblmeta = nil
	-- Dispose local values.
	TileWidth = nil
	TileHeight = nil
	PostionX = nil
	PostionY = nil
	BaseX = nil
	BaseY = nil
	RenderX = nil
	RenderY= nil
	RenderWidth = nil
	RenderHeight = nil
	updateBuffer = nil
	renderRange = nil
	-- Remove the module.
	package.loaded[modname] = nil
	_G[modname]=nil
	modlv = nil
	modv = nil
	modname = nil
	return nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--End tilemap˜ module.
-----------------------------------------------------------------------------------------------------
