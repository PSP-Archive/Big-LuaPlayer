-----------------------------------------------------------------------------------------------------
-- Big LuaPlayer: BitmapFont Sample˜ v 1.00 year (C) company
-----------------------------------------------------------------------------------------------------

-- Set the Cpu Clock Frequencies.
Power.cpuSpeed(100)

-- load the screen module.
require "screen"

-- load the table_ext module.
table_ext = require "table_ext"

-- Define the application data.
local App_Running = true
local App_frameTime = 1000/60 -- (~16.67 ms)
local App_Timer = Timer.new()

-- Define the control enumerations.
local CTRL_ENUM_EXIT = Controls.START + Controls.CIRCLE -- (8 + 8192)

-- Define the colors.
local white = Color.new(255, 255, 255)
local white50 = Color.new(255, 255, 255, 127)
local white25 = Color.new(255, 255, 255, 64)
local black = Color.new(0, 0, 0)
local red = Color.new(255, 0, 0)
local green = Color.new(0, 255, 0)
local blue = Color.new(0, 0, 255)
local darkblue = Color.new(0, 0, 16)

-- Create a bmpFont object.
local MyBmpFont = BmpFont.new()

-- Add the font texture to the BmpFont object.
MyBmpFont:texture(Texture.load(System.Directory.eboot.."/System/BmpFont/font.png"))

-- Load the Glyphs table.
local tblGlyphs = table.load(System.Directory.eboot.."/System/BmpFont/font.txt")

local tmp = ""

-- Add the Glyphs table to the BmpFont object.
local slot = 255
for c, glyph in ipairs(tblGlyphs) do
	-- Add the glyph to the BmpFont object.
	-- INFO: This function wiil change the value of 'slot' and adds a character
	--       to the BmpFont object, if 'slot' is greater or equal to 'MyBmpFont:len()'.
	MyBmpFont:glyph(slot, glyph) -- (range: 0 - MyBmpFont:len())
	-- Link the character to a glyph slot.
	-- INFO: This function wiil change the value of 'slot' if 'slot' is greater
	--       or equal to 'MyBmpFont:len()'. This function can link multiple
	--       character to one glyph slot.
	MyBmpFont:charmap(string.char(c), slot)
end
slot = nil

-- Dispose the Glyphs table.
tblGlyphs = nil

-----------------------------------------------------------------------------------------------------
-- Draw function.
local function drawFunc()

	-- Clear the screen.
	GL.Clear(darkblue)

	-- Print some text using the print function.
	screen.print(0, 0, "Big LuaPlayer: BitmapFont Sample", white)
	screen.print(0, 14, string.format("Hz: %d", 1000/(App_frameTime+0.001)), white)
	screen.print(0, 259, string.format("RunTime: %f", os.clock()), white)

	-- Print some text using the BmpFont print function.
	MyBmpFont:print(14, 36, "Hello World in red", red)
	MyBmpFont:print(14, 52, "Hello World in green", green)
	MyBmpFont:print(14, 68, "Hello World in blue",  blue)

	MyBmpFont:print(14, 100, "Hello World with free char width", white, 0)
	MyBmpFont:print(14, 116, "Hello World with block char width 10", white, 10)
	MyBmpFont:print(14, 132, "Hello World with block char width 12", white, 12)

	MyBmpFont:print(14, 164, "Hello World with opacity 100%", white)
	MyBmpFont:print(14, 180, "Hello World with opacity  50%", white50)
	MyBmpFont:print(14, 196, "Hello World with opacity  25%", white25)

	MyBmpFont:print(17, 231, "Hello World with shadow", white50)
	MyBmpFont:print(14, 228, "Hello World with shadow", white)


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

-- Dispose the bmpFont.
MyBmpFont = nil

-- Dispose colors.
darkblue = nil
blue = nil
green = nil
red = nil
black = nil
white25 = nil
white50 = nil
white = nil

-- Dispose control enumerations.
CTRL_ENUM_EXIT = nil

-- Dispose application data.
App_Timer = nil
App_Running = nil

-- Dipose the table_ext module.
table_ext = table_ext.dispose()

-- Quit the application after the main loop breaks.
--System.exit()
