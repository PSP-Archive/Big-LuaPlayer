-----------------------------------------------------------------------------------------------------
-- Big LuaPlayer: Cel shading example˜ v 1.00 2005 (C) McZonk <www.teamemergencyexit.com>
--
-- Description:
--     Simple cel shading example written as xmas present for psp community
--     This samples is base on the celshading.c sample by McZonk
--
-----------------------------------------------------------------------------------------------------

-- Set the Cpu Clock Frequencies.
Power.cpuSpeed(100)

-- Render the load screen.
GL.Clear(GL.clr.white)
screen.print(0, 259, "Loading ...", GL.clr.black)
GL.Flip()

-- load the screen module.
require "screen"

-- Define the application data.
local App_Running = true
local App_frameTime = 1000/60 -- (~16.67 ms)
local App_Timer = Timer.new()
local App_useTexture = true
local App_DrawOutline = false

-- Define the control enumerations.
local CTRL_ENUM_EXIT = Controls.START + Controls.CIRCLE -- (8 + 8192)
local CTRL_ENUM_TOOGLE_TEX = Controls.CROSS -- (16384)
local CTRL_ENUM_TOOGLE_OUT = Controls.TRIANGLE -- (4096)

-- Define the colors.
local white = Color.new(255, 255, 255)
local black = Color.new(0, 0, 0)
local clear = Color.fromString("0xffffffff")
local object = Color.fromString("0xffff9966")
local outline = Color.fromString("0xff000000")

-- Define some constant values.
local TORUS_SLICES    = 48 -- numc
local TORUS_ROWS      = 48 -- numt
local TORUS_RADIUS    = 1.0
local TORUS_THICKNESS = 0.5
local LIGHT_DISTANCE  = 3.0
local PI              = (Gu.PI)

-- Generate Torus
local tblTorus_v0 = {}
local tblTorus_v1 = {}
for j = 0, TORUS_SLICES-1 do
	for i = 0, TORUS_ROWS-1 do
		local n = i + j*TORUS_ROWS
		local vert0 = {}
		local vert1 = {}
		local s, t = i+0.5, j
		local cs,ct,ss,st = 0, 0, 0, 0
		cs = math.cos(s*(2*PI)/TORUS_SLICES)
		ct = math.cos(t*(2*PI)/TORUS_ROWS)
		ss = math.sin(s*(2*PI)/TORUS_SLICES)
		st = math.sin(t*(2*PI)/TORUS_ROWS)

		vert0[1] = cs * ct
		vert0[2] = cs * st
		vert0[3] = ss

		vert0[4] = (TORUS_RADIUS + TORUS_THICKNESS * cs) * ct
		vert0[5] = (TORUS_RADIUS + TORUS_THICKNESS * cs) * st
		vert0[6] = TORUS_THICKNESS * ss

		vert1[1] = vert0[4] + vert0[1] * 0.1;
		vert1[2] = vert0[5] + vert0[2] * 0.1;
		vert1[3] = vert0[6] + vert0[3] * 0.1;

		tblTorus_v0[n+1] = vert0
		tblTorus_v1[n+1] = vert1
	end
end
local tblTorus_in = {}
for j = 0, TORUS_SLICES-1 do
	for i = 0, TORUS_ROWS-1 do
		local n = (i+(j*TORUS_ROWS))*6
		local i1 = math.fmod((i+1), TORUS_ROWS)
		local j1 = math.fmod((j+1), TORUS_SLICES)
		tblTorus_in[n+1] = i  + j  * TORUS_ROWS
		tblTorus_in[n+2] = i1 + j  * TORUS_ROWS
		tblTorus_in[n+3] = i  + j1 * TORUS_ROWS
		tblTorus_in[n+4] = i1 + j  * TORUS_ROWS
		tblTorus_in[n+5] = i1 + j1 * TORUS_ROWS
		tblTorus_in[n+6] = i  + j1 * TORUS_ROWS
	end
end
-- Define the Torus_v0 vertices object.
local vertTorus_v0 = Gum.verticesNew(
	Gum.PRIM_TRIANGLES,
	Gum.VERT_NORMAL_BITS + Gum.VERT_VERTEX_BITS + Gum.VERT_TRANSFORM_3D,
	tblTorus_v0,
	tblTorus_in
)
-- Define the Torus_v1 vertices object.
local vertTorus_v1 = Gum.verticesNew(
	Gum.PRIM_TRIANGLES,
	Gum.VERT_VERTEX_BITS + Gum.VERT_TRANSFORM_3D,
	tblTorus_v1,
	tblTorus_in
)
tblTorus_in = nil
tblTorus_v1 = nil
tblTorus_v0 = nil

--load the lightmap texture.
local lightmap = Texture.load("lightmap.png")

-----------------------------------------------------------------------------------------------------
-- Initialize Gu funcion.
local function initGuFunc()
	-- Initialize the GU System for 3D rendering. GL.Start() works too but then the current
	-- Gu settings won't be saved and restored after the 'finish' function.
	Gu.start3d()

	-- Setup GU
	Gu.depthRange(41250, 8340)
	Gu.depthFunc(Gu.FNC_GEQUAL)
	Gu.enable(Gu.STATE_DEPTH_TEST)
	Gu.frontFace(Gu.FF_CW)
	Gu.enable(Gu.STATE_CULL_FACE)
	Gu.enable(Gu.STATE_CLIP_PLANES)
	Gu.disable(Gu.STATE_TEXTURE_2D)
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- Draw function.
local rx, ry = 0, 0
local function drawFunc()
	-- Setup Gu funcion.
	initGuFunc()

	-- Clear the screen and depth buffer.
	Gu.clearColor(clear)
	Gu.clearDepth()
	Gu.clear(Gu.CLBF_COLOR_BUFFER_BIT + Gu.CLBF_DEPTH_BUFFER_BIT)

	-- Setup projection matrix. ('The Camara lens')
	Gum.matrixMode(Gum.MTRX_PROJECTION)
	Gum.loadIdentity()
	Gum.perspective(75, 16/9, 1, 1000) -- fovy, aspect, near, far

	-- Setup the view matrix. ('The position and angle of the Camara')
	Gum.matrixMode(Gum.MTRX_VIEW)
	Gum.loadIdentity()

	-- Setup model matrix. ('The matrix used to transform the 3D model')
	Gum.matrixMode(Gum.MTRX_MODEL)
	Gum.loadIdentity()
	Gum.translate(0, 0, -2.75) -- The x, y, z position.
	Gum.rotateXYZ(rx*(PI/180), ry*(PI/180), 0)

	-- Setup Texture.
	Gu.texImage(lightmap)
	Gu.texFunc(Gu.TFX_MODULATE, Gu.TCC_RGBA)
	Gu.texFilter(Gu.TFL_LINEAR, Gu.TFL_LINEAR)
	Gu.color(object)

	-- Setup light parameters.
	Gu.light(0, Gu.LTYPE_DIRECTIONAL, Gu.LCOM_DIFFUSE, 1, 0, 0)
	Gu.light(1, Gu.LTYPE_DIRECTIONAL, Gu.LCOM_DIFFUSE, 0, 1, 0)

	-- Set the texture-mapping mode.
	Gu.texMapMode(Gu.TMM_ENVIRONMENT_MAP, 0, 1)

	-- First pass: draw the torus
	if App_useTexture then
		Gu.enable(Gu.STATE_TEXTURE_2D)
		vertTorus_v0:draw()
		Gu.disable(Gu.STATE_TEXTURE_2D)
	else
		vertTorus_v0:draw()
	end

	-- Seconds pass: draw the outline
	if App_DrawOutline then
		Gu.frontFace(Gu.FF_CCW)
		Gu.color(outline)
		vertTorus_v1:draw()
	end

	-- Finish the GU and Sync. (This function is the same function as GL.Finish())
	Gu.finish3d()

	-- Clear the z buffer.
	GL.ClearZ()

	-- Print some text using the print function.
	screen.print(0, 0, "Big LuaPlayer: Cel shading example", black)
	screen.print(0, 14, string.format("%s the outline. X to toogle.", App_DrawOutline and "Draw" or "Don't draw"), black)
	screen.print(0, 28, string.format("%s the texture. ^ to toogle.", App_useTexture and "Use" or "Don't use"), black)
	screen.print(0, 42, string.format("Hz: %d - Use analog stick to rotate torus.", 1000/(App_frameTime+0.001)), black)
	screen.print(0, 259, string.format("RunTime: %f", os.clock()), black)

end
-----------------------------------------------------------------------------------------------------

-- Start the application timer.
App_Timer:start()

-----------------------------------------------------------------------------------------------------
-- Application main loop.
while App_Running do
	-- Exit the application on system request.
	if not System.running() then break end

	-- Draw the screen.
	drawFunc()

	-- Swap the buffer.
	GL.Flip()

	-- Update the controls.
	Controls.readPeek()

	if Controls.press(CTRL_ENUM_EXIT) then -- start or circle button pressed.
		-- Break the main loop.
		App_Running = false
	elseif Controls.press(CTRL_ENUM_TOOGLE_TEX) then -- cross button pressed.
		-- Enable or Disable outline rendering.
		App_DrawOutline = not App_DrawOutline
	elseif Controls.press(CTRL_ENUM_TOOGLE_OUT) then -- triangle button pressed.
		-- Enable or Disable the texture.
		App_useTexture = not App_useTexture
	end

	-- Update the rotation angle with the analog stick.
	local tx, ty = Controls.analogx()/127.0, Controls.analogy()/127.0;
	if math.abs(tx) <= 0.2 then tx = 0 else tx = (tx > 0.0) and (tx - 0.5) or (tx + 0.5) end
	if math.abs(ty) <= 0.2 then ty = 0 else ty = (ty > 0.0) and (ty - 0.5) or (ty + 0.5) end
	rx = math.fmod(rx + ( (tx ~= 0 or ty ~= 0) and tx or 0.2), 360)
	ry = math.fmod(ry + ( (tx ~= 0 or ty ~= 0) and ty or 0.4), 360)

	-- Store the frame time.
	App_frameTime = App_Timer:delta()
end
-----------------------------------------------------------------------------------------------------

-- Setup Gu funcion.
initGuFunc = nil

-- Dispose Draw function.
drawFunc = nil
rx, ry = nil, nil

-- Dispose the lightmap texture.
lightmap = nil

-- Dispose the vertirces object.
vertTorus_v0 = nil
vertTorus_v1 = nil

-- Dipose some constant values.
PI = nil
LIGHT_DISTANCE = nil
TORUS_THICKNESS = nil
TORUS_RADIUS = nil
TORUS_ROWS = nil
TORUS_SLICES = nil

-- Dispose colors.
outline = nil
object = nil
clear = nil
black = nil
white = nil

-- Dispose control enumerations.
CTRL_ENUM_TOOGLE_OUT = nil
CTRL_ENUM_TOOGLE_TEX = nil
CTRL_ENUM_EXIT = nil

-- Dispose application data.
App_DrawOutline = nil
App_useTexture = nil
App_Timer = nil
App_Running = nil

-- Quit the application after the main loop breaks.
--System.exit()
