-----------------------------------------------------------------------------------------------------
-- Big LuaPlayer: Resources˜ module v1.00 2014 (c) M4MSOFT
--
--   Gobal Values:
--     dirTileFont       - The location to search for the Texture file as a string.
--     stackSizeTileFont - The max size of the TileFont stack.
--     tblTileFont       - The table the stores the Filename, BmpFont and the number of glyphs slots,
--
--   Gobal Function:
--     function setTileFontStackSize(size) return nil
--     function freeTileFontStack() return nil
--     function loadTileFont(filename, add) return bmpFont, int
--     function dispose() return nil
--
-----------------------------------------------------------------------------------------------------

-- load the table_ext module.
local table_ext = require "table_ext"

-- Declare the 'Resources' module.
local modname = "Resources"
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
dirTileFont = "Resources"
stackSizeTileFont = 15
tblTileFont = table_ext.createEmpty({"", false, 0}, stackSizeTileFont) -- {[Filename], [bmpFont], [number of glyphs]}
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func setTileFontStackSize: Set the size of the Stack
--  @param size: The new stack size as integer. Range: size > 0.
-- @returns
--   @param nil
function setTileFontStackSize(size)
	stackSizeTileFont = size > 1 and size or 1
	-- Reduce the stack size if the stack is large then the max stack size.
	while table.getn(tblTileFont) > stackSizeTileFont do
		table.remove(tblTileFont)
		collectgarbage("collect")
	end
end
-----------------------------------------------------------------------------------------------------
-- @func freeTileFontStack: Free the Stack data
--  @param : takes no arguments.
-- @returns
--   @param nil
function freeTileFontStack()
	-- Reset the stack and collect the garbage.
	tblTileFont = table_ext.createEmpty({"", false, 0}, stackSizeTileFont)
	collectgarbage("collect")
end
-----------------------------------------------------------------------------------------------------
-- @func loadTileFont: Load a bmpFont from the stack or file and define the default glyhps slots.
--  @param filename: The file name of a Texture to use to build the basic TileFont.
--  @param add: Add the file to the stack as boolean. Default value: true
-- @returns
--   @param file: as bmpFont object.
--   @param slots: the number of glyphs slot as number.
--local function _loadconvert8Tex(filename, modlv)
--	-- Load the texture.
--	local tex32 = Texture.load(filename, false, GL.PSM_8888, false) or error("Invalid File: Cant load the texture file.", modlv+1)
--	-- build the convert palette.
--	local pltTex = Palette.fromImage(tex32, 256)
--	-- Create a 8 bit texture.
--	local tex8 = Texture.new(tex32:width(), tex32:height(), false, GL.PSM_T8)
--	-- Link the convert palette to the texture.
--	tex8:palette(pltTex)
--	-- Convert the 32 bit texture to a 8 bit texture using the blit function.
--	tex8:blit(0, 0, tex32)
--	-- Swizzle the texture.
--	tex8:swizzle()
--	-- Dispose the 32 bit texture and palette.
--	tex32 = nil
--	pltTex = nil
--	return tex8
--end
local function _loadTileFont(filename)
	-- Load the texture.
	-- local tex = _loadconvert8Tex(filename, modlv+1)
	local tex = Texture.load(filename)
	-- Create a bmpFont object.
	local bfnt = BmpFont.new()
	-- Add the font texture to the BmpFont object.
	bfnt:texture(tex)
	-- build the default glyph table.
	local h = tex:height()-1
	local w = tex:width()-1
	local slot = 0
	for y = 0, h, 32 do
		for x = 0, w, 32 do
			-- Add the Glyphs to the BmpFont object. {x, y, w, h, x0, y0, adv_x, adv_y}
			slot = slot + 1
			bfnt:glyph(slot, { x, y, 32, 32, 0, 0, 0, 0})
			-- Link the characters to a glyph slot.
			bfnt:charmap(string.char(slot), slot)
			-- Skip the values 10 and 13 because the can't be used properly.
			if slot == 10 or slot == 13 then
				slot = slot + 1
				bfnt:glyph(slot, {x, y, 32, 32, 0, 0, 0, 0})
				bfnt:charmap(string.char(slot), slot)
			end
		end
	end
	-- Set the height of the BmpFont object.
	bfnt:height(32)
	-- Return the bmpFont and the total number of create glyphs.
	return bfnt, slot
end
function loadTileFont(filename, add)
	local file = false
	local slots = 0
	-- Check if the object is already loaded and return the object.
	for i,tbl in ipairs(tblTileFont) do
		if tbl[1] == filename then
			file, slots = tbl[2], tbl[3]
			--Return the object.
			return file, slots
		end
	end
	-- Load the file in the object hasn't been loaded jet.
	if System.fileExsist(filename) then
		file, slots = _loadTileFont(filename)
	elseif System.fileExsist(dirTileFont.."/"..filename) then
		file, slots = _loadTileFont(dirTileFont.."/"..filename)
	end
	-- Add TileFont to the stack.
	if add == nil or true and add then
		table.insert(tblTileFont,1,{filename, file, slots})
		-- Reduce the stack size if the stack is large then the max stack size.
		stackSizeTileFont = stackSizeTileFont > 1 and stackSizeTileFont or 1
		while table.getn(tblTileFont) > stackSizeTileFont do
			table.remove(tblTileFont)
			collectgarbage("collect")
		end
	end
	--Return the object.
	return file, slots
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
	-- Dispose the moduletable and metatable.
	for i,v in pairs(tblmod) do
		tblmod[i]=nil
	end
	tblmeta = nil
	-- Dispose local values.
	_loadTileFont = nil
	--_loadconvert8Tex = nil
	table_ext = nil
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
--End Resources˜ module.
-----------------------------------------------------------------------------------------------------
