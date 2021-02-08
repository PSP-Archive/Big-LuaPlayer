-----------------------------------------------------------------------------------------------------
-- Big LuaPlayer: Ortho example˜ v 1.00 2006 (C) David Perry (Insert_witty_name)
--
-- Description:
--     This samples is base on the ortho.c sample by David Perry (Insert_witty_name)
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
local App_useTexture = true
local App_DrawOutline = false

-- Define the control enumerations.
local CTRL_ENUM_EXIT = Controls.START + Controls.CIRCLE -- (8 + 8192)
local CTRL_ENUM_UP = Controls.UP -- (16)
local CTRL_ENUM_DOWN = Controls.DOWN -- (64)

-- Define the colors.
local white = Color.new(255, 255, 255)
local black = Color.new(0, 0, 0)

-- Define some constant values.
local PI              = (Gu.PI)

-- Define the basis vertices object.
local vertBasic = Gum.verticesNew(
	Gum.PRIM_TRIANGLES,
	Gum.VERT_COLOR_BITS + Gum.VERT_VERTEX_BITS + Gum.VERT_TRANSFORM_3D,
	{
		{Color.fromString("0xFF0000FF"),  0.0,-50.0, 0.0},
		{Color.fromString("0xFF00FF00"), 50.0, 50.0, 0.0},
		{Color.fromString("0xFFFF0000"),-50.0, 50.0, 0.0}
	}
)

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
	Gu.disable(Gu.STATE_TEXTURE_2D)
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- Draw function.
local pos = {240, 136, 0}
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
	Gum.ortho(0, 480, 272, 0, -1, 1) -- left, right, bottum, top, near, far

	-- Setup the view matrix. ('The position and angle of the Camara')
	Gum.matrixMode(Gum.MTRX_VIEW)
	Gum.loadIdentity()

	-- Setup model matrix. ('The matrix used to transform the 3D model')
	Gum.matrixMode(Gum.MTRX_MODEL)
	Gum.loadIdentity()
	Gum.translate(table.unpack(pos))
	Gum.rotateXYZ(0, 0, os.clock()/1.25)

	-- Draw triangle
	vertBasic:draw()

	-- Finish the GU and Sync. (This function is the same function as GL.Finish())
	Gu.finish3d()

	-- Clear the z buffer.
	GL.ClearZ()

	-- Print some text using the print function.
	screen.print(0, 0, "Big LuaPlayer: Ortho example", white)
	screen.print(0, 14, string.format("x: %.2f y: %.2f z: %.2f", table.unpack(pos)), white)
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
	end

	if Controls.buttons(CTRL_ENUM_UP) then -- up button pressed.
		pos[3] = pos[3] + 1/100
	end
	if Controls.buttons(CTRL_ENUM_DOWN) then -- down button pressed.
		pos[3] = pos[3] - 1/100
	end

	if math.abs(Controls.analogx()) > 64 then -- Use analogx
		pos[1] = pos[1] + Controls.analogx() / 128
	end
	if math.abs(Controls.analogy()) > 64 then -- Use analogy
		pos[2] = pos[2] + Controls.analogy() / 128
	end

	-- Store the frame time.
	App_frameTime = App_Timer:delta()
end
-----------------------------------------------------------------------------------------------------

-- Setup Gu funcion.
initGuFunc = nil

-- Dispose Draw function.
drawFunc = nil
pos = nil

-- Dispose the vertirces object.
vertBasic = nil

-- Dispose colors.
black = nil
white = nil

-- Dispose control enumerations.
CTRL_ENUM_DOWN = nil
CTRL_ENUM_UP = nil
CTRL_ENUM_EXIT = nil

-- Dispose application data.
App_Timer = nil
App_Running = nil

-- Quit the application after the main loop breaks.
--System.exit()
