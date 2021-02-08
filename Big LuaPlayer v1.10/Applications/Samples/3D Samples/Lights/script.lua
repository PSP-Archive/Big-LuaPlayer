-----------------------------------------------------------------------------------------------------
-- Big LuaPlayer: lights example˜ v 1.00 2005 (C) Jesper Svennevid
--
-- Description:
--     This samples is base on the lights.c sample by Jesper Svennevid
--
-----------------------------------------------------------------------------------------------------

-- Set the Cpu Clock Frequencies.
Power.cpuSpeed(333)

-- Render the load screen.
GL.Clear()
screen.print(0, 259, "Loading ...", GL.clr.white)
GL.Flip()

-- load the screen module.
require "screen"

-- load the geometry module.
geometry = require "geometry"

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
local clear = Color.fromString("0xff554433")
local ambient = Color.fromString("0xff222222")
local object = Color.fromString("0xffff7777")
local colors = {
	Color.fromString("0xffff0000"), -- blue
	Color.fromString("0xff00ff00"), -- green
	Color.fromString("0xff0000ff"), -- red
	Color.fromString("0xffff00ff")  -- magenta
}

-- Define some constant values.
local GRID_COLUMNS    = 32
local GRID_ROWS       = 32
local GRID_SIZE       = 10
local TORUS_SLICES    = 48 -- numc
local TORUS_ROWS      = 48 -- numt
local TORUS_RADIUS    = 1.0
local TORUS_THICKNESS = 0.5
local LIGHT_DISTANCE  = 3.0
local PI              = (Gu.PI)

-- Define and generate the Torus vertices object.
local vertTorus = Gum.verticesNew(
	Gum.PRIM_TRIANGLES,
	Gum.VERT_NORMAL_BITS + Gum.VERT_VERTEX_BITS + Gum.VERT_TRANSFORM_3D,
	geometry.generateTorusNP(TORUS_ROWS, TORUS_SLICES, TORUS_RADIUS, TORUS_THICKNESS)
)

-- Define and generate the grid vertices object.
local vertGrid = Gum.verticesNew(
	Gum.PRIM_TRIANGLES,
	Gum.VERT_NORMAL_BITS + Gum.VERT_VERTEX_BITS + Gum.VERT_TRANSFORM_3D,
	geometry.generateGridNP(GRID_COLUMNS, GRID_ROWS, GRID_SIZE, GRID_SIZE)
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
	-- enable Gu lights.
	Gu.enable(Gu.STATE_LIGHTING);
	Gu.enable(Gu.STATE_LIGHT0);
	Gu.enable(Gu.STATE_LIGHT1);
	Gu.enable(Gu.STATE_LIGHT2);
	Gu.enable(Gu.STATE_LIGHT3);
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- Draw function.
local val = 0
local function drawFunc()
	-- Setup Gu funcion.
	initGuFunc()

	-- Clear the screen and depth buffer.
	Gu.clearColor(clear)
	Gu.clearDepth()
	Gu.clear(Gu.CLBF_COLOR_BUFFER_BIT + Gu.CLBF_DEPTH_BUFFER_BIT)

	-- setup a light
	local d = val*(PI/180)*LIGHT_DISTANCE
	for i = 0, 3 do
		Gu.light(i, Gu.LTYPE_POINTLIGHT, Gu.LCOM_DIFFUSE_AND_SPECULAR, math.cos(i*(PI/2)+d), 0, math.sin(i*(PI/2)+d))
		Gu.lightColor(i, Gu.LCOM_DIFFUSE, colors[i+1])
		Gu.lightColor(i, Gu.LCOM_SPECULAR, white)
		Gu.lightAtt(i, 0.0, 1.0, 0.0)
	end
	Gu.specular(12.0)
	Gu.ambient(ambient)

	-- Setup projection matrix. ('The Camara lens')
	Gum.matrixMode(Gum.MTRX_PROJECTION)
	Gum.loadIdentity()
	Gum.perspective(75, 16/9, 1, 1000) -- fovy, aspect, near, far

	-- Setup the view matrix. ('The position and angle of the Camara')
	Gum.matrixMode(Gum.MTRX_VIEW)
	Gum.loadIdentity()
	Gum.translate(0, 0, -3.5) -- The x, y, z position.

	-- Setup model matrix for grid. ('The matrix used to transform the 3D model')
	Gum.matrixMode(Gum.MTRX_MODEL)
	Gum.loadIdentity()
	Gum.translate(0, -1.5, 0) -- The x, y, z position.

	-- Draw the Grid.
	Gu.color(object)
	vertGrid:draw()

	-- Setup model matrix for torus. ('The matrix used to transform the 3D model')
	Gum.matrixMode(Gum.MTRX_MODEL)
	Gum.loadIdentity()
	Gum.rotateXYZ(
		math.fmod(val*0.79, 360) * (PI/180),
		math.fmod(val*0.98, 360) * (PI/180),
		math.fmod(val*1.32, 360) * (PI/180)
	)

	-- draw Torus.
	Gu.color(white)
	vertTorus:draw()

	-- Finish the GU and Sync. (This function is the same function as GL.Finish())
	Gu.finish3d()

	-- Clear the z buffer.
	GL.ClearZ()

	-- Print some text using the print function.
	screen.print(0, 0, "Big LuaPlayer: lights example", white)
	screen.print(0, 14, string.format("Hz: %d", 1000/(App_frameTime+0.001)), white)
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
	end

	-- Update the rotation value
	val = math.fmod(val + 1, 252000)

	-- Store the frame time.
	App_frameTime = App_Timer:delta()
end
-----------------------------------------------------------------------------------------------------

-- Setup Gu funcion.
initGuFunc = nil

-- Dispose Draw function.
drawFunc = nil
val = nil

-- Dispose the vertirces object.
vertGrid = nil
vertTorus = nil

-- Dispose some constant values.
PI = nil
LIGHT_DISTANCE = nil
TORUS_THICKNESS = nil
TORUS_RADIUS = nil
TORUS_ROWS = nil
TORUS_SLICES = nil
GRID_SIZE = nil
GRID_ROWS = nil
GRID_COLUMNS = nil

-- Dispose colors.
colors = nil
object = nil
ambient = nil
clear = nil
black = nil
white = nil

-- Dispose control enumerations.
CTRL_ENUM_EXIT = nil

-- Dispose application data.
App_Timer = nil
App_Running = nil

-- Dispose the geometry module.
geometry = geometry.dispose()

-- Quit the application after the main loop breaks.
--System.exit()
