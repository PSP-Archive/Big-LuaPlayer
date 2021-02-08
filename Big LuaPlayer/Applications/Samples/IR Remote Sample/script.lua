-----------------------------------------------------------------------------------------------------
-- Big LuaPlayer: IR Remote Sample˜ v 1.00 year (C) company
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
local CTRL_ENUM_LEFT = Controls.LEFT + Controls.LTRIGGER -- (128 + 256)
local CTRL_ENUM_RIGHT = Controls.RIGHT + Controls.RTRIGGER -- (32 + 512)

-- Define the colors.
local white = Color.new(255, 255, 255)
local black = Color.new(0, 0, 0)
local gold = Color.new( 255, 216, 0)

-- Load the bmpFont with a restricted charset. ("PRaelusty")
-- INFO: Less characters in the charset means less memory waist.
local fntVerdana16 = BmpFont.load(System.Directory.eboot.."/System/Font/verdana.ttf", 30, 0, false, "PRaelusty")
local rectFont = {x = 0, y = 0, w = 0, h = fntVerdana16:height()-1}

-- Create a bmpFont object.
local bfntRemote = BmpFont.new()
-- Add the font texture to the BmpFont object.
bfntRemote:texture(Texture.load("remote.png"))
-- Add the Glyphs to the BmpFont object. {x, y, w, h, x0, y0, adv_x, adv_y}
bfntRemote:glyph(1, {  0, 0, 60, 60, 0, 0, 60, 0})
bfntRemote:glyph(2, { 60, 0, 60, 60, 0, 0, 60, 0})
bfntRemote:glyph(3, {120, 0, 60, 60, 0, 0, 60, 0})
-- Link the characters to a glyph slot.
bfntRemote:charmap("1", 1)
bfntRemote:charmap("2", 2)
bfntRemote:charmap("3", 3)
-- Set the height of the BmpFont object.
bfntRemote:height(60)

-- Define the ir command values.
local IR_TYPE      = 0x0014 -- (20)
local IR_DEV_DVD   = 0x1b5a -- (7259)
local IR_CMD_RESET = 0x0015 -- (21)
local IR_CMD_PLAY  = 0x0032 -- (50)
local IR_CMD_PAUSE = 0x0039 -- (57)
local IR_COUNT     = 0x0014 -- (20)

local intIrResult = 0
local intIrCmd = 2
local tblIrCmd = {IR_CMD_RESET, IR_CMD_PLAY, IR_CMD_PAUSE, "Reset", "Play", "Pause"}
local strIrCmd = tblIrCmd[3+intIrCmd]

-----------------------------------------------------------------------------------------------------
-- Center the font rectangle.
local function center_font()
	rectFont.w = fntVerdana16:measureText(strIrCmd).width
	rectFont.y = math.floor((65 - rectFont.h) / 2 + 197)
	rectFont.x = math.floor((480 - rectFont.w) / 2)
end
center_font()
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- Draw function.
local function drawFunc()

	-- Clear the screen.
	GL.Clear(black)

	-- Print some text using the print function.
	screen.print(0, 0, "Big LuaPlayer: IR Remote Sample", white)
	screen.print(0, 14, string.format("Hz: %d", 1000/(App_frameTime+0.001)), white)
	screen.print(0, 28, "This example can be used with a PS2", white)
	screen.print(0, 259, string.format("RunTime: %f IR: %d", os.clock(), intIrResult), white)

	-- Print the Remote buttons.
	for idx = 1, 3 do
		bfntRemote:print(-60+135*idx, 137, idx, idx == intIrCmd and gold or white)
	end

	-- Print the nmae of the selected Remote button.
	fntVerdana16:print(rectFont.x, rectFont.y, strIrCmd, gold)

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
	elseif Controls.buttons(CTRL_ENUM_LEFT) then -- left button or trigger held.
		-- Select the next IR remote command.
		intIrCmd = intIrCmd < 2 and 3 or intIrCmd - 1
		strIrCmd = tblIrCmd[3+intIrCmd]
		center_font()
		System.sleep(120)
	elseif Controls.buttons(CTRL_ENUM_RIGHT) then -- right button or trigger held.
		-- Select the next IR remote command.
		intIrCmd = intIrCmd > 2 and 1 or intIrCmd + 1
		strIrCmd = tblIrCmd[3+intIrCmd]
		center_font()
		System.sleep(120)
	elseif Controls.buttons(CTRL_ENUM_CONFIRM) then -- cross button held.
		-- Send the selected IR remote command.
		intIrResult = System.irSendCmd(IR_TYPE, IR_DEV_DVD, tblIrCmd[intIrCmd], IR_COUNT)
		System.sleep(90)
    end

	-- Store the frame time.
	App_frameTime = App_Timer:delta()
end
-----------------------------------------------------------------------------------------------------

-- Dispose Draw function.
drawFunc = nil

-- Dispose the font center function.
center_font = nil

-- Dispose the ir command values.
strIrCmd = nil
tblIrCmd = nil
intIrCmd = nil
blnIrResult = nil

IR_COUNT = nil
IR_CMD_PAUSE = nil
IR_CMD_PLAY = nil
IR_CMD_RESET = nil
IR_DEV_DVD = nil
IR_TYPE = nil

-- Dispose the remote bmpFont.
bfntRemote = nil

-- Dispose the bmpFont.
rectFont = nil
fntVerdana16 = nil

-- Dispose colors.
gold = nil
black = nil
white = nil

-- Dispose control enumerations.
CTRL_ENUM_EXIT = nil
CTRL_ENUM_RIGHT = nil
CTRL_ENUM_LEFT = nil
CTRL_ENUM_CONFIRM = nil

-- Dispose application data.
App_Timer = nil
App_Running = nil

-- Quit the application after the main loop breaks.
--System.exit()
