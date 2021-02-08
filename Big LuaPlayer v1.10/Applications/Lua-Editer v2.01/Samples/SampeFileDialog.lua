-----------------------------------------------------------------------------------------------------
--Phoenix Game Engine: FileDialog sample˜ v 1.00 2012 (C) M4MSOFT
-----------------------------------------------------------------------------------------------------

--load the screen module.
require "screen"

--load the FileDialog module.
FileDialog = require "FileDialog"

--Set the FileDialog setting.
FileDialog.Index = 2 --The default FileDialog button. (File = 0, Filename = 1, Select = 2, Ok = 3, Cancel = 4)
FileDialog.FileName = "Untitled.lua" --the current selected FileName.
FileDialog.Path = System.currentDirectory() or "" --the current selected path.
FileDialog.Text = "File Dialog" --The title of the dialog.

--Set Application Data
local App_Running = true

--Set Colors
local white = Color.new(255, 255, 255, 224)
local black = Color.new(0, 0, 0, 255)
local red = Color.new(255, 0, 0, 208)
local yellow = Color.new(255, 218, 0, 224)

-----------------------------------------------------------------------------------------------------
-- Draw Function
local function drawFunc ()

	-- Clear the screen
	GL.Clear(black)

	--Print some text using the print function
	screen.print( 0, 0, "Phoenix Game Engine: FileDialog sample", white)
	screen.print( 5, 19, "Warning: The FileDialog can delete/overwrite files !", red)
	screen.print( 5, 35, "Press the X button to show the FileDialog.", yellow)
	screen.print( 0, 259, string.format("RunTime: %s", os.clock()), white)

	--Print the file dialog info.
	screen.print( 5, 63, "FileDialog:", yellow)
	screen.print( 10, 77, string.format("Button: %s", FileDialog.Button), yellow)
	screen.print( 10, 91, string.format("FileName: %s", FileDialog.FileName), yellow)
	screen.print( 10, 105, string.format("File: %s", tostring(FileDialog.isFile)), yellow)
	screen.print( 10, 119, string.format("Directory: %s", tostring(FileDialog.isDirectory)), yellow)
	screen.print( 10, 133, string.format("Exist: %s", tostring(FileDialog.Exist)), yellow)
	screen.print( 10, 147, string.format("ItemName: %s", FileDialog.SelectedItem.name), yellow)
	screen.print( 10, 161, string.format("IntSize: %s", FileDialog.SelectedItem.intSize), yellow)
	screen.print( 10, 175, string.format("Size: %s", string.sub(FileDialog.SelectedItem.size,3,-1)), yellow)
	screen.print( 10, 189, string.format("ftype: %s", FileDialog.SelectedItem.ftype), yellow)
	screen.print( 10, 203, string.format("MS: %s", tostring(FileDialog.blnMS)), yellow)

	if FileDialog.SelectedItem.icon then
		FileDialog.SelectedItem.icon:activate()
		FileDialog.SelectedItem.icon:draw(240, 168, 16*5, 16*5, 0, 0, 16, 16, 0, 255)
	end

end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--Application Main Loop.
while App_Running do
	--Exit the application on system request.
	if not System.running() then break end

	--Draw Screen
	drawFunc()

	--swap the buffer.
	GL.Flip()

	--update the controls
	Controls.readPeek()

	if Controls.press(8) then --start button pressed.
		--Break the main loop.
		App_Running = false
	elseif Controls.press(16384) then --cross button pressed.
		--Show the FileDialog and get the return value.
		local blnResult = FileDialog.show()

		if blnResult then --Ok button pressed.
			--Show the selected filename.
			Utility.msgShow(string.format("You have selected the %s: '%s' .",
										  FileDialog.isDirectory and "directory" or "file",
										  FileDialog.FullFileName),
							  0,
							  drawFunc)
		else --Cancel button pressed.
			--Show the message dialog.
			Utility.msgShow("The FileDialog has been cancel.",
							0,
							drawFunc)
		end

		--free the return value.
		blnResult = nil
	end

end
-----------------------------------------------------------------------------------------------------

--Dispose Application Data
App_Running = nil

--Dispose Colors
white = nil
black = nil
red = nil
yellow = nil

--Dispose Draw function.
drawFunc = nil

--Dispose the FileDialog module.
if FileDialog and FileDialog.dispose then
	FileDialog.dispose()
end

--Quit the application after the main loop breaks
--pge.exit()
