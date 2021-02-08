-----------------------------------------------------------------------------------------------------
-- Big LuaPlayer: DebugScreen sample� v 1.00 year (C) company
-----------------------------------------------------------------------------------------------------

-- Set the Cpu Clock Frequencies.
Power.cpuSpeed(19)

-- Define the application data.
local App_Running = true

-- Define the control enumerations.
local CTRL_ENUM_EXIT = Controls.START + Controls.CIRCLE -- (8 + 8192)
local CTRL_ENUM_CONFIRM = Controls.CROSS -- (16384)

-- Define the colors.
local white = Color.new(255, 255, 255)
local darkBlue = Color.new(0, 0, 64)
local yellow = Color.new(255, 255, 0)

-----------------------------------------------------------------------------------------------------
-- pause function.
local function pause()
	while App_Running do
		-- Exit the application on system request.
		if not System.running() then break end

		-- Update the controls.
		Controls.read()

		if Controls.press(CTRL_ENUM_EXIT) then -- start or circle button pressed.
			-- Break the main loop.
			App_Running = false
		elseif Controls.press(CTRL_ENUM_CONFIRM) then -- cross button pressed.
			-- Break the pause loop.
			break
		end

	end
	if not System.running() then App_Running = false end
	return App_Running
end
-----------------------------------------------------------------------------------------------------

-- Initalize the DebugScreen.
DebugScreen.init()

-- Set The back and text color of the DebugScreen.
DebugScreen.backColor(darkBlue)
DebugScreen.textColor(white)

-- Clear the debug screen.
DebugScreen.clear()

-- Print some text using the printf function.
DebugScreen.printf(
	"Big LuaPlayer: DebugScreen sample",
	string.format("RunTime: %f", os.clock())
)

-- Set the current print postion.
DebugScreen.position(0, 3)

-- Print some text using the printf function.
DebugScreen.printf("'Hello' is printed on the screen with the debug print function.")
DebugScreen.printf("Press the start or circle button to exit the application.")

-- Print some text using the print function.
DebugScreen.print(224, 132, "hello", yellow)

-- Pause the application.
pause()

-- Dispose pause function.
pause = nil

-- Dispose colors.
yellow = nil
darkBlue = nil
white = nil

-- Dispose control enumerations.
CTRL_ENUM_EXIT = nil
CTRL_ENUM_CONFIRM = nil

-- Dispose application data.
App_Running = nil

-- Quit the application after the main loop breaks
--System.exit()
