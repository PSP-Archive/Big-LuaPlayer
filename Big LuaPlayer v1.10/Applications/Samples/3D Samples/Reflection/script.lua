-----------------------------------------------------------------------------------------------------
-- Big LuaPlayer: Reflection example˜ v 1.00 2005 (C) McZonk
--
-- Description:
--     This samples is base on the reflection.c sample by McZonk
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

-- Define the control enumerations.
local CTRL_ENUM_EXIT = Controls.START + Controls.CIRCLE -- (8 + 8192)

-- Define the colors.
local white = Color.new(255, 255, 255)
local black = Color.new(0, 0, 0)
local clear = Color.fromString("0xff554433")

-- Define some constant values.
local PI = Gu.PI

-- Define the Cube vertices object.
local colors = {
	Color.fromString("0xff6666ff"), -- light red
	Color.fromString("0xff66ff66"), -- light green
	Color.fromString("0xffff6666"), -- light blue
}
local vertCube = Gum.verticesNew(
	Gum.PRIM_TRIANGLES,
	Gum.VERT_TEXTURE_BITS + Gum.VERT_COLOR_BITS + Gum.VERT_VERTEX_BITS + Gum.VERT_TRANSFORM_3D,
	{
		{0, 0, colors[1],-1,-1, 1}, {1, 0, colors[1], 1,-1, 1}, {1, 1, colors[1], 1, 1, 1},
		{0, 0, colors[1],-1,-1, 1}, {1, 1, colors[1], 1, 1, 1}, {0, 1, colors[1],-1, 1, 1},

		{1, 1, colors[2], 1, 1, 1}, {0, 1, colors[2], 1,-1, 1}, {0, 0, colors[2], 1,-1,-1},
		{1, 1, colors[2], 1, 1, 1}, {0, 0, colors[2], 1,-1,-1}, {1, 0, colors[2], 1, 1,-1},

		{0, 1, colors[3],-1, 1, 1}, {0, 0, colors[3], 1, 1, 1}, {1, 0, colors[3], 1, 1,-1},
		{0, 1, colors[3],-1, 1, 1}, {1, 0, colors[3], 1, 1,-1}, {1, 1, colors[3],-1, 1,-1},

		{1, 1, colors[1], 1,-1,-1}, {0, 1, colors[1],-1,-1,-1}, {0, 0, colors[1],-1, 1,-1},
		{1, 1, colors[1], 1,-1,-1}, {0, 0, colors[1],-1, 1,-1}, {1, 0, colors[1], 1, 1,-1},

		{1, 0, colors[2],-1,-1,-1}, {1, 1, colors[2],-1,-1, 1}, {0, 1, colors[2],-1, 1, 1},
		{1, 0, colors[2],-1,-1,-1}, {0, 1, colors[2],-1, 1, 1}, {0, 0, colors[2],-1, 1,-1},

		{0, 1, colors[3],-1,-1,-1}, {0, 0, colors[3], 1,-1,-1}, {1, 0, colors[3], 1,-1, 1},
		{0, 1, colors[3],-1,-1,-1}, {1, 0, colors[3], 1,-1, 1}, {1, 1, colors[3],-1,-1, 1},
	}
)
colors = nil

-- Define the mirror vertices object.
local colors = {Color.fromString("0xaa000000")} -- alpha black
local vertMirror = Gum.verticesNew(
	Gum.PRIM_TRIANGLES,
	Gum.VERT_TEXTURE_BITS + Gum.VERT_COLOR_BITS + Gum.VERT_VERTEX_BITS + Gum.VERT_TRANSFORM_3D,
	{
		{0, 0, colors[1], -2,  0, -2}, {2, 2, colors[1],  2,  0,  2}, {2, 0, colors[1],  2,  0, -2},
		{0, 0, colors[1], -2,  0, -2}, {0, 2, colors[1], -2,  0,  2}, {2, 2, colors[1],  2,  0,  2}
	}
)
colors = nil

-- Define the border vertices object.
local colors = {Color.fromString("0xff0055aa")}
local vertBorder = Gum.verticesNew(
	Gum.PRIM_TRIANGLES,
	Gum.VERT_TEXTURE_BITS + Gum.VERT_COLOR_BITS + Gum.VERT_VERTEX_BITS + Gum.VERT_TRANSFORM_3D,
	{
		{0, 0, colors[1], -2.125, -0.01, -2.125},
		{2, 2, colors[1],  2.125, -0.01,  2.125},
		{2, 0, colors[1],  2.125, -0.01, -2.125},

		{0, 0, colors[1], -2.125, -0.01, -2.125},
		{0, 2, colors[1], -2.125, -0.01,  2.125},
		{2, 2, colors[1],  2.125, -0.01,  2.125}
	}
)
colors = nil

--load the logo texture.
local logo = Texture.load("logo.png", false, GL.PSM_4444)

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
	Gu.frontFace(Gu.FF_CCW)
	Gu.enable(Gu.STATE_CULL_FACE)
	Gu.enable(Gu.STATE_CLIP_PLANES)
	Gu.disable(Gu.STATE_TEXTURE_2D)
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- Draw function.
local function drawFunc()
	local t = os.clock()*45
	local move = math.abs(math.sin(t*(PI/180)))+1
	local rot = t*(PI/180)

	-- Setup Gu funcion.
	initGuFunc()

	-- Clear the screen and depth buffer.
	Gu.clearColor(clear)
	Gu.clearDepth()
	Gu.clearStencil()
	Gu.clear(Gu.CLBF_COLOR_BUFFER_BIT + Gu.CLBF_STENCIL_BUFFER_BIT + Gu.CLBF_DEPTH_BUFFER_BIT)

	-- Setup projection matrix. ('The Camara lens')
	Gum.matrixMode(Gum.MTRX_PROJECTION)
	Gum.loadIdentity()
	Gum.perspective(60, 16/9, 0.5, 1000) -- fovy, aspect, near, far

	-- Setup the view matrix. ('The position and angle of the Camara')
	Gum.matrixMode(Gum.MTRX_VIEW)
	Gum.loadIdentity()
	Gum.translate(0, -0.5, -5.5)
	Gum.rotateXYZ(30*(PI/180), t*(PI/180), 0)

	-- Setup model matrix. ('The matrix used to transform the 3D model')
	Gum.matrixMode(Gum.MTRX_MODEL)
	Gum.loadIdentity()

	-- Draw the Mirror view.
	--{
		Gum.pushMatrix()
		Gum.scale(1, -1, 1)

		-- Draw Mirror.
		Gu.frontFace(Gu.FF_CCW)
		Gu.enable(Gu.STATE_STENCIL_TEST) -- Enable Stencil test
		Gu.depthMask(true)
		Gu.stencilFunc(Gu.FNC_ALWAYS, 1, 1) -- always set 1 bit in 1 bit mask
		Gu.stencilOp(Gu.STENCIL_KEEP, Gu.STENCIL_KEEP, Gu.STENCIL_REPLACE) -- keep value on failed test (fail and zfail) and replace on pass
		vertMirror:draw()
		Gu.depthMask(false)

		-- Setup Texture.
		Gu.enable(Gu.STATE_TEXTURE_2D)
		Gu.texImage(logo)
		Gu.texFunc(Gu.TFX_ADD, Gu.TCC_RGB)
		Gu.texFilter(Gu.TFL_LINEAR, Gu.TFL_LINEAR)
		Gu.texScale(1, 1)
		Gu.texOffset(0, 0)
		Gu.frontFace(Gu.FF_CW)
		Gu.stencilFunc(Gu.FNC_EQUAL, 1, 1); -- allow drawing where stencil is 1
		Gu.stencilOp(Gu.STENCIL_KEEP, Gu.STENCIL_KEEP, Gu.STENCIL_KEEP); -- keep the stencil buffer unchanged

		-- Setup model matrix for cube. ('The matrix used to transform the 3D model')
		Gum.translate(0, move, 0)
		Gum.rotateXYZ(0, rot*-0.83, 0)

		-- Draw Cube
		vertCube:draw()

		Gu.disable(Gu.STATE_TEXTURE_2D)
		Gu.disable(Gu.STATE_STENCIL_TEST)

		Gum.popMatrix()
	--}

	-- Draw normal view
	--{
		-- Setup Texture.
		Gu.enable(Gu.STATE_TEXTURE_2D)
		--Gu.texImage(logo)
		Gu.texFunc(Gu.TFX_REPLACE, Gu.TCC_RGBA)
		--Gu.texFilter(Gu.TFL_LINEAR, Gu.TFL_LINEAR)
		--Gu.texScale(1, 1)
		--Gu.texOffset(0, 0)
		Gu.frontFace(Gu.FF_CCW)
		Gu.enable(Gu.STATE_BLEND)
		Gu.blendFunc(Gu.OP_ADD, Gu.BLD_SRC_ALPHA, Gu.BLD_ONE_MINUS_SRC_ALPHA, 0, 0)

		-- Draw the mirror.
		vertMirror:draw()

		Gu.disable(Gu.STATE_BLEND)
		Gu.disable(Gu.STATE_TEXTURE_2D)

		-- Draw the border.
		vertBorder:draw()

		-- Setup Texture.
		Gu.enable(Gu.STATE_TEXTURE_2D)
		--Gu.texImage(logo)
		Gu.texFunc(Gu.TFX_ADD, Gu.TCC_RGB)
		--Gu.texFilter(Gu.TFL_LINEAR, Gu.TFL_LINEAR)
		--Gu.texScale(1, 1)
		--Gu.texOffset(0, 0)

		-- Setup model matrix for cube. ('The matrix used to transform the 3D model')
		Gum.translate(0, move, 0)
		Gum.rotateXYZ(0, rot*-0.83, 0)

		-- Draw Cube
		vertCube:draw()

		Gu.disable(Gu.STATE_TEXTURE_2D)
	--}

	-- Finish the GU and Sync. (This function is the same function as GL.Finish())
	Gu.finish3d()

	-- Clear the z buffer.
	GL.ClearZ()

	-- Print some text using the print function.
	screen.print(0, 0, "Big LuaPlayer: Reflection example", white)
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

	-- Store the frame time.
	App_frameTime = App_Timer:delta()
end
-----------------------------------------------------------------------------------------------------

-- Setup Gu funcion.
initGuFunc = nil

-- Dispose Draw function.
drawFunc = nil

-- Dispose the logo texture.
logo = nil

-- Dispose the vertirces object.
vertBorder = nil
vertMirror = nil
vertCube = nil

-- Dipose some constant values.
PI = nil

-- Dispose colors.
clear = nil
black = nil
white = nil

-- Dispose control enumerations.
CTRL_ENUM_EXIT = nil

-- Dispose application data.
App_Timer = nil
App_Running = nil

-- Quit the application after the main loop breaks.
--System.exit()
