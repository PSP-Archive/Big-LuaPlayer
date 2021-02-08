-----------------------------------------------------------------------------------------------------
-- Big LuaPlayer: string_ext˜ module v1.07 2013 (c) M4MSOFT
-----------------------------------------------------------------------------------------------------

-- Additions to the string module
local extname = "string_ext"
local modname = "string"
local modv = "1.07"
local modlv = 2
module (modname, package.seeall)
package.loaded[extname] = string

_M = nil
_PACKAGE = nil
_NAME = nil
_VERSION = nil

_ext = {}
_ext._PACKAGE = nil
_ext._M = nil
_ext._NAME = nil
_ext._VERSION = modv

-- Declare moduletable.
local tblmod = package.loaded[modname]

-----------------------------------------------------------------------------------------------------
-- @func index: Return a character from the string
--  @param self: string
--  @param n: index.
-- @returns
--   @param s_: string.sub (s, n, n)
function index(self, n)
	if type(self) ~= "string" then error("bad argument #1 'self' to '"..modname..".index' (string expected, got "..type(self)..")", modlv) end
	if type (n) == "number" then
		return sub(self, n, n)
	else
		error("bad argument #2 'n' to '"..modname..".index' (number expected, got "..type(n)..")", modlv)
	end
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func insert: insert a string in to a string.
--  @param self: string
--  @param n: index.
--  @param v: string.
--  @param [r]: replace the text as boolean.
-- @returns
--   @param s_: the new string
function insert(self, n, v, r)
	if type(self) ~= "string" then error("bad argument #1 'self' to '"..modname..".insert' (string expected, got "..type(self)..")", modlv) end
	if type (n) == "number" then
		if type (v) == "number" then v = tostring(v) end
		if type (v) == "string" then
			local r = r or false
			if r then
				if n < 0 then n = math.max(len(self)+1+n, 1) end
				return sub(self, 1, n-1)..v..(n > 0 and sub(self, n+len(v), -1) or "")
			else
				return sub(self, 1, n-1)..v..(n ~= 0 and sub(self, n, -1) or "")
			end
		else
			error("bad argument #3 'v' to '"..modname..".insert' (string expected, got "..type(v)..")", modlv)
		end
	else
		error("bad argument #2 'n' to '"..modname..".insert' (number expected, got "..type(n)..")", modlv)
	end
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func remove: remove a part from a string.
--  @param self: string
--  @param n: index.
--  @param [l]: the lenght of the part to remove.
-- @returns
--   @param s_: the new string
function remove(self, n, l)
	if type(self) ~= "string" then error("bad argument #1 'self' to '"..modname..".remove' (string expected, got "..type(self)..")", modlv) end
	if type(n) ~= "number" then error("bad argument #1 'n' to '"..modname..".remove' (number expected, got "..type(n)..")", modlv) end
	local l = l or 1
	if type(l) ~= "number" then error("bad argument #1 'l' to '"..modname..".remove' (number expected, got "..type(l)..")", modlv) end
	if n == 0 or I == 0 then return self end
	if n < 0 then n = math.max(len(self)+1+n, 1) end
	if l < 0 then l = math.max(len(self)-n+2+l, 0) end
	return sub(self,1,n-1)..sub(self,n+l,-1)
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func ltrim: Remove leading matter from a string
--  @param self: string
--  @param [c]: the character to trim as string.
-- @returns
--   @param s_: string without leading c
function ltrim(self, c)
	if type(self) ~= "string" then error("bad argument #1 'self' to '"..modname..".ltrim' (string expected, got "..type(self)..")", modlv) end
	local c = c or " "
	if type(c) ~= "string" then error("bad argument #2 'c' to '"..modname..".ltrim' (string expected, got "..type(c)..")", modlv) end
	local n = 1
	while sub(self,n,n) == c do
		n = n + 1
	end
	c = nil
	return string.sub(self,n,-1)
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func rtrim: Remove trailing matter from a string
--  @param self: string
--  @param [c]: the character to trim as string.
-- @returns
--   @param s_: string without trailing c
function rtrim(self, c)
	if type(self) ~= "string" then error("bad argument #1 'self' to '"..modname..".rtrim' (string expected, got "..type(self)..")", modlv) end
	local c = c or " "
	if type(c) ~= "string" then error("bad argument #2 'c' to '"..modname..".rtrim' (string expected, got "..type(c)..")", modlv) end
	local n = len(self)
	while sub(self,n,n) == c do
		n = n - 1
	end
	c = nil
	return sub(self,1,n)
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func trim: Remove leading and trailing matter from a string
--  @param self: string
--  @param [c]: the character to trim as string.
-- @returns
--   @param s_: string without leading/trailing c
function trim(self, c)
	if type(self) ~= "string" then error("bad argument #1 'self' to '"..modname..".trim' (string expected, got "..type(self)..")", modlv) end
	local c = c or " "
	if type(c) ~= "string" then error("bad argument #2 'c' to '"..modname..".trim' (string expected, got "..type(c)..")", modlv) end
	local n = 1
	while sub(self,n,n) == c do
		n = n + 1
	end
	local l = len(self)
	while sub(self,l,l) == c do
		l = l - 1
	end
	c = nil
	return sub(self,n,l)
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func dispose: remove the module
--  @param : takes no arguments
-- @returns
--   @param nil
function dispose()
	-- Dispose the moduletable.
	tblmod.index = nil
	tblmod.insert = nil
	tblmod.remove = nil
	tblmod.ltrim = nil
	tblmod.rtrim = nil
	tblmod.trim = nil
	tblmod.dispose = nil
	tblmod = nil
	-- Remove the module.
	_ext._PACKAGE = nil
	_ext._M = nil
	_ext._NAME = nil
	_ext._VERSION = nil
	_ext = nil
	package.loaded[extname] = nil
	_G[extname]=nil
	modlv = nil
	modv = nil
	modname = nil
	extname = nil
	return nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--End string_ext˜ module.
-----------------------------------------------------------------------------------------------------

-- Changelog:
  -- v1.07 29-01-2013:
