-----------------------------------------------------------------------------------------------------
-- Big LuaPlayer: Palette Sample˜ v 1.00 year (C) company
-----------------------------------------------------------------------------------------------------

-- Set the Cpu Clock Frequencies.
Power.cpuSpeed(100)

-- Define the application data.
local App_Running = true
local App_frameTime = 1000/60 -- (~16.67 ms)
local App_Timer = Timer.new()

-- Define the control enumerations.
local CTRL_ENUM_EXIT = Controls.START + Controls.CIRCLE -- (8 + 8192)
local CTRL_ENUM_SWAP = Controls.CROSS -- (16384)
local CTRL_ENUM_SORT = Controls.TRIANGLE -- (4096)
local CTRL_ENUM_SQUARE = Controls.SQUARE -- (32768)

-- Define the colors.
local white = Color.new(255, 255, 255)
local black = Color.new(0, 0, 0)

-- Load the unswizzled 32 bit texture.
local texVolcarona32 = Texture.load("volcarona.png", false, GL.PSM_8888, false)
-- build the convert palette.
local pltVolcarona = Palette.fromImage(texVolcarona32, 16)
-- Create a 4 bit texture.
local texVolcarona4 = Texture.new(texVolcarona32:width(), texVolcarona32:height(), false, GL.PSM_T4)
-- Link the convert palette to the texture.
texVolcarona4:palette(pltVolcarona)
-- Convert the 32 bit texture to a 4 bit texture using the blit function.
texVolcarona4:blit(0, 0, texVolcarona32)
-- Dispose the 32 bit texture
texVolcarona32 = nil
-- Load the last saved palette from a file.
pltVolcarona = Palette.load("volcarona.plt") or pltVolcarona
-- Link the loaded palette to the texture.
texVolcarona4:palette(pltVolcarona)

-----------------------------------------------------------------------------------------------------
-- Swap the color in the palette
math.randomseed(os.clock())
function Palette.swap(self, count)
	local clr = false
	local i1,i2 = 1, 1
	for n = 1, count do
		i1, i2 = math.random(1, self:len()-1), math.random(1, self:len()-1)
		clr = self:color(i1)
		self:color(i1, self:color(i2))
		self:color(i2, clr)
	end
end
-- Add the swap function to the palette metateble.
local pltMeta = getmetatable(Palette.object)
pltMeta.swap = Palette.swap
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- Draw function.
local function drawFunc ()

	-- Clear the screen
	GL.Clear(black)

	-- Print some text using the print function.
	screen.print(0, 0, "Big LuaPlayer: Palette Sample", white)
	screen.print(0, 14, string.format("Hz: %d", 1000/(App_frameTime+0.001)), white)
	screen.print(0, 28, "Press the 'cross' button to swap the colors", white)
	screen.print(0, 42, "Press the 'triangle' button to sort the colors", white)
	screen.print(0, 259, string.format("RunTime: %f", os.clock()), white)

	-- Draw the texture.
	texVolcarona4:Begin(false)
	Texture.Add(142, 54, 192, 192)
	Texture.End()

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

	-- swap the buffer.
	GL.Flip()

	-- Update the controls.
	Controls.readPeek()

	if Controls.press(CTRL_ENUM_EXIT) then -- start or circle button pressed.
		-- Break the main loop.
		App_Running = false
	elseif Controls.press(CTRL_ENUM_SQUARE) then -- square button pressed.
		if texVolcarona4:palette() then
			-- Remove the Palette.
			texVolcarona4:palette(nil)
		else
		    -- Link the palette to the texture.
			texVolcarona4:palette(pltVolcarona)
		end
	elseif Controls.press(CTRL_ENUM_SWAP) then -- cross button pressed.
		-- Swap the colors in the palette.
		pltVolcarona:swap(math.random(1, 3))
	elseif Controls.press(CTRL_ENUM_SORT) then -- triangle button pressed.
		-- Sort the colors in the palette.
		pltVolcarona:sort()
    end

	-- Store the frame time.
	App_frameTime = App_Timer:delta()
end
-----------------------------------------------------------------------------------------------------

-- Dispose the swap function.
pltMeta.swap = nil
Palette.swap = nil
pltMeta = nil

-- Dispose Draw function.
drawFunc = nil

-- Save the palette to a file.
pltVolcarona:save("volcarona.plt")

--Dispose the palette and texture
pltVolcarona = nil
texVolcarona32 = nil

-- Dispose colors.
black = nil
white = nil

-- Dispose control enumerations.
CTRL_ENUM_SORT = nil
CTRL_ENUM_SWAP = nil
CTRL_ENUM_EXIT = nil

-- Dispose application data.
App_Timer = nil
App_Running = nil

-- Quit the application after the main loop breaks
--System.exit()
