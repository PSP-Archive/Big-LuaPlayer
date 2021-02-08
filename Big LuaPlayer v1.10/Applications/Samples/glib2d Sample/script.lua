-----------------------------------------------------------------------------------------------------
-- Big LuaPlayer: glib2d Sample˜ v 1.00 year (C) company
-----------------------------------------------------------------------------------------------------

-- Set the Cpu Clock Frequencies.
Power.cpuSpeed(100)

-- load the screen module.
require "screen"

-- load the ScrollBar module
ScrollBar = require "ScrollBar"

-- Define the application data.
local App_Running = true
local App_frameTime = 1000/60 -- (~16.67 ms)
local App_Timer = Timer.new()
local App_Menu =
{
	count = 22,
	name = "GL Object Menu:",
	items = false
}
local App_ScrollBar = ScrollBar.new(10, App_Menu.count, 150, 15)

-----------------------------------------------------------------------------------------------------
-- Reset the current object properties.
local function ResetProperties()
	App_Menu.items =
	{
		{text = "Object: %s", enabled = true, n = 1, min = 1, max = 5, str = {"rect", "line", "quad", "point", "triangle"}},
		{text = "Texture: %s", enabled = true, n = 1, min = 1, max = 2, str = {"enabled", "diabled"}},
		{text = "Linear: %s", enabled = true, n = 2, min = 1, max = 2, str = {"enabled", "diabled"}},
		{text = "Repeat: %s", enabled = true, n = 2, min = 1, max = 2, str = {"enabled", "diabled"}},
		{text = "TFX: %s", enabled = true, n = 1, min = 1, max = 5, str = {"MODULATE", "DECAL", "BLEND", "REPLACE", "ADD"}},
		{text = "Crop X: %d", enabled = true, n = 0, min =  0, max = 19, str = false},
		{text = "Crop Y: %d", enabled = true, n = 0, min =  0, max = 19, str = false},
		{text = "Crop W: %d", enabled = true, n = 19, min = 1, max = 128, str = false},
		{text = "Crop H: %d", enabled = true, n = 19, min = 1, max = 128, str = false},
		{text = "Scale W: %d", enabled = true, n = 200, min = -256, max = 256, str = false},
		{text = "Scale H: %d", enabled = true, n = 200, min = -256, max = 256, str = false},
		{text = "factor: %d", enabled = true, n = 100, min = 75, max = 125, str = false},
		{text = "Coord X: %d", enabled = true, n = 240, min = 0, max = 479, str = false},
		{text = "Coord Y: %d", enabled = true, n = 136, min = 0, max = 271, str = false},
		{text = "Angle: %d", enabled = true, n = 0, min = 0, max = 359, str = false},
		{text = "Center X: %d", enabled = true, n = 9, min = -256, max = 256, str = false},
		{text = "Center Y: %d", enabled = true, n = 9, min = -256, max = 256, str = false},
		{text = "Color R: %d", enabled = true, n = 255, min = 0, max = 255, str = false},
		{text = "Color G: %d", enabled = true, n = 255, min = 0, max = 255, str = false},
		{text = "Color B: %d", enabled = true, n = 255, min = 0, max = 255, str = false},
		{text = "Color A: %d", enabled = true, n = 255, min = 0, max = 255, str = false},
		{text = "Reset All%s", enabled = true, n = 1, min = 1, max = 1, str = {""}}
	}
end
ResetProperties()
-- Update the Application Menu.
local function UpdateMenu()
	local objType = App_Menu.items[1]["n"]
	if objType == 1 or objType == 3 then -- rect or quad
		App_Menu.items[ 2].enabled = true
		App_Menu.items[ 3].enabled = true
		App_Menu.items[ 4].enabled = true
		App_Menu.items[ 5].enabled = true
		App_Menu.items[ 6].enabled = true
		App_Menu.items[ 7].enabled = true
		App_Menu.items[ 8].enabled = true
		App_Menu.items[ 9].enabled = true
	else
		App_Menu.items[ 2].enabled = false
		App_Menu.items[ 3].enabled = false
		App_Menu.items[ 4].enabled = false
		App_Menu.items[ 5].enabled = false
		App_Menu.items[ 6].enabled = false
		App_Menu.items[ 7].enabled = false
		App_Menu.items[ 8].enabled = false
		App_Menu.items[ 9].enabled = false
	end
end
-----------------------------------------------------------------------------------------------------

-- Define the control enumerations.
local CTRL_ENUM_EXIT = Controls.START + Controls.CIRCLE -- (8 + 8192)
local CTRL_ENUM_UP = Controls.UP -- (16)
local CTRL_ENUM_DOWN = Controls.DOWN -- (64)
local CTR_ENUM_RIGHT = Controls.RIGHT -- (32)
local CTR_ENUM_LEFT = Controls.LEFT -- (128)
local CTRL_ENUM_CONFIRM = Controls.CROSS -- (16384)

-- Define the colors.
local white = Color.new(255, 255, 255)
local gray = Color.new(192, 192, 192)
local black = Color.new(0, 0, 0)
local semiGray = Color.new(32, 32, 32, 128)
local gold = Color.new(255, 216, 0)

-- Load the bmpFont.
local fntVerdana14 = BmpFont.load(System.Directory.eboot.."/System/Font/verdana.ttf", 14)

-- Load the texture.
local tex = Texture.load("Hippo.png")

-----------------------------------------------------------------------------------------------------
-- Function Set object Properties
local function SetTexObjectProperties()
	-- Set the texture filter.
	GL.SetTexLinear(App_Menu.items[3]["n"] == 1)
	-- enabled or diabled the Texture repeat function.
	GL.SetTexRepeat(App_Menu.items[4]["n"] == 1)
	-- Set The Tecture blend function.
	GL.SetTexTFX(App_Menu.items[5]["n"] - 1)
	-- Set The Texture source location.
	-- INFO: x and y can not be lower then zero, w and h can not be zero or lower.
	GL.SetCrop(App_Menu.items[6]["n"], App_Menu.items[7]["n"], App_Menu.items[8]["n"], App_Menu.items[9]["n"])
end
local function SetObjectProperties()
	-- Set the Scale width and height only for rectangle objects
	GL.SetScaleWH(App_Menu.items[10]["n"], App_Menu.items[11]["n"])
	-- Set the Global Scale.
	GL.SetGlobalScale(App_Menu.items[12]["n"]/100)
	-- Set the object position.
	GL.SetCoordXY(App_Menu.items[13]["n"], App_Menu.items[14]["n"])
	GL.SetCoordXYRelative(-App_Menu.items[10]["n"]/2, -App_Menu.items[11]["n"]/2)
	-- Set the Rotation Angle.
	GL.SetRotation(App_Menu.items[15]["n"])
	-- Set the Rotation center.
	GL.SetRotationCenterRelative(
		(App_Menu.items[10]["n"]-1)/18*App_Menu.items[16]["n"],
		(App_Menu.items[11]["n"]-1)/18*App_Menu.items[17]["n"]
	)
	-- Set the object Color. Can be used for all object Types.
	GL.SetColor(Color.new(App_Menu.items[18]["n"], App_Menu.items[19]["n"], App_Menu.items[20]["n"], App_Menu.items[21]["n"]))
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- Function Draw ScrollBar
local function drawScrollBar(x, y)
	-- Begin line rendering.
	GL.BeginLines(GL.STRIP)
	-- Add four lines.
	GL.AddQuad(x, y, 9, App_ScrollBar["size"]+1, 0, white)
	GL.AddPoint(x, y)
	-- End line rendering.
	GL.End()
	-- Begin rectangle rendering.
	GL.BeginRects()
	-- Add a rectangle.
	GL.AddRect(x+1,y+1+App_ScrollBar["position"], 8, App_ScrollBar["bloksize"], 0, gold)
	-- End rectangle rendering.
	GL.End()
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- Draw function.
local function drawFunc()

	-- Clear the screen.
	GL.Clear(black)

	-- Print some text using the print function.
	screen.print(0, 0, "Big LuaPlayer: glib2d Sample", white)
	screen.print(0, 14, string.format("Hz: %d", 1000/(App_frameTime+0.001)), white)
	screen.print(0, 259, string.format("RunTime: %f", os.clock()), white)

	-- Draw the object.
	local objType = App_Menu.items[1]["n"]
	if objType == 1 then -- rect
		-- Begin rectangle drawing.
		if App_Menu.items[2]["n"] == 1 then
			GL.BeginRects(tex)
			-- Set object Tecture properties
			SetTexObjectProperties()
		else
			GL.BeginRects()
		end
		-- Set object properties
		SetObjectProperties()
		-- Add the rectangle object.
		GL.Add()
		-- Stop rectangle drawing.
		GL.End()
	elseif objType == 2 then -- line
		-- Begin line drawing.
		GL.BeginLines(GL.STRIP)
		-- Set object properties
		SetObjectProperties()
		-- Add the line object.
		GL.Add()
		GL.SetCoordXYRelative( App_Menu.items[10]["n"],  0); GL.Add()
		GL.SetCoordXYRelative( 0,  App_Menu.items[11]["n"]); GL.Add()
		GL.SetCoordXYRelative(-App_Menu.items[10]["n"],  0); GL.Add()
		GL.SetCoordXYRelative( 0, -App_Menu.items[11]["n"]); GL.Add()
		-- Stop line drawing.
		GL.End()
	elseif objType == 3 then -- quad
		-- Begin quad drawing.
		if App_Menu.items[2]["n"] == 1 then
			GL.BeginQuads(tex)
			-- Set object Tecture properties
			SetTexObjectProperties()
		else
			GL.BeginQuads()
		end
		-- Set object properties
		SetObjectProperties()
		-- Add the quad object.
		GL.Add()
		GL.SetCoordXYRelative( App_Menu.items[10]["n"], 0); GL.Add()
		GL.SetCoordXYRelative( 0, App_Menu.items[11]["n"]); GL.Add()
		GL.SetCoordXYRelative(-App_Menu.items[10]["n"], 0); GL.Add()
		-- Stop quad drawing.
		GL.End()
	elseif objType == 4 then -- point
		-- Begin point drawing.
		GL.BeginPoints()
		-- Set object properties
		SetObjectProperties()
		-- Add the point object.
		GL.Add()
		GL.SetCoordXYRelative( App_Menu.items[10]["n"], 0); GL.Add()
		GL.SetCoordXYRelative( 0, App_Menu.items[11]["n"]); GL.Add()
		GL.SetCoordXYRelative(-App_Menu.items[10]["n"], 0); GL.Add()
		-- Stop point drawing.
		GL.End()
	elseif objType == 5 then -- triangle
		-- Begin triangle drawing.
		GL.BeginTriangles()
		-- Set object properties
		SetObjectProperties()
		-- Add the triangle object.
		GL.Add()
		GL.SetCoordXYRelative(App_Menu.items[10]["n"], 0); GL.Add()
		GL.SetCoordXYRelative(-App_Menu.items[10]["n"]/2, App_Menu.items[11]["n"]); GL.Add()
		-- Stop triangle drawing.
		GL.End()
	end

	GL.ResetGlobalScale()

	-- Draw the Menu.
	screen.fillRect(17, 29, 162, 167, semiGray)
	fntVerdana14:print(18, 30, App_Menu.name, white)
	local t = false
	for idx, node in App_ScrollBar:vnodes() do
		-- Get the item table.
		t = App_Menu.items[node]
		-- highlight the text if the current node is the selected node.
		if App_ScrollBar["selected"] == node then
			fntVerdana14:print(18, 45+15*idx, ">> "..string.format(t["text"], t["str"] and t["str"][t["n"]] or t["n"]), t["enabled"] and gold or gray)
		else
			fntVerdana14:print(18, 45+15*idx, " "..string.format(t["text"], t["str"] and t["str"][t["n"]] or t["n"]), t["enabled"] and white or gray)
		end
	end
	t = nil

	-- Draw the ScrollBar.
	drawScrollBar(170, 45)

end
-----------------------------------------------------------------------------------------------------

-- Start the application timer.
App_Timer:start()

-- Define the pad data.
local padcount = 10

-----------------------------------------------------------------------------------------------------
-- Application main loop.
while App_Running do
	-- Exit the application on system request.
	if not System.running() then break end

    -- Draw the screen.
    drawFunc()

	-- Swap the buffer.
	GL.Flip()

	-- Slow down the Application if a button is pressed.
	if Controls.buttons() > 0 and padcount > 0 then
		padcount = padcount - 1
		System.sleep(80)
	elseif padcount ~= 10 and Controls.buttons() == 0 then
		padcount = 10
	end

	-- Update the controls.
	Controls.readPeek()

	if Controls.press(CTRL_ENUM_EXIT) then -- start or circle button pressed.
		-- Break the main loop.
		App_Running = false
	elseif Controls.buttons(CTRL_ENUM_UP) then -- up button pressed.
		App_ScrollBar = App_ScrollBar - 1
		System.sleep(30)
	elseif Controls.buttons(CTRL_ENUM_DOWN) then -- down button pressed.
		App_ScrollBar = App_ScrollBar + 1
		System.sleep(30)
	elseif Controls.buttons(CTR_ENUM_LEFT) then -- left button pressed
		local selected = App_ScrollBar["selected"]
		local item = App_Menu.items[selected]
		if item.enabled then
			-- Update the selected object propertie.
			item["n"] = item["n"] > item["min"] and item["n"] - 1 or item["max"]
			if selected == 1 then UpdateMenu() end
		end
		System.sleep(5)
	elseif Controls.buttons(CTR_ENUM_RIGHT) then -- right button pressed
		local selected = App_ScrollBar["selected"]
		local item = App_Menu.items[selected]
		if item.enabled then
			-- Update the selected object propertie.
			item["n"] = item["n"] < item["max"] and item["n"] + 1 or item["min"]
			if selected == 1 then UpdateMenu() end
		end
		System.sleep(5)
	elseif Controls.press(CTRL_ENUM_CONFIRM) then -- cross button pressed.
		local selected = App_ScrollBar["selected"]
		local item = App_Menu.items[selected]
		if item.enabled and selected == 22 then
			-- Reset the object properties
			ResetProperties()
		elseif item.enabled then
			-- Update the selected object propertie.
			item["n"] = item["n"] < item["max"] and item["n"] + 1 or item["min"]
			if selected == 1 then UpdateMenu() end
		end
    end

	-- Store the frame time.
	App_frameTime = App_Timer:delta()
end
-----------------------------------------------------------------------------------------------------

-- Dispose the pad data.
padcount = nil

-- Dispose Draw function.
SetObjectProperties = nil
SetTexObjectProperties = nil
drawScrollBar = nil
drawFunc = nil

-- Dispose the texture.
tex = nil

-- Dispose the bmpFont.
fntVerdana14 = nil

-- Dispose colors.
gold = nil
semiGray = nil
black = nil
gray = nil
white = nil

-- Dispose control enumerations.
CTRL_ENUM_CONFIRM = nil
CTR_ENUM_LEFT = nil
CTR_ENUM_RIGHT = nil
CTRL_ENUM_DOWN = nil
CTRL_ENUM_UP = nil
CTRL_ENUM_EXIT = nil

-- Dispose object properties reset function.
UpdateMenu = nil
ResetProperties = nil

-- Dispose application data.
App_ScrollBar = nil
App_Menu = nil
App_Timer = nil
App_Running = nil

-- Quit the application after the main loop breaks.
--System.exit()
