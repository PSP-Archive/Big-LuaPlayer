-----------------------------------------------------------------------------------------------------
-- Big LuaPlayer: screen module v1.00 2012 (c) M4MSOFT
-----------------------------------------------------------------------------------------------------

-- Declare the 'screen' module.
local modname = "screen"
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

-- Load the screen font.
tblmod.BFont = BmpFont.load(System.Directory.eboot.."/System/Font/monaco.ttf", 12)

-----------------------------------------------------------------------------------------------------
-- @func flip: swap the buffers.
--  @param n:
-- @returns
--   @param nil
tblmod.flip = GL.Flip
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func blit: draw a texture to the screen.
--  @param x: postion as integer
--  @param y: postion as integer
--  @param img: the texture
--  @param [alpha]: alpha as integer
--  @param [sx]: source postion as integer
--  @param [sy]: source postion as integer
--  @param [sw]: source width as integer
--  @param [sh]: source width as integer
-- @returns
--   @param nil
function blit(x, y, img, alpha, sx, sy, sw, sh)
	GL.BeginRects(img or error("bad argument #3 img to '"..modname..".blit' (texture expected, got "..type(img)..")", modlv))
	if alpha and alpha < 255 then GL.SetAlpha(alpha or 255) end
	if sx then
		local sw, sh = sw or img:width(), sh or img:height()
		GL.AddTex(x,y,sw,sh,sx,sy,sw,sh)
	else
		GL.AddRect(x,y)
	end
	GL.End()
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func waitVblankStart: wait for vertical blank start.
--  @param n:
-- @returns
--   @param nil
tblmod.waitVblankStart = GL.WaitVblankStart
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func print: print a text using the screen.Font.
--  @param x: postion as integer
--  @param y: postion as integer
--  @param str: the text
--  @param clr:
-- @returns
--   @param nil
function print(x,y,str,clr)
	BFont:print(x,y,str,clr)
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func drawRect: draw a rectangle to the buffer.
--  @param x: postion as integer
--  @param y: postion as integer
--  @param w: width as integer
--  @param h: width as integer
--  @param clr:
-- @returns
--   @param nil
function drawRect(x,y,w,h,clr)
	GL.BeginLines(GL.STRIP); GL.AddQuad(x,y,w,h,0,clr); GL.AddPoint(x, y); GL.End()
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func fillRect: draw a rectangle to the buffer.
--  @param x: postion as integer
--  @param y: postion as integer
--  @param w: width as integer
--  @param h: width as integer
--  @param clr:
-- @returns
--   @param nil
function fillRect(x,y,w,h,clr)
	GL.BeginRects(); GL.AddRect(x,y,w,h,0,clr); GL.End()
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func drawLine: draw a line to the buffer.
--  @param x0: postion as integer
--  @param y0: postion as integer
--  @param x1: postion as integer
--  @param y1: postion as integer
--  @param clr: the Color as pge.gfx.color
-- @returns
--   @param nil
function drawLine(x0,y0,x1,y1,clr)
	GL.BeginLines(); GL.AddLine(x0,y0,x1,y1,0,clr); GL.End()
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func save: save the screen buffer as a file.
--  @param path: the file name
-- @returns
--   @param nil
function save(path)
	GL.screenBuffer:save(path)
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func slowClear: clear the screen.
--  @param clr: the Color as color
-- @returns
--   @param nil
local mod_black = GL.clr.black:clone()
function slowClear(clr)
	GL.Start(); GL.Clear(clr or mod_black); GL.Finish()
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func clear: clear the screen.
--  @param clr: the Color as color
-- @returns
--   @param nil
tblmod.clear = GL.Clear
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func dispose: remove the module.
--  @param : takes no arguments
-- @returns
--   @param nil
function dispose()
	-- Dispose the moduletable and metatable.
	tblmod = nil
	tblmeta = nil
	-- Dispose local values.
	mod_black = nil
	-- Remove the module.
	package.loaded[modname] = nil
	_G[modname] = nil
	modlv = nil
	modv = nil
	modname = nil
	return nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--End screen module.
-----------------------------------------------------------------------------------------------------
