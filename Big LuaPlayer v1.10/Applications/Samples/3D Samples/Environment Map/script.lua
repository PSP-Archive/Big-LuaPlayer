-----------------------------------------------------------------------------------------------------
-- Big LuaPlayer: Environment Map˜ v 1.00 2005 (C) Jesper Svennevid, Renaldas Zioma <rej@scene.lt>
--
-- Description:
--     Sample to demonstrate environment mapping functionality.
--     This samples is base on the beginobject.c sample by Jesper Svennevid, Renaldas Zioma
--
-----------------------------------------------------------------------------------------------------

-- Set the Cpu Clock Frequencies.
Power.cpuSpeed(100)

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
local App_Env = true
local App_Tor = true

-- Define the control enumerations.
local CTRL_ENUM_EXIT = Controls.START + Controls.CIRCLE -- (8 + 8192)
local CTRL_ENUM_TOOGLE_ENV = Controls.CROSS -- (16384)
local CTRL_ENUM_TOOGLE_TOR = Controls.TRIANGLE -- (4096)

-- Define the colors.
local white = Color.new(255, 255, 255)
local black = Color.new(0, 0, 0)
local clear = Color.fromString("0xff554433")
local light = Color.fromString("0xffff4040")
local ambient = Color.fromString("0xff202020")

-- Define some constant values.
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

--load the envmap texture.
local envmap = Texture.load("envmap.png", false, GL.PSM_4444)

-----------------------------------------------------------------------------------------------------
-- Initialize Gu funcion.
local function initGuFunc()
	-- Initialize the GU System for 3D rendering. GL.Start() works too but then the current
	-- Gu settings won't be saved and restored after the 'finish' function.
	Gu.start3d()

	-- Setup GU
	Gu.depthRange(65535, 0)
	Gu.depthFunc(Gu.FNC_GEQUAL)
	Gu.enable(Gu.STATE_DEPTH_TEST)
	Gu.frontFace(Gu.FF_CW)
	Gu.enable(Gu.STATE_CULL_FACE)
	Gu.enable(Gu.STATE_CLIP_PLANES)
	Gu.enable(Gu.STATE_TEXTURE_2D)
	-- enable Gu lights.
	Gu.enable(Gu.STATE_LIGHTING);
	Gu.enable(Gu.STATE_LIGHT0);
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- Draw function.
local valTorus = 0
local valEnvMap = 0
local function drawFunc()
	-- Setup Gu funcion.
	initGuFunc()

	-- Clear the screen and depth buffer.
	Gu.clearColor(clear)
	Gu.clearDepth()
	Gu.clear(Gu.CLBF_COLOR_BUFFER_BIT + Gu.CLBF_DEPTH_BUFFER_BIT)

	-- setup a light.
	Gu.light(0, Gu.LTYPE_DIRECTIONAL, Gu.LCOM_DIFFUSE, 0, 0, 1)
	Gu.lightColor(0, Gu.LCOM_DIFFUSE, light)
	Gu.lightAtt(0, 1.0, 0.0, 0.0)
	Gu.ambient(ambient)

	-- Setup projection matrix. ('The Camara lens')
	Gum.matrixMode(Gum.MTRX_PROJECTION)
	Gum.loadIdentity()
	Gum.perspective(75, 16/9, 1, 1000) -- fovy, aspect, near, far

	-- Setup the view matrix. ('The position and angle of the Camara')
	Gum.matrixMode(Gum.MTRX_VIEW)
	Gum.loadIdentity()

	-- Setup envmap texture.
	Gu.texImage(envmap)
	Gu.texFunc(Gu.TFX_MODULATE, Gu.TCC_RGB)
	Gu.texFilter(Gu.TFL_LINEAR, Gu.TFL_LINEAR)
	Gu.ambientColor(white)

	-- envmap matrix is transposed
	-- since it is passed to GU as columns
	local angle = -2.0 * valEnvMap * (PI/180)
	local cs, sn = math.cos(angle), math.sin(angle)
	Gu.light(2, Gu.LTYPE_DIRECTIONAL, Gu.LCOM_DIFFUSE, cs, sn, 0.0 )
	Gu.light(3, Gu.LTYPE_DIRECTIONAL, Gu.LCOM_DIFFUSE, -sn, cs, 0.0 )

	-- setup envmap texture coord generation
	Gu.texMapMode(
		Gu.TMM_ENVIRONMENT_MAP, -- envmap mode on
		2, -- use 2nd light position as an envmap matrix 1st column
		3  -- use 3rd light position as an envmap matrix 2nd column
	)

	-- Setup model matrix. ('The matrix used to transform the 3D model')
	Gum.matrixMode(Gum.MTRX_MODEL)
	Gum.loadIdentity()
	Gum.translate(0, 0, -2.5) -- The x, y, z position.
	Gum.rotateXYZ(
		math.fmod(valTorus*0.79, 360) * (PI/180),
		math.fmod(valTorus*0.98, 360) * (PI/180),
		math.fmod(valTorus*1.32, 360) * (PI/180)
	)

	-- Set current primitive color.
	Gu.color(white)

	-- draw torus
	vertTorus:draw()

	-- Finish the GU and Sync. (This function is the same function as GL.Finish())
	Gu.finish3d()

	-- Clear the z buffer.
	GL.ClearZ()

	-- Print some text using the print function.
	screen.print(0, 0, "Big LuaPlayer: Object Drawing", white)
	screen.print(0, 14, string.format("%s environment map. X to toogle.", App_Env and "Rotate" or "Don't rotate"), white)
	screen.print(0, 28, string.format("%s torus. ^ to toogle.", App_Tor and "Rotate" or "Don't rotate"), white)
	screen.print(0, 42, string.format("Hz: %d", 1000/(App_frameTime+0.001)), white)
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
	elseif Controls.press(CTRL_ENUM_TOOGLE_ENV) then -- cross button pressed.
		-- Enable or Disable environment map rotation.
		App_Env = not App_Env
	elseif Controls.press(CTRL_ENUM_TOOGLE_TOR) then -- triangle button pressed.
		-- Enable or Disable torus rotation.
		App_Tor = not App_Tor
	end

	-- Update the environment map and torus rotations
	valEnvMap = App_Env and math.fmod(valEnvMap + 1, 360) or valEnvMap
	valTorus = App_Tor and math.fmod(valTorus + 1, 252000) or valTorus

	-- Store the frame time.
	App_frameTime = App_Timer:delta()
end
-----------------------------------------------------------------------------------------------------

-- Setup Gu funcion.
initGuFunc = nil

-- Dispose Draw function.
drawFunc = nil
valEnvMap = nil
valTorus = nil

-- Dipose some constant values.
PI = nil
LIGHT_DISTANCE = nil
TORUS_THICKNESS = nil
TORUS_RADIUS = nil
TORUS_ROWS = nil
TORUS_SLICES = nil

-- Dispose the envmap texture.
envmap = nil

-- Dispose the vertirces object.
vertTorus = nil

-- Dispose colors.
ambient = nil
light = nil
clear = nil
black = nil
white = nil

-- Dispose control enumerations.
CTRL_ENUM_TOOGLE_TOR = nil
CTRL_ENUM_TOOGLE_ENV = nil
CTRL_ENUM_EXIT = nil

-- Dispose application data.
App_Tor = nil
App_Env = nil
App_Timer = nil
App_Running = nil

-- Dispose the geometry module.
geometry = geometry.dispose()

-- Quit the application after the main loop breaks.
--System.exit()
