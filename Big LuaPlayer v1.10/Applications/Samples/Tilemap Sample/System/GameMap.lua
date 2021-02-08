-----------------------------------------------------------------------------------------------------
-- Big LuaPlayer: GameMap˜ module v1.00 2014 (c) M4MSOFT
--
--   Gobal Function:
--     function newWorld(w, h) return tbl World      -- Free the current World table and create a new one.
--     function loadWorld(filename) return tbl World -- Free the current World table and load one form a file.
--     function saveWorld(filename) return nil       -- Save the current World table to a file.
--     function freeWorld() return nil               -- Free the current World table.
--     function addTileFont(filename) return nil     -- Add a Font file to the tilemap engine.
--     function removeTileFont(indx) return nil      -- Remove a Font from World and tilemap.fonts list.
--
--     function dispose() return nil return nil
--
-----------------------------------------------------------------------------------------------------

-- load the table_ext module.
local table_ext = require "table_ext"

-- load the tilemap module.
local tilemap = require "tilemap"

-- load the Resources module.
local Resources =require "Resources"

-- Declare the 'GameMap' module.
local modname = "GameMap"
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
world = false -- the gobal map object.
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func newWorld: Build a new GameMap object.
--  @param w: The max tile width of the tilemap. Tilemap can be smaller or bigger.
--  @param h: The max tile height of the tilemap. Tilemap can be smaller or bigger.
-- @returns
--   @param _r: as tilemap object.
function newWorld(w, h)
	if world then
		-- Free the world table.
		world = table_ext.free(world) or false
		-- Reset the tilemap.fonts list.
		while table.getn(tilemap.fonts) > 0 do table.remove(tilemap.fonts) end
	end
	-- build the object.
	world = tilemap.new()
	-- Set the data.
	world.w = math.floor(w or 0) >= 0 and math.floor(w or 0) or 0
	world.h = math.floor(h or 0) >= 0 and math.floor(h or 0) or 0
	world.tblfonts = {} -- {[Filename], [number of glyphs]}
	 world.intfonts = 0
	world.objData = {}
	-- Return the world table.
	return world
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func loadWorld: load a world data from a file.
--  @param filename: The file to load.
-- @returns
--   @param _r: as tilemap object.
function loadWorld(filename)
	-- Free the world table.
	world = table_ext.free(world) or false
	--Reset the tilemap.fonts list.
	while table.getn(tilemap.fonts) > 0 do table.remove(tilemap.fonts) end
	-- load the file.
	world = tilemap.load(filename) or error("Invalid File: Can't load the requested File", modlv)
	-- load the required fonts for drawing the tilemap.
	local tbl = false
	local bfnt = false
	for n = 1, world.intfonts do
		tbl = (world.tblfonts or {})[n] or false
		if tbl and tbl[1] then
			-- Load the font from the Resources stack.
			bfnt, tbl[2] = Resources.loadTileFont(tbl[1], true)
			-- Add the font to the tilemap.fonts list.
			table.insert(tilemap.fonts, bfnt or error("Invalid GameMap File: Can't load the requested Font File", modlv))
		end
	end
	tbl = nil
	collectgarbage("collect")
	return world
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func saveWorld: Save the world data to a file.
--  @param filename: The file name as string.
-- @returns
--   @param nil
function saveWorld(filename)
	-- Save the tilemap.
	if world then table_ext.save(world, filename) end
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func free: free the world table.
--  @param : takes no arguments.
-- @returns
--   @param nil
function freeWorld()
	if not world then return end
	-- Free the world table.
	world = table_ext.free(world) or false
	--Reset the tilemap.fonts list.
	while table.getn(tilemap.fonts) > 0 do table.remove(tilemap.fonts) end
	return nil
end
-----------------------------------------------------------------------------------------------------


-----------------------------------------------------------------------------------------------------
-- @func addTileFont: Add a new font to the Tilemap object.
--  @param filename: The file name of a Texture to use to build the basic TileFont.
-- @returns
--   @param nil
function addTileFont(filename)
	if not world then return end
	-- Load and add the bfnt to the tilemap.fonts list.
	local bfnt, slots = Resources.loadTileFont(filename, true)
	table.insert(tilemap.fonts, bfnt or error("Invalid Font File: Can't load the requested Font File", modlv))
	-- Add the TileFont to the world table.
	table.insert(world.tblfonts ,{filename, slots}) -- {[Filename], [number of glyphs]}
	world.intfonts = table.getn(world.tblfonts)
	return nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func removeTileFont: Remove a font from the Tilemap object and world table.
--  @param indx: The Font index number.
-- @returns
--   @param nil
function removeTileFont(indx)
	if not world or indx > world.intfonts then return end
	-- Remove the font from the tilemap.fonts list.
	table.remove(tilemap.fonts, indx).
	-- Remove the font info from the world table.
	table.remove(world.tblfonts, indx).
	world.intfonts = table.getn(world.tblfonts)
	-- Remove the Tiles that used the current removed font and update the tile data of the remaining tiles.
	local data = world.data or {}
	local tile = false
	local w,h = table.maxn(data), 0
	for nx = 1, w do
		h = table.maxn(data[nx] or {})
		for ny = 1 , h do
			tile = data[nx] and data[nx][ny] or false
			if tile then
				local n = 1
				while n < table.getn(tile) do
					if tile[n] == indx then
						table.remove(tile, n)
						table.remove(tile, n)
						n = n - 2
					elseif tile[n] > indx then
						tile[n] = tile[n] - 1
					end
					n = n + 2
				end
				n = nil
			end
		end
	end
	return nil
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
	Resources = nil
	tilemap = nil
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
--End GameMap˜ module.
-----------------------------------------------------------------------------------------------------
