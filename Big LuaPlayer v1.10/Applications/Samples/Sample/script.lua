-----------------------------------------------------------------------------------------------------
-- Big LuaPlayer: New Application˜ v 1.00 year (C) company
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

-----------------------------------------------------------------------------------------------------
-- Draw function.
local function drawFunc()

	-- Clear the screen.
	GL.Clear(black)

	-- Print some text using the print function.
	screen.print(0, 0, "Big LuaPlayer: New Application", white)
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

-- Dispose Draw function.
drawFunc = nil

-- Dispose colors.
black = nil
white = nil

-- Dispose control enumerations.
CTRL_ENUM_EXIT = nil

-- Dispose application data.
App_Timer = nil
App_Running = nil

-- Quit the application after the main loop breaks.
--System.exit()
