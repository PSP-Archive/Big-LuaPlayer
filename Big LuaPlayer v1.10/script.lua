-----------------------------------------------------------------------------------------------------
-- Big LuaPlayer: Application loader˜ (for "BLP - v1.10") v 1.01 2014 (C) M4MSOFT
-----------------------------------------------------------------------------------------------------

-- Set the Cpu Clock Frequencies.
Power.cpuSpeed(333)

-- Define the application data.
local App_loaded = false
local App_Running = true
local App_current = {{dir = "Applications", n = 1, cursor = 0, selected = 1}}
local App_Menu = {count = 0, items = {}}
local App_ScrollBar = false
local App_Timezone = 1 -- Timezone Amsterdam + 1 Hour
local App_Time = os.date("!*t")
local App_Sound = false

-- Get the daylightsavings time offset.
if App_Time.month * 100 + App_Time.day >= 331 and App_Time.month * 100 + App_Time.day <= 1031 then
    App_Timezone = App_Timezone + 1
end

-- Define the control enumerations.
local CTRL_ENUM_EXIT = Controls.START + Controls.CIRCLE -- (8 + 8192)
local CTRL_ENUM_UP = Controls.UP -- (16)
local CTRL_ENUM_DOWN = Controls.DOWN -- (64)
local CTRL_ENUM_CANCEL = Controls.CIRCLE -- (8192)
local CTRL_ENUM_CONFIRM = Controls.CROSS -- (16384)

-- Define the colors.
local black = Color.new( 0, 0, 0)
local white = Color.new( 255, 255, 255)
local gold = Color.new( 255, 216, 0)
local clrBack = Color.new( 0, 0, 0)
local clrMonth = false

-- Get the color of the month.
local hue = (App_Time.month-1) * 30
if hue <= 60 then clrMonth = Color.new(128, 32*hue/15, 0)
elseif hue <= 120 then clrMonth = Color.new(128 - 32*(hue-60)/15, 128, 0)
elseif hue <= 180 then clrMonth = Color.new(0, 128, 32*(hue-120)/15)
elseif hue <= 240 then clrMonth = Color.new(0, 128 - 32*(hue-180)/15, 128)
elseif hue <= 300 then clrMonth = Color.new(32*(hue-240)/15, 0, 128)
elseif hue <= 360 then clrMonth = Color.new(128, 0, 128 - 32*(hue-300)/15)
end
hue = nil

-- Define the bmpfont.
local Font_comic = false

-- Define the textures
local Tex_background = false
local Tex_bar = false
local Tex_bar_background = false
local Tex_cursor = false
local Tex_directory = false
local Tex_script = false

-----------------------------------------------------------------------------------------------------
-- Set the background color.
local function set_backcolor()
	local lum = App_Time["hour"] * 60 + App_Time["min"]
	lum = lum >= 600 and lum < 1320 and 128-(lum-600)*2/15 or (lum >= 1320 and 32+(lum-1320)*2/15 or 32+(lum+120)*2/15)
	clrBack:red(clrMonth:red()*lum/128)
	clrBack:green(clrMonth:green()*lum/128)
	clrBack:blue(clrMonth:blue()*lum/128)
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- Get the current time.
local function get_time()
	App_Time = os.date("!*t")
	App_Time["hour"] = App_Time["hour"] + App_Timezone
    if App_Time["hour"] >= 24 then App_Time["hour"] = App_Time["hour"] - 24 end
    if App_Time["hour"] <  0  then App_Time["hour"] = App_Time["hour"] + 24 end
    return string.format("%02d:%02d:%02d", App_Time["hour"], App_Time["min"], App_Time["sec"])
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- Set the package search path.
local function set_package(str)
	if type(str) == "boolean" then
		str = System.currentDirectory()
		package.path = string.format("%s/System/Library/?.lua;%s/System/?.lua;%s/?.lua;%s",str,str,str,package.path)
	elseif type(str) == "string" or type(str) == "nil" then
		package.path = str or System.Directory.eboot
		package.path = string.format("%s/System/Library/?.lua;%s/System/?.lua;%s/?.lua",package.path,package.path,package.path)
	end
	str = nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- Get the menu items.
local function get_items(path)
	-- Get the contents of the directory.
	local tblContent = System.listDirectory(path) or {}

	-- Filter out unwanted files.
	local done = false
	while not done do
		done = true
		for idx,file in ipairs(tblContent) do
			done = false
			if string.sub(file.name, 1, 1) == "." or not file.directory then
				table.remove(tblContent, idx)
				break
			end
			if idx == table.getn(tblContent) then
				done = true
			end
		end
	end
	done = nil

	-- Index the files.
	for idx,file in ipairs(tblContent) do
		local tblSubContent = System.listDirectory(path.."/"..file.name) or {}
		local blnScript = false
		local blnImage = false
		-- Check the sub contents.
		for subidx, subfile in ipairs(tblSubContent) do
			if string.lower(subfile.name) == "script.lua" then blnScript = true end
			if string.lower(subfile.name) == "icon.png" then blnImage = true end
		end
		-- Clear unwanted table field.
		file.size = nil
		file.directory = nil
		file.time = nil
		-- Set new the table field.
		file.script = blnScript
		if blnImage then
			file.image = Texture.load(path.."/"..file.name.."/icon.png")
		elseif blnScript then
			file.image = Tex_script
		else
			file.image = Tex_directory
		end
	end

	-- Sort the contents so sub-directories appear first sorted alphabetically and files appear after, sorted alphabetically.
	table.sort(tblContent, function(a, b) if a.script == b.script then return a.name < b.name else return not a.script end end)

	-- Return the menu items.
	return tblContent
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- Load the application.
local function App_load()
	if App_loaded then return end

	-- Set the Cpu Clock Frequencies.
	Power.cpuSpeed(333)

	-- Set the package search path.
	set_package(System.Directory.eboot)

	-- Set currentDirectory.
	System.currentDirectory(System.Directory.eboot)

	-- Clean Finish the GU system.
	GL.CleanFinish()

	-- Clear the GL Stack.
	GL.ClearStack()

	-- Reset the Global Scale factor.
	GL.ResetGlobalScale()

	-- Clear the GL.Font Stack.
	Font.clearStack()

	-- Reset the lua math library.
	if math.vfpu then System.vfpuMath(false) end

	--load screen module
	screen = require "screen"

	--Draw text: Loading ...
	screen.clear(black)
	screen.print(0, 259, "loading ...", white)
	screen.flip()

	-- Reload the ScrollBar module
	package.loaded["ScrollBar"] = nil
	ScrollBar = require "ScrollBar"

	-- Load the bmpfont.
	Font_comic = BmpFont.load("System/Font/comic.ttf", 18)

	-- load the textures
	Tex_background = Texture.load("Resources/Image/background.png")
	Tex_bar = Texture.load("Resources/Image/bar.png")
	Tex_bar_background = Texture.load("Resources/Image/bar_background.png")
	Tex_cursor = Texture.load("Resources/Image/cursor.png")
	Tex_directory = Texture.load("Resources/Image/directory.png")
	Tex_script = Texture.load("Resources/Image/script.png")

	-- Collect the application scripts.
	App_Menu.items = get_items(App_current[1].dir)
	App_Menu.count = table.getn(App_Menu.items)

	-- Activaite or deactivaite the confirm button.
	if App_Menu.count > 0 then
		CTRL_ENUM_UP = Controls.UP
		CTRL_ENUM_DOWN = Controls.DOWN
		CTRL_ENUM_CONFIRM = Controls.CROSS
	else
		CTRL_ENUM_UP = 0
		CTRL_ENUM_DOWN = 0
		CTRL_ENUM_CONFIRM = 0
	end

	-- Activaite or deactivaite the cancel button.
	if table.getn(App_current) > 1 then
		CTRL_ENUM_EXIT = Controls.START
		CTRL_ENUM_CANCEL = Controls.CIRCLE
	else
		CTRL_ENUM_EXIT = Controls.START + Controls.CIRCLE
		CTRL_ENUM_CANCEL = 0
	end

	-- Build the ScrollBar object.
	App_ScrollBar = ScrollBar.new(5, App_Menu.count, 181, 18)

	-- Set the cursor location.
	App_ScrollBar:set(App_current[1].n, App_current[1].cursor)
	if App_ScrollBar.selected ~= App_current[1].selected then
		App_ScrollBar:set(false, false, App_current[1].selected)
	end

	-- Load the appliction sound.
	App_Sound = Sound.load("System/Sound/menu.wav") or false

	-- Initalize one sound channel.
	Sound.init(1)

	-- Set the Cpu Clock Frequencies.
	Power.cpuSpeed(100)

	App_loaded = true
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- Dispose some application data.
local function App_dipose()
	if not App_loaded then return end

	-- Terminate the sound channels.
	Sound.term()

	-- Dipose the appliction sound.
	App_Sound = false

	-- store the cursor location.
	App_current[1].n = App_ScrollBar.n
	App_current[1].cursor = App_ScrollBar.cursor
	App_current[1].selected = App_ScrollBar.selected

	-- Dipose the ScrollBar object.
	App_ScrollBar = false

	-- Dipose the application menu.
	App_Menu.count = 0
	App_Menu.items = {}

	-- Dipose the textures
	Tex_script = false
	Tex_directory = false
	Tex_cursor = false
	Tex_bar_background = false
	Tex_bar = false
	Tex_background = false

	-- Dipose the bmpfont.
	Font_comic = false

	-- Dispose the ScrollBar module.
	ScrollBar = ScrollBar.dispose()

	-- Collect Garbage.
    collectgarbage ("collect")

	App_loaded = false
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- Open a Directory.
local function OpenDirectory(path)

	-- Set the Cpu Clock Frequencies.
	Power.cpuSpeed(333)

	-- store the cursor location.
	App_current[1].n = App_ScrollBar.n
	App_current[1].cursor = App_ScrollBar.cursor
	App_current[1].selected = App_ScrollBar.selected

	-- Insert the new application state to the 'App_current' table.
	table.insert(App_current, 1, {dir = path, n = 1, cursor = 0, selected = 1})

	-- Collect the application scripts.
	App_Menu.items = get_items(App_current[1].dir)
	App_Menu.count = table.getn(App_Menu.items)

	-- Activaite or deactivaite the confirm button.
	if App_Menu.count > 0 then
		CTRL_ENUM_UP = Controls.UP
		CTRL_ENUM_DOWN = Controls.DOWN
		CTRL_ENUM_CONFIRM = Controls.CROSS
	else
		CTRL_ENUM_UP = 0
		CTRL_ENUM_DOWN = 0
		CTRL_ENUM_CONFIRM = 0
	end

	-- Build the ScrollBar object.
	App_ScrollBar = ScrollBar.new(5, App_Menu.count, 181, 18)

	-- Activaite the cancel button.
	if CTRL_ENUM_CANCEL ~= Controls.CIRCLE then
		CTRL_ENUM_EXIT = Controls.START
		CTRL_ENUM_CANCEL = Controls.CIRCLE
	end

	-- Set the Cpu Clock Frequencies.
	Power.cpuSpeed(100)

end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- Close a Directory.
local function CloseDirectory()

	-- Set the Cpu Clock Frequencies.
	Power.cpuSpeed(333)

	-- Remove the current application state from the 'App_current' table.
	table.remove(App_current, 1)

	-- Collect the application scripts.
	App_Menu.items = get_items(App_current[1].dir)
	App_Menu.count = table.getn(App_Menu.items)

	-- Activaite or deactivaite the confirm button.
	if App_Menu.count > 0 then
		CTRL_ENUM_UP = Controls.UP
		CTRL_ENUM_DOWN = Controls.DOWN
		CTRL_ENUM_CONFIRM = Controls.CROSS
	else
		CTRL_ENUM_UP = 0
		CTRL_ENUM_DOWN = 0
		CTRL_ENUM_CONFIRM = 0
	end

	-- Build the ScrollBar object.
	App_ScrollBar = ScrollBar.new(5, App_Menu.count, 181, 18)

	-- Set the cursor location.
	App_ScrollBar:set(App_current[1].n, App_current[1].cursor)
	if App_ScrollBar.selected ~= App_current[1].selected then
		App_ScrollBar:set(false, false, App_current[1].selected)
	end

	-- Deactivaite the cancel button.
	if CTRL_ENUM_CANCEL == Controls.CIRCLE and table.getn(App_current) == 1 then
		CTRL_ENUM_EXIT = Controls.START + Controls.CIRCLE
		CTRL_ENUM_CANCEL = 0
	end

	-- Set the Cpu Clock Frequencies.
	Power.cpuSpeed(100)

end
-----------------------------------------------------------------------------------------------------


-----------------------------------------------------------------------------------------------------
-- Function Draw ScrollBar
local function drawScrollBar(x, y)

    -- Draw ScrollBar background texture.
    screen.blit( x, y, Tex_bar_background)

	-- Begin Texture rendering.
	GL.BeginRects(Tex_bar)

	-- Disable the bilinear texture filter.
	GL.SetTexLinear(false)

	-- Add the mid bar texture,
	local h = App_ScrollBar["bloksize"] - 18
	if h > 0 then GL.AddTex(x, y+App_ScrollBar["position"]+9, 18, h, 0, 15, 18, 2) end

	-- Add the top bar texture,
	GL.AddTex(x, y+App_ScrollBar["position"], 18, 9, 0, 0, 18, 9)

	-- Add the bottom bar texture,
	GL.AddTex(x, y+App_ScrollBar["position"]+h+9, 18, 9, 0, 23, 18, 9)

	-- End Texture rendering.
	GL.End()

end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- Draw function.
local function drawFunc()

	-- Set the background color.
	set_backcolor()

	-- Clear the screen.
	GL.Clear(clrBack)

	-- Draw the background texture.
	screen.blit(0, 0, Tex_background)

	-- Draw the Menu.
	for idx, node in App_ScrollBar:vnodes() do
		-- highlight the text if the current node is the selected node.
		if App_ScrollBar["selected"] == node then
			Font_comic:print(19+36+36, 40+8+idx*36, App_Menu.items[node].name or "", gold)
            screen.blit(18+36, 40+2+idx*36, App_Menu.items[node].image or Tex_directory)
			--Draw Cursor
			screen.blit(18, 42+idx*36, Tex_cursor)
		else
            Font_comic:print(19+36+18, 40+8+idx*36, App_Menu.items[node].name or "", white)
            screen.blit(18+18, 40+2+idx*36, App_Menu.items[node].image or Tex_directory)
		end
	end

	-- Draw the ScrollBar.
	drawScrollBar(446, 39)

	-- Print the current time.
    Font_comic:print( 19, 229, get_time(), white)

	-- Print Current and Total App's.
    screen.print( 275, 16, string.format( "%s From %s", App_ScrollBar["selected"], App_ScrollBar["maxn"]), white)

	-- Collect Garbage.
	collectgarbage("collect")

    -- Print Memory in use by Lua.
    screen.print(19,251,string.format("Memory in use by Lua: %d kb", collectgarbage("count")), white)

end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- Run a lua script.
local function RunLuaScript(dir, name)
	local path = string.format("%s/%s", dir, name)

	-- Set the Cpu Clock Frequencies.
	Power.cpuSpeed(333)

	-- Load the lua script.
	local script, msg_error = loadfile(path.."/script.lua")

	-- Set the Cpu Clock Frequencies.
	Power.cpuSpeed(100)

	--Check if the script has been loaded.
	if script then

		-- Dispose some application data.
		App_dipose()

		-- Set the current directory.
		System.currentDirectory(path)

		-- Set the package search path.
		set_package(true)

		-- Run the lua script.
		local bln_error, Result = pcall(script)

		-- Terminate the sound channels.
		Sound.term()

		-- Dipose the script
		script = nil
		collectgarbage("collect")

		-- Reload the application.
		App_load()

		if not bln_error then -- Catch Error
			-- Show Error Message
			Utility.msgShow(string.format("App: %s\n\nRuntime Error: Failed to run the script.\nDetails: %s\n\nPress the circle button to continue.",
			                              name, tostring(Result or "Unknown Error, no details.")),0,drawFunc)
			--error(Result or "Unknown Error, no details.")
		end

		-- Dispose errpr Message.
		bln_error = nil
		Result = nil

	else -- Catch Error
		-- Show Error Message
		Utility.msgShow(string.format("App: %s\n\nBuild Error: Failed to load the script.\nDetails: %s\n\nPress the circle button to continue.",
									  name, tostring(msg_error or "Unknown Error, no details.")),0,drawFunc)
		--error(msg_error or "Unknown Error, no details.")
		script = nil
		msg_error = nil
		collectgarbage("collect")
	end

end
-----------------------------------------------------------------------------------------------------

-- load the application.
App_load()

-- Define the pad data.
local padcount = 10

-----------------------------------------------------------------------------------------------------
-- Application main loop.
while App_Running do
	-- Exit the application on system request.
	if not System.running() then break end

	-- Draw the screen.
	drawFunc()

	-- swap the buffer.
	GL.Flip(1)

	-- Slow down the Application if a button is pressed.
	if Controls.buttons() > 0 and padcount > 0 then
		padcount = padcount - 1
		System.sleep(80)
	elseif padcount ~= 7 and Controls.buttons() == 0 then
		padcount = 7
	end

	-- Update the controls.
	Controls.readPeek()

	if Controls.press(CTRL_ENUM_EXIT) then -- start or circle button pressed.
		-- Break the main loop.
		App_Running = false
	end

	if Controls.buttons(CTRL_ENUM_UP) then -- up button pressed.
		App_ScrollBar = App_ScrollBar - 1
		if App_Sound then App_Sound:play(true) end
		System.sleep(30)
	elseif Controls.buttons(CTRL_ENUM_DOWN) then -- down button pressed.
		App_ScrollBar = App_ScrollBar + 1
		if App_Sound then App_Sound:play(true) end
		System.sleep(30)
	elseif Controls.press(CTRL_ENUM_CANCEL) then -- circle button pressed.
		-- Close the current directory.
		CloseDirectory()
	elseif Controls.press(CTRL_ENUM_CONFIRM) then -- cross button pressed.
		if App_Menu.items[App_ScrollBar["selected"]]["script"] then
			-- Try to run the selected lua script.
			RunLuaScript(App_current[1].dir or "", App_Menu.items[App_ScrollBar["selected"]].name or "")
		else
			-- Open the directory.
			OpenDirectory(string.format("%s/%s", App_current[1].dir or "", App_Menu.items[App_ScrollBar["selected"]].name or ""))
		end
	end

end
-----------------------------------------------------------------------------------------------------

-- Dispose the pad data.
padcount = nil

-- Dispose draw functions.
drawFunc = nil
drawScrollBar = nil

-- Dipspose support functions.
RunLuaScript = nil
CloseDirectory = nil
OpenDirectory = nil
App_dipose = nil
App_load = nil
get_items = nil
set_package = nil
get_time = nil
set_backcolor = nil

-- Dipose the textures
Tex_script = nil
Tex_directory = nil
Tex_cursor = nil
Tex_bar_background = nil
Tex_bar = nil
Tex_background = nil

-- Dispose the bmpfont.
Font_comic = nil

-- Dispose colors.
clrMonth = nil
clrBack = nil
gold = nil
white = nil
black = nil

-- Dispose control enumerations.
CTRL_ENUM_EXIT = nil
CTRL_ENUM_UP = nil
CTRL_ENUM_DOWN = nil
CTRL_ENUM_CONFIRM = nil

-- Terminate the sound channels.
Sound.term()

-- Dispose application data.
App_Sound = nil
App_Time = nil
App_Timezone = nil
App_ScrollBar = nil
App_Menu = nil
App_current = nil
App_Running = nil
App_loaded = nil

-- Quit the application after the main loop breaks.
--System.exit()
