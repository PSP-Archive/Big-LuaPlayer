-----------------------------------------------------------------------------------------------------
-- Big LuaPlayer: FileDialog˜ Module v1.05 2012 (C) M4MSOFT
-----------------------------------------------------------------------------------------------------

-- Adds to the 'Array' module
module ("FileDialog", package.seeall)

_PACKAGE = nil
_M = nil
_NAME = nil
_VERSION = "1.05"

-----------------------------------------------------------------------------------------------------
--Create module settings.
Index = 2 --The default FileDialog button. (File = 0, Filename = 1, Select = 2, Ok = 3, Cancel = 4)
FileName = "Untitled.lua" --the current selected FileName.
Path = System.currentDirectory() or "" --the current selected path.
Text = "File Dialog" --The title of the dialog.
--Return values (Return values are read only.)
blnMS = (string.sub(Path,1,5) ~= "ms0:/")
Button = "Cancel"
FullFileName = "" --The complete last selected file name.
isFile = false
isDirectory = false
Exist = false --True if the item exist (This field is read only.)
SelectedItem = {name = "", intSize = 0, size = "", icon = false, ftype = "None"} --(This field is read only.)
-----------------------------------------------------------------------------------------------------

--Set Application Colors
local clrContolLightLight = Color.new( 255, 255, 255, 255)
local clrControlLight = Color.new( 241, 239, 226, 255)
local clrControl = Color.new( 236, 233, 216, 255)
local clrControlDark = Color.new( 172, 168, 153, 255)
local clrControlDarkDark = Color.new( 113, 111, 100, 255)
local clrControlText = Color.new( 0, 0, 0, 255)
local clrControlBorderLight = Color.new( 0, 72 ,241 , 255)
local clrControlBorderDark = Color.new( 0, 19, 140, 255)
local clrCaptionText = Color.new( 255, 255, 255, 255)
local clrGrayText = Color.new( 172, 168, 153, 255)
local clrWindow = Color.new( 255, 255, 255, 255)
local clrWindowText = Color.new( 0, 0, 0, 255)
local clrWindowFrame = Color.new( 127, 157, 185, 255)
local clrButtonFace = Color.new( 236, 233, 216, 255)
local clrButtonHighlight = Color.new( 255, 255, 255, 255)
local clrButtonShadow = Color.new( 172, 168, 153, 255)
local clrButtonBorden = Color.new( 0, 60, 116, 255)
local clrButtonSelected = Color.new( 255, 140, 0, 255)
local clrHighlight = Color.new( 49, 106, 197, 255)
local clrHighlightText = Color.new( 255, 255, 255, 255)
local clrInfo = Color.new( 255, 255, 225, 255)
local clrInfoText = Color.new( 0, 0, 0, 255)
local clrMenuBar = Color.new( 236, 233, 216, 255)
local clrMenuBack = Color.new( 241, 239, 226, 255)
local clrMenuDark = Color.new( 172, 168, 153, 255)
local clrMenuGrayText = Color.new( 172,168, 153, 255)
local clrMenuText = Color.new( 0, 0, 0, 255)
local clrScrollBarLight = Color.new( 255, 255, 255, 255)
local clrScrollBar= Color.new( 195, 213, 253, 255)
local clrScrollBarDark = Color.new( 77, 97, 133, 255)
local clrScrollBarBack = Color.new(212,208,200)

--Load Images
local strImagePath = System.Directory.eboot.."/System/Image"
local ImageDelete = Texture.load(strImagePath.."/Delete.png")
local ImageCopy = Texture.load(strImagePath.."/Copy.png")
local ImagePaste = Texture.load(strImagePath.."/Paste.png")
local ImageMap = Texture.load(strImagePath.."/Map.png")
local ImageDisc = Texture.load(strImagePath.."/Disc.png")
local ImageFlash = Texture.load(strImagePath.."/Flash.png")
local ImageMS = Texture.load(strImagePath.."/MS.png")
local ImageDefault = Texture.load(strImagePath.."/Default_File.png")
local ImageFont = Texture.load(strImagePath.."/Font_File.png")
local ImageImage = Texture.load(strImagePath.."/Image_File.png")
local ImageLua = Texture.load(strImagePath.."/Lua_File.png")
local ImageMedia = Texture.load(strImagePath.."/Media_File.png")
local ImageText = Texture.load(strImagePath.."/Text_File.png")
local ImageZip = Texture.load(strImagePath.."/Zip_File.png")

--Set Application Data
local App_Running = true
local App_MemoryOffset = 4194304 --4.0 mb
local App_LastIndex = {}
local App_drawStartIndex = 0
local App_SelectedIndex = 1
local App_Cursorlocation = 1
local App_showFileMenu = false
local App_FileMenu =   {
						 CreateFile = { text = "Create File" ,location = { x = 16 + 16 ,y = 29} ,size = { x = 9 * 20  ,y = 9} ,index = 0, image = false, enable = true},
						 CreateDirectory = { text = "Create Directory" ,location = { x = 16 + 16 ,y = 29 + 19 * 1} ,size = { x = 9 * 20  ,y = 9} ,index = 1, image = false, enable = true},
						 Unzip = { text = "Unzip File" ,location = { x = 16 + 16 ,y = 29 + 19 * 2} ,size = { x = 9 * 20  ,y = 9} ,index = 2, image = false, enable = false},
						 Copy = { text = "Copy Dir/File" ,location = { x = 16 + 16 ,y = 29 + 19 * 3} ,size = { x = 9 * 20  ,y = 9} ,index = 3, image = ImageCopy, enable = true},
						 Paste = { text = "Paste Dir/File" ,location = { x = 16 + 16 ,y = 29 + 19 * 4} ,size = { x = 9 * 20  ,y = 9} ,index = 4, image = ImagePaste, enable = false},
						 Delete = { text = "Delete Dir/File" ,location = { x = 16 + 16 ,y = 29 + 19* 5} ,size = { x = 9 * 20  ,y = 9} ,index = 5, image = ImageDelete, enable = true},
						 Rename = { text = "Rename Dir/File" ,location = { x = 16 + 16 ,y = 29 + 19 * 6} ,size = { x = 9 * 20  ,y = 9} ,index = 6, image = false, enable = true},
						 Exit = { text = "Exit" ,location = { x = 16 + 16 ,y = 29 + 19 * 7} ,size = { x = 9 * 20  ,y = 9} ,index = 7, image = false, enable = true}
						}
local App_MenuArray =  {
						 zFile = { text = "File:" ,location = { x = 5 ,y = 19} ,size = { x = 9 * 5  ,y = 14} ,index = 0 },
						 FileName = { text = "<FileName>" ,location = { x = 5 + 8 * 7 ,y = 19} ,size = { x = 9 * 10  ,y = 14} ,index = 1 },
						 Select = { text = "<Select>" ,location = { x = 5 + 8 * 19 ,y = 19} ,size = { x = 9 * 8 ,y = 14} ,index = 2 },
						 Ok = { text = "<Ok>" ,location = { x = 5 + 8 * 29 ,y = 19} ,size = { x = 9 * 4 ,y = 14} ,index = 3 },
						 Cancel = { text = "<Cancel>" ,location = { x = 5 + 8 * 35 ,y = 19} ,size = { x = 9 * 8 ,y = 14} ,index = 4 }
						}
local App_SelectedMenu = 2
local App_MenuIndex = 0
local App_FileName = "Untitled,lua"
local App_Path = System.currentDirectory()
local App_Seperator = "\\"

local App_ClipBoard = false

--Load ScrollBar Images
local BarDown = Texture.load(strImagePath.."/ScrollDown.png")
local BarUp = Texture.load(strImagePath.."/ScrollUp.png")
local Bar_Size = 178
local Bar_StepSize = 0
local Bar_Location = 39 + 5

-----------------------------------------------------------------------------------------------------
local SetBar = function (Value)
	Bar_Size = 178 * (11 / math.max( Value or 0, 11))
	Bar_Size = math.floor( Bar_Size)
	Bar_Size = math.max( Bar_Size, 9)
	if math.max( Value or 0, 11) > 11 then
		Bar_StepSize = (178 - Bar_Size) / ( math.max( Value or 0, 11) - 11)
	else
		Bar_StepSize = 0
	end
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--Function Draw Bar
local drawbar = function( x, y, height)
	--Set ScrollBar size
	x = math.floor(x)
	y = math.floor(y)
	height= math.max(height,9)
	y = math.max( y, 44)
	y = math.min( y, 44 + 178 - height)

	--Draw ScrollBar
	screen.fillRect(x + 1, y + 1, 9, height, clrScrollBarDark)
	screen.fillRect(x, y, 9, height, clrScrollBarLight)
	screen.fillRect(x + 1, y + 1, 7, height - 2, clrScrollBar)
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--Fuction ShowMesssage
local ShowMesssage = function (title, text)
	local title = title or "Message:"
	local text = text or "Please wait..."

	local intWidth = 0

	intWidth = math.max(string.len(title) * 9 + 16, string.len(text) * 9 + 16 ,150 + 20)
	screen.fillRect( (480 - intWidth) / 2 + 2, 119, intWidth, 39, clrControlDark)
	screen.fillRect( (480 - intWidth) / 2, 117, intWidth, 39, clrInfoText)
	screen.fillRect( (480 - intWidth) / 2 + 1, 118, intWidth - 2, 37, clrInfo)
	screen.print( math.floor((480 - 9 * string.len(title)) / 2), 122 , title, clrInfoText)
	screen.print( math.floor((480 - 9 * string.len(text)) / 2), 139 , text, clrInfoText)

	-- Flip buffers.
	GL.Flip()

	intWidth = nil

	title = nil
	Message = nil
end
-----------------------------------------------------------------------------------------------------

--set Application files
local intFileCount = 0
local Files = {}
local App_contentsDirectory = false

-----------------------------------------------------------------------------------------------------
--Set Menu
local SetMenu = function()
	if intFileCount > 0 then
		App_FileMenu.Rename.enable = true
		App_FileMenu.Delete.enable = true
		App_FileMenu.Unzip.enable = false --(Files[App_SelectedIndex].ftype == "Zip")
	else
		App_FileMenu.Delete.enable = false
		App_FileMenu.Rename.enable = false
		App_FileMenu.Unzip.enable = false
	end
	if App_ClipBoard then
		App_FileMenu.Paste.enable = true
	else
		App_FileMenu.Paste.enable = false
	end
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--CombinePath
CombinePath = function (strPath, strFilename)
	if string.sub(strPath,-1,-1) == "/" or strPath == "" then
	   return string.format("%s%s" , strPath, strFilename)
	else
	   return string.format("%s/%s" , strPath, strFilename)
	end
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
local SetCursor = function(n)
	--Select the next Dir/File
	App_SelectedIndex = math.min(n or 1, intFileCount)
	App_Cursorlocation  = math.min(App_SelectedIndex, 11)
	App_drawStartIndex = 0
	if intFileCount > 11 and App_Cursorlocation == 11 and App_SelectedIndex <= intFileCount then
		App_drawStartIndex = App_SelectedIndex - 11
	end
	Bar_Location = 44 + Bar_StepSize * App_drawStartIndex
	--Set Menu
	SetMenu()
	App_FileName = Files[App_SelectedIndex].name
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func Check_Directory: Check if the directory exist.
--  @param name: the Directory path and name
-- @returns
--   @param result: true or false
local Check_Directory = function(str)
	if string.sub(str,-1,-1) ~= "/" then str = string.format("%s/",str) end
	return next(System.listDirectory(str) or {}) and true or false
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--Load the device list
local Load_Dev = function()
	App_contentsDirectory = {
			   {directory = false, name = "disc0:/" , size = 0, iconZ = ImageDisc},
			   {directory = false, name = "flash0:/", size = 0, iconZ = ImageFlash},
			   {directory = false, name = "flash1:/" , size = 0, iconZ = ImageFlash},
			   {directory = false, name = "flash2:/", size = 0, iconZ = ImageFlash},
			   {directory = false, name = "ms0:/", size = 0, iconZ = ImageMS}
			  }
	for idx, dev in ipairs(App_contentsDirectory) do
		dev.directory = Check_Directory(dev.name)
	end
	local done = false
	while not done do -- filter out unwanted devices
		done = true
		for idx,dev in ipairs(App_contentsDirectory) do
			done = false
			if dev.directory == false then
				table.remove( App_contentsDirectory, idx)
				break
			end
			if idx == table.getn(App_contentsDirectory) then
				done = true
			end
		end
	end
	done = nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--Load the Filelist From a Dirictory
local Load_Directory = function(strPath)
	if string.sub(strPath,-1,-1) ~= "/" then strPath = string.format("%s/",strPath) end
	local CheckDir = (string.sub(strPath,1,5) ~= "ms0:/")
	App_contentsDirectory = false
	App_contentsDirectory = System.listDirectory(strPath) or false

	if not App_contentsDirectory or strPath == "/" then
		--Load the dev list
		Load_Dev()
		App_Path = ""
		strPath = ""
	end

	-- Filter out unwanted files.
	local done = false
	while not done do
		done = true
		for idx,file in ipairs(App_contentsDirectory) do
			done = false
			if file.name == "." or file.name == ".." then
				table.remove(App_contentsDirectory, idx)
				break
			end
			if idx == table.getn(App_contentsDirectory) then
				done = true
			end
		end
	end
	done = nil

	-- Sort the contents so sub-directories appear first sorted alphabetically and files appear after, sorted alphabetically.
	table.sort(App_contentsDirectory, function(a,b) if a.directory == b.directory then return a.name < b.name else return a.directory end end)

	intFileCount = 0
	App_drawStartIndex = 0
	App_SelectedIndex = 1
	App_Cursorlocation = 1
	Files = {}

	local intSize = 0
	for idx, file in ipairs(App_contentsDirectory) do
		intFileCount = intFileCount + 1
		Files[intFileCount] = {}
		--Set Name
		Files[intFileCount].name = file.name
		intSize = file.size or 0
		Files[intFileCount].intSize = intSize
		if intSize < 1000 then
			intSize = string.format(" - %d byte",intSize)
		elseif intSize < 1000 ^ 2 then
			intSize = string.format(" - %.1f kb",intSize/1024+0.05)
		elseif intSize < 1000 ^ 3 then
			intSize = string.format(" - %.1f mb",intSize/1024 ^ 2+0.05)
		else
			intSize = string.format(" - %.1f Gb",intSize/1024 ^ 3+0.05)
		end
		Files[intFileCount].size = intSize

		-- defaults
		Files[intFileCount].icon = nil
		Files[intFileCount].ftype = "None"

		if file.directory and not CheckDir or (CheckDir and Check_Directory(strPath..file.name)) then
			-- DEFINE AND SET DIR TYPES
			Files[intFileCount].icon = file.iconZ or ImageMap
			Files[intFileCount].ftype = "Dir"
			Files[intFileCount].size = ""
		else -- is a file
			-- DEFINE AND SET FILE TYPES
			if string.lower(string.sub(file.name, -4)) == ".lua" then
				Files[intFileCount].icon = ImageLua
				Files[intFileCount].ftype = "Text"
			elseif string.lower(string.sub(file.name, -4)) == ".txt" or string.lower(string.sub(file.name, -4)) == ".inf" then
				Files[intFileCount].icon = ImageText
				Files[intFileCount].ftype = "Text"
			elseif string.lower(string.sub(file.name, -4)) == ".png" or string.lower(string.sub(file.name, -4)) == ".bmp" or
					string.lower(string.sub(file.name, -4)) == ".jpg" or string.lower(string.sub(file.name, -4)) == ".tga"  or
					string.lower(string.sub(file.name, -4)) == ".tif" or string.lower(string.sub(file.name, -4)) == ".gif" or
					string.lower(string.sub(file.name, -4)) == ".bml" then
				Files[intFileCount].icon = ImageImage
				Files[intFileCount].ftype = "Image"
			elseif string.lower(string.sub(file.name, -4)) == ".wav" or string.lower(string.sub(file.name, -4)) == ".mp4" or
					string.lower(string.sub(file.name, -4)) == ".mp3" or string.lower(string.sub(file.name, -4)) == ".at3" or
					string.lower(string.sub(file.name, -4)) == ".mp2" or string.lower(string.sub(file.name, -4)) == ".ogg" or
					string.lower(string.sub(file.name, -4)) == ".mpg" then
				Files[intFileCount].icon = ImageMedia
				Files[intFileCount].ftype = "Media"
			elseif string.lower(string.sub(file.name, -4)) == ".pgf" or string.lower(string.sub(file.name, -4)) == ".ttf" then
				Files[intFileCount].icon = ImageFont
				Files[intFileCount].ftype = "Font"
			elseif string.lower(string.sub(file.name, -4)) == ".zip" then
				Files[intFileCount].icon = ImageZip
				Files[intFileCount].ftype = "Zip"
			else
				Files[intFileCount].icon = ImageDefault
				Files[intFileCount].ftype = "All"
			end
		end -- setting icon

	end -- loop through dir()
	intSize = nil
	CheckDir = nil
	if intFileCount > 0 and App_SelectedMenu ~= 1 then App_FileName = Files[1].name end
	--reset the scrollBar
	SetBar(intFileCount)
	--Set Menu
	SetMenu()
	return "Load Complete"
end -- function call
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--CkeckFileName
local CheckFileName = function (strFileName)
	local blnResult = false
	if string.find(strFileName,":") == nil and string.find(strFileName,"*") == nil and
	   string.find(strFileName,"?") == nil and string.find(strFileName,'"') == nil and
	   string.find(strFileName,"<") == nil and string.find(strFileName,'>') == nil and
	   string.find(strFileName,"/") == nil and string.find(strFileName,"|") == nil and
	   string.find(strFileName, App_Seperator) == nil then
		blnResult = true
	end
	return blnResult
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--CkeckFileNameExsist
local CheckFileNameExist = function (strFileName)
	local blnResult = false
	if intFileCount > 0 then
		for n = 1, intFileCount do
			if string.lower(Files[n].name) == string.lower(strFileName) then
				blnResult = true
				break
			end
		end
	end
	return blnResult
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--Draw Function
local drawFunc = function ()

	-- Clear The Screen.
	GL.Clear(clrControl)

	--Draw Main Window
	screen.fillRect( 0, 0, 480, 272, clrControlBorderDark)
	screen.fillRect( 1, 1, 480 - 2, 272 - 2, clrControlBorderLight)
	screen.drawLine( 2, 15, 480 - 3, 15, clrControlBorderDark)
	screen.fillRect( 2, 16, 480 - 4, 272 - 18, clrControl)
	screen.drawLine( 2, 16, 480 - 3, 16, clrControlLight)
	screen.drawLine( 2, 16, 2, 272 - 3, clrControlLight)
	screen.drawLine( 2, 272 - 3, 480 - 3, 272 - 3, clrControlDark)
	screen.drawLine( 480 - 3, 272 - 3, 480 - 3, 16, clrControlDark)
	screen.drawLine( 2, 272 - 13 -3, 480-3 ,272-13 -3 , clrControlDark)
	screen.print( 3, 272 - 11 -3, string.format("Path: %s",App_Path), clrControlText)

	--Draw ScrollBar Window
	screen.fillRect( 480 - 12, 35, 9, 272 - 76, clrScrollBarBack)
	screen.blit( 468, 35, BarUp)
	screen.blit( 468, 228 - 6, BarDown)
	drawbar( 468, Bar_Location , Bar_Size)

	--Draw RichTextBox
	screen.fillRect( 3, 35, 480 - 15, 272 - 76, clrWindowFrame)
	screen.fillRect( 4, 36 , 480 - 17, 272 - 78, clrWindow)

	--Draw Dir's/File's into the TextBox
	for n = 1, math.min( intFileCount, 11) do--App_TextLines do
		if n + App_drawStartIndex == App_SelectedIndex then
			screen.fillRect( 5 + 17, 40 + 17 * ( n - 1 ), 480 - 38, 17, clrHighlight)
			screen.print( 5 + 20 , 42 + 17 * ( n - 1 ), Files[n + App_drawStartIndex].name..Files[n + App_drawStartIndex].size, clrHighlightText)
		else
			screen.print( 5 + 20 , 42 + 17 * ( n - 1 ), Files[n + App_drawStartIndex].name..Files[n + App_drawStartIndex].size, clrWindowText)
		end
		screen.blit( 5, 38 + 17 * ( n - 1 ), Files[n + App_drawStartIndex].icon)
	end

	--Draw TextBox File Name
	screen.print( 7, 245 - 6,"File Name:", clrControlText)
	screen.fillRect( 7 + 9 * 10, 242 - 6, 480 - 105, 16, clrWindowFrame)
	screen.fillRect( 8 + 9 * 10, 243 - 6, 480 - 107, 14, clrWindow)
	screen.blit( 459, 239, BarDown)
	screen.print( 8 + 9 * 10, 245 - 6, App_FileName, clrWindowText)

	--Draw Menu
	screen.fillRect( 2, 16, 480 - 4, 17, clrMenuBar)
	screen.drawLine( 2, 33, 480 - 3, 33 , clrMenuDark)

	for idx ,Menu in pairs(App_MenuArray) do
		if App_SelectedMenu == Menu.index then
			screen.fillRect( Menu.location.x - 2 , Menu.location.y - 2, Menu.size.x + 1, Menu.size.y + 1, clrHighlight)
			screen.print( Menu.location.x, Menu.location.y, Menu.text, clrHighlightText)
		else
			screen.print( Menu.location.x, Menu.location.y, Menu.text, clrMenuText)
		end
	end

	--Draw File Menu
	if App_showFileMenu == true then
		for idx , Menu in pairs(App_FileMenu) do
			screen.fillRect( Menu.location.x - 21 , Menu.location.y - 4 , Menu.size.x + 30, Menu.size.y + 11, clrMenuDark)
			screen.fillRect( Menu.location.x - 20 , Menu.location.y - 3 , Menu.size.x + 28, Menu.size.y + 9, clrMenuBack)
			if not Menu.image == false and Menu.enable == true then
				screen.blit( Menu.location.x - 18 , Menu.location.y - 2 , Menu.image)
			elseif not Menu.image == false then
				screen.blit( Menu.location.x - 18 , Menu.location.y - 2 , Menu.image, 64)
			end
			if App_MenuIndex == Menu.index then
				screen.fillRect( Menu.location.x , Menu.location.y -1, Menu.size.x + 6, Menu.size.y + 5, clrHighlight)
				if Menu.enable == true then
					screen.print( Menu.location.x + 2, Menu.location.y + 2-2, Menu.text, clrHighlightText)
				else
					screen.print( Menu.location.x + 2, Menu.location.y + 2-2, Menu.text, clrGrayText)
				end
			else
				screen.fillRect( Menu.location.x, Menu.location.y -1, Menu.size.x + 6, Menu.size.y + 5, clrMenuBack)
				if Menu.enable == true then
					screen.print( Menu.location.x + 2, Menu.location.y + 2-2, Menu.text, clrMenuText)
				else
					screen.print( Menu.location.x + 2, Menu.location.y + 2-2, Menu.text, clrGrayText)
				end
			end
		end
	end

	--Print Window Text
	if collectgarbage("step") then collectgarbage("collect") end
	screen.print( 4, 2, string.format("%s - %d kb", Text, collectgarbage("count")), clrCaptionText)

end
-----------------------------------------------------------------------------------------------------


-----------------------------------------------------------------------------------------------------
--CopyDir
local function CopyDir(strPath, strDir)
	local strFullPath = CombinePath(strPath, strDir)
	if string.sub(strFullPath,-1,-1) ~= "/" then strFullPath = string.format("%s/",strFullPath) end
	local dirContent = System.listDirectory(strFullPath) or false
	local CheckDir = (string.sub(strPath,1,5) ~= "ms0:/")
	if dirContent then
		-- Filter out unwanted files.
		local done = false
		while not done do
			done = true
			for idx,file in ipairs(dirContent) do
				done = false
				if file.name == "." or file.name == ".." then
					table.remove(dirContent, idx)
					break
				end
				if idx == table.getn(dirContent) then
					done = true
				end
			end
		end
		done = nil
		if type(App_ClipBoard) ~= "table" then
			App_ClipBoard = {}
		end
		table.insert(App_ClipBoard,
			{ data = false,
			  name = strDir,
			  cd = true })
		for idx, file in ipairs(dirContent) do
			if file.directory and not CheckDir or (CheckDir and Check_Directory(strFullPath..file.name)) then
				CopyDir(strPath, CombinePath(strDir,file.name))
			else
				local intRam = (System.Memory.freeRam() or 0) - App_MemoryOffset
				if intRam >= (file.size or 0) then
					local ioFile = io.open( strFullPath..file.name, "rb")
					if ioFile then
						local tblClip = { data = {},
										  name = CombinePath(strDir,file.name),
										  cd = false }
						table.insert(App_ClipBoard, tblClip)
						local dataBlock = tblClip.data
						local intBlock = 0
						repeat
							intBlock = intBlock + 1
							dataBlock[intBlock] = ioFile:read(262144)
						until not dataBlock[intBlock]
						intBlock = nil
						dataBlock = nil
						tblClip = nil
						ioFile:close()
					end
					ioFile = nil
				end
				intRam = nil
			end
		end
	end
	CheckDir = nil
	dirContent = nil
	strFullPath = nil
	if collectgarbage("step") then collectgarbage("collect") end
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--DeleteDir
local function DeleteDir(strPath, strDir)
	local strFullPath = CombinePath(strPath, strDir)
	if string.sub(strFullPath,-1,-1) ~= "/" then strFullPath = string.format("%s/",strFullPath) end
	local dirContent = System.listDirectory( strFullPath ) or false
	local CheckDir = (string.sub(strPath,1,5) ~= "ms0:/")
	if dirContent then
		-- Filter out unwanted files.
		local done = false
		while not done do
			done = true
			for idx,file in ipairs(dirContent) do
				done = false
				if file.name == "." or file.name == ".." then
					table.remove(dirContent, idx)
					break
				end
				if idx == table.getn(dirContent) then
					done = true
				end
			end
		end
		done = nil
		for idx, file in ipairs(dirContent) do
			if file.directory and not CheckDir or (CheckDir and Check_Directory(strFullPath..file.name)) then
				DeleteDir(strFullPath, file.name)
				System.removeDirectory(strFullPath..file.name)
			else
				System.removeFile(strFullPath..file.name)
			end
		end
	end
	CheckDir = nil
	dirContent = nil
	strFullPath = nil
	if collectgarbage("step") then collectgarbage("collect") end
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--PasteData
local function PasteData(strPath)
	local CheckDir = (string.sub(strPath,1,5) ~= "ms0:/")
	local strFullPath = ""
	local blnResult = false
	if type(App_ClipBoard) == "table" then
		for idx, file in ipairs(App_ClipBoard) do
			strFullPath = CombinePath(strPath, file.name)
			if file.cd then
				if not CheckDir then
					System.createDirectory(strFullPath)
				elseif not Check_Directory(strFullPath) then
					System.createDirectory(strFullPath)
				end
				blnResult = true
			elseif file.data then
				local ioFile = io.open(strFullPath, "wb")
				if ioFile then
					blnResult = true
					local dataBlock = file.data
					local intBlock = 1
					while dataBlock[intBlock] do
						ioFile:write(dataBlock[intBlock])
						intBlock = intBlock + 1
					end
					intBlock = nil
					dataBlock = nil
					ioFile:close()
				end
				ioFile = nil
			end
		end
	end
	strFullPath = nil
	CheckDir = nil
	return blnResult
end
-----------------------------------------------------------------------------------------------------

--Control Data
local padcount = 0

-----------------------------------------------------------------------------------------------------
-- @func show: remove the module.
--  @param : takes no arguments
-- @returns
--   @param dlgResult: true or false
function show()
	local dlgResult = false
	Button = "Cancel"

	--Load the currentDirectory
	App_Running = true
	App_LastIndex = {}
	App_showFileMenu = false
	App_SelectedMenu = Index or 2
	App_MenuIndex = 0
	App_FileName = FileName or "Untitled.lua"
	FileName = App_FileName
	App_Path = Path or System.currentDirectory()
	Path = App_Path
	if string.find(App_Path,"/") == nil and App_Path ~= "" then
		App_Path = string.format("%s/" , App_Path)
	end
	local bln_error, result = pcall(Load_Directory, App_Path)
	if not bln_error then --Catch Error
		App_Path = ""
		Load_Directory(App_Path)
		App_LastIndex = {}
	end
	bln_error = nil
	result = nil

	FullFileName = CombinePath(Path, FileName)
	blnMS = (string.sub(FullFileName,1,5) == "ms0:/")
	Exist = CheckFileNameExist(FileName)
	isFile =  false
	isDirectory = false
	if Exist and intFileCount ~= 0 then
		SelectedItem = Files[App_SelectedIndex] or {}
		if SelectedItem.ftype == "Dir" then isDirectory = true else isFile = true end
	else
		SelectedItem = {name = "", intSize = 0, size = "", icon = false, ftype = "None"}
	end

	--Apllication Main Loop
	while App_Running do
		--Exit the application on system request.
		if not System.running() then break end

		-- Draw Screen.
		drawFunc()

		-- Flip buffers.
		GL.Flip()

		-- Slow down the Application if a button is pressed.
		if Controls.buttons() > 0 and padcount > 0 then
			padcount = padcount - 1
			System.sleep(70)
		elseif padcount ~= 10 and Controls.buttons() == 0 then
			padcount = 10
		end

		-- Update the controls.
		Controls.readPeek()

		if Controls.press(8) then --start Button Pressed
			--Break The Main Loop
			dlgResult = false
			Button = "Cancel"
			App_Running = false
			System.sleep(70)
		elseif Controls.press(1) then --select Button Pressed
			App_showFileMenu = false
			App_SelectedMenu = 2
			System.sleep(160)
		elseif Controls.buttons(256) then --ltrigger Button Pressed
			 --Hide File Menu
			if App_showFileMenu == true then App_showFileMenu = false end
			App_SelectedMenu = App_SelectedMenu - 1
			if App_SelectedMenu < 0 then App_SelectedMenu = 4 end
			System.sleep(70)
		elseif Controls.buttons(512) then --rtrigger Button Pressed
			--Hide File Menu
			if App_showFileMenu == true then App_showFileMenu = false end
			App_SelectedMenu = App_SelectedMenu + 1
			if App_SelectedMenu > 4 then App_SelectedMenu = 0 end
			System.sleep(70)
		elseif Controls.buttons(16) then --up Button Pressed
			if App_showFileMenu == true then --File Menu
				App_MenuIndex = App_MenuIndex - 1
				if App_MenuIndex < 0 then App_MenuIndex = 7 end
				System.sleep(70)
			elseif App_SelectedMenu == 2 and intFileCount ~= 0 then --<Select>
				--Select the next Dir/File
				App_SelectedIndex = App_SelectedIndex - 1
				App_Cursorlocation = App_Cursorlocation - 1
				App_Cursorlocation = math.max(App_Cursorlocation, 1)
				if App_SelectedIndex < 1 then
					App_SelectedIndex = intFileCount
					App_Cursorlocation = math.min(intFileCount, 11)
					App_drawStartIndex = App_SelectedIndex - math.min(11, intFileCount)
					Bar_Location = 44 + Bar_StepSize * App_drawStartIndex
				end
				if intFileCount > 11 and App_Cursorlocation == 1 and App_SelectedIndex >= 1 then
					App_drawStartIndex = App_SelectedIndex - 1
					Bar_Location = 44 + Bar_StepSize * App_drawStartIndex
				end
				--Set Menu
				SetMenu()
				System.sleep(35)
				App_FileName = Files[App_SelectedIndex].name
			end
		elseif Controls.buttons(64) then --down Button Pressed
			if App_showFileMenu == true then --File Menu
				App_MenuIndex = App_MenuIndex + 1
				if App_MenuIndex > 7 then App_MenuIndex = 0 end
				System.sleep(70)
			elseif App_SelectedMenu == 2 and intFileCount ~= 0 then --<Select>
				--Select the next Dir/File
				App_SelectedIndex = App_SelectedIndex + 1
				App_Cursorlocation = App_Cursorlocation + 1
				App_Cursorlocation = math.min(App_Cursorlocation, 11)
				if App_SelectedIndex > intFileCount then
					App_SelectedIndex = 1
					App_Cursorlocation = 1
					App_drawStartIndex = 0
					Bar_Location = 44 + Bar_StepSize * App_drawStartIndex
				end
				if intFileCount > 11 and App_Cursorlocation == 11 and App_SelectedIndex <= intFileCount then
					App_drawStartIndex = App_SelectedIndex - 11
					Bar_Location = 44 + Bar_StepSize * App_drawStartIndex
				end
				--Set Menu
				SetMenu()
				System.sleep(35)
				App_FileName = Files[App_SelectedIndex].name
			end
		elseif Controls.buttons(128) then --left Button Pressed
			if App_SelectedMenu == 2 then --<Select>
				--Load_Directory
				if string.sub(App_Path,-1,-1) == "/" then
					App_Path = ""
					Load_Directory(App_Path)
					System.sleep(160)
				elseif string.find(App_Path,"/") ~= nil then
					--App_Path = string.reverse(string.sub(string.reverse(App_Path),string.find(string.reverse(App_Path),"/") + 1,-1))
					App_Path = string.gsub(App_Path, "(.+)/(.+)","%1") or ""
					if string.find(App_Path,"/") == nil and App_Path ~= "" then
						App_Path = string.format("%s/" , App_Path)
					end
					Load_Directory(App_Path)
					SetCursor(App_LastIndex[1] or 1)
					table.remove(App_LastIndex, 1)
					System.sleep(160)
				end
			end
		elseif Controls.buttons(32) then --right Button Pressed
			if App_SelectedMenu == 2 then --<Select>
				--Load_Directory
				if intFileCount ~= 0 then
					if Files[App_SelectedIndex].ftype == "Dir" then
						App_Path = CombinePath(App_Path, Files[App_SelectedIndex].name)
						table.insert(App_LastIndex, 1, App_SelectedIndex)
						Load_Directory(App_Path)
						System.sleep(160)
					end
				end
			end
		elseif Controls.press(16384) then --cross Button Pressed
			--Show File Menu if App_SelectedMenu == 0
			if App_SelectedMenu  == 0 and App_showFileMenu == false then
				App_showFileMenu = true
				App_MenuIndex = 0
				--Draw Screen
				drawFunc()
				-- Flip buffers.
				GL.Flip()
				System.sleep(160)
			elseif App_showFileMenu == true then
				if App_MenuIndex == 0 and App_FileMenu.CreateFile.enable then --Create File
					--Shows sony's OSK and ask the user to change the file Name
					local result, new_FileName = Utility.oskShow("Enter a File Name:","New File.txt", 0)
					new_FileName = new_FileName or "New File.txt"
					--Check Result
					if result ~= Utility.OSK_RESULT_CANCELLED then
						--Create a new File.
						if CheckFileNameExist(new_FileName) == false and CheckFileName(new_FileName) then
							Utility.msgShow(string.format("File Name: '%s'",new_FileName),0,drawFunc)
							file = io.open(CombinePath(App_Path, new_FileName), "a+")
							file:close()
							file = nil
							Load_Directory(App_Path)
						elseif CheckFileName(new_FileName) == false then --FileName incorect
							Utility.msgShow(string.format('The file name is no valid file name.\nIncorect symbols(%s/:*?"<>|)',App_Seperator),0,drawFunc)
						else --File Already exist.
							Utility.msgShow("The file already exist.",0,drawFunc)
						end
					end
					new_FileName, result = nil,nil
					App_showFileMenu = false
					App_SelectedMenu = 2
					System.sleep(160)
				elseif App_MenuIndex == 1 and App_FileMenu.CreateDirectory.enable then --Create Directory
					--Shows sony's OSK and ask the user to change the Directory Name
					local result, new_FileName = Utility.oskShow("Enter a Directory Name:","New Directory", 0)
					new_FileName = new_FileName or "New Directory"
					--Check Result
					if result ~= Utility.OSK_RESULT_CANCELLED then
						--Create a new Directory.
						if CheckFileNameExist(new_FileName) == false and CheckFileName(new_FileName) then
							Utility.msgShow(string.format("Directory Name: '%s'",new_FileName),0,drawFunc)
							System.createDirectory(CombinePath(App_Path, new_FileName))
							Load_Directory(App_Path)
						elseif CheckFileName(new_FileName) == false then --FileName incorect
							Utility.msgShow(string.format('The file name is no valid file name.\nIncorect symbols(%s/:*?"<>|)',App_Seperator),0,drawFunc)
						else --File Already exist.
							Utility.msgShow("The directory already exist.",0,drawFunc)
						end
					end
					new_FileName, result = nil,nil
					App_showFileMenu = false
					App_SelectedMenu = 2
					System.sleep(160)
				elseif App_MenuIndex == 2 and App_FileMenu.Unzip.enable then --Unzip file
					if Files[App_SelectedIndex].ftype == "Zip" then
						local zipExtracted = false
						local zipFile = pge.zip.open(CombinePath(App_Path, Files[App_SelectedIndex].name))
						if zipFile then
							local strPath = System.currentDirectory()
							System.currentDirectory(App_Path)
							ShowMesssage("Message:","Unzipping the file, please wait...")
							zipExtracted = zipFile:extract()
							zipFile:close()
							System.currentDirectory(strPath)
							strPath = nil
						end
						zipFile = nil
						if not zipExtracted then
							--Show The Error Message.
							ShowMesssage("Invalid zip file:","Can't unzip the file")
							while System.running() do
								--update the controls
								if collectgarbage("step") then collectgarbage("collect") end
								Controls.read()
								if Controls.press() > 0 then
									break
								end
							end
						else
							Load_Directory(App_Path)
						end
						zipExtracted = nil
						App_showFileMenu = false
						App_SelectedMenu = 2
						System.sleep(160)
						--Set Menu
						SetMenu()
					end
				elseif App_MenuIndex == 3 and intFileCount ~= 0 and App_FileMenu.Copy.enable then --Copy Dir/File
					if Files[App_SelectedIndex].ftype ~= "Dir" then
						App_ClipBoard = false
						collectgarbage("collect")
						drawFunc()
						GL.Flip()
						drawFunc()
						GL.Flip()
						ShowMesssage("Message:","Coping the file, please wait...")
						local intRam = (System.Memory.freeRam() or 0) - App_MemoryOffset
						if intRam >= Files[App_SelectedIndex].intSize then
							--Copy the file to the ClipBoard
							local file = io.open( CombinePath(App_Path, Files[App_SelectedIndex].name), "rb")
							if file then
								App_ClipBoard = {}
								App_ClipBoard[1] = { data = {},
													 name = Files[App_SelectedIndex].name,
													 cd = false }
								local dataBlock = App_ClipBoard[1].data
								local intBlock = 0
								repeat
									intBlock = intBlock + 1
									dataBlock[intBlock] = file:read(262144)
								until not dataBlock[intBlock]
								intBlock = nil
								dataBlock = nil
								file:close()
							end
							file = nil
						else
							--Show The Error Message.
							ShowMesssage("Out of memory:","Can't copy the file.")
							while System.running() do
								--update the controls
								if collectgarbage("step") then collectgarbage("collect") end
								Controls.read()
								if Controls.press() > 0 then
									break
								end
							end
						end
						intRam = nil
					elseif Files[App_SelectedIndex].ftype == "Dir" then
						App_ClipBoard = false
						collectgarbage("collect")
						drawFunc()
						GL.Flip()
						drawFunc()
						GL.Flip()
						ShowMesssage("Message:","Coping the directory, please wait...")
						--Copy the dir to the ClipBoard
						CopyDir(App_Path, Files[App_SelectedIndex].name)
					end
					App_showFileMenu = false
					App_SelectedMenu = 2
					System.sleep(160)
					--Set Menu
					SetMenu()
				elseif App_MenuIndex == 4 and App_FileMenu.Paste.enable then --Paste Dir/File
					local blnCheck = true
					if CheckFileNameExist(App_ClipBoard[1].name) then
						blnCheck = false
						if Utility.msgShow(string.format("Confirm Action:\nThe dir/file already exist.\nDo you want to replace the dir/file: '%s'", App_ClipBoard[1].name),
										   Utility.MSG_BTN_YESMO + Utility.MSG_DEFAULT_BTN_NO, drawFunc) then
							blnCheck = true
						end
					end
					if blnCheck then
						App_showFileMenu = false
						App_SelectedMenu = 2
						System.sleep(160)
						ShowMesssage("Message:","Pasting the dir/file(s), please wait...")
						if PasteData(App_Path) then
							Load_Directory(App_Path)
						end
						--Set Menu
						SetMenu()
					end
					blnCheck = nil
				elseif App_MenuIndex == 5 and intFileCount ~= 0 and App_FileMenu.Delete.enable then --Delete File
					-- Delete a Dir/File (Also delete all content of a Dir !!!)
					if Files[App_SelectedIndex].ftype ~= "Dir" then
						local blnCheck = false
						if Utility.msgShow(string.format("Confirm Action:\nDo you want to delete the file: '%s'", Files[App_SelectedIndex].name),
										   Utility.MSG_BTN_YESMO + Utility.MSG_DEFAULT_BTN_NO, drawFunc) then
							blnCheck = true
						end
						if blnCheck then
							ShowMesssage("Message:","Deleting the file, please wait...")
							System.removeFile(CombinePath(App_Path, Files[App_SelectedIndex].name))
							Load_Directory(App_Path)
							App_showFileMenu = false
							App_SelectedMenu = 2
							System.sleep(160)
						end
						blnCheck = nil
					elseif Files[App_SelectedIndex].ftype == "Dir" then
						local blnCheck = false
						if Utility.msgShow(string.format("Confirm Action:\nDo you want to delete the directory: '%s'", Files[App_SelectedIndex].name),
										   Utility.MSG_BTN_YESMO + Utility.MSG_DEFAULT_BTN_NO, drawFunc) then
							blnCheck = true
						end
						if blnCheck then
							ShowMesssage("Message:","Deleting the dir/file(s), please wait...")
							DeleteDir(App_Path, Files[App_SelectedIndex].name)
							System.removeDirectory( CombinePath(App_Path, Files[App_SelectedIndex].name))
							Load_Directory(App_Path)
							App_showFileMenu = false
							App_SelectedMenu = 2
							System.sleep(160)
						end
						blnCheck = nil
					end
				elseif App_MenuIndex == 6 and intFileCount ~= 0 and App_FileMenu.Rename.enable then --Rename Dir/File
					--Shows sony's OSK and ask the user to change the Name
					local result, new_FileName = Utility.oskShow("Enter New Name:",Files[App_SelectedIndex].name, 0)
					new_FileName = new_FileName or "New Name"
					--Check Result
					if result ~= Utility.OSK_RESULT_CANCELLED then
						--Create a new Directory.
						if CheckFileNameExist(new_FileName) == false and CheckFileName(new_FileName) then
							Utility.msgShow(string.format("New Name: '%s'",new_FileName),0,drawFunc)
							os.rename(CombinePath(App_Path, Files[App_SelectedIndex].name), CombinePath(App_Path, new_FileName))
							Load_Directory(App_Path)
						elseif CheckFileName(new_FileName) == false then --FileName incorect
							Utility.msgShow(string.format('The file name is no valid file name.\nIncorect symbols(%s/:*?"<>|)',App_Seperator),0,drawFunc)
						else --File Already exist.
							Utility.msgShow("The file already exist.",0,drawFunc)
						end
					end
					new_FileName, result = nil,nil
					App_showFileMenu = false
					App_SelectedMenu = 2
					System.sleep(160)
				elseif App_MenuIndex == 7 and App_FileMenu.Exit.enable then --Exit
					--Return result Break Main Loop
					dlgResult = false
					Button = "Cancel"
					App_Running = false
					System.sleep(70)
				end
			elseif App_SelectedMenu == 1 then --<FileName>
				--Shows sony's OSK and ask the user to change the Directory Name
				local result, new_FileName = Utility.oskShow("Enter a File Name:",App_FileName, 0)
				new_FileName = new_FileName or "script.lua"
				--Check Result
				if result ~= Utility.OSK_RESULT_CANCELLED then
					--Create a new Directory.
					if CheckFileName(new_FileName) then
						App_FileName = string.format("%s%s", new_FileName ,"")
					else --FileName incorect
						Utility.msgShow(string.format('The file name is no valid file name.\nIncorect symbols(%s/:*?"<>|)',App_Seperator),0,drawFunc)
					end
				end
				new_FileName, result = nil,nil
				System.sleep(160)
			elseif App_SelectedMenu == 2  then --<Select>
				 --Load_Directory
				if intFileCount ~= 0 then
					if Files[App_SelectedIndex].ftype == "Dir" then
						App_Path = CombinePath(App_Path, Files[App_SelectedIndex].name)
						table.insert(App_LastIndex, 1, App_SelectedIndex)
						Load_Directory(App_Path)
						System.sleep(160)
					end
				end
			elseif App_SelectedMenu == 3 then --<Ok>
				--Return result Break Main Loop
				dlgResult = true
				Button = "Ok"
				FileName = App_FileName
				Path = App_Path
				FullFileName = CombinePath(App_Path, App_FileName)
				blnMS = (string.sub(FullFileName,1,5) == "ms0:/")
				Exist = CheckFileNameExist(App_FileName)
				isFile =  false
				isDirectory = false
				if Exist and intFileCount ~= 0 then
					SelectedItem = Files[App_SelectedIndex] or {}
					if SelectedItem.ftype == "Dir" then isDirectory = true else isFile = true end
				else
					SelectedItem = {name = "", intSize = 0, size = "", icon = false, ftype = "None"}
				end
				App_Running = false
				System.sleep(70)
			elseif App_SelectedMenu == 4 then --<Cancel>
				--Return result Break Main Loop
				dlgResult = false
				Button = "Cancel"
				App_Running = false
				System.sleep(70)
			end
		elseif Controls.press(8192) then --circle Button Pressed
			--Hide File Menu
			if App_showFileMenu == true then
				App_showFileMenu = false
				App_SelectedMenu = 2
				System.sleep(160)
			elseif App_SelectedMenu  == 2 then --<Select>
				--Load_Directory
				if string.sub(App_Path,-1,-1) == "/" then
					App_Path = ""
					Load_Directory(App_Path)
					System.sleep(160)
				elseif string.find(App_Path,"/") ~= nil then
					--App_Path = string.reverse(string.sub(string.reverse(App_Path),string.find(string.reverse(App_Path),"/") + 1,-1))
					App_Path = string.gsub(App_Path, "(.+)/(.+)","%1") or ""
					if string.find(App_Path,"/") == nil and App_Path ~= "" then
						App_Path = string.format("%s/" , App_Path)
					end
					Load_Directory(App_Path)
					SetCursor(App_LastIndex[1] or 1)
					table.remove(App_LastIndex, 1)
					System.sleep(160)
				end
			end
		elseif Controls.press(4096) then --triangle Button Pressed
			--Show the file Menu
			if App_showFileMenu == false then
				App_showFileMenu = true
				App_SelectedMenu  = 0
				App_MenuIndex = 0
				System.sleep(70)
			end
		end

	end

	--free the clipBoard data
	App_ClipBoard = false
	App_FileMenu.Paste.enable = false
	collectgarbage("collect")

	return dlgResult
end
-----------------------------------------------------------------------------------------------------


----------------------------------------------------------------------------------------------------
local tblDispose = {}
tblDispose.Color = function()
	clrContolLightLight = nil
	clrControlLight = nil
	clrControl = nil
	clrControlDark = nil
	clrControlDarkDark = nil
	clrControlText = nil
	clrControlBorderLight = nil
	clrControlBorderDark = nil
	clrCaptionText = nil
	clrGrayText = nil
	clrWindow = nil
	clrWindowText = nil
	clrWindowFrame = nil
	clrButtonFace = nil
	clrButtonHighlight = nil
	clrButtonShadow = nil
	clrButtonBorden = nil
	clrButtonSelected = nil
	clrHighlight = nil
	clrHighlightText = nil
	clrInfo = nil
	clrInfoText = nil
	clrMenuBar = nil
	clrMenu = nil
	clrMenuDark = nil
	clrMenuGrayText = nil
	clrMenuText = nil
	clrScrollBarLight = nil
	clrScrollBar = nil
	clrScrollBarDark = nil
	clrScrollBarBack = nil
	return nil
end
tblDispose.Image = function()
	strImagePath = nil
	ImageDelete = nil
	ImageCopy = nil
	ImagePaste = nil
	ImageMap = nil
	ImageDisc = nil
	ImageMS = nil
	ImageFlash = nil
	ImageDefault = nil
	ImageFont = nil
	ImageImage = nil
	ImageLua = nil
	ImageMedia = nil
	ImageText = nil
	ImageZip = nil
	BarDown = nil
	BarUp = nil
end
tblDispose.App = function()
	App_Running = nil
	App_MemoryOffset = nil
	App_LastIndex = nil
	App_drawStartIndex = nil
	App_SelectedIndex = nil
	App_Cursorlocation = nil
	App_showFileMenu = nil
	App_FileMenu = nil
	App_MenuArray = nil
	App_SelectedMenu = nil
	App_MenuIndex = nil
	App_FileName = nil
	App_Path = nil
	App_ClipBoard = nil
end
----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func dispose: remove the module.
--  @param : takes no arguments
-- @returns
--   @param nil
function dispose()

	FileDialog.CombinePath = nil
	FileDialog.show = nil
	FileDialog.dispose = nil

	--Dipose the module settings
	FileDialog.Index = nil
	FileDialog.FileName = nil
	FileDialog.Path = nil
	FileDialog.Text = nil
	FileDialog.blnMS = nil
	FileDialog.Button = nil
	FileDialog.FullFileName = nil
	FileDialog.isFile = nil
	FileDialog.isDirectory = nil
	FileDialog.Exist = nil
	FileDialog.SelectedItem = nil

	--Dispose Application Colors
	tblDispose.Color = tblDispose.Color()

	--Free All images
	tblDispose.Image = tblDispose.Image()

	--Dispose Application Data
	tblDispose.App = tblDispose.App()
	tblDispose = nil

	--Dispose function set menu
	SetMenu = nil
	SetCursor = nil

	--Dipose Bar Data
	Bar_Size = nil
	Bar_StepSize = nil
	Bar_Location = nil
	SetBar = nil
	drawbar = nil

	ShowMesssage = nil

	--Dispose Application files
	intFileCount = nil
	Files = nil
	App_contentsDirectory = nil

	--Dispose Load_Directory function
	Load_Dev = nil
	Load_Directory = nil

	--Dispoae File Check Functions
	Check_Directory = nil
	CheckFileName = nil
	CheckFileNameExist = nil

	--Dispose Control Data
	padcount = nil

	--Dipsose Functions
	drawfunc = nil

	CopyDir = nil
	DeleteDir = nil
	PasteData = nil

	FileDialog._PACKAGE = nil
	FileDialog._M = nil
	FileDialog._NAME = nil
	FileDialog._VERSION = nil
	FileDialog = nil
	package.loaded["FileDialog"] = nil

	return nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
----End FileDialog˜ module.
-----------------------------------------------------------------------------------------------------

-- Changelog:
  -- v1.03 ??-??-2010:
  -- v1.05 06-09-2012:
	-- The FileDialog can now be used as a module.
