-----------------------------------------------------------------------------------------------------
-- Big LuaPlayer: Object Drawing˜ v 1.00 year (C) company
--
-- Description:
--     This Application should had showed how to draw some 3D objects. But it
--     draw just a funny 3D rotating shape. (?)
--
-----------------------------------------------------------------------------------------------------

-- Set the Cpu Clock Frequencies.
Power.cpuSpeed(100)

-- load the screen module.
require "screen"

-- Define the application data.
local App_Running = true
local App_frameTime = 1000/60 -- (~16.67 ms)
local App_Timer = Timer.new()
local App_Flags = false

-- Define the control enumerations.
local CTRL_ENUM_EXIT = Controls.START + Controls.CIRCLE -- (8 + 8192)
local CTRL_ENUM_TOOGLE = Controls.CROSS -- (16384)

-- Define the colors.
local white = Color.new(255, 255, 255)
local black = Color.new(0, 0, 0)
local colors = {
	Color.fromString("0xffff0000"), -- blue
	Color.fromString("0xffffff00"), -- cyan
	Color.fromString("0xff00ff00"), -- green
	Color.fromString("0xff00ffff"), -- yellow
	Color.fromString("0xff0000ff"), -- red
	Color.fromString("0xffff00ff"), -- magenta
}

-- Define some constant values.
local TORUS_SLICES    = 24 -- numc
local TORUS_ROWS      = 24 -- numt
local TORUS_RADIUS    = 1.0
local TORUS_THICKNESS = 0.5
local LIGHT_DISTANCE  = 3.0
local PI              = (Gu.PI)

-- Generate Torus */
local tblTorus_v0 = {}
for j = 0, TORUS_SLICES-1 do
	for i = 0, TORUS_ROWS-1 do
		local n = i+j*TORUS_ROWS
		local vert = {}
		local s, t = i+0.5, j
		local cs,ct,ss,st = 0, 0, 0, 0

		cs = math.cos(s*(2*PI)/TORUS_SLICES)
		ct = math.cos(t*(2*PI)/TORUS_ROWS)
		ss = math.sin(s*(2*PI)/TORUS_SLICES)
		st = math.sin(t*(2*PI)/TORUS_ROWS)

		vert[1] = (TORUS_RADIUS + TORUS_THICKNESS * cs) * ct
		vert[2] = (TORUS_RADIUS + TORUS_THICKNESS * cs) * st
		vert[3] = TORUS_THICKNESS * ss

		tblTorus_v0[n+1] = vert
	end
end
-- Define the Torus_v0 vertices object.
local vertTorus_v0 = Gum.verticesNew(
	Gum.PRIM_TRIANGLES,
	Gum.VERT_VERTEX_BITS + Gum.VERT_TRANSFORM_3D,
	tblTorus_v0
)
tblTorus_v0 = nil

-- Define the BBox vertices object.
local vertBBox = Gum.verticesNew(
	Gum.PRIM_LINES,
	Gum.VERT_VERTEX_BITS + Gum.VERT_TRANSFORM_3D,
	{
		{ -1.0, -1.0, -1.0 }, { -1.0, -1.0,  1.0 },
		{ -1.0,  1.0, -1.0 }, { -1.0,  1.0,  1.0 },
		{  1.0, -1.0, -1.0 }, {  1.0, -1.0,  1.0 },
		{  1.0,  1.0, -1.0 }, {  1.0,  1.0,  1.0 }
	}
)

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
local function drawFunc()
	-- Setup Gu funcion.
	initGuFunc()

	-- Clear the screen and depth buffer.
	Gu.clearColor()
	Gu.clearDepth()
	Gu.clear(Gu.CLBF_COLOR_BUFFER_BIT + Gu.CLBF_DEPTH_BUFFER_BIT)

	-- Setup projection matrix. ('The Camara lens')
	Gum.matrixMode(Gum.MTRX_PROJECTION)
	Gum.loadIdentity()
	Gum.perspective(90, 16/9, 0.1, 100) -- fovy, aspect, near, far

	-- Setup the view matrix. ('The position and angle of the Camara')
	Gum.matrixMode(Gum.MTRX_VIEW)
	Gum.loadIdentity()
	Gum.translate(0, 0, -0.5);

	-- Setup model matrix. ('The matrix used to transform the 3D model')
	Gum.matrixMode(Gum.MTRX_MODEL)
	Gum.loadIdentity()

	-- Begin drawing the 3D models.
	local sngTime = os.clock()/1.25
	for n = 1, 12 do
		local r = n/12 * PI * 2 + sngTime

		-- rotate and translate the current matrix.
		Gum.rotateXYZ(0, r, 0)
		Gum.translate(0, 0, n/12 * 0.50)

		-- Draw Bounding Box
		if App_Flags then
			Gu.color(white) -- Set current primitive color.
			vertBBox:draw() -- render the 'vertBBox' vertices list.
		end

		-- rotate the current matrix.
		Gum.rotateXYZ(n/12 * sngTime * 0.5324, n/12 * sngTime * 1.2211, 0)
		-- Set current primitive color.
		Gu.color(colors[math.fmod(n,6)+1])
		-- render the 'vertTorus_v0' vertices list.
		vertTorus_v0:draw()

		-- Reset matrix.
		Gum.loadIdentity()
	end

	-- Finish the GU and Sync. (This function is the same function as GL.Finish())
	Gu.finish3d()

	-- Clear the z buffer.
	GL.ClearZ()

	-- Print some text using the print function.
	screen.print(0, 0, "Big LuaPlayer: Object Drawing", white)
	screen.print(0, 14, string.format("%s Bounding Box. X to toogle.", App_Flags and "Draw" or "Don't draw"), white)
	screen.print(0, 28, string.format("Hz: %d", 1000/(App_frameTime+0.001)), white)
	screen.print(0, 259, string.format("RunTime: %f", os.clock()), white)

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
	elseif Controls.press(CTRL_ENUM_TOOGLE) then -- cross button pressed.
		-- Enable or Disable Bounding Box.
		App_Flags = not App_Flags
	end

	-- Store the frame time.
	App_frameTime = App_Timer:delta()
end
-----------------------------------------------------------------------------------------------------

-- Setup Gu funcion.
initGuFunc = nil

-- Dispose Draw function.
drawFunc = nil

-- Dipose some constant values.
PI = nil
LIGHT_DISTANCE = nil
TORUS_THICKNESS = nil
TORUS_RADIUS = nil
TORUS_ROWS = nil
TORUS_SLICES = nil

-- Dispose the vertirces object.
vertTorus_v0 = nil
vertBBox = nil

-- Dispose colors.
colors = nil
black = nil
white = nil

-- Dispose control enumerations.
CTRL_ENUM_EXIT = nil

-- Dispose application data.
App_Flags = nil
App_Timer = nil
App_Running = nil

-- Quit the application after the main loop breaks.
--System.exit()
