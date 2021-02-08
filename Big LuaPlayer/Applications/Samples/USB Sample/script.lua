-----------------------------------------------------------------------------------------------------
-- Big LuaPlayer: USB sample˜ v 1.00 year (C) company
-----------------------------------------------------------------------------------------------------

-- Set the Cpu Clock Frequencies.
Power.cpuSpeed(100)

-- Define the application data.
local App_Running = true

-- Define the control enumerations.
local CTRL_ENUM_EXIT = Controls.START + Controls.CIRCLE -- (8 + 8192)
local CTRL_ENUM_CONFIRM = Controls.CROSS -- (16384)

-----------------------------------------------------------------------------------------------------
-- pause function.
local function pause()
	while App_Running do
		-- Exit the application on system request.
		if not System.running() then break end

		-- Update the controls.
		Controls.readPeek()

		if Controls.press(CTRL_ENUM_EXIT) then -- start or circle button pressed.
			-- Break the main loop.
			App_Running = false
		elseif Controls.press(CTRL_ENUM_CONFIRM) then -- cross button pressed.
			-- Break the pause loop.
			break
		end

	end
	if not System.running() then App_Running = false end
	return App_Running
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- Application main loop
while App_Running do
	-- Exit the application on system request.
	if not System.running() then break end

	-- Initalize the DebugScreen.
	DebugScreen.init()

	-- Initalize the usb driver.
	DebugScreen.printf("Press the cross button to initalize the usb driver.")
	if not pause() then break end
	if System.usb.initialize() then
		-- Activate the usb driver.
		DebugScreen.printf("usb driver initialized","press the cross button to activate the usb driver.")
		if not pause() then break end
		if System.usb.activate() then
			--Get the state of the usb driver.
			DebugScreen.printf("usb driver state:",
							  string.format("  Activated: %s", tostring(System.usb.isActivated())),
							  string.format("  Cable connected: %s", tostring(System.usb.cable())),
							  string.format("  Connection established: %s", tostring(System.usb.connected()))," ")

			-- Activate the usb driver.
			DebugScreen.printf("Press the cross button to deactivate the usb driver.")
			if not pause() then break end
			if System.usb.deactivate() then
				-- Activate the usb driver.
				DebugScreen.printf("usb driver deactivated","Press the cross button to terminate the usb driver.")
				if not pause() then break end
				if System.usb.terminate() then
					DebugScreen.printf("usb driver terminated", "Press cross button to continue.")
					if not pause() then break end
				else -- Catch error.
					DebugScreen.printf("Error: Can't terminate the usb driver.", "Press cross button to continue.")
					if not pause() then break end
				end
			else -- Catch error.
				DebugScreen.printf("Error: Can't deactivate the usb driver.", "Press cross button to continue.")
				if not pause() then break end
			end
		else -- Catch error.
			--Get the state of the usb driver.
			DebugScreen.printf("usb driver state:",
							  string.format("  Activated: %s", tostring(System.usb.isActivated())),
							  string.format("  Cable connected: %s", tostring(System.usb.cable())),
							  string.format("  Connection established: %s", tostring(System.usb.connected()))," ")

			System.usb.terminate()
			DebugScreen.printf("Error: Can't activate the usb driver.", "Press cross button to continue.")
			if not pause() then break end
		end
	else -- Catch error.
		DebugScreen.printf(string.format("Error: Can't initalize the usb driver. (%d)", intError), "Press cross button to continue.")
		if not pause() then break end
	end

end
-----------------------------------------------------------------------------------------------------

-- Terminate the usb driver.
System.usb.terminate()

-- Dispose pause function.
pause = nil

-- Dispose control enumerations.
CTRL_ENUM_EXIT = nil
CTRL_ENUM_CONFIRM = nil

-- Dispose application data.
App_Running = nil

-- Quit the application after the main loop breaks
--System.exit()
