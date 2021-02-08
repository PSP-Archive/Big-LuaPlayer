-----------------------------------------------------------------------------------------------------
-- rectangle˜ module v1.00 2012 (c) M4MSOFT
-----------------------------------------------------------------------------------------------------

-- Declare the 'rectangle' module.
local modname = "rectangle"
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
-- @func new: Build a new rectangle object.
--  @param x: The x value as number.
--  @param y: The y value as number.
--  @param w: The width as number.
--  @param h: The height as number.
--  @param [_r]: The object table.
-- @returns
--   @param _r: as rectangle.
function new(x, y, w, h, _r)
	-- build the object.
	local _r = setmetatable ({} or _r, tblmeta)
	-- Set the data.
	_r["x"] = x
	_r["y"] = y
	_r["w"] = w
	_r["h"] = h
	-- Return the object.
	return _r
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func clone: Clone the object.
--  @param r: as rectangle object.
--  @param [_r]: The object table.
-- @returns
--   @param _r: as rectangle.
function clone(r, _r)
	-- build the object.
	local _r = setmetatable ({} or _r, tblmeta)
	-- Copy the data.
	--for i,v in pairs(r) do _r[i]=v end
	_r["x"] = r["x"]
	_r["y"] = r["y"]
	_r["w"] = r["w"]
	_r["h"] = r["h"]
	-- Return the object.
	return _r
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func intersect: Check if the rectangles are intersected.
--  @param r1: as rectangle object.
--  @param r2: as rectangle object.
-- @returns
--   @param _r: result as boolean.
function intersect(r1, r2)
   return r1["x"]+r1["w"] > r2["x"] and
		  r1["x"] < r2["x"]+r2["w"] and
		  r1["y"]+r1["h"] > r2["h"] and
		  r1["y"] < r2["y"]+r2["h"] or false
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func free: free the object
--  @param r: as rectangle object.
-- @returns
--   @param nil
function free(r)
	-- Free the data.
	--for i,v in pairs(r) do r[i]=nil end
	r["x"] = nil
	r["y"] = nil
	r["w"] = nil
	r["h"] = nil
	r = nil
	return nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- Define the module metamethods.
tblmeta.__tostring = function(r) return tostring(r["x"])..", "..tostring(r["y"]..", "..r["w"])..", "..tostring(r["h"]) end
tblmeta.__eq = intersect
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
--End rectangle˜ module.
-----------------------------------------------------------------------------------------------------
