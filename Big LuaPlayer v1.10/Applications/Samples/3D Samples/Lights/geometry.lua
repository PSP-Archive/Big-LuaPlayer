-----------------------------------------------------------------------------------------------------
--Big LuaPlayer: geometry˜ module v1.00 year (c) company
-----------------------------------------------------------------------------------------------------

-- Declare the 'geometry' module.
local modname = "geometry"
local modv = "1.00"
local modlv = 2
module (modname, package.seeall)

_PACKAGE = nil
_M = nil
_NAME = nil
_VERSION = modv

-- Define some constant values.
local PI = (Gu and Gu.PI or math.pi)

-- Declare moduletable and metatable.
local tblmod = package.loaded[modname]

-----------------------------------------------------------------------------------------------------
-- @func generateTorus: Build a Torus model.
--  @param [slices]: The slices value as number.
--  @param [rows]: The rows value as number.
--  @param [radius]: The radius value as number.
--  @param [thickness]: The thickness value as number.
--  @param [texture]: The texture value as boolean.
--  @param [color]: The color value as boolean.
--  @param [normal]: The normal value as boolean.
--  @param [position]: The position value as boolean.
-- @returns
--   @param v0, i0: as vertices table, as indices talbe.
function generateTorus(slices, rows, radius, thickness, texture, color, normal, position)
	local v0 = {}
	local i0 = {}
	for j = 0, slices-1 do
		for i = 0, rows-1 do
			local n = i + j*rows
			local vert = {}
			local s, t = i + 0.5, j
			local cs, ct, ss, st = 0, 0, 0, 0
			local offset = 1

			cs = math.cos(s*(2*PI)/slices)
			ct = math.cos(t*(2*PI)/rows)
			ss = math.sin(s*(2*PI)/slices)
			st = math.sin(t*(2*PI)/rows)

			if texture then
				vert[offset+0] = cs * ct
				vert[offset+1] = cs * st
				offset = offset + 2
			end
			if color then
				vert[offset+0] = Color.new(128+(cs*ct)*127, 128+(cs*st)*127, 128+ss*127, 255)
				offset = offset + 1
			end
			if normal then
				vert[offset+0] = cs * ct
				vert[offset+1] = cs * st
				vert[offset+2] = ss
				offset = offset + 3
			end
			if position then
				vert[offset+0] = (radius + thickness * cs) * ct
				vert[offset+1] = (radius + thickness * cs) * st
				vert[offset+2] = thickness * ss
				offset = offset + 3
			end

			v0[n+1] = vert
		end
	end
	-- TODO: generate degenerates instead, so we can tristrip the torus
	for j = 0, slices-1 do
		for i = 0, rows-1 do
			local n = (i+(j*rows))*6
			local i1 = math.fmod((i+1), rows)
			local j1 = math.fmod((j+1), slices)
			i0[n+1] = i  + j  * rows
			i0[n+2] = i1 + j  * rows
			i0[n+3] = i  + j1 * rows
			i0[n+4] = i1 + j  * rows
			i0[n+5] = i1 + j1 * rows
			i0[n+6] = i  + j1 * rows
		end
	end
	return v0, i0
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func generateTorusTCNP: Build a Torus model with texture, color, normal and positions components.
--  @param [slices]: The slices value as number.
--  @param [rows]: The rows value as number.
--  @param [radius]: The radius value as number.
--  @param [thickness]: The thickness value as number.
-- @returns
--   @param v0, i0: as vertices table, as indices talbe.
function generateTorusTCNP(slices, rows, radius, thickness)
	return	generateTorus(slices, rows, radius, thickness, true, true, true, true)
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func generateTorusTCP: Build a Torus model with texture, color and positions components.
--  @param [slices]: The slices value as number.
--  @param [rows]: The rows value as number.
--  @param [radius]: The radius value as number.
--  @param [thickness]: The thickness value as number.
-- @returns
--   @param v0, i0: as vertices table, as indices talbe.
function generateTorusTCP(slices, rows,radius, thickness)
	return generateTorus(slices, rows, radius, thickness,true, true, false, true)
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func generateTorusNP: Build a Torus model with normal and positions components.
--  @param [slices]: The slices value as number.
--  @param [rows]: The rows value as number.
--  @param [radius]: The radius value as number.
--  @param [thickness]: The thickness value as number.
-- @returns
--   @param v0, i0: as vertices table, as indices talbe.
function generateTorusNP(slices, rows, radius, thickness)
	return generateTorus(slices, rows, radius, thickness,false,false,true, true)
end
-----------------------------------------------------------------------------------------------------


-----------------------------------------------------------------------------------------------------
-- @func generateGrid: Build a Grid model.
--  @param [slices]: The slices value as number.
--  @param [rows]: The rows value as number.
--  @param [width]: The radius value as number.
--  @param [depth]: The thickness value as number.
--  @param [texture]: The texture value as boolean.
--  @param [color]: The color value as boolean.
--  @param [normal]: The normal value as boolean.
--  @param [position]: The position value as boolean.
-- @returns
--   @param v0, i0: as vertices table, as indices talbe.
function generateGrid(columns, rows, width, depth, texture, color, normal, position)
	local v0 = {}
	local i0 = {}
	local ic = 1.0 / columns;
	local ir = 1.0 / rows;

	for j = 0, rows-1 do
		for i = 0, columns-1 do
			local n = i + j*rows
			local vert = {}
			local offset = 1

			if texture then
				vert[offset+0] = ic * i
				vert[offset+1] = ir * j
				offset = offset + 2
			end
			if color then
				vert[offset+0] = Color.new(255,255,255)
				offset = offset + 1
			end
			if normal then
				vert[offset+0] = 0.0;
				vert[offset+1] = 1.0;
				vert[offset+2] = 0.0;
				offset = offset + 3
			end
			if position then
				vert[offset+0] = ((ic * i)-0.5) * width
				vert[offset+1] = 0.0
				vert[offset+2] = ((ir * j)-0.5) * depth
				offset = offset + 3
			end

			v0[n+1] = vert
		end
	end
	for j = 0, rows-2 do
		for i = 0, columns-2 do
			local n = (i+(j*rows))*6
			i0[n+1] = i     + j     * columns
			i0[n+2] = (i+1) + j     * columns
			i0[n+3] = i     + (j+1) * columns
			i0[n+4] = (i+1) + j     * columns
			i0[n+5] = (i+1) + (j+1) * columns
			i0[n+6] = i     + (j+1) * columns
		end
	end
	return v0, i0
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func generateGrid: Build a Grid model with texture, color, normal and positions components.
--  @param [slices]: The slices value as number.
--  @param [rows]: The rows value as number.
--  @param [width]: The radius value as number.
--  @param [depth]: The thickness value as number.
-- @returns
--   @param v0, i0: as vertices table, as indices talbe.
function generateGridTCNP(columns, rows, width, depth)
	return generateGrid(columns,rows,width,depth,true, true, true, true)
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func generateGrid: Build a Grid model with normal and positions components.
--  @param [slices]: The slices value as number.
--  @param [rows]: The rows value as number.
--  @param [width]: The radius value as number.
--  @param [depth]: The thickness value as number.
--  @param [texture]: The texture value as boolean.
--  @param [color]: The color value as boolean.
--  @param [normal]: The normal value as boolean.
--  @param [position]: The position value as boolean.
-- @returns
--   @param v0, i0: as vertices table, as indices talbe.
function generateGridNP(columns, rows, width, depth)
	return generateGrid(columns,rows,width,depth, false, false, true, true)
end
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
	-- Dispose local values.
	PI = nil
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
--End geometry˜ module.
-----------------------------------------------------------------------------------------------------
