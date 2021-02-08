-----------------------------------------------------------------------------------------------------
-- Big LuaPlayer: Lua-Editer˜ v 2.01 2012 (c) M4MSOFT
-----------------------------------------------------------------------------------------------------

-- Set the Cpu Clock Frequencies.
Power.cpuSpeed(200)

-- load the screen module.
require "screen"

-- Load the string_ext module
string_ext = require "string_ext"

-- Load the Editer module
Editer = require "Editer"

--load the BitmapFont objects
local basicFont = BmpFont.fromImage("Resources/Font/monaca(Basic).png",9,13,nil,false)
local colorFont = BmpFont.fromImage("Resources/Font/monaca(white).png",9,13,nil,false)

--Set Application Colors
local white = Color.new(255, 255, 255)
local clr_transparent = Color.new(255, 255, 255, 128)
local clr_ContolLightLight = Color.new( 255, 255, 255, 255)
local clr_ControlLight = Color.new( 241, 239, 226, 255)
local clr_Control = Color.new( 236, 233, 216, 255)
local clr_ControlDark = Color.new( 172, 168, 153, 255)
local clr_ControlDarkDark = Color.new( 113, 111, 100, 255)
local clr_ControlText = Color.new( 0, 0, 0, 255)
local clr_ControlBorderLight = Color.new( 0, 72 ,241 , 255)
local clr_ControlBorderDark = Color.new( 0, 19, 140, 255)
local clr_CaptionText = Color.new( 255, 255, 255, 255)
local clr_GrayText = Color.new( 172, 168, 153, 255)
local clr_Window = Color.new( 255, 255, 255, 255)
local clr_WindowText = Color.new( 0, 0, 0, 255)
local clr_WindowFrame = Color.new( 127, 157, 185, 255)
local clr_ButtonFace = Color.new( 236, 233, 216, 255)
local clr_ButtonHighlight = Color.new( 255, 255, 255, 255)
local clr_ButtonShadow = Color.new( 172, 168, 153, 255)
local clr_ButtonBorden = Color.new( 0, 60, 116, 255)
local clr_ButtonSelected = Color.new( 255, 140, 0, 255)
local clr_Highlight = Color.new( 49, 106, 197, 255)
local clr_HighlightText = Color.new( 255, 255, 255, 255)
local clr_Info = Color.new( 255, 255, 225, 255)
local clr_InfoText = Color.new( 0, 0, 0, 255)
local clr_MenuBar = Color.new( 236, 233, 216, 255)
local clr_MenuBack = Color.new( 241, 239, 226, 255)
local clr_MenuDark = Color.new( 172, 168, 153, 255)
local clr_MenuGrayText = Color.new( 172,168, 153, 255)
local clr_MenuText = Color.new( 0, 0, 0, 255)
local clr_ScrollBarLight = Color.new( 255, 255, 255, 255)
local clr_ScrollBar= Color.new( 195, 213, 253, 255)
local clr_ScrollBarDark = Color.new( 77, 97, 133, 255)
local clr_ScrollBarBack = Color.new( 212, 208, 200, 255)
local clr_Greentext = Color.new( 0, 144, 0, 255)

--Load Images
local strImagePath = System.Directory.eboot.."/System/Image"
local ImageMap = Texture.load(strImagePath.."/Map.png")
local ImageNew = Texture.load(strImagePath.."/New.png")
local ImageOpen = Texture.load(strImagePath.."/Open.png")
local ImageSave = Texture.load(strImagePath.."/Save.png")
local ImageBarDown = Texture.load(strImagePath.."/ScrollDown.png")
local ImageBarLeft = Texture.load(strImagePath.."/ScrollLeft.png")
local ImageBarRight = Texture.load(strImagePath.."/ScrollRight.png")
local ImageBarUp = Texture.load(strImagePath.."/ScrollUp.png")
strImagePath = nil

--Set Application Data
local App_Title = "Lua Editer v 2.01"
local App_Running = true
local App_ShowCursor = true
local App_CursorTime = 0
local App_fnc = {}
--Set Application MenuBar data
local App_MenuIndex = 6
local App_MenuIndexMax = 7
local App_MenuBar = {
					 File = { text = "File:"     , x = 5 + 8 * 0 , y = 19, w = 9 * 5, h = 14, index = 1, enable = true },
					 Copy = { text = "<Copy>"    , x = 5 + 8 * 7 , y = 19, w = 9 * 6, h = 14, index = 2, enable = false },
					 Paste = { text = "<Paste>"  , x = 5 + 8 * 15, y = 19, w = 9 * 7, h = 14, index = 3, enable = false },
					 Osk = { text = "<OSK>"      , x = 5 + 8 * 24, y = 19, w = 9 * 5, h = 14, index = 4, enable = true },
					 Select = { text = "<Select>", x = 5 + 8 * 31, y = 19, w = 9 * 8, h = 14, index = 5, enable = true },
					 Type = { text = "<Type>"    , x = 5 + 8 * 41, y = 19, w = 9 * 6, h = 14, index = 6, enable = true },
					 Goto = { text = "<Goto>"    , x = 5 + 8 * 49, y = 19, w = 9 * 6, h = 14, index = 7, enable = true }
					}
local App_kMenuBar = {"File", "Copy", "Paste", "Osk", "Select", "Type", "Goto"}
local App_fncMenuBar = {}
--Set Application Menu data
local App_Menu = {
				  FileMenu = {
							  items = {
									   New =      { text = "New"        , index = 1, image = ImageNew, enable = true},
									   Open =     { text = "Open"       , index = 2, image = ImageOpen, enable = true},
									   Save =     { text = "Save"       , index = 3, image = ImageSave, enable = true},
									   SaveAs =   { text = "Save As"    , index = 4, image = false, enable = true},
									   Close =    { text = "Close"      , index = 5, image = false, enable = false},
									   FileList = { text = "File List:" , index = 6, image = false, enable = true},
									   Explorer = { text = "Explorer"   , index = 7, image = ImageMap, enable = true},
									   Exit =     { text = "Exit"       , index = 8, image = false, enable = true}
									  },
							  key = {"New", "Open", "Save", "SaveAs", "Close", "FileList", "Explorer", "Exit"},
							  x = 16+16, y = 10,
							  w = 9 * 10, h = 19,
							  index = 1, indexMax = 8,
							  show = false
							 },
				  FileList = {
							  items = {
									   New =      { text = "New"      , index = 1, image = ImageNew, enable = true},
									   Open =     { text = "Open"     , index = 2, image = ImageOpen, enable = true},
									   SaveAll =  { text = "Save All" , index = 3, image = ImageSave, enable = false},
									   CloseAll = { text = "Close All", index = 4, image = false, enable = false},
							  },
							  key = {"New", "Open", "SaveAll", "CloseAll"},
							  x = 16+16 + 10*10, y = 10+8 + 3*19,
							  w = 9 * 4, h = 19,
							  index = 1, indexMax = 4,
							  show = false
							 }
				 }
local App_Menulist = {App_Menu.FileMenu, App_Menu.FileList}
local App_fnc_Menu = {}
local App_fnc_MenuList = {}
local App_File = Editer.new()
local App_FileList = {App_File}
local App_SelectedFile = 1
local App_FileCount = 1
local App_MaxFile = 10

App_Menu.FileList.indexMax = 4 + App_FileCount
App_Menu.FileList.items[App_SelectedFile] = { text = "", index = 4 + App_SelectedFile, image = false, enable = false}

local App_TypeSleep = 85
local App_TypeCount = 10
local App_TypeButtonSleep = 45
local App_TriggerSleep = 45
local App_MenuSleep = 25
local App_CursorSleep = 8
local App_Sleep = 75
local App_Count = 10
local App_ButtonSleep = 10

-----------------------------------------------------------------------------------------------------
--load the settings.
local file = io.open("settings.txt", "r")
if file then
	local b,k,n = false,"",false
	for line in file:lines() do
		b = string.find(line, "=") or false
		if b then
			k = string.trim(string.sub(line, 1, b-1) or "")
			n = tonumber(string.sub(line, b+1, -1) or "",10) or false
			if n then
				if k == "App_TypeSleep" then App_TypeSleep = n
				elseif k == "App_TypeCount" then App_TypeCount = math.max(n, 0)
				elseif k == "App_TypeButtonSleep" then App_TypeButtonSleep = n
				elseif k == "App_TriggerSleep" then App_TriggerSleep = n
				elseif k == "App_MenuSleep" then App_MenuSleep = n
				elseif k == "App_CursorSleep" then App_CursorSleep = n
				elseif k == "App_Sleep" then App_Sleep = n
				elseif k == "App_Count" then App_Count = math.max(n, 0)
				elseif k == "App_ButtonSleep" then App_ButtonSleep = n
				end
			end
		end
	end
	file:close()
	b,k,n = nil,nil,nil
end
file = nil
-----------------------------------------------------------------------------------------------------

-- Get the text Symbols.
local App_TextSymbols = ""
for line in io.lines("Resources/Key_Words/TextSymbols.bin") do
	App_TextSymbols = line
end

local App_Syntax = {"break", "do", "else", "elseif", "end", "false", "for",
					"function", "goto", "if", "in", "local", "nil", "not", "or",
					"repeat", "return", "then", "true", "until", "while"}

--set clipboard data
local Data_Clipboard = { lines = 0, text = {} }

-----------------------------------------------------------------------------------------------------
--Function update File List items.
local setFileList = function()
	local intWidth = 4
	local blnSave = false
	for idx, item in ipairs(App_Menu.FileList.items) do
	   --if item.index > 2 then
	   item.text = string.format("%d: %s",idx, string.sub(App_FileList[idx].FileName, 1, 24))
	   if not App_FileList[idx].saved then
		   blnSave = true
		   item.text = string.format("%s*",item.text)
	   end
	   intWidth = math.max(intWidth, string.len(item.text))
	   item.index = idx + 4
	   item.enable = App_SelectedFile ~= idx
	end
	App_Menu.FileList.w = 9 * intWidth
	App_Menu.FileList.items.SaveAll.enable = blnSave
	App_Menu.FileMenu.items.Save.enable = not App_File.saved
	if App_FileCount > 1 then
		App_Menu.FileList.items.CloseAll.enable = true
		App_Menu.FileMenu.items.Close.enable = true
	else
		App_Menu.FileList.items.CloseAll.enable = false
		App_Menu.FileMenu.items.Close.enable = false
	end
	if App_FileCount >= 10 then
		App_Menu.FileList.items.New.enable = false
		App_Menu.FileList.items.Open.enable = false
	else
		App_Menu.FileList.items.New.enable = true
		App_Menu.FileList.items.Open.enable = true
	end
	App_Menu.FileList.y = 18+3*19 + (App_FileCount == 10 and 7 or 0)- math.max(App_FileCount-5, 0)*19
	blnSave = nil
	intWidth = nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--Function Draw vBar
local drawvbar = function( x, y, height)
	--Set ScrollBar size
	x = math.floor(x)
	y = math.floor(y)
	height = math.max( height, 9)
	y = math.max( y, 44)
	y = math.min( y, 44 + 194 - height)

	--Draw ScrollBar
	screen.fillRect(x + 1, y + 1, 9, height, clr_ScrollBarDark)
	screen.fillRect(x, y, 9, height, clr_ScrollBarLight)
	screen.fillRect(x + 1, y + 1, 7, height - 2, clr_ScrollBar)
	x = nil
	y = nil
	height = nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--Function Draw hBar
local drawhbar = function( x, y, width)
	--Set ScrollBar size
	x = math.floor(x)
	y = math.floor(y)
	width = math.max( width, 9)
	x = math.max( x, 11)
	x = math.min( x, 11 + 447 - width)

	--Draw ScrollBar
	screen.fillRect(x + 1, y + 1, width, 9, clr_ScrollBarDark)
	screen.fillRect(x, y, width, 9, clr_ScrollBarLight)
	screen.fillRect(x + 1, y + 1, width -2, 7, clr_ScrollBar)
	x = nil
	y = nil
	width = nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--Draw Cursor
local drawCursor = function ( x, y ,color)
	App_CursorTime =  App_CursorTime  + 1
	if App_CursorTime > 10 then
		App_CursorTime  = 0
		App_ShowCursor = not App_ShowCursor
	end
	if App_ShowCursor then
		screen.drawLine(3 + 9 * (x - 1), 37 + 13 * ( y - 1 ) ,7 + 9 * (x - 1), 37 + 13 * ( y - 1 ) ,color)
		screen.drawLine(5 + 9 * (x - 1), 37 + 13 * ( y - 1 ) ,5 + 9 * (x - 1), 49 + 13 * ( y - 1 ) ,color)
		screen.drawLine(3 + 9 * (x - 1), 49 + 13 * ( y - 1 ) ,7 + 9 * (x - 1), 49 + 13 * ( y - 1 ) ,color)
	end
	x = nil
	y = nil
	color = nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--Draw the Seleted Text
local drawSelectedText = function ( Nidx, x, line, lineStart, lineEnd)
	if x + 1 >= lineStart and x + 51 + 1 <= lineEnd then --select All
		screen.fillRect( 5, 37 + 13 * ( Nidx - 1 ), math.max(9 * math.min( string.len(line), 51 ) - 3, 0), 14, clr_Highlight)
		colorFont:print( 5, 38 + 13 * ( Nidx - 1 ), string.sub( line, 1 + x, 51 + x ), clr_HighlightText)
	elseif x + 1 >= lineStart then --selected text / black
		screen.fillRect( 5, 37 + 13 * ( Nidx - 1 ), 9 * math.max((lineEnd - 1 - x ), 0), 14, clr_Highlight)
		colorFont:print( 5, 38 + 13 * ( Nidx - 1 ), string.sub( line, x + 1, lineEnd - 1), clr_HighlightText)
	elseif x + 51  + 1<= lineEnd then --black / selected text
		screen.fillRect( 5 + 9 * ( lineStart - 1 - x ), 37 + 13 * ( Nidx - 1 ), math.max((9 * (51 - lineStart + 1 + x) - 3), 0), 14, clr_Highlight)
		colorFont:print( 5 + 9 * ( lineStart - 1 - x ), 38 + 13 * ( Nidx - 1 ), string.sub( line, ( x + ( lineStart - x )), 51 + x), clr_HighlightText)
	else --black / selected text / back
		screen.fillRect( 5 + 9 * ( lineStart - 1 - x ), 37 + 13 * (Nidx - 1), 9 * math.max(( lineEnd - lineStart), 0), 14, clr_Highlight)
		colorFont:print( 5 + 9 * ( lineStart - 1 - x ), 38 + 13 * ( Nidx - 1 ), string.sub( line, ( x + (lineStart - x)), lineEnd - 1 ), clr_HighlightText)
	end
	Nidx = nil
	x = nil
	line = nil
	lineStart = nil
	lineEnd = nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--Draw Function
local drawFunc = function ()

	--Clear The Screen
	GL.Clear(clr_Control)

	--Draw Main Window
	screen.fillRect( 0, 0, 480, 272, clr_ControlBorderDark)
	screen.fillRect( 1, 1, 480 - 2, 272 - 2, clr_ControlBorderLight)
	screen.drawLine( 2, 15, 480 - 3, 15, clr_ControlBorderDark)
	screen.fillRect( 2, 16, 480 - 4, 272 - 18, clr_Control)
	screen.drawLine( 2, 16, 480 - 3, 16, clr_ControlLight)
	screen.drawLine( 2, 16, 2, 272 - 3, clr_ControlLight)
	screen.drawLine( 2, 272 - 3, 480 - 3, 272 - 3, clr_ControlDark)
	screen.drawLine( 480 - 3, 272 - 3, 480 - 3, 16, clr_ControlDark)
	screen.drawLine( 2, 272 - 13 -3, 480-3 ,272-13 -3 , clr_ControlDark)
	screen.print( 3, 272 - 11 - 3, string.format("Line:%s | File:%s | Saved:%s", App_File["pntSelected"]["y"], App_File["FileName"], tostring(App_File["saved"]) ), clr_ControlText)

	--Draw ScrollBar Window
	screen.fillRect( 2, 272 - 22 -3, 480 - 14, 9, clr_ScrollBarBack)
	screen.blit( 2, 250 - 3, ImageBarLeft)
	screen.blit( 459, 250 - 3, ImageBarRight)
	drawhbar( App_File["hBar_Location"], 250 - 3, App_File["hBar_Size"])
	screen.fillRect( 480 - 12, 35, 9, 272 - 60, clr_ScrollBarBack)
	screen.blit( 468, 35, ImageBarUp)
	screen.blit( 468, 241 - 3, ImageBarDown)
	drawvbar( 468, App_File["vBar_Location"], App_File["vBar_Size"])

	--Draw RichTextBox
	screen.fillRect( 3, 35, 480 - 15, 272 - 60, clr_WindowFrame)
	screen.fillRect( 4, 36 , 480 - 17, 272 - 62 , clr_Window)

	--Draw Text into TextBox
	local line = false
	local intGreen = -1
	local xD,yD = 0,0
	local tS,tE = 0,0
	local tblSelect = false
	for n = 1, 16 do
		xD = App_File["pntDraw"]["x"]
		yD = App_File["pntDraw"]["y"]
		line = App_File["text"][n+yD] or false
		if not line then break end
		intGreen = App_File["text_green"][n+yD] or false
		if not intGreen then
			--Calculate green text start location.
			intGreen = string.find(line, "()-()-" ) or 0
			App_File["text_green"][n+yD] = intGreen
			if collectgarbage("step") then collectgarbage("collect") end
		end
		--Text draw function.
		if intGreen == 0 then
			basicFont:print( 5, 38 + 13 * ( n - 1 ), string.sub( line, 1 + xD, 51 + xD), white)
		else --Draw the green text
			if xD + 1 >= intGreen then --All Green.
				colorFont:print( 5, 38 + 13 * ( n - 1 ), string.sub( line, 1 + xD, 51 + xD), clr_Greentext)
			else --black / green.
				basicFont:print( 5, 38 + 13 * ( n - 1 ), string.sub( line, 1 + xD, math.min(51, intGreen - xD - 1) + xD), white)
				colorFont:print( 5 + 9 * (intGreen - 1 - xD) , 38 + 13 * ( n - 1 ), string.sub( line, (xD + (intGreen - xD)), 51 + xD), clr_Greentext)
			end
		end

		--Draw the Selected Text
		tblSelect = App_File["selectedText"]
		if tblSelect["startline"]["selected"] and tblSelect["endline"]["selected"] then
			if tblSelect["startline"]["start"] ~= 0 then
				if n + yD == tblSelect["startline"]["line"] and n + yD == tblSelect["endline"]["line"] then --one line selected
					tS = tblSelect["startline"]["start"]
					tE = tblSelect["endline"]["start"]
					drawSelectedText (n, xD, line, tS, tE)
				elseif n + yD == tblSelect["startline"]["line"] then --more line seleted (the First line)
					tS = tblSelect["startline"]["start"]
					tE = string.len(line) + 1
					drawSelectedText (n, xD, line, tS, tE)
				elseif n + yD == tblSelect["endline"]["line"] then --more line seleted (the last line)
					tS = 1
					tE = tblSelect["endline"]["start"]
					drawSelectedText (n, xD, line, tS, tE)
				elseif n + yD <= tblSelect["endline"]["line"] and n + yD + 1 > tblSelect["startline"]["line"] then --more line seleted (the middle lines)
					tS = 1
					tE = string.len(line) + 1
					drawSelectedText (n, xD, line, tS, tE)
				end
			end
		end

	end
	tblSelect = nil
	tE,tS = nil,nil
	yD,xD = nil,nil
	intGreen = nil
	line = nil

	--Draw the Cursor
	drawCursor( App_File["pntCursor"]["x"] ,App_File["pntCursor"]["y"] , clr_WindowText)

	--Draw Menu
	screen.fillRect( 2, 16, 480 - 4, 17, clr_MenuBar)
	screen.drawLine( 2, 33, 480 - 3, 33 , clr_MenuDark)

	--Draw the menuBar.
	for idx, Menu in pairs(App_MenuBar) do
		if App_MenuIndex == Menu.index then
			screen.fillRect( Menu.x - 2 , Menu.y - 2, Menu.w + 1, Menu.h + 1, clr_Highlight)
			if Menu.enable == true then
				screen.print( Menu.x, Menu.y, Menu.text, clr_HighlightText)
			else
				screen.print( Menu.x, Menu.y, Menu.text, clr_GrayText)
			end
		else
			if Menu.enable == true then
				screen.print( Menu.x, Menu.y, Menu.text, clr_MenuText)
			else
				screen.print( Menu.x, Menu.y, Menu.text, clr_GrayText)
			end
		end
	end

	--Print Window Text
	screen.print( 4, 2, string.format("%s - %s - %d kb", string.sub(App_File["FileName"], 1, 22), App_Title, collectgarbage("count") ), clr_CaptionText)

	--Draw application Menus.
	for idx0, Menu in pairs(App_Menulist) do
		if Menu.show then
			local x,y,w,h = Menu.x, Menu.y, Menu.w, Menu.h
			for idx1, item in pairs(Menu.items) do
				y = Menu.y + item.index*h
				screen.fillRect( x - 21 , y - 4 , w + 30, h + 1, clr_MenuDark)
				screen.fillRect( x - 20 , y - 3 , w + 28, h - 1, clr_MenuBack)
				if item.image and item.enable then
					screen.blit( x - 18, y - 2, item.image)
				elseif item.image then
					screen.blit( x - 18, y - 2, item.image, 64)
				end
				if Menu.index == item.index then
					screen.fillRect( x, y - 1, w + 6, h - 5, clr_Highlight)
					if item.enable then
						screen.print( x + 2, y, item.text, clr_HighlightText)
					else
						screen.print( x + 2, y, item.text, clr_MenuGrayText)
					end
				else
					screen.fillRect( x, y -1, w + 6, h - 5, clr_MenuBack)
					if item.enable then
						screen.print( x + 2, y, item.text, clr_MenuText)
					else
						screen.print( x + 2, y, item.text, clr_MenuGrayText)
					end
				end
			end
			x,y,w,h = nil,nil,nil,nil
		end
	end

end
-----------------------------------------------------------------------------------------------------


--Control Data
local padcount = 0
local pad = {}

local Type_index = 1
local Type_KeyWords = 0
local Type_Output = false
local Type_infoText = "< All Symbols >"
local Type_Symbols =   {AllSymbols = { name = "< All Symbols >", index = 0, text = App_TextSymbols},
						 UpperCase = { name = "< Upper Case >", index = 1, text = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" },
						 LowerCase = { name = "< Lower Case >", index = 2, text = "abcdefghijklmnopqrstuvwxyz"},
						 Numbers = { name = "< Numbers >", index = 3, text = "0123456789."},
						 MathSymbols = { name = "< Math Symbols >", index = 4, Selected = 1, text = "()*+-.^<=>/~"},
						 Symbols =  { name = "< Symbols >", index = 5, text = string.format("%s%s%s%s",
									  string.sub(App_TextSymbols, 1, 43),
									  string.sub(App_TextSymbols, 54, 60),
									  string.sub(App_TextSymbols, 87, 92),
									  string.sub(App_TextSymbols, 119, 122))
									},
						 KeyWords = { name = "< Key Words >", index = 6, intlines = 0, line = {} },
						 GL = { name = "< GL >", index = 7, intlines = 0, line = {} },
						 TablePrinter = { name = "< Table Printer >", index = 8, intlines = 0, line = {} }
						}
for line in io.lines("Resources/Key_Words/KeyWords.txt") do
	Type_Symbols.KeyWords.intlines = Type_Symbols.KeyWords.intlines + 1
	Type_Symbols.KeyWords.line[Type_Symbols.KeyWords.intlines] = line
end

for line in io.lines("Resources/Key_Words/GL.txt") do
	Type_Symbols.GL.intlines = Type_Symbols.GL.intlines + 1
	Type_Symbols.GL.line[Type_Symbols.GL.intlines] = line
end

-----------------------------------------------------------------------------------------------------
local _BuildTable = function(tblPrnt, strLine)
	local n, nMax = 0, 0
	local tline = tblPrnt.line
	local blnPoint = false
	local strLast,str ="",""
	local TableIndex = {}
	local n1,n2,n3 = -1,-1,-1

	--Remove the last point from the string.
	str = string.sub(strLine, -1, -1)
	if str == "." or str == ":" then
		blnPoint = true
		strLine = string.sub(strLine,1,-2) or ""
	end
	--Remove the first part of the string.
	n = string.find(strLine,'[%a%d"#$%%&'..".:%[%]_`']+$") or false
	if n then
		strLine = string.sub(strLine,n,-1) or ""
		n = string.find("["..strLine.."]", "%b[]$") or false
		if n then strLine = string.sub(strLine,n,-1) or "" end
	else
		strLine = ""
	end

	--Collect the keys.
	while strLine ~= "" do
		strLast = strLine
		--Remove the first point from the string.
		str = string.sub(strLine, 1, 1)
		if str == "." or str == ":" then
			strLine = string.sub(strLine,2,-1) or ""
			if strLine == "" then break end
		end
		n1 = {string.find(strLine,"^[%a%d_]+")} or false
		n2 = {string.find(strLine,"%b[]")}
		if n1[1] and n2[1] then
			n = n1[1] < n2[1] and n1[1] or n2[1] or 0
		else
			n = n1[1] or n2[1] or 0
		end
		if n ~= 0 then
			if n1[1] and n1[2] and n == n1[1] then
				str = string.sub(strLine,n1[1],n1[2]) or ""
				if str ~= "" and str ~= "." and str ~= ":" and n ~= 0 then
					table.insert(TableIndex,str)
				end
				strLine = string.sub(strLine,n1[2]+1,-1) or ""
			elseif n2[1] and n2[2] and n == n2[1] then
				str = string.sub(strLine,n2[1]+1,n2[2]-1) or ""
				if str ~= "" and str ~= "." and str ~= ":" then
					table.insert(TableIndex,str)
				end
				strLine = string.sub(strLine, n2[2]+1, -1) or ""
			end
		else
			if strLine ~= "" and strLine ~= "." and strLine ~= ":" then
				table.insert(TableIndex,strLine)
			end
			strLine = ""
			break
		end
		--Break the loop if the text didn't changed.
		if strLast == strLine then
			strLine = ""
			break
		end
	end

	--Collect the items.
	value = nil
	local indexKey = ""
	local value = package.loaded["_G"] or {}
	nMax = table.getn(TableIndex)
	for n = 1, nMax do
		--difine the table key.
		indexKey = TableIndex[n]
		n2 = {string.find(indexKey,'%b""')}
		if not n2[1] or not n2[2] then n2 = {string.find(indexKey,"%b''")} end
		if n2[1] and n2[2] then
			indexKey = string.sub(indexKey,n2[1]+1,n2[2]-1) or indexKey
		else
			indexKey = tonumber(indexKey) or indexKey
		end
		value = value[indexKey]
		str = type(value)
		if value == nil or str == "nil" then
			break
		elseif str == "string" then
			table.insert(tline, string.format(' = "%s"',value))
		elseif str == "number" or str == "boolean" then
			table.insert(tline, string.format(" = %s",tostring(value)))
		elseif str == "function" then
			table.insert(tline, string.format("() --%s",tostring(value)))
		elseif str == "thread" or str == "userdata" then
			table.insert(tline, string.format(" --%s",str))
		elseif n == nMax and str == "table" then
			for n in pairs(value) do
				if n then table.insert(tline,
					type(n) ~= "number" and (not blnPoint and "." or "")..tostring(n) or
					string.format( "[%d]", n)
					)
				end
			end
		end
	end
	value = nil
	indexKey = nil
	n1,n2,n3 = nil,nil,nil
	TableIndex = nil
	strLast,str = nil,nil
	blnPoint = nil,nil

	--Build the default item table.
	if table.getn(tline) == 0 then
		local value = package.loaded["_G"] or {}
		for n in pairs(value) do
			if n then table.insert(tline,
				type(n) ~= "number" and tostring(n) or
				string.format( "[%d]", n)
				)
			end
		end
		value = nil
		for i,v in ipairs(App_Syntax) do table.insert(tline, v) end
	end

	--Sort the item table.
	table.sort(tline, function(a, b)
					  return a < b
				  end)

	tline = nil
	n,nMax = nil,nil
	strLine = nil
	tblPrnt = nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
local BuildTable = function()
	local tblPrnt = Type_Symbols.TablePrinter
	tblPrnt.line = {}
	tblPrnt.intlines = 1
	local strLine = App_File.text[App_File.pntSelected.y]
	--Check the cursor position.
	if App_File.pntSelected.x-1 > 0 then
		strLine = string.sub(strLine, 1, App_File.pntSelected.x-1)
	else
		strLine = ""
	end
	--Build the table.
	pcall(_BuildTable, tblPrnt, strLine)
	--Add items to the item table.
	table.insert(tblPrnt.line, 1, ".")
	table.insert(tblPrnt.line, 2, ",")
	table.insert(tblPrnt.line, 3, ":")
	table.insert(tblPrnt.line, 4, ";")
	table.insert(tblPrnt.line, 5, "'")
	table.insert(tblPrnt.line, 6, '"')
	table.insert(tblPrnt.line, 7, "[]")
	table.insert(tblPrnt.line, 8, "{}")
	table.insert(tblPrnt.line, 9, "=")
	table.insert(tblPrnt.line, 10, "()")
	--Set the type index.
	tblPrnt.intlines = table.getn(tblPrnt.line)
	if Type_KeyWords == 8 and tblPrnt.intlines > 10 then
		Type_index = 11
	elseif Type_KeyWords == 8 and Type_index > tblPrnt.intlines then
		Type_index = 1
	end
	strLine = nil
	tblPrnt = nil
	collectgarbage("collect")
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- Show the Type Window
local Type_Input = function()
	Type_Output = false
	local strText = ""
	local intWidth = 0
	local bnlBuild = true
	if Type_KeyWords == 8 then
	   BuildTable()
	   bnlBuild = false
	end
	while System.running() do

		-- Draw Screen.
		drawFunc()

		-- Draw the Type Window
		if Type_KeyWords < 6 then
			strText = string.sub(App_TextSymbols, Type_index, Type_index)
		elseif Type_KeyWords == 6 then
			strText = Type_Symbols.KeyWords.line[Type_index]
		elseif  Type_KeyWords == 7 then
			strText = Type_Symbols.GL.line[Type_index]
		elseif Type_KeyWords == 8 then
			strText = Type_Symbols.TablePrinter.line[Type_index] or "No Data."
		end
		intWidth = math.max(string.len(strText) * 9 + 16 ,150 + 20)
		screen.fillRect( (480 - intWidth) / 2 + 2, 119, intWidth, 39, clr_ControlDark)
		screen.fillRect( (480 - intWidth) / 2, 117, intWidth, 39, clr_InfoText)
		screen.fillRect( (480 - intWidth) / 2 + 1, 118, intWidth - 2, 37, clr_Info)
		basicFont:print( math.floor((480 - 9 * string.len(Type_infoText)) / 2), 122, Type_infoText, white)
		basicFont:print( math.floor((480 - 9 * string.len(strText)) / 2), 139, strText, white)

		-- swap the buffer.
		GL.Flip()

		-- Slow down the Application if a button is pressed.
		if Controls.buttons() > 0 and padcount > 0 then
			padcount = padcount - 1
			System.sleep(App_TypeSleep)
		elseif padcount ~= App_TypeCount and Controls.buttons() == 0 then
			padcount = App_TypeCount
			--Collect garbage.
			collectgarbage("collect")
		end

		-- Update the controls.
		Controls.readPeek()

		if Controls.buttons(256) or Controls.buttons(512) or Controls.buttons(4096)
		   or Controls.buttons(32768) or Controls.buttons(8192)
		   or Controls.buttons(1) or Controls.buttons(8) then
			-- Break the Type Loop and cancel the input
			Type_Output = false
			break
		elseif Controls.buttons(16) then -- up Button Pressed
			Type_index = Type_index - 1
			if Type_KeyWords < 6 then
				if Type_index < 1 then Type_index = string.len(App_TextSymbols) end
			elseif Type_KeyWords == 6 then
				if Type_index < 1 then Type_index = Type_Symbols.KeyWords.intlines end
			elseif Type_KeyWords == 7 then
				if Type_index < 1 then Type_index = Type_Symbols.GL.intlines end
			elseif Type_KeyWords == 8 then
				if Type_index < 1 then Type_index = Type_Symbols.TablePrinter.intlines end
			end
			System.sleep(App_TypeButtonSleep)
		elseif Controls.buttons(64) then -- down Button Pressed
			Type_index = Type_index + 1
			if Type_KeyWords < 6 then
				if Type_index > string.len(App_TextSymbols) then Type_index = 1 end
			elseif Type_KeyWords == 6 then
				if Type_index > Type_Symbols.KeyWords.intlines then Type_index = 1 end
			elseif Type_KeyWords == 7 then
				if Type_index > Type_Symbols.GL.intlines then Type_index = 1 end
			elseif Type_KeyWords == 8 then
				if Type_index > Type_Symbols.TablePrinter.intlines then Type_index = 1 end
			end
			System.sleep(App_TypeButtonSleep)
		elseif Controls.buttons(128) then -- left Button Pressed
			Type_KeyWords = Type_KeyWords - 1
			if Type_KeyWords < 0 then Type_KeyWords = 8 end
			if Type_KeyWords == 8 and bnlBuild then
				BuildTable()
				bnlBuild = false
			end
			for Nidx , Object in pairs(Type_Symbols) do
				if Object.index == Type_KeyWords then
					Type_infoText = Object.name
					Type_index = 1
					App_TextSymbols = Object.text or ""
				end
			end
			System.sleep(App_TypeButtonSleep)
		elseif Controls.buttons(32) then -- right Button Pressed
			Type_KeyWords = Type_KeyWords + 1
			if Type_KeyWords > 8 then Type_KeyWords = 0 end
			if Type_KeyWords == 8 and bnlBuild then
				BuildTable()
				bnlBuild = false
			end
			for Nidx , Object in pairs(Type_Symbols) do
				if Object.index == Type_KeyWords then
					Type_infoText = Object.name
					Type_index = 1
					App_TextSymbols = Object.text or ""
				end
			end
			System.sleep(App_TypeButtonSleep)
		elseif Controls.buttons(16384) then -- cross Button Pressed
			-- Break the Type Loop
			Type_Output = strText
			break
		end

	end

	bnlBuild = nil
	intWidth = nil
	strText = nil

	return Type_Output
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- Define the application main control functions.
App_fnc.none = function() end
App_fnc.close = function()
	local Check = true
	if App_FileCount > 1 then
		for n = 1, App_FileCount do
		   if not App_FileList[n].saved then
			   Check = false
			   break
		   end
		end
		if not Check then
			if Utility.msgShow("Warning: (Close Application)\nDo you want to continue without saving the files.",
							   Utility.MSG_BTN_YESMO + Utility.MSG_DEFAULT_BTN_NO) then
				Check = true
			end
		end
	else
		if not App_File.saved then
			Check = false
			if Utility.msgShow(string.format("Warning: (Close Application)\nDo you want to continue without saving the file: '%s'.", App_File.FileName),
			                   Utility.MSG_BTN_YESMO + Utility.MSG_DEFAULT_BTN_NO) then
				Check = true
			end
		end
	end
	if Check then
		-- Break the main loop.
		App_Running = false
	end
	Check = nil
end
App_fnc.select = function()
	-- Select the next MenuBar item.
	App_MenuIndex = 5
	App_fncMenuBar[App_MenuIndex+App_MenuIndexMax]()
end
App_fnc.ltrigger = function()
	-- Select the next MenuBar item.
	App_File:select(true)
	-- Update the menubar.
	App_MenuBar.Copy.enable = App_File.selected
	App_MenuIndex = App_MenuIndex - 1
	if App_MenuIndex < 1 then App_MenuIndex = App_MenuIndexMax end
	App_fncMenuBar[App_MenuIndex+App_MenuIndexMax]()
	System.sleep(App_TriggerSleep)
end
App_fnc.rtrigger = function()
	-- Select the next MenuBar item.
	App_File:select(true)
	-- Update the menubar.
	App_MenuBar.Copy.enable = App_File.selected
	App_MenuIndex = App_MenuIndex + 1
	if App_MenuIndex > App_MenuIndexMax then App_MenuIndex = 1 end
	App_fncMenuBar[App_MenuIndex+App_MenuIndexMax]()
	System.sleep(App_TriggerSleep)
end
App_fnc.up = function()
	-- Move the cursor up.
	App_File:CursorUp(1)
	System.sleep(App_CursorSleep)
end
App_fnc.down = function()
	-- Move the cursor down.
	App_File:CursorDown(1)
	System.sleep(App_CursorSleep)
end
App_fnc.left = function()
	-- Move the cursor left.
	App_File:CursorLeft(1)
	System.sleep(App_CursorSleep)
end
App_fnc.right = function()
	-- Move the cursor right.
	App_File:CursorRight(1)
	System.sleep(App_CursorSleep)
end
App_fnc.cross = function()
	-- Activate the application MenuBar function.
	App_fncMenuBar[App_MenuIndex]()
end
App_fnc.circle = function()
	-- delete the text.
	App_File:delete()
	-- Update the menubar.
	App_MenuBar.Copy.enable = App_File.selected
	System.sleep(App_ButtonSleep)
end
App_fnc.square = function()
	-- Create a new line.
	App_File:newLine()
	-- Update the menubar.
	App_MenuBar.Copy.enable = App_File.selected
	System.sleep(App_ButtonSleep)
end
App_fnc.triangle = function()
	-- Place a space.
	App_File:write(Editer.chrSpace, false)
	-- Update the menubar.
	App_MenuBar.Copy.enable = App_File.selected
	System.sleep(App_ButtonSleep)
end
App_fnc.showCursor = function()
	-- Show the Cursor if a control Key is Pressed.
   App_CursorTime = 0
   App_ShowCursor = true
end
App_fnc.type = function()
	-- Show the input dialog.
	local strInput = Type_Input()
	if strInput then App_File:write(strInput, false) end
	strInput = nil
	-- Update the menubar.
	App_MenuBar.Copy.enable = App_File.selected
	System.sleep(5)
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- Create the Pad object.
pad.start = Controls.press
pad.fnc_start = App_fnc.close
pad.select = Controls.press
pad.fnc_select = App_fnc.select
pad.l = Controls.buttons
pad.fnc_l = App_fnc.ltrigger
pad.r = Controls.buttons
pad.fnc_r = App_fnc.rtrigger
pad.up = Controls.buttons
pad.fnc_up = App_fnc.up
pad.down = Controls.buttons
pad.fnc_down = App_fnc.down
pad.left = Controls.buttons
pad.fnc_left = App_fnc.left
pad.right = Controls.buttons
pad.fnc_right = App_fnc.right
pad.cross = Controls.buttons
pad.fnc_cross = App_fnc.cross
pad.circle = Controls.buttons
pad.fnc_circle = App_fnc.circle
pad.square = Controls.buttons
pad.fnc_square = App_fnc.square
pad.triangle = Controls.buttons
pad.fnc_triangle = App_fnc.triangle
pad.any = Controls.buttons
pad.fnc_any = App_fnc.showCursor
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- Define the Menu functions.
App_fnc_Menu.activate = function()
	-- Set the key functions.
	pad.fnc_select = App_fnc_Menu.select
	pad.fnc_l = App_fnc_Menu.l
	pad.fnc_r = App_fnc_Menu.r
	pad.fnc_up = App_fnc_Menu.up
	pad.fnc_down = App_fnc_Menu.down
	pad.fnc_left = false
	pad.fnc_right = App_fnc_Menu.right
	pad.fnc_cross = App_fnc_Menu.cross
	pad.fnc_circle = App_fnc_Menu.circle
	-- Show the file menu.
	App_Menu.FileMenu.show = true
	-- update the menu.
	App_Menu.FileMenu.items.Save.enable = not App_File.saved
	App_Menu.FileMenu.items.Close.enable = App_FileCount > 1
end
App_fnc_Menu.deactivate = function()
	-- Reset the key functions.
	pad.fnc_select = App_fnc.select
	pad.fnc_l = App_fnc.ltrigger
	pad.fnc_r = App_fnc.rtrigger
	pad.fnc_up = App_fnc.up
	pad.fnc_down = App_fnc.down
	pad.fnc_left = App_fnc.left
	pad.fnc_right = App_fnc.right
	pad.fnc_cross = App_fnc.cross
	-- Hide the file menu.
	App_Menu.FileMenu.show = false
end
App_fnc_Menu.select = function()
	App_fnc_Menu.deactivate()
	App_fnc.select()
end
App_fnc_Menu.l = function()
	App_fnc_Menu.deactivate()
	App_fnc.ltrigger()
end
App_fnc_Menu.r = function()
	App_fnc_Menu.deactivate()
	App_fnc.rtrigger()
end
App_fnc_Menu.up = function()
	-- Select the next FileMenu item.
	App_Menu.FileMenu.index = App_Menu.FileMenu.index - 1
	if App_Menu.FileMenu.index < 1 then App_Menu.FileMenu.index = App_Menu.FileMenu.indexMax end
	System.sleep(App_MenuSleep)
end
App_fnc_Menu.down = function()
	-- Select the next FileMenu item.
	App_Menu.FileMenu.index = App_Menu.FileMenu.index + 1
	if App_Menu.FileMenu.index > App_Menu.FileMenu.indexMax then App_Menu.FileMenu.index = 1 end
	System.sleep(App_MenuSleep)
end
App_fnc_Menu.right = function()
	-- Activate a FileMenu item.
	local tblItems = App_Menu.FileMenu.items
	local intIndex = App_Menu.FileMenu.index
	if intIndex == 6 and tblItems.FileList.enable then
		App_fnc_MenuList.activate()
		-- App_fnc_Menu.deactivate()
	end
	tblItems = nil
	intIndex = nil
end
App_fnc_Menu.cross = function()
	-- Activate a FileMenu item.
	local tblItems = App_Menu.FileMenu.items
	local intIndex = App_Menu.FileMenu.index
	if intIndex == 1 and tblItems.New.enable then
		local Check = true
		if not App_File.saved then
			Check = false
			if Utility.msgShow(string.format("Warning: (New File)\nDo you want to continue without saving the file: '%s'.", App_File.FileName),
			                   Utility.MSG_BTN_YESMO + Utility.MSG_DEFAULT_BTN_NO) then
				Check = true
			end
		end
		if Check then
			App_File:free()
			App_File = Editer.new()
			App_FileList[App_SelectedFile] = App_File
			-- Update the menubar.
			App_MenuBar.Copy.enable = App_File.selected
		end
		Check = nil
		App_fnc_Menu.deactivate()
	elseif intIndex == 2 and tblItems.Open.enable then
		local Check = true
		if not App_File.saved then
			Check = false
			if Utility.msgShow(string.format("Warning: (Load File)\nDo you want to continue without saving the file: '%s'.", App_File.FileName),
							   Utility.MSG_BTN_YESMO + Utility.MSG_DEFAULT_BTN_NO) then
				Check = true
			end
		end
		if Check then
			App_File = App_File:load()
			App_FileList[App_SelectedFile] = App_File
			-- Update the menubar.
			App_MenuBar.Copy.enable = App_File.selected
		end
		Check = nil
		App_fnc_Menu.deactivate()
	elseif intIndex == 3 and tblItems.Save.enable then
		App_File:save()
		App_fnc_Menu.deactivate()
	elseif intIndex == 4 and tblItems.SaveAs.enable then
		App_File:saveAs()
		App_fnc_Menu.deactivate()
	elseif intIndex == 5 and tblItems.Close.enable then
		if App_FileCount > 1 then
			local Check = true
			if not App_File.saved then
				Check = false
				if Utility.msgShow(string.format("Warning: (Close File)\nDo you want to continue without saving the file: '%s'.", App_File.FileName),
				                   Utility.MSG_BTN_YESMO + Utility.MSG_DEFAULT_BTN_NO) then
					Check = true
				end
			end
			if Check then
				table.remove(App_FileList, App_SelectedFile)
				table.remove(App_Menu.FileList.items)
				App_FileCount = App_FileCount - 1
				if App_SelectedFile > App_FileCount then App_SelectedFile = 1 end
				App_File:free()
				App_File = App_FileList[App_SelectedFile]
				App_Menu.FileList.indexMax = 4 + App_FileCount
				App_Menu.FileList.index = 4 + App_SelectedFile
				setFileList()
				-- Update the menubar.
				App_MenuBar.Copy.enable = App_File.selected
			end
			Check = nil
		end
		App_fnc_Menu.deactivate()
	elseif intIndex == 6 and tblItems.FileList.enable then
		App_fnc_MenuList.activate()
		--App_fnc_Menu.deactivate()
	elseif intIndex == 7 and tblItems.Explorer.enable then
		_G.FileDialog = require "FileDialog"
		FileDialog.Index = 2
		FileDialog.FileName = App_File.FileName
		FileDialog.Path = App_File.Path
		FileDialog.Text = "Explorer"
		FileDialog.show()
		App_fnc_Menu.deactivate()
	elseif intIndex == 8 and tblItems.Exit.enable then
		-- Close the application
		App_fnc.close()
		App_fnc_Menu.deactivate()
	end
	tblItems = nil
	intIndex = nil
end
App_fnc_Menu.circle = function()
	App_fnc_Menu.deactivate()
	App_fncMenuBar[App_MenuIndex+App_MenuIndexMax]()
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- Define the File ListMenu functions.
App_fnc_MenuList.activate = function()
	setFileList()
	-- Set the key functions.
	pad.fnc_select = App_fnc_MenuList.select
	pad.fnc_l = App_fnc_MenuList.l
	pad.fnc_r = App_fnc_MenuList.r
	pad.fnc_up = App_fnc_MenuList.up
	pad.fnc_down = App_fnc_MenuList.down
	pad.fnc_left = App_fnc_MenuList.left
	pad.fnc_right = false
	pad.fnc_cross = App_fnc_MenuList.cross
	pad.fnc_circle = App_fnc_MenuList.circle
	-- Show the File List menu.
	App_Menu.FileList.show = true
end
App_fnc_MenuList.deactivate = function()
	-- Reset the key functions.
	pad.fnc_select = App_fnc.select
	pad.fnc_l = App_fnc.ltrigger
	pad.fnc_r = App_fnc.rtrigger
	pad.fnc_up = App_fnc.up
	pad.fnc_down = App_fnc.down
	pad.fnc_left = App_fnc.left
	pad.fnc_right = App_fnc.right
	pad.fnc_cross = App_fnc.cross
	-- Hide the File List menu.
	App_Menu.FileList.show = false
	-- Show the file menu.
	if App_Menu.FileMenu.show then App_fnc_Menu.activate() end
end
App_fnc_MenuList.select = function()
	App_Menu.FileMenu.show = false
	App_fnc_MenuList.deactivate()
	-- if not App_Menu.FileMenu.show then App_fnc.select() end
	App_fnc.select()
end
App_fnc_MenuList.l = function()
	App_Menu.FileMenu.show = false
	App_fnc_MenuList.deactivate()
	-- if not App_Menu.FileMenu.show then App_fnc.ltrigger() end
	App_fnc.ltrigger()
end
App_fnc_MenuList.r = function()
	App_Menu.FileMenu.show = false
	App_fnc_MenuList.deactivate()
	-- if not App_Menu.FileMenu.show then App_fnc.rtrigger() end
	App_fnc.rtrigger()
end
App_fnc_MenuList.up = function()
	-- Select the next File List Menu item.
	App_Menu.FileList.index = App_Menu.FileList.index - 1
	if App_Menu.FileList.index < 1 then App_Menu.FileList.index = App_Menu.FileList.indexMax end
	System.sleep(App_MenuSleep)
end
App_fnc_MenuList.down = function()
	-- Select the next File List Menu item.
	App_Menu.FileList.index = App_Menu.FileList.index + 1
	if App_Menu.FileList.index > App_Menu.FileList.indexMax then App_Menu.FileList.index = 1 end
	System.sleep(App_MenuSleep)
end
App_fnc_MenuList.left = function()
	if App_Menu.FileMenu.show then
		App_fnc_MenuList.deactivate()
	end
end
App_fnc_MenuList.cross = function()
	-- Activate a FileMenu item.
	local tblItems = App_Menu.FileList.items
	local intIndex = App_Menu.FileList.index
	if intIndex == 1 and tblItems.New.enable then
		if App_FileCount < App_MaxFile then
			App_FileCount = App_FileCount + 1
			App_SelectedFile = App_FileCount
			App_FileList[App_SelectedFile] = Editer.new()
			App_File = App_FileList[App_SelectedFile]
			App_Menu.FileList.indexMax = 4 + App_FileCount
			App_Menu.FileList.items[App_FileCount] = { text = "", index = 4 + App_FileCount, image = false, enable = false}
			setFileList()
			-- Update the menubar.
			App_MenuBar.Copy.enable = App_File.selected
		end
		App_Menu.FileMenu.show = false
		App_fnc_MenuList.deactivate()
	elseif intIndex == 2 and tblItems.Open.enable then
		if App_FileCount < App_MaxFile then
			App_File = Editer.new()
			App_File.FileName = App_FileList[App_SelectedFile].FileName
			App_File.Path = App_FileList[App_SelectedFile].Path
			App_File.FullFileName = App_FileList[App_SelectedFile].FullFileName
			App_File = App_File:load()
			if App_File.loaded then
				App_FileCount = App_FileCount + 1
				App_SelectedFile = App_FileCount
				App_FileList[App_SelectedFile] = App_File
				App_Menu.FileList.indexMax = 4 + App_FileCount
				App_Menu.FileList.items[App_FileCount] = { text = "", index = 4 + App_FileCount, image = false, enable = false}
				setFileList()
				-- Update the menubar.
				App_MenuBar.Copy.enable = App_File.selected
			else
				App_File = App_FileList[App_SelectedFile]
				-- Update the menubar.
				App_MenuBar.Copy.enable = App_File.selected
			end
		end
		App_Menu.FileMenu.show = false
		App_fnc_MenuList.deactivate()
	elseif intIndex == 3 and tblItems.SaveAll.enable then
		for n = 1, App_FileCount do
			App_FileList[n]:save()
		end
		setFileList()
		App_Menu.FileMenu.show = false
		App_fnc_MenuList.deactivate()
	elseif intIndex == 4 and tblItems.CloseAll.enable then
		local Check = true
		if App_FileCount > 1 then
			for n = 1, App_FileCount do
				if not App_FileList[n].saved then
					Check = false
					break
				end
			end
			if not Check then
				if Utility.msgShow("Warning: (Closing Files)\nDo you want to continue without saving the files.",
								   Utility.MSG_BTN_YESMO + Utility.MSG_DEFAULT_BTN_NO) then
					Check = true
				end
			end
			if Check then
				-- Close all the files.
				for n = 1, App_FileCount do
					App_FileList[n] = App_FileList[n]:free()
					App_Menu.FileList.items[n] = nil
				end
				App_FileCount = 1
				App_SelectedFile = 1
				App_File = Editer.new()
				App_FileList[App_SelectedFile] = App_File
				App_Menu.FileList.indexMax = 4 + App_FileCount
				App_Menu.FileList.index = 4 + App_SelectedFile
				App_Menu.FileList.items[App_FileCount] = { text = "", index = 4 + App_FileCount, image = false, enable = false}
				setFileList()
				-- Update the menubar.
				App_MenuBar.Copy.enable = App_File.selected
			end
			Check = nil
		end
		App_Menu.FileMenu.show = false
		App_fnc_MenuList.deactivate()
	elseif intIndex > 4 and tblItems[intIndex - 4].enable then
		App_SelectedFile = intIndex - 4
		App_File = App_FileList[App_SelectedFile]
		setFileList()
		-- Update the menubar.
		App_MenuBar.Copy.enable = App_File.selected
	end
	tblItems = nil
	intIndex = nil
end
App_fnc_MenuList.circle = function()
	App_fnc_MenuList.deactivate()
	if not App_Menu.FileMenu.show then App_fncMenuBar[App_MenuIndex+App_MenuIndexMax]() end
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- Define the MenuBar functions.
App_fncMenuBar[1] = App_fnc_Menu.activate
App_fncMenuBar[2] = function()
	-- Copy the selected text.
	if App_MenuBar.Copy.enable then
		-- Copy the text.
		local intLlines = 0
		local tblSelect = App_File.selectedText
		Data_Clipboard.text = {}
		for n = tblSelect.startline.line, tblSelect.endline.line, 1 do
			intLlines = intLlines + 1
			if tblSelect.startline.line == tblSelect.endline.line then
				Data_Clipboard.text[intLlines] = string.sub(App_File.text[n], tblSelect.startline.start, tblSelect.endline.start - 1)
			elseif n == tblSelect.startline.line then
				Data_Clipboard.text[intLlines] = string.sub(App_File.text[n], tblSelect.startline.start, -1)
			elseif n == tblSelect.endline.line then
				Data_Clipboard.text[intLlines] = string.sub(App_File.text[n], 1, tblSelect.endline.start - 1)
			else
				Data_Clipboard.text[intLlines] = App_File.text[n]
			end
		end
		Data_Clipboard.lines = intLlines
		App_MenuBar.Paste.enable = true
		tblSelect = nil
		intLines = nil
		-- Collect garbage.
		collectgarbage("collect")
	end
end
App_fncMenuBar[3] = function()
	-- Paste the data.
	if App_MenuBar.Paste.enable then
		for n = 1, Data_Clipboard.lines, 1 do
			if n ~= 1 then App_File:newLine() end
			App_File:write(Data_Clipboard.text[n] or "--(Memory Error)", false)
		end
		-- Update the menubar.
		App_MenuBar.Copy.enable = App_File.selected
		-- Collect garbage.
		collectgarbage("collect")
	end
end
App_fncMenuBar[4] = function()
	-- Show the OSK.
	local result, new_Text = Utility.oskShow("Type the text:")
	if result ~= Utility.OSK_RESULT_CANCELLED then
		App_File:write(new_Text, false)
		-- Update the menubar.
		App_MenuBar.Copy.enable = App_File.selected
	end
	new_Text,result = nil,nil
end
App_fncMenuBar[5] = function()
	App_File:select()
	-- Update the menubar.
	App_MenuBar.Copy.enable = App_File.selected
end
App_fncMenuBar[6] = App_fnc.type
App_fncMenuBar[7] = function()
	-- Goto
	local result, new_Number = Utility.oskShow("Enter the line number:","",Utility.OSK_TYPE_DIGIT)
	if result == Utility.OSK_RESULT_CHANGED then
		new_Number = tonumber( new_Number, 10) or 1
		new_Number = math.floor(new_Number)
		new_Number = math.max( new_Number, 1)
		new_Number = math.min( new_Number, App_File.textLines)
		App_File:CursorLeft(App_File.pntSelected.x-1, true)
		if new_Number < App_File.pntSelected.y then
			App_File:CursorUp(App_File.pntSelected.y-new_Number)
		elseif new_Number > App_File.pntSelected.y then
			App_File:CursorDown(new_Number-App_File.pntSelected.y)
		end
	end
end
-- Set the MenuBar keys.
App_fncMenuBar.FileKey = function()
	pad.cross = Controls.press
	pad.circle = Controls.press
	pad.fnc_circle = false
	pad.fnc_square = false
	pad.fnc_triangle = false
end
App_fncMenuBar.SelectKey = function()
	pad.cross = Controls.press
	pad.circle = Controls.buttons
	pad.fnc_circle = App_fnc.circle
	pad.fnc_square = App_fnc.square
	pad.fnc_triangle = App_fnc.triangle
end
App_fncMenuBar.defaultKey = function()
	pad.cross = Controls.buttons
	pad.circle = Controls.buttons
	pad.fnc_circle = App_fnc.circle
	pad.fnc_square = App_fnc.square
	pad.fnc_triangle = App_fnc.triangle
end
App_fncMenuBar[1+App_MenuIndexMax] = App_fncMenuBar.FileKey
App_fncMenuBar[2+App_MenuIndexMax] = App_fncMenuBar.defaultKey
App_fncMenuBar[3+App_MenuIndexMax] = App_fncMenuBar.defaultKey
App_fncMenuBar[4+App_MenuIndexMax] = App_fncMenuBar.defaultKey
App_fncMenuBar[5+App_MenuIndexMax] = App_fncMenuBar.SelectKey
App_fncMenuBar[6+App_MenuIndexMax] = App_fncMenuBar.defaultKey
App_fncMenuBar[7+App_MenuIndexMax] = App_fncMenuBar.defaultKey
-----------------------------------------------------------------------------------------------------


-----------------------------------------------------------------------------------------------------
-- Apllication Main Loop
while App_Running do
	-- Exit the application on system request.
	if not System.running() then break end

	-- Draw Screen.
	drawFunc()

	-- swap the buffer.
	GL.Flip()

	-- Slow down the Application if a button is pressed.
	if Controls.buttons() > 0 and padcount > 0 then
		padcount = padcount - 1
		System.sleep(App_Sleep)
	elseif padcount ~= App_Count and Controls.buttons() == 0 then
		padcount = App_Count
		--Collect garbage.
		collectgarbage("collect")
	end

	-- Update the controls.
	Controls.read()

	if pad["fnc_start"] and pad["start"](8) then pad["fnc_start"]() -- start button pressed.
	elseif pad["fnc_select"] and pad["select"](1) then pad["fnc_select"]() -- select button pressed.
	elseif pad["fnc_l"] and pad["l"](256) then pad["fnc_l"]()-- l button pressed.
	elseif pad["fnc_r"] and pad["r"](512) then pad["fnc_r"]() -- r button pressed.
	elseif pad["fnc_up"] and pad["up"](16) then pad["fnc_up"]() -- up button pressed.
	elseif pad["fnc_down"] and pad["down"](64) then pad["fnc_down"]() -- down button pressed.
	elseif pad["fnc_left"] and pad["left"](128) then pad["fnc_left"]() -- left button pressed.
	elseif pad["fnc_right"] and pad["right"](32) then pad["fnc_right"]() -- right button pressed.
	elseif pad["fnc_cross"] and pad["cross"](16384) then pad["fnc_cross"]() -- cross button pressed.
	elseif pad["fnc_circle"] and pad["circle"](8192) then pad["fnc_circle"]() -- circle button pressed.
	elseif pad["fnc_square"] and pad["square"](32768) then pad["fnc_square"]() -- square button pressed.
	elseif pad["fnc_triangle"] and pad["triangle"](4096) then pad["fnc_triangle"]() -- triangle button pressed.
	end

	if pad["fnc_any"] and pad["any"](62457) then pad["fnc_any"]() end

	-- Collect the garbage.
	if collectgarbage("step") then collectgarbage("collect") end

end
-----------------------------------------------------------------------------------------------------

-- Dispoe Control Data
pad = nil
padcount = nil

-- Dispose the BitmapFont objects
basicFont = nil
colorFont = nil

-- Dispose the Editer module
Editer.dispose()

-- Dispose the string_ext module
string_ext.dispose()

-- Dipose the FileDialog module
if FileDialog and FileDialog.dispose then
	FileDialog.dispose()
end

-- Dispoe Application Colors
white = nil
clr_transparent = nil
clr_ContolLightLight = nil
clr_ControlLight = nil
clr_Control = nil
clr_ControlDark = nil
clr_ControlDarkDark = nil
clr_ControlText = nil
clr_ControlBorderLight = nil
clr_ControlBorderDark = nil
clr_CaptionText = nil
clr_GrayText = nil
clr_Window = nil
clr_WindowText = nil
clr_WindowFrame = nil
clr_ButtonFace = nil
clr_ButtonHighlight = nil
clr_ButtonShadow = nil
clr_ButtonBorden = nil
clr_ButtonSelected = nil
clr_Highlight = nil
clr_HighlightText = nil
clr_Info = nil
clr_InfoText = nil
clr_MenuBar = nil
clr_Menu = nil
clr_MenuDark = nil
clr_MenuGrayText = nil
clr_MenuText = nil
clr_ScrollBarLight = nil
clr_ScrollBar = nil
clr_ScrollBarDark = nil
clr_ScrollBarBack = nil
clr_Greentext = nil

-- Free All images
ImageMap = nil
ImageNew = nil
ImageOpen = nil
ImageSave = nil
ImageBarDown = nil
ImageBarLeft = nil
ImageBarRight = nil
ImageBarUp = nil

-- Dispoe Application Data
App_Title = nil
App_Running = nil
App_ShowCursor = nil
App_CursorTime = nil
App_fnc = nil
App_MenuIndex = nil
App_MenuIndexMax = nil
App_MenuBar = nil
App_kMenuBar = nil
App_fncMenuBar = nil
App_Menu = nil
App_Menulist = nil
App_fnc_Menu = nil
App_fnc_MenuList = nil
App_File = nil
App_FileList = nil
App_SelectedFile = nil
App_FileCount = nil
App_MaxFile = nil

App_TypeSleep = nil
App_TypeCount = nil
App_TypeButtonSleep = nil
App_TriggerSleep = nil
App_MenuSleep = nil
App_CursorSleep = nil
App_Sleep = nil
App_Count = nil
App_ButtonSleep = nil

-- App_BackUpFile = nil
App_TextSymbols = nil
App_Syntax = nil

-- Dispose clipboard data
if type(Data_Clipboard) == "table" then Data_Clipboard.text = nil end
Data_Clipboard = nil

-- Dispose Type data
Type_index = nil
Type_KeyWords = nil
Type_Output = nil
Type_infoText = nil
Type_Symbols = nil
_BuildTable = nil
BuildTable = nil

-- Dipsose Functions
drawfunc = nil
drawSelectedText = nil
drawCursor = nil
drawhbar = nil
drawvbar = nil
