-----------------------------------------------------------------------------------------------------
-- Big LuaPlayer: ScrollBar˜ module v1.00 2013 (c) M4MSOFT
-----------------------------------------------------------------------------------------------------

-- Declare the 'ScrollBar' module.
local modname = "ScrollBar"
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
-- @func new: Build a new scrollBar object.
--  @param len: the total amount of visble scroll steps.
--  @param maxn: the total amount of scroll steps.
--  @param size: the draw size of the scrollBar.
--  @param [min]: the minimum blok size of the scrollBar.
--  @param [max]: the maximum blok size of the scrollBar.
-- @returns
--   @param tbl: as point.
function new(len, maxn, size, min, max)
	-- build the object.
	local tbl = setmetatable ({}, tblmeta)

	-- Set the data.
	tbl.n = 1              -- first visble scroll item.
	tbl.len = len or 0     -- total visble scroll items.
	tbl.maxn = maxn or len -- total scroll items.

	tbl.len = math.min(tbl.maxn, tbl.len)
	tbl.maxn = math.max(tbl.maxn, tbl.len)

	tbl.cursor = 0   -- the cursor position.
	tbl.selected = 1 -- Selected scroll items.

	tbl.size = size           -- the draw size of the scrollBar.
	tbl.bloksize = 0          -- the blok size of the scrollBar.
	tbl.minsize = min or 0    -- the minimum blok size of the scrollBar.
	tbl.maxsize = max or size -- the maximum blok size of the scrollBar.
	tbl.step = 0              -- the step size of the scrollBar.
	tbl.position = 0          -- the draw position of the scrollBar.
	update(tbl)               -- update the scrollBar object.

	-- Return the object.
	return tbl
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func clone: Clone the object.
--  @param self: as scrollBar object.
-- @returns
--   @param tbl: as point.
function clone(self)
	-- Check the arguments.
	if type(self) ~= "table" then error("bad argument #1 'self' to '"..modname..".clone' (scrollBar expected, got "..type(self)..")", modlv) end
	-- build the object.
	local tbl = setmetatable ({}, tblmeta)
	-- Copy the data.
	for i,v in pairs(self) do tbl[i]=v end
	-- Return the object.
	return tbl
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func update: update the scrollBar object.
--  @param self: as scroll object.
-- @returns
--   @param nil
function update(self)
	if self then
		if math.max(self["maxn"], self["len"]) > self["len"] then
			self["bloksize"] =  math.min(math.max(math.floor(self["size"] * self["len"] / self["maxn"]), self["minsize"]), self["maxsize"])
			self["step"] = (self["size"] - self["bloksize"]) / (self["maxn"] - self["len"])
		else
			self["bloksize"] = self["size"]
			self["step"] = 0
		end
		--Calculate the bar postion.
		self["position"] = math.floor(self["step"] * (self["n"]-1))
	end
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func set: Set the cursor location.
--  @param self: as scrollBar object.
--  @param [n]: the first item visble.
--  @param [cursor]: the position of the cursor.
--  @param [item]: the selected item.
-- @returns
--   @param nil
function set(self, n, cursor, item)
	if self then
		if item then
			item = math.max(math.min(item, self["maxn"]),1)
			self["n"] = math.min(item, math.max(self["maxn"]-self["len"]+1,1))
			self["cursor"] = item - self["n"]
		else
			self["cursor"] = math.max(math.min(cursor or self["cursor"], self["len"]-1),0)
			self["n"] = math.max(math.min(n or self["n"], self["maxn"]-self["cursor"]),1)
		end
		--Calculate the bar postion.
		self["position"] = math.floor(self["step"] * (self["n"]-1))
		--Calculate the selected postion.
		self["selected"] = self["n"] + self["cursor"]
	end
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func _scrollAdd: Add a value to the scrollBar.
--  @param self: as scrollBar object.
--  @param n: the number to add as integer.
--  @param block: as boolean.
-- @returns
--   @param s_: as number
local _scrollAdd = function(self, n, block)
	self["cursor"] = self["cursor"] + n
	if self["cursor"] < 0 then
		n = self["cursor"]
		self["n"] = self["n"] + n
		if self["n"] < 1 then
			if not block then
				self["n"] = self["maxn"] - self["len"] + 1
				self["cursor"] = self["len"]-1
			else
				self["n"] = 1
				self["cursor"] = 0
			end
		else
			self["cursor"] = 0
		end
		--Calculate the bar postion.
		self["position"] = math.floor(self["step"] * (self["n"]-1))
	elseif self["cursor"] > self["len"]-1 then
		n = self["cursor"] - self["len"] + 1
		self["n"] = self["n"] + n
		if self["n"] > self["maxn"] - self["len"] + 1 then
			if not block then
				self["n"] = 1
				self["cursor"] = 0
			else
				self["n"] = self["maxn"] - self["len"] + 1
				self["cursor"] = self["len"]-1
			end
		else
			self["cursor"] = self["len"]-1
		end
		--Calculate the bar postion.
		self["position"] = math.floor(self["step"] * (self["n"]-1))
	end
	--Calculate the selected postion.
	self["selected"] = self["n"] + self["cursor"]
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func add: Move the cursor down.
--  @param self: as scrollBar object.
--  @param step: the size of the step as unsigned integer.
--  @param block: as boolean.
-- @returns
--   @param nil
function add(self, step, block)
	if type(self) == "table" then
		_scrollAdd(self, step or 1, block or false)
	else
		error("bad argument #1 'self' to '"..modname..".add' (scrollBar expected, got "..type(self)..")", modlv)
	end
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func sub: Move the cursor up.
--  @param self: as scrollBar object.
--  @param step: the size of the step as unsigned integer.
--  @param block: as boolean.
-- @returns
--   @param nil
function sub(self, step, block)
	if type(self) == "table" then
		_scrollAdd(self, -(step or 1), block or false)
	else
		error("bad argument #1 'self' to '"..modname..".sub' (scrollBar expected, got "..type(self)..")", modlv)
	end
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func vnodes: iterate over all current visble nodes.
--  @param self: as scrollBar object.
-- @returns
--   @param f_: iterator function
--   @param self: the table, as above
--   @param l_: self["len"]
function vnodes(self)
	return function (s, i)
		i = i + 1;
		if i < s["len"] then
			return i, s["n"] + i
		end
	end,
	self, -1
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func free: free the object
--  @param self: as scrollBar object.
-- @returns
--   @param nil
function free(self)
	-- Check the arguments.
	if type(self) ~= "table" then error("bad argument #1 'self' to '"..modname..".free' (scrollBar expected, got "..type(self)..")", modlv) end
	-- Free the data.
	for i,v in pairs(self) do self[i]=nil end
	self = nil
	return nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func _getNArg: Get the x value.
--  @param a: as scrollBar or number.
--  @param b: as scrollBar or number.
-- @returns
--   @param s_: as number
local _getNArg = function(a, b, add)
	if type(a) == "table" and type(b) == "number" then return a, b
	elseif type(b) == "table" and type(a) == "number" then return b, a
	elseif type(a) == "table" and a == b then return a, 0
	else return error("attempt to perform arithmetic on a table value", modlv+1) end
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- Define the module metamethods.
tblmeta.__add = function(a, b) a,b = _getNArg(a, b); _scrollAdd(a,  b, false); return a end
tblmeta.__sub = function(a, b) a,b = _getNArg(a, b); _scrollAdd(a, -b, false); return a end
tblmeta.__tostring = function(self) return string.format("%s: [n = %d, cursor = %d, selected = %d]", modname, self["n"], self["cursor"], self["selected"]) end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func dispose: Remove the module.
--  @param : takes no arguments.
-- @returns
--   @param nil
function dispose()
	-- Dispose the moduletable and metatable.
	tblmod = nil
	tblmeta = nil
	-- Dispose local values.
	_scrollAdd = nil
	_getNArg = nil
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
--End ScrollBar˜ module.
-----------------------------------------------------------------------------------------------------

--[[info: (Code Sample)

--load the module
ScrollBar = require "ScrollBar"

-- Create the menu table.
local App_Menu = {"Option 1", "Option 2", "Option 3", "Option 4"}

-- Create a ScrollBar object
VscrollBar = ScrollBar.new(3, 4, 272)

-- Move the cursor up.
VscrollBar = VscrollBar + 1

-- Print the menu.
for idx, node in VscrollBar:vnodes() do
	if VscrollBar["selected"] == node then
	    -- Print the Cursor
		print(">> "..App_Menu[node])
	else
		print(App_Menu[node])
	end
end

-- free the ScrollBar object.
VscrollBar = VscrollBar:free()

-- dispose the module
ScrollBar = ScrollBar.dispose()

--end Sample]]
