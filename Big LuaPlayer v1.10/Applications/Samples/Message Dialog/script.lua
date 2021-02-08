-----------------------------------------------------------------------------------------------------
-- Big LuaPlayer: Message Dialog Sample˜ v 1.00 year (C) company
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
local CTRL_ENUM_CONFIRM = Controls.CROSS -- (16384)

-- Define the colors.
local white = Color.new(255, 255, 255)
local black = Color.new(0, 0, 0)
local red =  Color.new(255, 0, 0)

-- Load the bmpFont.
local fntVerdana16 = BmpFont.load(System.Directory.eboot.."/System/Font/verdana.ttf", 16)
local rectFont = {x = 0, y = 0, w = 0, h = fntVerdana16:height()-1}

-- Define message dialog properties.
local enabled = false
local pressed = 0
local exittext = "Press the 'cross' button to show the Message Dialog."

-----------------------------------------------------------------------------------------------------
-- Center the font rectangle.
local function center_font()
	rectFont.w = fntVerdana16:measureText(exittext).width
	rectFont.y = math.floor((272 - rectFont.h) / 2)
	rectFont.x = math.floor((480 - rectFont.w) / 2)
end
center_font()
-----------------------------------------------------------------------------------------------------

-- Define some draw data.
local counter = 0
local rect = {x = 190, y = 106, w = 100, h = 60}

-----------------------------------------------------------------------------------------------------
-- Draw function.
local function drawFunc()

	-- Clear the screen.
	GL.Clear(black)

	-- Draw a rectangle.
	screen.fillRect(rect.x, rect.y, rect.w, rect.h, red)

	-- Print some text using the print function.
	screen.print(0, 0, "Big LuaPlayer: Message Dialog Sample", white)
	screen.print(0, 14, string.format("Hz: %d", 1000/(App_frameTime+0.001)), white)
	screen.print(0, 259, string.format("RunTime: %f", os.clock()), white)

	-- If message dialog not shown draw some text.
	if not enabled then
		fntVerdana16:print(rectFont.x, rectFont.y, exittext, white)
	end

	-- Increment counter.
	counter = counter + 1

	-- Set the rectangle to a random screen position.
	if counter >= 60 then
		counter = 0
		rect.x = math.random(0, 480-rect.w)
		rect.y = math.random(0, 272-rect.h)
	end

	-- Finish the GU and Sync.
	GL.Finish();

	if enabled then
		-- Utility.msgUpdate() must be called after GL.Finish(), but before GL.Flip().
		pressed = Utility.msgUpdate()

		if pressed == Utility.MSG_RESULT_YES then
			exittext = "User selected yes."
			center_font()
			enabled = false
			CTRL_ENUM_EXIT = Controls.START + Controls.CIRCLE
			-- Show the Message Dialog using the show function.
			Utility.msgShow(exittext, 0, drawFunc)
		elseif pressed == Utility.MSG_RESULT_NO then
			exittext = "User selected no."
			center_font()
			enabled = false
			CTRL_ENUM_EXIT = Controls.START + Controls.CIRCLE
			-- Show the Message Dialog using the show function.
			Utility.msgShow(exittext, 0, drawFunc)
		elseif pressed == Utility.MSG_RESULT_BACK then
			exittext = "User cancelled."
			center_font()
			enabled = false
			CTRL_ENUM_EXIT = Controls.START + Controls.CIRCLE
			-- Show the Message Dialog using the show function.
			Utility.msgShow(exittext, 0, drawFunc)
		elseif pressed ~= Utility.DIALOG_RUNNING then
			exittext = "Internal error."
			center_font()
			enabled = false
			CTRL_ENUM_EXIT = Controls.START + Controls.CIRCLE
			-- Show the Message Dialog using the show function.
			Utility.msgShow(exittext, 0, drawFunc)
		end
	end

	-- Store the frame time.
	App_frameTime = App_Timer:delta()
end
-----------------------------------------------------------------------------------------------------

-- Start the application timer.
App_Timer:start()

-- Set seed for the pseudo-random generator.
math.randomseed(os.clock())

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
	elseif Controls.press(CTRL_ENUM_CONFIRM) and not enabled then -- cross button pressed.
		-- Init the message dialog and check it was successful. Add yes and no buttons and set the highlighted button to 'No'.
		Utility.msgInit("Select a option", Utility.MSG_BTN_YESMO + Utility.MSG_DEFAULT_BTN_NO)
		enabled = true
		CTRL_ENUM_EXIT = Controls.START
    end

end
-----------------------------------------------------------------------------------------------------

-- Abort the Message dialog.
Utility.msgAbort()

-- Dispose Draw function.
drawFunc = nil
rect = nil
counter = nil

-- Dispose the font center function.
center_font = nil

-- Dispose the message dialog properties.
exittext = nil
pressed = nil
enabled = nil

-- Dispose the bmpFont.
rectFont = nil
fntVerdana16 = nil

-- Dispose colors.
red = nil
black = nil
white = nil

-- Dispose control enumerations.
CTRL_ENUM_CONFIRM = nil
CTRL_ENUM_EXIT = nil

-- Dispose application data.
App_Timer = nil
App_frameTime = nil
App_Running = nil

-- Quit the application after the main loop breaks.
--System.exit()
