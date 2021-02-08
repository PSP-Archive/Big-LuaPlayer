-----------------------------------------------------------------------------------------------------
-- Big LuaPlayer: Render to target Sample˜ v 1.00 year (C) company
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
local ivory = GL.clr.ivory:clone()

-- Create a empty texture, and store it in the vram.
-- INFO: Render targets must reside in VRAM!
local tex = Texture.new(128, 128, true)
-- Clear the Texture using the texture clear function.
tex:clear()

-----------------------------------------------------------------------------------------------------
-- Define the drawStar function.
local function drawStar(clr, x, y, in_rad, out_rad, step, angle)
	-- Convert angle in degrees to radians.
	angle = math.rad(angle)
	-- double the step size.
	step = math.floor(step) * (in_rad ~= out_rad and 2 or 1)
	-- Begin triangle rendering.
	GL.BeginTriangles(GL.FAN)
	-- Set the object properties.
	GL.SetCoordXY(x, y)
	GL.SetColor(clr)
	-- Add the first object to the object list.
	GL.Add()
	-- Calculate all object positions.
	local size = math.pi*2/step
	for n = 0, step do
		-- Add the next point to the object list.
		GL.AddPoint(
			math.sin(angle+n*size) * (math.fmod(n, 2) == 0 and in_rad or out_rad) + x,
			math.cos(angle+n*size) * (math.fmod(n, 2) == 0 and in_rad or out_rad) + y
		)
	end
	-- End triangle rendering.
	GL.End()
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- Draw function.
local angle = 0
local star = {
	n = {64,64,8,8,8,0,GL.clr.black:clone(),0,0,0,0,0,0,0,0},
	o = {64,64,8,8,8,0,GL.clr.black:clone()}
}
math.randomseed(os.clock())
local function drawFunc()

	-- Set the texture as the render target (instead of the screen)
	if not GL.SetDrawTarget(tex) then
		error("Failed to set texture as target.")
	end

	-- Clear the texture using the GL Clear function.
	GL.Clear(ivory)

	-- Draw the Star on the texture.
	drawStar(star.o[7], table.unpack(star.o,1,6))

	-- Reset the Draw Target.
	if not GL.ResetDrawTarget() then
		error("Failed to set screen as target.")
	end

	-- Clear the screen.
	GL.Clear(black)

	-- Print some text using the print function.
	screen.print(0, 0, "Big LuaPlayer: Render to target Sample", white)
	screen.print(0, 14, string.format("Hz: %d", 1000/(App_frameTime+0.001)), white)
	screen.print(0, 259, string.format("RunTime: %f", os.clock()), white)

	-- Draw the Texture.
	tex:Begin()
	GL.SetCoordMode(GL.CENTER)
	GL.SetRotation(angle)
	tex:Add(240, 136)
	tex:End()

	-- Update the angle.
	angle = angle < 359.5 and angle + 0.5 or 0.0
	-- Update the Star.
	local step = 0
	for n = 1, 7 do
		if n ~= 7 then
			step = (star.o[n] < star.n[n] and 1 or (star.o[n] > star.n[n] and -1 or 0))
			if step ~= 0 then
				if n ~= 5 then
					star.o[n] = star.o[n] + step
				elseif star.n[15] == 0 then
					star.o[n] = star.o[n] + step
					star.n[15] = 32
				else
					star.n[15] = star.n[15] - 1
				end
			elseif star.n[n+7] == 0 then
				if n < 3 then -- Set the new x or y position.
					star.n[n] = math.random(0, 127)
				elseif n < 5 then -- Set the inner or outer rad size.
					star.n[n] = math.random(8, 32)
				elseif n == 5 then -- Set the step size.
					star.n[n] = math.random(2, 8)
				elseif n == 6 then -- Set the new rotation angle.
					star.n[n] = math.random(0, 359)
				end
				star.n[n+7] = math.random(16, 160)
			else
				star.n[n+7] = star.n[n+7] - 1
			end
		elseif star.o[n] == star.n[n] then -- Set the new color value.
			if star.n[n+7] == 0 then
				star.n[n] = Color.new(math.random(0, 255), math.random(0, 255), math.random(0, 255))
				star.n[n+7] = math.random(16, 160)
			else
				star.n[n+7] = star.n[n+7] - 1
			end
		else
			-- Update the color.
			local c0, c1, f0, f1 = star.o[n], star.n[n], 0, 0
			f0, f1 = c0:red(), c1:red()
			c0:red(f0 + (f0 < f1 and 1 or (f0 > f1 and -1 or 0)))
			f0, f1 = c0:green(), c1:green()
			c0:green(f0 + (f0 < f1 and 1 or (f0 > f1 and -1 or 0)))
			f0, f1 = c0:blue(), c1:blue()
			c0:blue(f0 + (f0 < f1 and 1 or (f0 > f1 and -1 or 0)))
			c0,c1,f0,f1 = nil,nil,nil,nil
		end
	end

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

-- Dispose Draw function.
star = nil
angle = nil
drawFunc = nil
drawStar = nil

-- Dispose the Texture.
tex = nil

-- Dispose colors.
ivory = nil
black = nil
white = nil

-- Dispose control enumerations.
CTRL_ENUM_EXIT = nil

-- Dispose application data.
App_Timer = nil
App_Running = nil

-- Quit the application after the main loop breaks.
--System.exit()
