-----------------------------------------------------------------------------------------------------
-- Big LuaPlayer: LED Timer˜ v 1.00 2013 (C) M4MSOFT
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
local CTRL_ENUM_EXIT = Controls.START -- (8)
local CTRL_ENUM_RESET = Controls.CIRCLE -- (8192)
local CTRL_ENUM_START = Controls.CROSS -- (16384)

-- Define the colors.
local white = Color.new(255, 255, 255)
local black = Color.new(0, 0, 0)
local green = Color.new(0, 255, 0)

-- Create a bmpFont object from a texture file.
LEDfont = BmpFont.fromImage(System.Directory.eboot.."/System/BmpFont/Ledfonts.png", 19, 32, ".:/0123456789")

-- Get the current size of the glyph list.
local slot = LEDfont:len()

-- Add the spacebar glyph table to the glyph list.
LEDfont:glyph(slot, {0, 0, 0, 0, 0, 0, 19, 0})
-- Link the spacebar character to a glyph slot.
LEDfont:charmap(" ", slot)

-- Dipose the used value.
slot = nil

-- Create the Timer
local timer = Timer.new()
local intTimestate = 1
local intTime = 0

-----------------------------------------------------------------------------------------------------
-- @func printcaption: print the text.
--  @param x: postion as integer
--  @param y: postion as integer
--  @param text: text as string
--  @param clr: text color as color
--  @param f: factor as single
-- @returns
--   @param nil
function BmpFont.printcaption(self,x,y,text,clr,f)
	local text = text or ""
	local i,iMax = 1, string.len(text)
	local slot,tGlyph,tex = 0,false,self:texture()
	local f = f or 1
	x = x + self:offsetX() * f -- draw x start position
	y = y + self:offsetY() * f-- draw y start position
	tex:Begin(false) -- Begin texture rendering.
	GL.SetColor(clr) -- Set the Text Color.
	for i = 1, iMax do
		slot = self:charmap(string.sub(text, i, i)) -- Get the glyph slot index.
		if slot > 0 then
			tGlyph = self:glyph(slot) -- Get the glyph.
			if tGlyph[3] > 0 and tGlyph[4] > 0 then  -- Check the width and height.
				tex:Add(                          -- Add the character to the object list.
					x + tGlyph[5] * f, y + tGlyph[6] * f, -- draw x and y position
					tGlyph[3] * f, tGlyph[4] * f, -- draw width and height
					tGlyph[1], tGlyph[2],         -- source x and y position
					tGlyph[3], tGlyph[4]          -- source width and height
				)
			end
			x = x + tGlyph[7] * f -- advance x
			y = y + tGlyph[8] * f -- advance y
		end
	end
	tex:End() -- End texture rendering.
	text = nil
	i,iMax = nil,nil,nil
	slot,tGlyph,tex = nil,nil,nil
	f = nil
end
-- Add the printcaption function to the BmpFont metateble.
local bfntMeta = getmetatable(BmpFont.object)
bfntMeta.printcaption = BmpFont.printcaption
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- Draw function.
local intDrawCount = 0
local strSeperator = ":"
local function drawFunc()

	-- Clear the screen.
	GL.Clear(black)

	-- Print some text using the print function.
	screen.print(0, 0, "Big LuaPlayer: Timer Application", white)
	screen.print(0, 14, string.format("Hz: %d", 1000/(App_frameTime+0.001)), white)
	screen.print(42, 179+10, "Press Cross: Start/Stop, Press Circle: Reset", green)
	screen.print(0, 259, string.format("RunTime: %f", os.clock()), white)

	-- Draw the Timer box.
	screen.drawRect(39, 93, 401, 85, green)

	-- Get the current time.
	intTime = timer:time()

	-- Switch the Seperator if the timer is running.
	if intDrawCount >= 25 then
		strSeperator = strSeperator == ":" and " " or ":"
		intDrawCount = 0
	elseif intTimestate == 1 then
		intDrawCount = intDrawCount + 1
	elseif strSeperator ~= ":" or intDrawCount ~= 0 then
		strSeperator = ":"
		intDrawCount = 0
	end

	-- Print the time using the printcaption function.
	LEDfont:printcaption(50, 104, string.format(
		"%02d"..strSeperator.."%02d"..strSeperator.."%02d",
		select(2, math.modf(intTime / 1000 / 3600000)) * 24,
		select(2, math.modf(intTime / 1000 /    3600)) * 60,
		select(2, math.modf(intTime / 1000 /      60)) * 60
		), green, 2)

	-- Print the time using the print function.
	LEDfont:print(354, 104+32, string.format(
		strSeperator.."%03d",
		select(2, math.modf(intTime / 1000)) * 1000
		), green)

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

	if Controls.press(CTRL_ENUM_EXIT) then --start button pressed.
        --Break the main loop.
        App_Running = false
	elseif Controls.press(CTRL_ENUM_RESET) then --circle button pressed.
		--Reset the timer
		timer:reset()
		intTimestate = 0
	elseif Controls.press(CTRL_ENUM_START) then --cross button pressed.
		--Start/Stop the timer.
		if intTimestate == 0 then
			timer:start()
			intTimestate = 1
		else
			timer:stop()
			intTimestate = 0
		end
    end

	-- Store the frame time.
	App_frameTime = App_Timer:delta()
end
-----------------------------------------------------------------------------------------------------

-- Dispose Draw function.
drawFunc = nil

-- Dispose the printcaption function.
bfntMeta.printcaption = nil
BmpFont.printcaption = nil
bfntMeta = nil

-- Dispose the Timer
intTime = nil
intTimestate = nil
timer = nil

-- Dispose the BmpFont object.
LEDfont = nil

-- Dispose colors.
green = nil
black = nil
white = nil

-- Dispose control enumerations.
CTRL_ENUM_EXIT = nil

-- Dispose application data.
App_Timer = nil
App_Running = nil

-- Quit the application after the main loop breaks.
--System.exit()
