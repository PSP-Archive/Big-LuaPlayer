-----------------------------------------------------------------------------------------------------
-- Big LuaPlayer: Shadow Projection example˜ v 1.00 2005 (C) Jesper Svennevid Renaldas Zioma <rej@scene.lt>
--
-- Description:
--     shadowmap.c - Sample to demonstrate projective texture mapping
--     used for shadowmap implementation
--     This samples is base on the shadowmap.c sample by Jesper Svennevid Renaldas Zioma
--
--     This sample isn't working properly at the moment. Couldn't rewite all of the matrix functions
--     of the original file to lua scripting code.
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

-- Define the control enumerations.
local CTRL_ENUM_EXIT = Controls.START + Controls.CIRCLE -- (8 + 8192)

-- Define the colors.
local white = Color.new(255, 255, 255)
local gray = Color.new(96, 96, 96)
local black = Color.new(0, 0, 0)
local clear = Color.fromString("0xff554433")
local ambient = Color.fromString("0xff202020")
local light = Color.fromString("0xffff4040")
local grid = Color.fromString("0xffff7777")

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

-- Define the shadowmap.
local shadowmap = Texture.new(128, 128, true, GL.PSM_4444)

-----------------------------------------------------------------------------------------------------
--Gu xyz Move function.
local Gu_XYZ_Movement = function(XrYrZr, MovementXYZ)
	local value = {x = 0, y = 0, z = 0}
	local X,Y,Z = 0,0,0

	X = (XrYrZr.y / 180 * Gu.PI)
	Z = (XrYrZr.y / 180 * Gu.PI)
	Y = (XrYrZr.x / 180 * Gu.PI)

	--Beweging op de Z as
	value.x = math.sin(-X) * MovementXYZ.z * math.cos(Y)
	value.y = math.sin(Y) * MovementXYZ.z
	value.z = math.cos(-Z) * MovementXYZ.z * math.cos(Y)

	--Beweging op de Y as
	value.x = -math.sin(-X) * MovementXYZ.y * math.sin(Y) + value.x
	value.y = math.cos(-Y) * MovementXYZ.y + value.y
	value.z = -math.cos(-Z) * MovementXYZ.y * math.sin(Y) + value.z

	Y = (XrYrZr.z / 180 * Gu.PI)

	--Beweging op de X as
	value.x = math.cos(X) * MovementXYZ.x * math.cos(Y) + value.x
	value.y = math.sin(-Y) * MovementXYZ.x + value.y
	value.z = math.sin(Z) * MovementXYZ.x * math.cos(Y) + value.z

	X,Y,Z = nil,nil,nil
	XrYrZr = nil
	MovementXYZ = nil
	return value
end
-----------------------------------------------------------------------------------------------------

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
-- render to shadow map
local val = 0
local function drawShadow()
	-- Setup Gu funcion.
	initGuFunc()

	-- Set the texture as the render target (instead of the screen)
	if GL.SetDrawTarget(shadowmap) then
		-- Clear the screen and depth buffer.
		Gu.clearColor(white)
		Gu.clearDepth()
		Gu.clear(Gu.CLBF_COLOR_BUFFER_BIT + Gu.CLBF_DEPTH_BUFFER_BIT)

		-- Setup Light projection matrix.
		Gum.matrixMode(Gum.MTRX_PROJECTION)
		Gum.loadIdentity()
		Gum.perspective(75, 1, 0.1, 1000) -- fovy, aspect, near, far

		-- Setup the Light view matrix.
		Gum.matrixMode(Gum.MTRX_VIEW)
		Gum.loadIdentity()
		Gum.translate(0, 0.5, 0)
		Gum.rotateXYZ(0, (val-180)*0.79*(PI/180), 0)
		Gum.rotateXYZ(-1*(PI/180)*60, 0, 0)
		Gum.translate(0, 0, LIGHT_DISTANCE)
		Gum.fastInverse()

		-- Setup model matrix for torus. ('The matrix used to transform the 3D model')
		Gum.matrixMode(Gum.MTRX_MODEL)
		Gum.loadIdentity()
		Gum.translate(0, 0.5, 0)
		Gum.rotateXYZ(
			math.fmod(val*0.79, 360) * (PI/180),
			math.fmod(val*0.98, 360) * (PI/180),
			math.fmod(val*1.32, 360) * (PI/180)
		)

		-- shadow casters are drawn in black
		-- disable lighting and texturing
		Gu.disable(Gu.STATE_LIGHTING)
		Gu.disable(Gu.STATE_TEXTURE_2D)

		-- draw torus to shadow map
		Gu.color(gray)
		vertTorus:draw()

		-- Reset the Draw Target.
		GL.ResetDrawTarget()
	else
		error("Failed to set texture as target.")
	end

	-- Finish the GU and Sync. (This function is the same function as GL.Finish())
	Gu.finish3d()
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- Draw function.
local function drawFunc()
	-- Draw the shadow map texture.
	drawShadow()

	-- Setup Gu funcion.
	initGuFunc()

	-- clear screen
	Gu.clearColor(clear)
	Gu.clearDepth()
	Gu.clear(Gu.CLBF_COLOR_BUFFER_BIT + Gu.CLBF_DEPTH_BUFFER_BIT)

	-- Setup projection matrix. ('The Camara lens')
	Gum.matrixMode(Gum.MTRX_PROJECTION)
	Gum.loadIdentity()
	Gum.perspective(75, 16/9, 0.5, 1000) -- fovy, aspect, near, far

	-- Setup the view matrix. ('The position and angle of the Camara')
	Gum.matrixMode(Gum.MTRX_VIEW)
	Gum.loadIdentity()
	Gum.translate(0, 0, -5)

	-- setup the orbiting light
	local x,y,z = 0, 0, LIGHT_DISTANCE
	local xr,yr,zr = 60, val*0.79, 0
	local tbl = Gu_XYZ_Movement({x = xr, y = yr, z = zr},{x = x, y = y, z = z})

	Gu.light(0, Gu.LTYPE_SPOTLIGHT, Gu.LCOM_DIFFUSE, 0+tbl.x, 0.5+tbl.y, 0+tbl.z)
	Gu.lightSpot(0, -1*xr*(PI/180), yr*(PI/180), zr, 5.0, 0.6)
	Gu.lightColor(0, Gu.LCOM_DIFFUSE, light)
	Gu.lightAtt(0, 1.0, 0.0, 0.0)
	Gu.ambient(ambient)
	Gu.enable(Gu.STATE_LIGHTING)
	Gu.enable(Gu.STATE_LIGHT0)

	-- Setup model matrix for torus. ('The matrix used to transform the 3D model')
	Gum.matrixMode(Gum.MTRX_MODEL)
	Gum.loadIdentity()
	Gum.translate(0, 0.5, 0)
	Gum.rotateXYZ(
		math.fmod(val*0.79, 360) * (PI/180),
		math.fmod(val*0.98, 360) * (PI/180),
		math.fmod(val*1.32, 360) * (PI/180)
	)

	-- draw torus
	Gu.color(white)
	vertTorus:draw()

	-- setup texture projection
	Gu.texMapMode(Gu.TMM_TEXTURE_MATRIX, 0, 0)
	Gu.texProjMapMode(Gu.TPM_POSITION)

	-- Setup shadowmap texture.
	Gu.texImage(shadowmap, 0, 0)
	Gu.texFunc(Gu.TFX_MODULATE, Gu.TCC_RGB)
	Gu.texFilter(Gu.TFL_LINEAR, Gu.TFL_LINEAR)
	Gu.enable(Gu.STATE_TEXTURE_2D)

	-- Setup model matrix for grid. ('The matrix used to transform the 3D model')
	Gum.matrixMode(Gum.MTRX_MODEL)
	Gum.loadIdentity()
	Gum.translate(0, -1.5, 0)

	-- Setup projection/view/mode texture matrix for texture.
	Gum.matrixMode(Gum.MTRX_TEXTURE)
	Gum.loadIdentity()
	Gum.perspective(75, 1, 0, 1000) -- fovy, aspect, near, far
	Gum.translate(0, 0.5, 0)
	Gum.rotateXYZ(0, (val-180)*0.79*(PI/180), 0)
	Gum.rotateXYZ(-1*(PI/180)*60, 0, 0)
	Gum.translate(0, 0, LIGHT_DISTANCE)
	Gum.translate(0.5, -0.5, 0.0)
	Gum.translate(0.5, -0.5, 0)
	Gum.scale(0.5, 0.5, 0)
	Gum.translate(0, -1.5, 0)
	Gum.fullInverse()

	-- Draw the grid.
	Gu.color(grid)
	vertGrid:draw()

	-- Finish the GU and Sync. (This function is the same function as GL.Finish())
	Gu.finish3d()

	-- Clear the z buffer.
	GL.ClearZ()

	-- Print some text using the print function.
	screen.print(0, 0, "Big LuaPlayer: Shadow Projection example example", white)
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

-- Dispose Draw function.
drawFunc = nil
drawShadow = nil
val = nil
Gu_XYZ_Movement = nil

-- Setup Gu funcion.
initGuFunc = nil

-- Dispose the shadowmap texture.
shadowmap = nil

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
grid = nil
light = nil
ambient = nil
clear = nil
black = nil
gray = nil
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
