-----------------------------------------------------------------------------------------------------
--Phoenix Game Engine: point˜ module v1.00 year (c) company
-----------------------------------------------------------------------------------------------------

-- Declare the 'point' module.
local modname = "point"
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
-- @func new: Build a new point object.
--  @param [x]: The x value as number.
--  @param [y]: The y value as number.
-- @returns
--   @param tbl: as point.
function new(x, y)
	-- Check the arguments.
	if type(x) ~= "number" then error("bad argument #1 'x' to '"..modname..".new' (number expected, got "..type(x)..")", modlv) end
	if type(y) ~= "number" then error("bad argument #2 'y' to '"..modname..".new' (number expected, got "..type(y)..")", modlv) end
	-- build the object.
	local tbl = setmetatable ({}, tblmeta)
	-- Set the data.
	tbl.x = x
	tbl.y = y
	-- Return the object.
	return tbl
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func clone: Clone the object.
--  @param self: as point object.
-- @returns
--   @param tbl: as point.
function clone(self)
	-- Check the arguments.
	if type(self) ~= "table" then error("bad argument #1 'self' to '"..modname..".clone' (point expected, got "..type(self)..")", modlv) end
	-- build the object.
	local tbl = setmetatable ({}, tblmeta)
	-- Copy the data.
	for i,v in pairs(self) do tbl[i]=v end
	-- Return the object.
	return tbl
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func free: free the object
--  @param self: as point object.
-- @returns
--   @param nil
function free(self)
	-- Check the arguments.
	if type(self) ~= "table" then error("bad argument #1 'self' to '"..modname..".free' (point expected, got "..type(self)..")", modlv) end
	-- Free the data.
	for i,v in pairs(self) do self[i]=nil end
	self = nil
	return nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func _getx: Get the x value.
--  @param self: as point or number.
-- @returns
--   @param s_: as number
local _getx = function(self)
	return type(self) == "number" and self or
		   type(self["x"]) == "number" and self["x"] or
		   error("attempt to perform arithmetic on a "..type(self["x"]).." value", modlv+1)
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func _gety: Get the y value.
--  @param self: as point or number.
-- @returns
--   @param s_: as number
local _gety = function(self)
	return type(self) == "number" and self or
		   type(self["y"]) == "number" and self["y"] or
		   error("attempt to perform arithmetic on a "..type(self["y"]).." value", modlv+1)
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- Define the module metamethods.
tblmeta.__add = function(pnt1, pnt2) return new(_getx(pnt1)+_getx(pnt2), _gety(pnt1)+_gety(pnt2)) end
tblmeta.__sub = function(pnt1, pnt2) return new(_getx(pnt1)-_getx(pnt2), _gety(pnt1)-_gety(pnt2)) end
tblmeta.__mul = function(pnt1, pnt2) return new(_getx(pnt1)*_getx(pnt2), _gety(pnt1)*_gety(pnt2)) end
tblmeta.__div = function(pnt1, pnt2)
	return new(_getx(pnt2) ~= 0 and _getx(pnt1)/_getx(pnt2) or math.huge,
			   _gety(pnt2) ~= 0 and _gety(pnt1)/_gety(pnt2) or math.huge)
end
tblmeta.__mod = function(pnt1, pnt2) return new(_getx(pnt1)%_getx(pnt2), _gety(pnt1)%_gety(pnt2)) end
tblmeta.__pow = function(pnt1, pnt2) return new(_getx(pnt1)^_getx(pnt2), _gety(pnt1)^_gety(pnt2)) end
tblmeta.__unm = function(self) return new(-self["x"], -self["y"]) end
tblmeta.__tostring = function(self) return tostring(self.x)..", "..tostring(self.y) end
tblmeta.__eq = function(pnt1, pnt2) return pnt1["x"] == pnt2["x"] and _pnt1["y"] == pnt2["y"] end
tblmeta.__lt = function(pnt1, pnt2) return _getx(pnt1) < _getx(pnt2) and _gety(pnt1) < _gety(pnt2) end
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
	_getx = nil
	_gety = nil
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
--End point˜ module.
-----------------------------------------------------------------------------------------------------

--[[info: (Code Sample)

--load the module
Point = require "Point"

--Create a new object
pnt = Point(5, 5)

--Test the metamethods of the point object.
str = tostring(pnt)             --"5, 5"
str = tostring(2 + pnt)         --"7, 5"
str = tostring(2 - pnt - 2)     --"-5, -5"
str = tostring(2 * pnt * pnt)   --"25, 25"
str = tostring(pnt / 2)         --"2.5, 2.5"
str = tostring(pnt % (pnt - 2)) --"3, 3"
str = tostring(pnt ^ pnt)       --"3125, 3125"
str = tostring(-pnt)            --"-5, -5"
str = tostring(pnt == -pnt)     --"false"
str = tostring(pnt ~= -pnt)     --"true"
str = tostring(pnt < pnt)       --"false"
str = tostring(pnt > pnt)       --"false"
str = tostring(pnt <= pnt)      --"true"
str = tostring(pnt >= pnt)      --"true"
str = nil

--clone the point
pntClone = pnt:clone()

--free the points
pnt = pnt:free()
pntClone = pntClone:free()

--dispose the module
Point = Point.dispose()

--end Sample]]
