-----------------------------------------------------------------------------------------------------
-- Big LuaPlayer: table_ext˜ Module v1.01 2012 (C) M4MSOFT
--
--  Orignale:
--    title: table.save
--    version: 0.3
--    date: ??-??-????
--    company: ????
--    source: http://lua-users.org/wiki/SaveTableToFile
--
--    info:
--      table.save(table, path)
--        Save Table to File
--        Functions, Userdata and indices of these will not be saved
--        Strings will be converted to enclosed long brakets, makes it
--        easier to save regarding special chars
--        References are preserved
--
--      table.load(file, path)
--        returns a previously saved table
--
-----------------------------------------------------------------------------------------------------

-- Additions to the table module
local extname = "table_ext"
local modname = "table"
local modv = "1.01"
local modlv = 2
module (modname, package.seeall)
package.loaded[extname] = table

_M = nil
_PACKAGE = nil
_NAME = nil
_VERSION = nil

_ext = {}
_ext._PACKAGE = nil
_ext._M = nil
_ext._NAME = nil
_ext._VERSION = modv

-- Declare the moduletable.
local tblmod = package.loaded[modname]

-----------------------------------------------------------------------------------------------------
--Add the string function.
-- @func string.enclose: Enclose string by long brakets.
--  @param s: as string.
--  @param [l]: the max lenght of the brakets as interger.
-- @returns
--   @param s_: the new string.
local find = string.find
local rep = string.rep
local format = string.format
function string.enclose(s, l)
	if type(s) ~= "string" then error("bad argument #1 's' to 'string.enclose' (string expected, got "..type(s)..")", modlv) end
	if type(l) ~= "number" and type(l) ~= "nil" then error("bad argument #2 'l' to 'string.enclose' (number expected, got "..type(l)..")", modlv) end
	local n = 0
	while true do
		if l and n > l then
			error("the lenght of the brakets is to large (range: 0-"..l..")", 2)
		elseif find(s, "]"..rep("=",n).."]") or
			   find(s, "]"..rep("=",n).."$") then
			n = n + 1
		else
			return "["..rep("=",n).."["..s.."]"..rep("=",n).."]"
		end
	end
	n = nil
end
local enclose = string.enclose
-----------------------------------------------------------------------------------------------------


-----------------------------------------------------------------------------------------------------
-- @func _pickle: pickle the table.
--  @param src: the source table.
--  @param dst: the destine table.
--  @param ref: References table.
-- @returns
--   @param nil
local function _pickle(src, dst, ref)
	if type(src) ~= "table" then error("bad argument #1 src to '"..modname.."._pickle' (table expected, got "..type(src)..")", modlv+1) end
	if type(dst) ~= "table" then error("bad argument #2 dst to '"..modname.."._pickle' (table expected, got "..type(dst)..")", modlv+1) end
	local ref = ref or {}
	dst[1] = src
	ref[src] = 1
	for pi,pv in ipairs(dst) do
		for i,v in pairs(pv) do
			-- handle index
			if type(i) == "table" then
				if not ref[i] then
					insert(dst, i)
					ref[i] = maxn(dst)
				end
			end
			-- handle value
			if type(v) == "table" then
				if not ref[v] then
					insert(dst, v)
					ref[v] = maxn(dst)
				end
			end
		end
	end
	src,dst,ref = nil,nil,nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func _unpickle: Restore the pickled table.
--  @param ptr: pointer to the first item to unpickle.
--  @param src: the source table.
--  @param dst: pointer to the destine table/item.
--  @param [ref]: References table.
-- @returns
--   @param nil
local function _unpickle(ptr, src, dst, ref)
	local ref = ref or {}
	local eq = rawequal(ptr, dst)
	ref[ptr] = dst
	for i,v in pairs(ptr) do
		local i2 = i
		if type(i) == "table" then
			local ptr2 = src[i[1]]
			if ref[ptr2] then i2 = ref[ptr2] else
				i2 = eq and ptr2 or {}
				_unpickle(ptr2, src, i2, ref)
			end
			ptr2 = nil
		end
		local v2 = v
		if type(v) == "table" then
			local ptr2 = src[v[1]]
			if ref[ptr2] then v2 = ref[ptr2] else
				v2 = eq and ptr2 or {}
				_unpickle(ptr2, src, v2, ref)
			end
			ptr2 = nil
		end
		--dst[i2] = v2
		rawset(dst, i2, v2)
		i2,v2 = nil,nil
	end
	eq = nil
	ref = nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func unpickle: Restore the pickled table.
--  @param t: as table.
--  @param toSrc: as boolean.
-- @returns
--   @param t_unpickled: as data.
local bnl_cg = true
local function unpickle(t, toSrc)
	if type(t) ~= "table" then error("bad argument #1 t to '"..modname..".unpickle' (table expected, got "..type(t)..")", modlv+1) end
	if type(t[1]) == "table" then
		--Unpickle the table.
		local t_unpickled = toSrc and t[1] or {}
		if not pcall(_unpickle, t[1], t, t_unpickled) then error("invalid format to '"..modname.."._unpickle' (Can't unpickle the table)", modlv+1) end
		if bnl_cg then collectgarbage("collect") end
		--return the unpickled table.
		return t_unpickled or {}
	else
		--return the object.
		return t[1]
	end
end
-----------------------------------------------------------------------------------------------------


-----------------------------------------------------------------------------------------------------
-- @func size: Find the number of elements in a table.
--  @param t: as table.
-- @returns
--   @param n: number of elements in t.
function size(t)
	if type(t) ~= "table" then error("bad argument #1 t to '"..modname..".size' (table expected, got "..type(t)..")", modlv) end
	local n = 0
	for _ in pairs(t) do
		n = n + 1
	end
	return n
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func createEmpty: Create a empty table.
--  @param v: The default table value.
--  @param ...: The table size as interger.
-- @returns
--   @param t: as table.
function createEmpty(v, ...)
	bnl_cg = false
	local n,nMax = 0,select('#', ...)
	local function creating(_t, ...)
		n = n + 1
		local arg = n <= nMax and select(n, ...) or 0
		if arg <= 0 then _t = nil end
		for i = 1, arg do
			_t[i] = creating({}, ...) or clone(v)
		end
		arg = nil
		n = n - 1
		return _t
	end

	--Create the table.
	local t = creating({}, ...) or clone(v)
	creating = nil
	n,nMax = nil,nil
	collectgarbage("collect")
	bnl_cg = true

	--Dispose arguments.
	v = nil

	return t
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func load: Load the data from a file.
--  @param path: The file name as string.
-- @returns
--   @param t: as data.
function load(path)
	if type(path) ~= "string" then error("bad argument #1 path to '"..modname..".load' (string  expected, got "..type(path)..")", modlv) end
	local t = {}
	bnl_cg = false

	--Load the lua script.
	local script, msg_error = loadfile(path)
	--Run the script and collect the table.
	if script then
		t = script() or false
		if type(t) == "table" then
			t = unpickle(t)
		else t = {} end
	end

	--Dipose the script
	script, msg_error = nil, nil
	collectgarbage("collect")

	--Dispose arguments.
	path = nil

	--Return the table.
	bnl_cg = true
	return t
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func save: Save the data to a file.
--  @param t: The data as object.
--  @param path: The file name as string.
-- @returns
--   @param nil
function save(t, path)
	if type(path) ~= "string" then error("bad argument #2 path to '"..modname..".save' (string  expected, got "..type(path)..")", modlv) end

	local file = io.open(path, "w") or false
	if file then
		--write the file header.
		file:write(string.format("-%s%s","- This is a "..extname.."˜ module file. (v "..modv..")","\n"))

		--Pickle the table.
		local tbl,ptr = {},{}
		local si,sv = "",""
		if type(t) == "table" then
			_pickle(t, tbl, ptr)
		else tbl[1] = t end

		--Save the table.
		file:write("return {\n")
		for pi,pv in ipairs(tbl) do
			if type(pv) == "table" then
				file:write(" {")
				for i,v in pairs(pv) do
					-- escape functions
					if type(v) ~= "function" and type(v) ~= "userdata" then
						-- handle index
						if type(i) == "table" then si = "[{"..ptr[i].."}]" else
							si = (type(i) == "string" and "[ "..enclose(i, 50).." ]" ) or format( "[%d]", i)
						end
						-- handle value
						if type(v) == "table" then sv = "{"..ptr[v].."}" else
							sv = (type(v) == "string" and enclose(v, 50)) or tostring(v)
						end
						file:write(si.." = "..sv..", ")
					end
				end
				file:write("}, --idx = "..pi.."\n")
			elseif type(pv) ~= "function" and type(pv) ~= "userdata" then
				sv = (type(pv) == "string" and enclose(pv, 50)) or tostring(pv)
				file:write(" "..sv..", \n")
			end
		end
		file:write("}-- return the table.\n")
		tbl,ptr = nil,nil
		si,sv = nil,nil
		file:close()
	end
	file = nil
	collectgarbage("collect")

	--Dispose arguments.
	path = nil

	return nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func clone: Clone the table.
--  @param t: The data as object.
--  @param [nometa]: if non-nil don't copy metatable.
-- @returns
--   @param t: data as object.
function clone(t, nometa)
	if type(t) == "table" then
		--Pickle the table.
		local meta = not nometa
		local tbl,ptr = {},{}
		local ti,tv = 0,0
		_pickle(t, tbl, ptr)

		--clone the table.
		t = {}
		for pi,pv in ipairs(tbl) do
			if meta then
				t[pi] = setmetatable({}, getmetatable(pv))
			else t[pi] = {} end
			for i,v in pairs(pv) do
				ti,tv = i,v
				if type(i) == "table" then ti = {ptr[i]} end
				if type(v) == "table" then tv = {ptr[v]} end
				t[pi][ti] = tv
			end
		end
		tbl,ptr = nil,nil
		ti,tv = nil,nil

		--Unpickle the table.
		t = unpickle(t, meta)
		meta = nil
	end

	--Return the object.
	return t
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func clear: Clear the table.
--  @param t: as table.
--  @param v: The default table value.
-- @returns
--   @param t: as table.
function clear(t, v)
	if type(t) ~= "table" then error("bad argument #1 t to '"..modname..".clear' (table expected, got "..type(t)..")", modlv) end
	bln_cg = false

	--Pickle the table.
	local tbl = {}
	_pickle(t, tbl)

	--clear the table.
	for pi,pv in ipairs(tbl) do
		for ti,tv in pairs(pv) do
			if type(tv) ~= "table" then
				rawset(pv, ti, clone(v))
			end
		end
	end
	tbl = nil
	collectgarbage("collect")

	--Dispose arguments.
	v = nil

	bnl_cg = true
	return t
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func isEmpty: Check if the table is a empty table.
--  @param t: The data as object.
-- @returns
--   @param r_: as boolean.
function isEmpty(t)
	if type(t) == "table" then
		return not next(t)
	else return true end
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func free: Free the table.
--  @param t: as table.
-- @returns
--   @param nil
function free(t)
	if type(t) == "table" then
		--Pickle the table.
		local tbl = {}
		_pickle(t, tbl)
		--Dispose the table.
		for pi,pv in ipairs(tbl) do
			for i,v in pairs(pv) do
				rawset(pv, i, nil)
			end
		end
		tbl = nil
	end
	--Dispose arguments.
	t = nil
	--Return nil.
	return nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func dispose: remove the module.
--  @param : takes no arguments
-- @returns
--   @param nil
function dispose()
	-- Dispose the moduletable.
	tblmod.size = nil
	tblmod.createEmpty = nil
	tblmod.load = nil
	tblmod.save = nil
	tblmod.clone = nil
	tblmod.clear = nil
	tblmod.isEmpty = nil
	tblmod.free = nil
	tblmod.dispose = nil
	tblmod = nil
	--Dispose the string function.
	find = nil
	rep = nil
	format = nil
	string.enclose = nil
	enclose = nil
	--Dispose local functions
	_unpickle = nil
	bnl_cg = nil
	unpickle = nil
	_pickle = nil
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
--End table_ext˜ module.
-----------------------------------------------------------------------------------------------------

-- Changelog:
  -- v1.00 09-11-2012:
  -- v1.01 18-11-2012:
	-- Added table.size(t) return the size of the table.
	-- Updated table.clone(t [, nometa]) to clone the metatables.
