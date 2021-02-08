-----------------------------------------------------------------------------------------------------
-- Big LuaPlayer: Adhoc Sample˜ v 1.00 year (C) company
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
local App_Cursor = {{n = 1, cursor = 0}}
local App_MainMenu =
{
	count = 6,
	name = "Main Menu:",
	items =
	{
		{text = "Host Name"},
		{text = "Scan"},
		{text = "Connect"},
		{text = "Create"},
		{text = "Join"},
		{text = "Disconnect"},
	},
}
local App_Menu = App_MainMenu
local App_ScrollBarx = 150
local App_ScrollBar = ScrollBar.new(4, App_Menu.count, 60, 15)
local App_ConnectionName = "PSADHOC"
local App_SelectedConnection = ""
local App_mac = Utility.adhocGetMac()

-- Define the control enumerations.
local CTRL_ENUM_EXIT = Controls.START + Controls.CIRCLE -- (8 + 8192)
local CTRL_ENUM_UP = Controls.UP -- (16)
local CTRL_ENUM_DOWN = Controls.DOWN -- (64)
local CTRL_ENUM_CANCEL = 0
local CTRL_ENUM_CONFIRM = Controls.CROSS -- (16384)

-- Define the colors.
local white = Color.new(255, 255, 255)
local gray = Color.new(128, 128, 128)
local black = Color.new(0, 0, 0)
local gold = Color.new(255, 216, 0)

-- Load the bmpFont.
local fntVerdana14 = BmpFont.load(System.Directory.eboot.."/System/Font/verdana.ttf", 14)

-- Define the data table.
local tblData = {"0x00000000", 20 ,"nil", 0}

-- initalize the adhoc library.
if not Utility.adhocInit() then error("Can't initalize the adhoc library.") end

-----------------------------------------------------------------------------------------------------
-- convert a mac addres to a string.
local function bytetostring(s, l)
	return string.sub(string.format(string.rep("%02x:", l or string.len(s)), string.byte(s, 1, l or string.len(s))), 1, -2)
end
App_mac = bytetostring(App_mac, 6)
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
	screen.print(0, 0, "Big LuaPlayer: Adhoc Sample", white)
	screen.print(0, 14, string.format("Hz: %d", 1000/(App_frameTime+0.001)), white)
	screen.print(0, 28, string.format("Wlan:%s, state:%d, host:'%s', client:'%s'", tostring(System.wlan()), Utility.adhocGetState(), App_ConnectionName, App_SelectedConnection), white)
	screen.print(0, 259, string.format("RunTime: %f Mac: %s", os.clock(), App_mac), white)

	-- Draw the Menu.
	fntVerdana14:print(9, 45, App_Menu.name, white)
	for idx, node in App_ScrollBar:vnodes() do
		-- highlight the text if the current node is the selected node.
		if App_ScrollBar["selected"] == node then
			fntVerdana14:print(18, 60+15*idx, string.format(">> %s", App_Menu.items[node].text), gold)
		else
			fntVerdana14:print(18, 60+15*idx, string.format(" %s", App_Menu.items[node].text), white)
		end
	end

	-- Draw the data table.
	for idx = 1, table.getn(tblData), 2 do fntVerdana14:print(9, 135+15*((idx-1)/2), tblData[idx], (tblData[idx+1] <= 5 and gray or white)) end

	-- Draw the ScrollBar.
	drawScrollBar(App_ScrollBarx, 60)

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

	-- Set the new send data.
	tblData[1] = string.format("0x%08x", Controls.buttons())

	-- Send the data.
	Utility.adhocSend(tblData[1])
	-- Recive the data.
	local data, mac = Utility.adhocRecv(10, 5000)
	if data then
		tblData[3] = string.format("%s - %s", data, bytetostring(mac, 6))
		tblData[4] = 20
	elseif tblData[4] > 0 then
		tblData[4] = tblData[4] - 1
		if tblData[4] == 0 then
			tblData[3] = "nil"
		end
	end

	if Controls.press(CTRL_ENUM_EXIT) then -- start or circle button pressed.
		-- Break the main loop.
		App_Running = false
	elseif Controls.buttons(CTRL_ENUM_UP) then -- up button pressed.
		App_ScrollBar = App_ScrollBar - 1
		System.sleep(90)
	elseif Controls.buttons(CTRL_ENUM_DOWN) then -- down button pressed.
		App_ScrollBar = App_ScrollBar + 1
		System.sleep(90)
	elseif Controls.press(CTRL_ENUM_CANCEL) then -- circle button pressed.
		-- Remove the current cursor state from the 'App_Cursor' table.
		table.remove(App_Cursor, 1)

		-- Restore the aplication menu.
		App_Menu = App_MainMenu

		-- Activaite or deactivaite the application buttons.
		CTRL_ENUM_EXIT = Controls.START + Controls.CIRCLE
		CTRL_ENUM_UP = Controls.UP
		CTRL_ENUM_DOWN = Controls.DOWN
		CTRL_ENUM_CANCEL = 0

		App_ScrollBarx = 150

		-- Build the ScrollBar object.
		App_ScrollBar = ScrollBar.new(4, App_Menu.count, 60, 15)

		-- Restore the cursor location.
		App_ScrollBar:set(App_Cursor[1].n, App_Cursor[1].cursor)
	elseif Controls.press(CTRL_ENUM_CONFIRM) then -- cross button pressed.
		local selected = App_ScrollBar["selected"]
		if App_Menu.name == "Main Menu:" then
			if selected == 1 then -- 'Host Name' selected.
				local Result, new_text = Utility.oskShow("Enter Host Name:", App_ConnectionName, 0, drawFunc)
				if Result == Utility.OSK_RESULT_CHANGED then
					App_ConnectionName = string.sub(new_text, 1, 7)
				end
				Result, new_text = nil, nil
			elseif selected == 2 then -- 'Scan' selected.
				-- Store the cursor location.
				App_Cursor[1].n = App_ScrollBar.n
				App_Cursor[1].cursor = App_ScrollBar.cursor

				-- Insert a new cursor state to the 'App_current' table.
				table.insert(App_Cursor, 1, {n = 1, cursor = 0})

				-- Create the new aplication menu.
				App_Menu = {count = 5, name = "Select group:", items = {}}

				-- Scan the adhoc channels.
				App_Menu.items = Utility.adhocScan() or {}
				App_Menu.count = table.getn(App_Menu.items)

				-- Activaite or deactivaite the application buttons.
				CTRL_ENUM_EXIT = Controls.START
				if App_Menu.count > 0 then
					CTRL_ENUM_UP = Controls.UP
					CTRL_ENUM_DOWN = Controls.DOWN
				else
					CTRL_ENUM_UP = 0
					CTRL_ENUM_DOWN = 0
				end
				CTRL_ENUM_CANCEL = Controls.CIRCLE

				-- Update the menu items.
				for idx = 1, App_Menu.count do
					App_Menu.items[idx].text = string.format("%s - %s", App_Menu.items[idx].name, bytetostring(App_Menu.items[idx].bssid))
				end
				table.insert(App_Menu.items, 1, {text = "none", name = ""})
				App_Menu.count = App_Menu.count + 1
				App_ScrollBarx = 300

				-- Build the ScrollBar object.
				App_ScrollBar = ScrollBar.new(4, App_Menu.count, 60, 15)
			elseif selected == 3 then -- 'Connect' selected.
				if not Utility.adhocConnect(Utility.ADHOC_CONNECT, App_ConnectionName) then
					Utility.msgShow("Error:\nCan't create the connection.", 0, drawFunc)
				end
			elseif selected == 4 then -- 'Create' selected.
				if not Utility.adhocConnect(Utility.ADHOC_CREATE, App_ConnectionName) then
					Utility.msgShow("Error:\nCan't create the connection.", 0, drawFunc)
				end
			elseif selected == 5 then -- 'Join' selected.
				if not Utility.adhocConnect(Utility.ADHOC_JOIN, App_SelectedConnection) then
					Utility.msgShow("Error:\nCan't create the connection.", 0, drawFunc)
				end
			elseif selected == 6 then -- 'Disconnect' selected.
				if not Utility.adhocDisconnect() then
					Utility.msgShow("Error:\nCan't disconnect the connection.", 0, drawFunc)
				end
			end
		else
			if selected > 1 then
				App_SelectedConnection = App_Menu.items[selected].name
				Utility.msgShow(
					string.format(
						"Info:\nname: %s\nbssid: %s\nunknown: %s\nunknown2: %d",
						App_Menu.items[selected].name,
						bytetostring(App_Menu.items[selected].bssid),
						bytetostring(App_Menu.items[selected].unknown),
						App_Menu.items[selected].unknown2
					), 0, drawFunc
				)
			else
				App_SelectedConnection = ""
			end

			-- Remove the current cursor state from the 'App_Cursor' table.
			table.remove(App_Cursor, 1)

			-- Restore the aplication menu.
			App_Menu = App_MainMenu

			-- Activaite or deactivaite the application buttons.
			CTRL_ENUM_EXIT = Controls.START + Controls.CIRCLE
			CTRL_ENUM_UP = Controls.UP
			CTRL_ENUM_DOWN = Controls.DOWN
			CTRL_ENUM_CANCEL = 0

			App_ScrollBarx = 150

			-- Build the ScrollBar object.
			App_ScrollBar = ScrollBar.new(4, App_Menu.count, 60, 15)

			-- Restore the cursor location.
			App_ScrollBar:set(App_Cursor[1].n, App_Cursor[1].cursor)
		end
		selected = nil
	end

	-- Store the frame time.
	App_frameTime = App_Timer:delta()
end
-----------------------------------------------------------------------------------------------------

-- Terminate the adhoc library.
Utility.adhocTerm()

-- Dispose string convert function.
bytetostring = nil

-- Dispose Draw function.
drawFunc = nil

-- Dispose the data table.
tblData = nil

-- Dispose the bmpFont.
fntVerdana14 = nil

-- Dispose colors.
gold = nil
black = nil
white = nil

-- Dispose control enumerations.
CTRL_ENUM_EXIT = nil
CTRL_ENUM_UP = nil
CTRL_ENUM_DOWN = nil
CTRL_ENUM_CANCEL = nil
CTRL_ENUM_CONFIRM = nil

-- Dispose application data.
App_mac = nil
App_SelectedConnection = nil
App_ConnectionName = nil
App_ScrollBar = nil
App_ScrollBarx = nil
App_Menu = nil
App_MainMenu = nil
App_Cursor = nil
App_Timer = nil
App_Running = nil

-- Dispose the ScrollBar module.
ScrollBar = ScrollBar.dispose()

-- Quit the application after the main loop breaks.
--System.exit()
