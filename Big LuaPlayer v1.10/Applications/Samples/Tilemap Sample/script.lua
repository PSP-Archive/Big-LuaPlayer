-----------------------------------------------------------------------------------------------------
-- Big LuaPlayer: Tilemap Sample˜ v 1.00 year (C) company
-----------------------------------------------------------------------------------------------------

-- Slow Clear the draw buffers.
GL.drawBuffer:clear(GL.clr.black)
GL.Flip()
GL.drawBuffer:clear(GL.clr.black)
GL.Flip()

-- Render the load screen.
screen.print(0, 259, "Loading ...", GL.clr.white)
GL.Flip()

-- Set the Cpu Clock Frequencies.
Power.cpuSpeed(333)

-- load the screen module.
require "screen"

-- load Tilemap engine and supports modules.
table_ext = require "table_ext"
tilemap = require "tilemap"
Resources = require "Resources"
GameMap = require "GameMap"

-- set the texture color.
tilemap.color = Color.new(255 - 64, 255- 64, 255 - 16, 255)

-- Load or Create the GameMap.
local bln, map = pcall(GameMap.loadWorld, "map.txt")
if not bln or not map then
	map = GameMap.newWorld(32,24)
	GameMap.addTileFont("Grass.png")
	map.data = table_ext.createEmpty({1, string.char(2)}, map.w, map.h)
	--GameMap.saveWorld("map.txt")
	Utility.msgShow("No Tilemap Fonden:\nA new tilemap has been created\n\nPress the circle button to continue.")
end
bln = nil

-- Define the view point
local view = {x = 0, y = 0}
-- Update the veiw point.
map:update(view.x, view.y)

-- Activate the tile buffer.
tilemap.setBuffer(480, 272)

-- Define the application data.
local App_Running = true
local App_frameTime = 1000/60 -- (~16.67 ms)
local App_Timer = Timer.new()

-- Define the control enumerations.
local CTRL_ENUM_EXIT = Controls.START + Controls.CIRCLE -- (8 + 8192)
local CTRL_ENUM_UP = Controls.UP -- (16)
local CTRL_ENUM_DOWN = Controls.DOWN -- (64)
local CTRL_ENUM_RIGHT = Controls.RIGHT -- (32)
local CTRL_ENUM_LEFT = Controls.LEFT -- (128)

-- Define the colors.
local white = Color.new(255, 255, 255)
local black = Color.new(0, 0, 0)

-----------------------------------------------------------------------------------------------------
-- Draw function.
local function drawFunc()

	-- Clear the screen.
	GL.Clear(black)

	-- Render the map.
	map:renderBuffer()

	-- Print some text using the print function.
	screen.print(0, 0, "Big LuaPlayer: Tilemap Sample", white)
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
	App_frameTime = math.min(App_Timer:delta(), 128)

	if Controls.buttons(CTRL_ENUM_UP) then
		view.y = view.y + 4/16*App_frameTime
		-- Update the veiw point.
		map:update(view.x, view.y)
	elseif Controls.buttons(CTRL_ENUM_DOWN) then
		view.y = view.y - 4/16*App_frameTime
		-- Update the veiw point.
		map:update(view.x, view.y)
	end
	if Controls.buttons(CTRL_ENUM_LEFT) then
		view.x = view.x + 4/16*App_frameTime
		-- Update the veiw point.
		map:update(view.x, view.y)
	elseif Controls.buttons(CTRL_ENUM_RIGHT) then
		view.x = view.x - 4/16*App_frameTime
		-- Update the veiw point.
		map:update(view.x, view.y)
	end

end
-----------------------------------------------------------------------------------------------------

-- Dispose Draw function.
drawFunc = nil

-- Dispose colors.
black = nil
white = nil

-- Dispose control enumerations.
CTRL_ENUM_LEFT = nil
CTRL_ENUM_RIGHT = nil
CTRL_ENUM_DOWN = nil
CTRL_ENUM_UP = nil
CTRL_ENUM_EXIT = nil

-- Dispose application data.
App_Timer = nil
App_frameTime = nil
App_Running = nil

-- Dispose some data.
view = nil
map = GameMap.freeWorld()
Resources.freeTileFontStack()

-- Dispose the loaded modules.
GameMap = GameMap.dispose()
Resources = Resources.dispose()
tilemap = tilemap.dispose()
table_ext = table_ext.dispose()

-- Quit the application after the main loop breaks.
--System.exit()
