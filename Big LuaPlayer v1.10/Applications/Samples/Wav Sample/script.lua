-----------------------------------------------------------------------------------------------------
-- Big LuaPlayer: Wav Sample˜ v 1.00 year (C) company
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

local CTRL_ENUM_STOPALL = Controls.SELECT -- (1)
local CTRL_ENUM_STOP = Controls.LTRIGGER -- (256)
local CTRL_ENUM_LOOP = Controls.RTRIGGER -- (512)

local CTRL_ENUM_WAV_1 = Controls.CROSS -- (16384)
local CTRL_ENUM_WAV_2 = Controls.TRIANGLE -- (4096)


-- Define the colors.
local white = Color.new(255, 255, 255)
local black = Color.new(0, 0, 0)

-- Load the bmpFont.
local fntVerdana14 = BmpFont.load(System.Directory.eboot.."/System/Font/verdana.ttf", 14)

-- Define audio properties.
local loop1 = false
local loop2 = false

-- Load the Wav's.
local sixteenbitmono = Sound.load("16bitmono.wav")
local sixteenbitstereo = Sound.load("16bitstereo.wav")

-- Initalize two sound channel.
Sound.init(2)

-----------------------------------------------------------------------------------------------------
-- Draw function.
local function drawFunc()

	-- Clear the screen.
	GL.Clear(black)

	-- Print some text using the print function.
	screen.print(0, 0, "Big LuaPlayer: Wav Sample", white)
	screen.print(0, 14, string.format("Hz: %d", 1000/(App_frameTime+0.001)), white)
	screen.print(0, 259, string.format("RunTime: %f", os.clock()), white)

	fntVerdana14:print(20, 30, "Press 'CROSS' to play 16-bit mono sample.", white)
	fntVerdana14:print(20, 45, "Press 'TRIANGLE' to play 16-bit stereo sample.", white)

	fntVerdana14:print(20, 90, "Press 'LTRIGGER' and a play button to stop playing the sample.", white)
	fntVerdana14:print(20, 105, "Press 'RTRIGGER' and a play button to toggle the loop.", white)
	fntVerdana14:print(20, 120, "Press 'SELECT' to stop playing all samples.", white)

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

	if Controls.press(CTRL_ENUM_EXIT) then -- start button pressed.
		-- Break the main loop.
		App_Running = false
    end

	if Controls.buttons(CTRL_ENUM_STOPALL) then -- select button pressed.
	    -- Stop Playing all Wav's.
		Sound.stopAll()
	elseif Controls.buttons(CTRL_ENUM_STOP) then -- l trigger held.
		-- stop playing 8 a wav sample.
		if Controls.press(CTRL_ENUM_WAV_1) then -- cross button pressed.
			sixteenbitmono:stop()
		elseif Controls.press(CTRL_ENUM_WAV_2) then -- triangle button pressed.
			sixteenbitstereo:stop()
		end
	elseif Controls.buttons(CTRL_ENUM_LOOP) then -- r trigger held.
		-- Enable or disable loop.
		if Controls.press(CTRL_ENUM_WAV_1) then -- cross button pressed.
			loop1 = not loop1
			sixteenbitmono:loop(loop1)
		elseif Controls.press(CTRL_ENUM_WAV_2) then -- triangle button pressed.
			loop2 = not loop2
			sixteenbitstereo:loop(loop2)
		end
	elseif Controls.press(CTRL_ENUM_WAV_1) then -- cross button pressed.
	   -- play 16 bit mono sample.
		sixteenbitmono:play(true, loop1)
	elseif Controls.press(CTRL_ENUM_WAV_2) then -- triangle button pressed.
		-- play 16 bit stereo sample.
		sixteenbitstereo:play(true, loop2)
    end

	-- Store the frame time.
	App_frameTime = App_Timer:delta()
end
-----------------------------------------------------------------------------------------------------

-- Dispose Draw function.
drawFunc = nil

-- Terminate the sound channels.
Sound.term()

-- Dipose the Wav's.
sixteenbitstereo = nil
sixteenbitmono = nil

-- Dispose audio properties.
loop2 = nil
loop1 = nil

-- Dispose the bmpFont.
fntVerdana14 = nil

-- Dispose colors.
black = nil
white = nil

-- Dispose control enumerations.
CTRL_ENUM_WAV_2 = nil
CTRL_ENUM_WAV_1 = nil
CTRL_ENUM_LOOP = nil
CTRL_ENUM_STOP = nil
CTRL_ENUM_STOPALL = nil
CTRL_ENUM_EXIT = nil

-- Dispose application data.
App_Timer = nil
App_Running = nil

-- Quit the application after the main loop breaks.
--System.exit()
