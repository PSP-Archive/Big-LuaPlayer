-----------------------------------------------------------------------------------------------------
-- Big LuaPlayer: Paint˜ v 1.53 2013-2014 (c) M4MSOFT
-----------------------------------------------------------------------------------------------------

-- Set the Cpu Clock Frequencies.
Power.cpuSpeed(200)

--load the Graphics module.
require "Graphics"

--Set Application Colors
local transparent = Color.new(255, 255, 255, 128)
local ContolLightLight = Color.new(255, 255, 255, 255)
local ControlLight = Color.new(241, 239, 226, 255)
local Control = Color.new(236, 233, 216, 255)
local ControlDark = Color.new(172, 168, 153, 255)
local ControlDarkDark = Color.new(113, 111, 100, 255)
local ControlText = Color.new(0, 0, 0, 255)
local ControlBorderLight = Color.new(0, 72 ,241 , 255)
local ControlBorderDark = Color.new(0, 19, 140, 255)
local CaptionText = Color.new(255, 255, 255, 255)
local GrayText = Color.new(172, 168, 153, 255)
local Window = Color.new(255, 255, 255, 255)
local WindowText = Color.new(0, 0, 0, 255)
local WindowFrame = Color.new(127, 157, 185, 255)
local ButtonFace = Color.new(236, 233, 216, 255)
local ButtonHighlight = Color.new(255, 255, 255, 255)
local ButtonShadow = Color.new(172, 168, 153, 255)
local ButtonBorden = Color.new(0, 60, 116, 255)
local ButtonSelected = Color.new(255, 140, 0, 255)
local Highlight = Color.new(49, 106, 197, 255)
local HighlightText = Color.new(255, 255, 255, 255)
local Info = Color.new(255, 255, 225, 255)
local InfoText = Color.new(0, 0, 0, 255)
local MenuBar = Color.new(236, 233, 216, 255)
local MenuBack = Color.new(241, 239, 226, 255)
local MenuDark = Color.new(172, 168, 153, 255)
local MenuGrayText = Color.new(172,168, 153, 255)
local MenuText = Color.new(0, 0, 0, 255)
local ScrollBarLight = Color.new(255, 255, 255, 255)
local ScrollBar= Color.new(195, 213, 253, 255)
local ScrollBarDark = Color.new(77, 97, 133, 255)
local ScrollBarBack = Color.new(212, 208, 200, 255)
local Greentext = Color.new(0, 144, 0, 255)

--Load Images
local strImagePath = System.Directory.eboot.."/System/Image"
local ImageMap = Texture.load(strImagePath.."/Map.png")
local ImageNew = Texture.load(strImagePath.."/New.png")
local ImageOpen = Texture.load(strImagePath.."/Open.png")
local ImageSave = Texture.load(strImagePath.."/Save.png")
local ImageDelete = Texture.load(strImagePath.."/Delete.png")
local ImageCut = Texture.load(strImagePath.."/Cut.png")
local ImageCopy = Texture.load(strImagePath.."/Copy.png")
local ImagePaste = Texture.load(strImagePath.."/Paste.png")
local ImageDelete = Texture.load(strImagePath.."/Delete.png")
local ImageBackground = Texture.load("Resources/Image/Background.png")
local ImageCursor = Texture.load(strImagePath.."/Cursor.png")
strImagePath = nil

--Set Application Data
local App_Title = "Paint v 1.53"
local App_Running = true
local App_ShowCursor = true
local App_CursorTime = 0
local App_showFileMenu = false
local App_FileMenu =   {
						 File = { text = "New", location = { x = 16 + 16, y = 38}, size = { x = 9 * 8, y = 9}, index = 0, image = ImageNew},
						 Open = { text = "Open", location = { x = 16 + 16, y = 38 + 19 * 1}, size = { x = 9 * 8, y = 9}, index = 1, image = ImageOpen},
						 Save = { text = "Save", location = { x = 16 + 16 ,y = 38 + 19 * 2}, size = { x = 9 * 8, y = 9}, index = 2, image = ImageSave},
						 SaveAs = { text = "Save As", location = { x = 16 + 16, y = 38 + 19 * 3}, size = { x = 9 * 8 , y = 9}, index = 3, image = false},
						 Explorer = { text = "Explorer", location = { x = 16 + 16, y = 38 + 19 * 4}, size = { x = 9 * 8 , y = 9}, index = 4, image = ImageMap},
						 Exit = { text = "Exit", location = { x = 16 + 16, y = 38 + 19 * 5}, size = { x = 9 * 8, y = 9}, index =5, image = false}
						}
local App_showViewMenu = false
local App_ViewMenu =   {
						 ZoomIn = { text = "Zoom In        :triangle", location = { x = 16 + 16, y = 38}, size = { x = 9 * 24, y = 9}, index = 0, image = false, enable = true},
						 ZoomOut = { text = "Zoom Out         :square", location = { x = 16 + 16, y = 38 + 19 * 1}, size = { x = 9 * 24, y = 9}, index = 1, image = false, enable = true},
						 ZoomTo = { text = "Zoom To Window", location = { x = 16 + 16, y = 38 + 19 * 2}, size = { x = 9 * 24, y = 9}, index = 2, image = false, enable = true},
						 Center = { text = "Center Image", location = { x = 16 + 16, y = 38 + 19 * 3}, size = { x = 9 * 24, y = 9}, index = 3, image = false, enable = true}
						}
local App_showImageMenu = false
local App_ImageMenu =   {
						 Unswizzle = { text = "Unswizzle", location = { x = 16 + 16, y = 38}, size = { x = 9 * 14, y = 9}, index = 0, image = false, enable = true},
						 Resize = { text = "Resize", location = { x = 16 + 16, y = 38 + 19 * 1}, size = { x = 9 * 14, y = 9}, index = 1, image = false, enable = true},
						 MoveImage = { text = "Move The Image", location = { x = 16 + 16, y = 38 + 19 *2}, size = { x = 9 * 14, y = 9} ,index = 2, image = false, enable = true},
						 RotateFlip = { text = "RotateFlip", location = { x = 16 + 16, y = 38 + 19 * 3}, size = { x = 9 * 14, y = 9}, index = 3, image = false, enable = true},
						 CutImage = { text = "Cut", location = { x = 16 + 16, y = 38 + 19 * 4}, size = { x = 9 * 14, y = 9} ,index = 4, image = ImageCut, enable = true},
						 CopyImage = { text = "Copy", location = { x = 16 + 16, y = 38 + 19 * 5}, size = { x = 9 * 14, y = 9} ,index = 5, image = ImageCopy, enable = true},
						 PasteImage = { text = "Paste", location = { x = 16 + 16, y = 38 + 19 * 6}, size = { x = 9 * 14, y = 9} ,index = 6, image = ImagePaste, enable = true},
						 DeleteImage = { text = "Delete", location = { x = 16 + 16, y = 38 + 19 * 7}, size = { x = 9 * 14, y = 9} ,index = 7, image = ImageDelete, enable = true},
						 SelectImage = { text = "Select", location = { x = 16 + 16, y = 38 + 19 * 8}, size = { x = 9 * 14, y = 9} ,index = 8, image = false, enable = true}
						}
local App_showGraphicsMenu = false
local App_GraphicsMenu =   {
						 BlendMode = { text = "Blend Mode", location = { x = 16 + 16, y = 38}, size = { x = 9 * 14, y = 9}, index = 0, image = false, enable = true},
						 ColorPicker = { text = "Color Picker", location = { x = 16 + 16, y = 38 + 19 * 1}, size = { x = 9 * 14, y = 9}, index = 1, image = false, enable = true},
						 Clear = { text = "Clear", location = { x = 16 + 16, y = 38 + 19 * 2}, size = { x = 9 * 14, y = 9}, index = 2, image = false, enable = true},
						 Pencil = { text = "Pencil", location = { x = 16 + 16, y = 38 + 19 * 3}, size = { x = 9 * 14, y = 9}, index = 3, image = false, enable = true},
						 FillRectangle = { text = "Fill Rectangle", location = { x = 16 + 16, y = 38 + 19 * 4}, size = { x = 9 * 14, y = 9}, index = 4, image = false, enable = true},
						 PaintBucket = { text = "Paint Bucket", location = { x = 16 + 16, y = 38 + 19 * 5}, size = { x = 9 * 14, y = 9}, index = 5, image = false, enable = true},
						 Recolor = { text = "Recolor", location = { x = 16 + 16, y = 38 + 19 * 6}, size = { x = 9 * 14, y = 9}, index = 6, image = false, enable = true}
						}
local App_MenuArray =  {
						 File = { text = "File:", location = { x = 5, y = 19}, size = { x = 9 * 5, y = 14}, index = 0, enable = true},
						 View = { text = "View:", location = { x = 5 + 9 * 6, y = 19}, size = { x = 9 * 5, y = 14}, index = 1, enable = true},
						 Image = { text = "Image:", location = { x = 5 + 9 * 12, y = 19}, size = { x = 9 * 6, y = 14}, index = 2, enable = true},
						 Rectangle = { text = "Graphics:", location = { x = 5 + 9 * 19, y = 19}, size = { x = 9 * 9, y = 14}, index = 3, enable = true},
						 Color = { text = "<Color>", location = { x = 5 + 9 * 29, y = 19}, size = { x = 9 * 7, y = 14}, index = 4, enable = true},
						 Palette = { text = "<Palette>", location = { x = 5 + 9 * 37, y = 19}, size = { x = 9 * 9, y = 14}, index = 5, enable = true}
						}
local App_SelectedMenu = 0
local App_MenuIndex = 0
local App_MaxMenuIndex = 5
local App_MaxZoom = 100

--Create a empty texture.
local App_MyBitmap = Texture.new(32, 32)
App_MyBitmap:clear(GL.clr.white)
local App_Graphics = Graphics.FromImage(App_MyBitmap)

local xr,yr = 0,0
local App_location = {x = 200 , y =96}
local App_factor = 1
local App_RectangleColor = Color.new( 255, 0, 0, 255)
local App_FileName = "Untitled.png"
local App_Path = System.currentDirectory()
local App_FullFileName = string.format("%s/%s", App_Path, App_FileName)
local App_FileSaved = true
local App_Swizzled = false
local App_BlendMode = Graphics.BlendMode.Enum_SourceOver
local App_Mode = "Normal"
local App_Color = Color.new( 0, 0, 0, 255)
local App_Cursor = {x = 0, y = 0}
local App_MenuMemory = 0

local App_Size = {x = 4, y = 36, width = 480 - 8, height = 272 - 54}

--set clipboard data
local Data_Selection = {Rect = { x = 0, y = 0, w = 0, h = 0},
						  Selected = false
				   }
local Data_Clipboard = { Rect = { x = 0, y = 0, w = 0, h = 0},
						  Selected = false,
						  tex = false
						}
local ClipboardShow = false

-----------------------------------------------------------------------------------------------------
--Fuction ShowMesssage
local ShowMesssage = function (title,text)
	local title = title or "Message:"
	local text = text or "Please wait..."

	local intWidth = 0

	--Initialize the GU (Note : Any graphical functions MUST be placed AFTER this)
	--System.draw()

	intWidth = math.max(string.len(title) * 9 + 16, string.len(text) * 9 + 16 ,150 + 20)
	screen.fillRect((480 - intWidth) / 2 + 2, 119, intWidth, 39, ControlDark)
	screen.fillRect((480 - intWidth) / 2, 117, intWidth, 39, InfoText)
	screen.fillRect((480 - intWidth) / 2 + 1, 118, intWidth - 2, 37, Info)
	screen.print(math.floor((480 - 9 * string.len(title)) / 2), 122 , title, InfoText)
	screen.print(math.floor((480 - 9 * string.len(text)) / 2), 139 , text, InfoText)

	--Finish the GU and Sync
	--System.endDraw()

	--Flip buffers
	screen.flip()

	intWidth = nil

	title = nil
	Message = nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--Fuction Update Menu's
local Update_Menu = function()
	--Disable Zoom Out
	if App_factor <= 0.25 then
		App_ViewMenu.ZoomOut.enable = false
	else
		App_ViewMenu.ZoomOut.enable = true
	end
	--Disable Zoom In
	if App_factor >= App_MaxZoom then
		App_ViewMenu.ZoomIn.enable = false
	else
		App_ViewMenu.ZoomIn.enable = true
	end

	--Disable Unswizzle
	App_ImageMenu.Unswizzle.enable = App_Swizzled
	--Disable Resize
	App_ImageMenu.Resize.enable = not App_Swizzled
	--Disable MoveImage
	App_ImageMenu.MoveImage.enable = not App_Swizzled
	--Disable RotateFlip
	App_ImageMenu.RotateFlip.enable = not App_Swizzled
	--Disable SelectImage
	App_ImageMenu.SelectImage.enable = not App_Swizzled

	--Disable Cut, Copy, Paste and Delete.
	if App_Swizzled then
		App_ImageMenu.CutImage.enable = false
		App_ImageMenu.CopyImage.enable = false
		App_ImageMenu.PasteImage.enable = false
		App_ImageMenu.DeleteImage.enable = false
		App_GraphicsMenu.ColorPicker.enable = false
		App_GraphicsMenu.Clear.enable = false
		App_GraphicsMenu.Pencil.enable = false
		App_GraphicsMenu.FillRectangle.enable = false
		App_GraphicsMenu.PaintBucket.enable = false
		App_GraphicsMenu.Recolor.enable = false
	else
		App_ImageMenu.PasteImage.enable = Data_Clipboard.Selected
		if Data_Selection.Selected then
			App_ImageMenu.CutImage.enable = true
			App_ImageMenu.CopyImage.enable = true
			App_ImageMenu.DeleteImage.enable = true
		else
			App_ImageMenu.CutImage.enable = false
			App_ImageMenu.CopyImage.enable = false
			App_ImageMenu.DeleteImage.enable = false
		end
		App_GraphicsMenu.ColorPicker.enable = true
		App_GraphicsMenu.Clear.enable = true
		App_GraphicsMenu.FillRectangle.enable = true
		App_GraphicsMenu.PaintBucket.enable = true
		App_GraphicsMenu.Recolor.enable = true
		if App_BlendMode == 0 or App_BlendMode == 1 then
			App_GraphicsMenu.Pencil.enable = true
			--App_GraphicsMenu.PaintBucket.enable = true
			--App_GraphicsMenu.Recolor.enable = true
		elseif App_BlendMode == 2 then
			App_GraphicsMenu.Pencil.enable = false
			--App_GraphicsMenu.PaintBucket.enable = false
			--App_GraphicsMenu.Recolor.enable = false
		end
	end

	--Enable BlendMode.
	App_GraphicsMenu.BlendMode.enable = true

end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--Function Zoom screen
local Zoom_screen = function ()
	local My_width = 0
	local My_height = 0
	--Zoom the Image to the screen Dimension
	if App_MyBitmap ~= nil then
		My_width = App_MyBitmap:width()
		My_height = App_MyBitmap:height()
		if My_width <= 0 then My_width = 1 end
		if My_height <= 0 then My_height = 1 end

		App_factor = App_Size.width / My_width
		App_factor = math.floor(App_factor*4)/4

		if My_height * App_factor > App_Size.height then
			App_factor = App_Size.height / My_height
			App_factor = math.floor(App_factor*4)/4
		end

		if App_factor <= 0.25 then App_factor = 0.25 end
		if App_factor >= App_MaxZoom then App_factor = App_MaxZoom end
	end
	My_width = nil
	My_height = nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--Function Center screen
local Center_screen = function ()
	local My_width = 0
	local My_height = 0
	--Center the Image to the screen
	if App_MyBitmap ~= nil then

		My_width = App_MyBitmap:width() * App_factor
		if My_width <= 0 then My_width = 1 end
		My_width = math.floor((My_width) / 2)

		My_height = App_MyBitmap:height() * App_factor
		if My_height <= 0 then My_height = 1 end
		My_height = math.floor((My_height) / 2)

		local screen_width = math.floor(App_Size.width / 2)
		local screen_height = math.floor(App_Size.height / 2)

		App_location.x = App_Size.x + screen_width - My_width
		App_location.y = App_Size.y + screen_height - My_height

		screen_width = nil
		screen_height = nil
	end
	My_width = nil
	My_height = nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--New File
local New_File = function ()
	App_FileName = "Untitled.png"
	App_Path = System.currentDirectory()
	App_FullFileName = string.format("%s/%s", App_Path, App_FileName)
	--Dispose the graphics object.
	if App_Graphics then App_Graphics = App_Graphics:free() or false end
	--Dispose the texture.
	if App_MyBitmap ~= nil then App_MyBitmap = false end
	--Create a empty texture.
	App_MyBitmap = Texture.new(32, 32) or false
	if not App_MyBitmap then error("Out of memory: Can't create the new image.") end
	App_MyBitmap:clear(GL.clr.white)
	--Create the graphics object.
	App_Graphics = Graphics.FromImage(App_MyBitmap)
	--Set Application Data
	App_MenuMemory = 0
	App_Cursor = {x = 0, y = 0}
	App_Mode = "Normal"
	App_FileSaved = true
	App_Swizzled = false
	App_BlendMode = Graphics.BlendMode.Enum_SourceOver
	App_factor = 1
	--Diselect the image.
	Data_Selection = { Rect = { x = 0, y = 0, w = 0, h = 0},
					   Selected = false }
	--Zoom to screen
	Zoom_screen()
	--Center To Sceen
	Center_screen()
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--Load File Function
local load_File = function (name)
	local file_Type = string.lower(string.sub(name, -4))

	--Dispose the graphics object.
	if App_Graphics then App_Graphics = App_Graphics:free() or false end
	--Dispose the texture.
	if App_MyBitmap ~= nil then App_MyBitmap = false end

	if file_Type == ".png" or file_Type == ".jpg" then --Load the file as a png file.
		App_MyBitmap = Texture.load(name) or false
		if App_MyBitmap then
			--Move the texture to the ram.
			--App_MyBitmap:toRam()
			--Create the graphics object.
			App_Graphics = Graphics.FromImage(App_MyBitmap)
			--Set Application Data
			App_MenuMemory = 0
			App_Cursor = {x = 0, y = 0}
			App_Mode = "Normal"
			App_FileSaved = true
			App_Swizzled = true
			App_BlendMode = Graphics.BlendMode.Enum_SourceOver
			App_factor = 1
			--Diselect the image.
			Data_Selection = { Rect = { x = 0, y = 0, w = 0, h = 0},
							   Selected = false }
			--Zoom to screen
			Zoom_screen()
			--Center To Sceen
			Center_screen()
		else
			Utility.msgShow(string.format("Error:\nCan't load the file: %s .", App_FileName))
			New_File()
		end
	elseif file_Type == ".bmp" then --Load the file as a Bitmap file.
		ShowMesssage("Message:","Loading the Bitmap file, please wait...")
		--Load the Bmp.
		App_Graphics = Graphics.FromFile(name)
		--Set My Bitmap.
		App_MyBitmap = false
		App_MyBitmap = App_Graphics.tex
		if App_MyBitmap then
			--Move the texture to the ram.
			--App_MyBitmap:toram()
			--Set Application Data
			App_MenuMemory = 0
			App_Cursor = {x = 0, y = 0}
			App_Mode = "Normal"
			App_FileSaved = true
			App_Swizzled = false
			App_BlendMode = Graphics.BlendMode.Enum_SourceOver
			App_factor = 1
			--Diselect the image.
			Data_Selection = { Rect = { x = 0, y = 0, w = 0, h = 0},
							   Selected = false }
			--Zoom to screen
			Zoom_screen()
			--Center To Sceen
			Center_screen()
		else
			Utility.msgShow(string.format("Error:\nCan't load the file: %s .", App_FileName))
			New_File()
		end
	else
		Utility.msgShow(string.format("Error:\nCan't load the file: %s .", App_FileName))
		New_File()
	end

	file = nil
	name = nil
	file_Type = nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--Save File Function
local save_File = function (name)
	local file_Type = string.lower(string.sub(name, -4))

	if not App_Swizzled then
		if file_Type == ".png" and App_MyBitmap then --Save the file as a png file.
			App_MyBitmap:save(name)
			App_FileSaved = true
		elseif file_Type == ".bmp" and App_Graphics and App_Graphics.tex then --Save the file as a Bitmap file.
			ShowMesssage("Message:","Saving the Bitmap file, please wait...")
			App_Graphics:SaveImage(name,32)
			App_FileSaved = true
		else
			error("Error:\nCan't save the file: %s .")
		end
	else
		error("Error:\nCan't save a swizzled image.")
	end
	name = nil
	file_Type = nil
	return "Save Complete"
end
-----------------------------------------------------------------------------------------------------

--Create A New File
New_File()
Update_Menu()

-----------------------------------------------------------------------------------------------------
--Draw Function
local drawFunc = function ()
	--Initialize the GU (Note : Any graphical functions MUST be placed AFTER this)
	--System.draw()

	--Clear The Screen
	screen.clear(Control)

	--Draw PictureBox Background.
	screen.blit( 4, 36, ImageBackground, 255, 4, 36, 480 - 8, 272 - 54)

	--Draw Image
	if App_MyBitmap ~= nil then

		local x = App_location.x + xr * App_factor
		local y = App_location.y + yr * App_factor
		local sw = App_MyBitmap:width()
		local sh = App_MyBitmap:height()
		local w = sw * App_factor
		local h = sh * App_factor
		local sx,sy = 0,0

		if x < 480 and y < 272 and x+w > 0 and y+h > 0 then
			if App_factor > 1 then
				local x0,y0 = 0,0
				local wM,hM = 0,0

				--Calculate x,y Offset
				x0 = math.fmod (x,App_factor)
				y0 = math.fmod (y,App_factor)

				--Calculate the max source width and height.
				wM = math.ceil((480-x0)/App_factor)
				hM = math.ceil((272-y0)/App_factor)

				--Calculate the source postion.
				sx = (x0-x)/App_factor
				sy = (y0-y)/App_factor

				--Correct the source and Zero location.
				if sx < 0 then
					sx = 0
				else
					x = x + sx*App_factor
				end
				if sy < 0 then
					sy = 0
				else
					y = y + sy*App_factor
				end

				--Calculate the source width and height.
				sw = sw - sx
				sh = sh - sy
				if sw > wM then sw = wM end
				if sh > hM then sh = hM end

				--Calculate the image width and height.
				w = sw * App_factor
				h = sh * App_factor

				x0,y0 = nil,nil
				wM,hM = nil,nil
			end
			--Draw Main Image
			App_MyBitmap:Begin(false)
			App_MyBitmap:Add(x,y,w,h,sx,sy,sw,sh)
			App_MyBitmap:End()
		end
		x,y,w,h = nil,nil,nil,nil
		sx,sy,sw,sh = nil,nil,nil,nil

		--set Color
		if App_RectangleColor:red() == 255 and App_RectangleColor:green() < 255 and App_RectangleColor:blue() == 0 then --increase G
			App_RectangleColor:green(App_RectangleColor:green() + 5)
		elseif App_RectangleColor:red() > 0 and App_RectangleColor:green() == 255 and App_RectangleColor:blue() == 0 then --decrease R
			App_RectangleColor:red(App_RectangleColor:red() - 5)
		elseif App_RectangleColor:red() == 0 and App_RectangleColor:green() == 255 and App_RectangleColor:blue() < 255 then --increase B
			App_RectangleColor:blue(App_RectangleColor:blue() + 5)
		elseif App_RectangleColor:red() == 0 and App_RectangleColor:green() > 0 and App_RectangleColor:blue() == 255 then --decrease G
			App_RectangleColor:green(App_RectangleColor:green() - 5)
		elseif App_RectangleColor:red() < 255 and App_RectangleColor:green() == 0 and App_RectangleColor:blue() == 255 then --increase R
			App_RectangleColor:red(App_RectangleColor:red() + 5)
		elseif App_RectangleColor:red() == 255 and App_RectangleColor:green() == 0 and App_RectangleColor:blue() > 0 then --decrease B
			App_RectangleColor:blue(App_RectangleColor:blue() - 5)
		end

		if App_factor ~= nil then
			--Draw Image Border
			screen.drawRect(App_location.x - 1, App_location.y - 1,
							math.floor(App_Graphics.width*App_factor + 1),
							math.floor(App_Graphics.height*App_factor + 1),
							App_RectangleColor)
			screen.drawRect(App_location.x - 2, App_location.y - 2,
							math.floor(App_Graphics.width*App_factor + 3),
							math.floor(App_Graphics.height*App_factor + 3),
							App_RectangleColor)
		end

		--Draw the Clipboard image.
		if ClipboardShow and Data_Clipboard.Selected and Data_Clipboard.tex then

			local x = App_location.x + Data_Clipboard.Rect.x * App_factor
			local y = App_location.y + Data_Clipboard.Rect.y * App_factor
			local sw = Data_Clipboard.tex:width()
			local sh = Data_Clipboard.tex:height()
			local w = sw * App_factor
			local h = sh * App_factor
			local sx,sy = 0,0

			if x < 480 and y < 272 and x+w > 0 and y+h > 0 then
				if App_factor > 1 then
					local x0,y0 = 0,0
					local wM,hM = 0,0

					--Calculate x,y Offset
					x0 = math.fmod(x,App_factor)
					y0 = math.fmod(y,App_factor)

					--Calculate the max source width and height.
					wM = math.ceil((480-x0)/App_factor)
					hM = math.ceil((272-y0)/App_factor)

					--Calculate the source postion.
					sx = (x0-x)/App_factor
					sy = (y0-y)/App_factor

					--Correct the source and Zero location.
					if sx < 0 then
						sx = 0
					else
						x = x + sx*App_factor
					end
					if sy < 0 then
						sy = 0
					else
						y = y + sy*App_factor
					end

					--Calculate the source width and height.
					sw = sw - sx
					sh = sh - sy
					if sw > wM then sw = wM end
					if sh > hM then sh = hM end

					--Calculate the image width and height.
					w = sw * App_factor
					h = sh * App_factor

					x0,y0 = nil,nil
					wM,hM = nil,nil
				end
				--Draw the Clipboard Image.
				Data_Clipboard.tex:Begin(false)
				Data_Clipboard.tex:Add(x,y,w,h,sx,sy,sw,sh)
				Data_Clipboard.tex:End()
			end

			x,y,w,h = nil,nil,nil,nil
			sx,sy,sw,sh = nil,nil,nil,nil

			--set the color.
			App_RectangleColor:alpha(128)
			--Draw the selection rectangle.
			screen.fillRect(App_location.x + Data_Clipboard.Rect.x*App_factor,
							App_location.y + Data_Clipboard.Rect.y*App_factor,
							Data_Clipboard.Rect.w*App_factor,
							Data_Clipboard.Rect.h*App_factor,
							App_RectangleColor)
			--Reset the color.
			App_RectangleColor:alpha(255)

		end

		if (Data_Selection.Selected and App_SelectedMenu == 2 and not ClipboardShow) or App_Mode == "Rect" then
			--set the color.
			App_RectangleColor:alpha(128)
			--Draw the selection rectangle.
			screen.fillRect(App_location.x + Data_Selection.Rect.x*App_factor,
							App_location.y + Data_Selection.Rect.y*App_factor,
							Data_Selection.Rect.w*App_factor,
							Data_Selection.Rect.h*App_factor,
							App_RectangleColor)
			--Reset the color.
			App_RectangleColor:alpha(255)
		end

		if App_Mode == "Picker" or App_Mode == "Pencil" or App_Mode == "Bucket" or App_Mode == "Recolor" then --draw the Cursor.
			local x = App_location.x + xr * App_factor + App_Cursor.x * App_factor
			local y = App_location.y + yr * App_factor + App_Cursor.y * App_factor

			--Calculate x,y Offset
			local x0 = math.floor(App_factor/2)
			local y0 = math.floor(App_factor/2)

			if App_factor > 10 then
				--Draw the cursor rectangle.
				screen.drawRect(math.floor(x),
								math.floor(y),
								math.floor(App_factor), math.floor(App_factor),
								App_RectangleColor)
			end

			--Draw the Cursor.
			ImageCursor:render(x0+x, y0+y)

			x,y = nil,nil
			x0,y0 = nil,nil
		end

	end

	--Draw Main Window
	screen.drawRect( 0, 0, 480-1, 272-1, ControlBorderDark)
	screen.drawRect( 1, 1, 480 - 3, 272 - 3, ControlBorderLight)
	screen.fillRect( 1, 1, 480 - 2, 15, ControlBorderLight)
	screen.drawLine( 2, 15, 480 - 3, 15, ControlBorderDark)
	screen.drawRect( 2, 34, 480 - 5, 272 - 51, Control)
	screen.fillRect( 2, 272 - 13 -3, 480 - 4, 14, Control)
	screen.drawLine( 2, 16, 480 - 3, 16, ControlLight)
	screen.drawLine( 2, 16, 2, 272 - 5, ControlLight)
	screen.drawLine( 2, 272 - 3, 480 - 2, 272 - 3, ControlDark)
	screen.drawLine( 480 - 3, 272 - 3, 480 - 3, 16, ControlDark)
	screen.drawLine( 2, 272-16, 480-2 ,272-16 , ControlDark)
	screen.print( 3, 272 - 14, string.format("File:%s | Saved:%s | Mode:%s", App_FileName, tostring(App_FileSaved), App_Mode ), ControlText)

	--Draw Menu
	screen.fillRect( 2, 16, 480 - 4, 17, MenuBar)
	screen.drawLine( 2, 33, 480 - 3, 33 , MenuDark)

	--Draw PictureBox Background.
	screen.drawRect( 3, 35, 480 - 7, 272 - 53, WindowFrame)

	for index, Menu in pairs(App_MenuArray) do
		if App_SelectedMenu == Menu.index then
			screen.fillRect( Menu.location.x - 2 , Menu.location.y - 2, Menu.size.x + 4, Menu.size.y + 1, Highlight)
			if Menu.enable == true then
				screen.print( Menu.location.x, Menu.location.y, Menu.text, HighlightText)
			else
				screen.print( Menu.location.x, Menu.location.y, Menu.text, GrayText)
			end
			--Draw File Menu
			if Menu.index == 0 and App_showFileMenu == true then
				for index2, Menu2 in pairs(App_FileMenu) do
					screen.fillRect( Menu2.location.x - 21 , Menu2.location.y - 4 , Menu2.size.x + 30, Menu2.size.y + 11, MenuDark)
					screen.fillRect( Menu2.location.x - 20 , Menu2.location.y - 3 , Menu2.size.x + 28, Menu2.size.y + 9, MenuBack)
					if not Menu2.image == false then screen.blit( Menu2.location.x - 18 , Menu2.location.y - 2 , Menu2.image) end
					if App_MenuIndex == Menu2.index then
						screen.fillRect( Menu2.location.x , Menu2.location.y -1, Menu2.size.x + 6, Menu2.size.y + 5, Highlight)
						screen.print( Menu2.location.x + 2, Menu2.location.y + 2-2, Menu2.text, HighlightText)
					else
						screen.fillRect( Menu2.location.x, Menu2.location.y -1, Menu2.size.x + 6, Menu2.size.y + 5, MenuBack)
						screen.print( Menu2.location.x + 2, Menu2.location.y + 2-2, Menu2.text, MenuText)
					end
				end
			end
			--Draw View Menu
			if Menu.index == 1 and App_showViewMenu == true then
				for index2, Menu2 in pairs(App_ViewMenu) do
					screen.fillRect( Menu2.location.x - 21 + Menu.location.x, Menu2.location.y - 4 , Menu2.size.x + 14, Menu2.size.y + 11, MenuDark)
					screen.fillRect( Menu2.location.x - 20 + Menu.location.x, Menu2.location.y - 3 , Menu2.size.x + 12, Menu2.size.y + 9, MenuBack)
					if App_MenuIndex == Menu2.index then
						screen.fillRect( Menu2.location.x - 18 + Menu.location.x, Menu2.location.y-1, Menu2.size.x + 8, Menu2.size.y + 5, Highlight)
						if Menu2.enable == true then
							screen.print( Menu2.location.x - 14 + Menu.location.x, Menu2.location.y, Menu2.text, HighlightText)
						else
							screen.print( Menu2.location.x - 14 + Menu.location.x, Menu2.location.y, Menu2.text, GrayText)
						end
					else
						screen.fillRect( Menu2.location.x - 18 + Menu.location.x, Menu2.location.y-1, Menu2.size.x + 8, Menu2.size.y + 5, MenuBack)
						if Menu2.enable == true then
							screen.print( Menu2.location.x - 14 + Menu.location.x, Menu2.location.y, Menu2.text, MenuText)
						else
							screen.print( Menu2.location.x - 14 + Menu.location.x, Menu2.location.y, Menu2.text, GrayText)
						end
					end
				end
			end
			--Draw Image Menu
			if Menu.index == 2 and App_showImageMenu == true then
				for index2, Menu2 in pairs(App_ImageMenu) do
					screen.fillRect( Menu2.location.x - 21 + Menu.location.x, Menu2.location.y - 4 , Menu2.size.x + 30, Menu2.size.y + 11, MenuDark)
					screen.fillRect( Menu2.location.x - 20 + Menu.location.x, Menu2.location.y - 3 , Menu2.size.x + 28, Menu2.size.y + 9, MenuBack)
					if not Menu2.image == false and Menu2.enable == true then
						screen.blit( Menu2.location.x - 18 + Menu.location.x, Menu2.location.y - 2 , Menu2.image)
					elseif not Menu2.image == false then
						screen.blit( Menu2.location.x - 18 + Menu.location.x, Menu2.location.y - 2 , Menu2.image, 64)
					end
					if App_MenuIndex == Menu2.index then
						screen.fillRect( Menu2.location.x + Menu.location.x, Menu2.location.y -1, Menu2.size.x + 6, Menu2.size.y + 5, Highlight)
						if Menu2.enable == true then
							screen.print( Menu2.location.x + 2 + Menu.location.x, Menu2.location.y + 2-2, Menu2.text, HighlightText)
						else
							screen.print( Menu2.location.x + 2 + Menu.location.x, Menu2.location.y + 2-2, Menu2.text, GrayText)
						end
					else
						screen.fillRect( Menu2.location.x + Menu.location.x, Menu2.location.y -1, Menu2.size.x + 6, Menu2.size.y + 5, MenuBack)
						if Menu2.enable == true then
							screen.print( Menu2.location.x + 2 + Menu.location.x, Menu2.location.y + 2-2, Menu2.text, MenuText)
						else
							screen.print( Menu2.location.x + 2 + Menu.location.x, Menu2.location.y + 2-2, Menu2.text, GrayText)
						end
					end
				end
			end
			--Draw Graphics Menu
			 if Menu.index == 3 and App_showGraphicsMenu == true then
				for index2, Menu2 in pairs(App_GraphicsMenu) do
					screen.fillRect( Menu2.location.x - 21 + Menu.location.x, Menu2.location.y - 4 , Menu2.size.x + 14, Menu2.size.y + 11, MenuDark)
					screen.fillRect( Menu2.location.x - 20 + Menu.location.x, Menu2.location.y - 3 , Menu2.size.x + 12, Menu2.size.y + 9, MenuBack)
					if App_MenuIndex == Menu2.index then
						screen.fillRect( Menu2.location.x - 18 + Menu.location.x, Menu2.location.y-1, Menu2.size.x + 8, Menu2.size.y + 5, Highlight)
						if Menu2.enable == true then
							screen.print( Menu2.location.x - 14 + Menu.location.x, Menu2.location.y, Menu2.text, HighlightText)
						else
							screen.print( Menu2.location.x - 14 + Menu.location.x, Menu2.location.y, Menu2.text, GrayText)
						end
					else
						screen.fillRect( Menu2.location.x - 18 + Menu.location.x, Menu2.location.y-1, Menu2.size.x + 8, Menu2.size.y + 5, MenuBack)
						if Menu2.enable == true then
							screen.print( Menu2.location.x - 14 + Menu.location.x, Menu2.location.y, Menu2.text, MenuText)
						else
							screen.print( Menu2.location.x - 14 + Menu.location.x, Menu2.location.y, Menu2.text, GrayText)
						end
					end
				end
			end
		else
			if Menu.enable == true then
				screen.print( Menu.location.x, Menu.location.y, Menu.text, MenuText)
			else
				screen.print( Menu.location.x, Menu.location.y, Menu.text, GrayText)
			end
		end
	end

	--Print Window Text
	--screen:print( 4, 2, string.format("%s - %s - %d kb", App_FileName,App_Title, collectgarbage("count") ), CaptionText)
	screen.print( 4, 2, string.format("%s - %s", App_FileName,App_Title ), CaptionText)

	--Finish the GU and Sync
	--System.endDraw()
end
-----------------------------------------------------------------------------------------------------

--Control Data
local padcount = 0

-----------------------------------------------------------------------------------------------------
--Update the zoom factor.
local UpdateZoom = function (value)
	local value = value or 0

	App_factor = App_factor + value
	if App_factor > App_MaxZoom then
		App_factor = App_MaxZoom
	elseif App_factor < 0.25 then
		App_factor = 0.25
	else
		--Update then App_location value.
		if App_MyBitmap ~= nil and value ~= 0 then
			local My_width = 0
			local My_height = 0

			My_width = App_MyBitmap:width() * (App_factor-value)
			if My_width <= 0 then My_width = 1 end
			My_width = math.floor((My_width) / 2)

			My_height = App_MyBitmap:height() * (App_factor-value)
			if My_height <= 0 then My_height = 1 end
			My_height = math.floor((My_height) / 2)

			local screen_width = math.floor(App_Size.width / 2)
			local screen_height = math.floor(App_Size.height / 2)

			local xoffset = App_location.x - App_Size.x - screen_width + My_width
			local yoffset = App_location.y - App_Size.y - screen_height + My_height

			My_width = App_MyBitmap:width() * (App_factor)
			if My_width <= 0 then My_width = 1 end
			My_width = math.floor((My_width) / 2)

			My_height = App_MyBitmap:height() * (App_factor)
			if My_height <= 0 then My_height = 1 end
			My_height = math.floor((My_height) / 2)

			App_location.x = xoffset + App_Size.x + screen_width - My_width
			App_location.y = yoffset + App_Size.y + screen_height - My_height

			My_width = nil
			My_height = nil
			screen_width = nil
			screen_height = nil
			xoffset = nil
			yoffset = nil
		end
	end

end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
local MoveZoom_Controls =  function ()

	--Resize the bitmap
	if Controls.buttons(32768) then --square Button Pressed
		Check = false
		--Zoom Out
		UpdateZoom(-0.25)
		Update_Menu()
	elseif Controls.buttons(4096) then --triangle Button Pressed
		Check = false
		--Zoom in
		UpdateZoom(0.25)
		Update_Menu()
	end

	--Move the Bitmap
	local y = Controls.analogy()
	local x = Controls.analogx()
	if y < - 40 or y > 40 then --analogY stick used.
		if y < 0 and y < -120 then y = -120 end
		if y > 0 and y > 120 then y = 120 end
		App_location.y = App_location.y + math.floor(y / 16)
	end
	if x < - 40 or x > 40 then --analogX stick used.
		if x < 0 and x < -120 then x = -120 end
		if x > 0 and x > 120 then x = 120 end
		App_location.x = App_location.x + math.floor(x / 16)
	end
	y,x = nil,nil

end
-----------------------------------------------------------------------------------------------------

local Window_InputSize = { x = 0, y = 0 }
local Window_OutPutSize = { x = 0, y = 0 }
local Window_CursorLocation = { x = 0, y = 0 }

-----------------------------------------------------------------------------------------------------
--Show the Resize Window
local Resize = function()
	Window_Result_Ok = false
	Window_InputSize = { x = App_MyBitmap:width(), y = App_MyBitmap:height() }
	Window_OutPutSize = { x = App_MyBitmap:width(), y = App_MyBitmap:height() }
	Window_CursorLocation = { x = 0, y = 0 }

	local intWidth = 9 * 22
	local intheight = 64
	local x,y = 0,0

	while System.running() do

		--Draw Screen
		drawFunc()

		--Initialize the GU (Note : Any graphical functions MUST be placed AFTER this)
		--System.draw()

		--Draw the Type Window
		x = (480 - intWidth) / 2
		y = (272 - intheight) / 2
		screen.fillRect( x + 2, y + 2, intWidth, intheight, ControlDark)
		screen.fillRect( x, y, intWidth, intheight, InfoText)
		screen.fillRect( x + 1, y + 1, intWidth - 2, intheight - 2, Info)
		screen.print( x + 4, y + 3, "--- Resize Image: ---", InfoText)
		screen.print( x + 4, y + intheight - 13, "O : Cancel X : Accept", InfoText)
		if Window_CursorLocation.y == 0 then
			screen.print( x + 4, y + 6 + 14 * 1, string.format(">>  < Width:%s >", Window_OutPutSize.x), InfoText)
			screen.print( x + 4, y + 6 + 14 * 2, string.format("   height:%s", Window_OutPutSize.y), InfoText)
		else
			screen.print( x + 4, y + 6 + 14 * 1, string.format("   Width:%s", Window_OutPutSize.x), InfoText)
			screen.print( x + 4, y + 6 + 14 * 2, string.format(">>  < height:%s >", Window_OutPutSize.y), InfoText)
		end

		--Finish the GU and Sync
		--System.endDraw()

		--Flip buffers
		screen.flip()

		--Slow down the Application if a button is pressed
		if Controls.buttons() > 0 and padcount > 0 then
			padcount = padcount - 1
			screen.waitVblankStart(5)
		elseif padcount ~= 10 and Controls.buttons() == 0 then
			padcount = 10
		end

		--update the controls
		Controls.readPeek()

		if Controls.press(1) or Controls.press(8) or Controls.press(256)
		or Controls.press(512) or Controls.press(8192) then --exit button pressed
			--Break the Type Loop and cancel the input
			Window_Result_Ok = false
			break
		elseif Controls.buttons(16) then --up button pressed
			Window_CursorLocation.y = Window_CursorLocation.y - 1
			if Window_CursorLocation.y < 0 then Window_CursorLocation.y = 1 end
		elseif Controls.buttons(64) then --down button pressed
			Window_CursorLocation.y = Window_CursorLocation.y + 1
			if Window_CursorLocation.y > 1 then Window_CursorLocation.y = 0 end
		elseif Controls.buttons(128) then --left button pressed
			if Window_CursorLocation.y == 0 then
				Window_OutPutSize.x = Window_OutPutSize.x - 1
				if Window_OutPutSize.x < 1 then Window_OutPutSize.x = 1 end
				App_Graphics.width = Window_OutPutSize.x
			else
				Window_OutPutSize.y = Window_OutPutSize.y - 1
				if Window_OutPutSize.y < 1 then Window_OutPutSize.y = 1 end
				App_Graphics.height = Window_OutPutSize.y
			end
		elseif Controls.buttons(32) then --right button pressed
			if Window_CursorLocation.y == 0 then
				Window_OutPutSize.x = Window_OutPutSize.x + 1
				App_Graphics.width = Window_OutPutSize.x
			else
				Window_OutPutSize.y = Window_OutPutSize.y + 1
				App_Graphics.height = Window_OutPutSize.y
			end
		elseif Controls.press(16384) then --cross button pressed
			Window_Result_Ok = true
			--Break the Type Loop
			break
		end

		--Move and Zoom Controls
		MoveZoom_Controls()

	end
	if not Window_Result_Ok
	or (Window_InputSize.x == Window_OutPutSize.x and Window_InputSize.y == Window_OutPutSize.y) then
		App_Graphics.width = Window_InputSize.x
		App_Graphics.height = Window_InputSize.y
	else --Resize the Image.
		--Draw Screen
		drawFunc()
		ShowMesssage("Message:","Updating the Image, Please wait...")
		--Create a empty texture.
		App_Graphics.tex = false
		App_Graphics.tex = Texture.new(App_Graphics.width, App_Graphics.height)
		if App_Graphics.tex then
			App_Graphics.tex:clear(GL.clr.white)
			--Move the texture to the ram.
			--App_Graphics.tex:toram()
			--set the BlendMode.
			App_Graphics.BlendMode = Graphics.BlendMode.Enum_SourceCopy
			--Draw the image.
			App_Graphics:DrawImage(App_MyBitmap)
			--Set My Bitmap.
			App_MyBitmap = false
			App_MyBitmap = App_Graphics.tex
			--Set Application Data
			if App_Cursor.x > App_Graphics.width-1 then App_Cursor.x = App_Graphics.width-1 end
			if App_Cursor.y > App_Graphics.height-1 then App_Cursor.y = App_Graphics.height-1 end
			App_Graphics.BlendMode = App_BlendMode
			App_Swizzled = false
			App_FileSaved = false
		else
			--Restore the Graphics Object.
			App_Graphics.tex = App_MyBitmap
			App_Graphics.width = App_MyBitmap:width()
			App_Graphics.height = App_MyBitmap:height()
			--Show The Error Message.
			ShowMesssage("Out of memory:","Can't resize the image.")
			while System.running() do
				--update the controls
				Controls.read()

				if Controls.press() > 0 then
					break
				end
			end
		end
	end

	intWidth = nil
	intheight = nil
	x,y = nil,nil

	return Window_Result_Ok
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--Show the MoveImage Window.
local MoveImage = function()
	Window_Result_Ok = false
	Window_InputSize = { x = 0, y = 0 }
	Window_OutPutSize = { x = 0, y = 0 }
	Window_CursorLocation = { x = 0, y = 0 }

	local intWidth = 9 * 22
	local intheight = 64
	local x,y = 0,0

	while System.running() do

		--Draw Screen
		drawFunc()

		--Initialize the GU (Note : Any graphical functions MUST be placed AFTER this)
		--System.draw()

		--Draw the Type Window
		x = (480 - intWidth) / 2
		y = (272 - intheight) / 2
		screen.fillRect( x + 2, y + 2, intWidth, intheight, ControlDark)
		screen.fillRect( x, y, intWidth, intheight, InfoText)
		screen.fillRect( x + 1, y + 1, intWidth - 2, intheight - 2, Info)
		screen.print( x + 4, y + 3, "---  Move Image:  ---", InfoText)
		screen.print( x + 4, y + intheight - 13, "O : Cancel X : Accept", InfoText)
		if Window_CursorLocation.y == 0 then
			screen.print( x + 4, y + 6 + 14 * 1, string.format(">>  < x:%s >", Window_OutPutSize.x), InfoText)
			screen.print( x + 4, y + 6 + 14 * 2, string.format("   y:%s", Window_OutPutSize.y), InfoText)
		else
			screen.print( x + 4, y + 6 + 14 * 1, string.format("   x:%s", Window_OutPutSize.x), InfoText)
			screen.print( x + 4, y + 6 + 14 * 2, string.format(">>  < y:%s >", Window_OutPutSize.y), InfoText)
		end

		--Finish the GU and Sync
		--System.endDraw()

		--Flip buffers
		screen.flip()

		--Slow down the Application if a button is pressed
		if Controls.buttons() > 0 and padcount > 0 then
			padcount = padcount - 1
			screen.waitVblankStart(5)
		elseif padcount ~= 10 and Controls.buttons() == 0 then
			padcount = 10
		end

		--update the controls
		Controls.readPeek()

		if Controls.press(1) or Controls.press(8) or Controls.press(256)
		or Controls.press(512) or Controls.press(8192) then --exit button pressed
			--Break the Type Loop and cancel the input
			Window_Result_Ok = false
			break
		elseif Controls.buttons(16) then --up button pressed
			Window_CursorLocation.y = Window_CursorLocation.y - 1
			if Window_CursorLocation.y < 0 then Window_CursorLocation.y = 1 end
		elseif Controls.buttons(64) then --down button pressed
			Window_CursorLocation.y = Window_CursorLocation.y + 1
			if Window_CursorLocation.y > 1 then Window_CursorLocation.y = 0 end
		elseif Controls.buttons(128) then --left button pressed
			if Window_CursorLocation.y == 0 then
				Window_OutPutSize.x = Window_OutPutSize.x - 1
				xr = Window_OutPutSize.x
			else
				Window_OutPutSize.y = Window_OutPutSize.y - 1
				yr = Window_OutPutSize.y
			end
		elseif Controls.buttons(32) then --right button pressed
			if Window_CursorLocation.y == 0 then
				Window_OutPutSize.x = Window_OutPutSize.x + 1
				xr = Window_OutPutSize.x
			else
				Window_OutPutSize.y = Window_OutPutSize.y + 1
				yr = Window_OutPutSize.y
			end
		elseif Controls.press(16384) then --cross button pressed
			Window_Result_Ok = true
			--Break the Type Loop
			break
		end

		--Move and Zoom Controls
		MoveZoom_Controls()

	end

	if not Window_Result_Ok
	or (Window_InputSize.x == Window_OutPutSize.x and Window_InputSize.y == Window_OutPutSize.y) then
		xr = 0
		yr = 0
	else --Move the image.
		--Draw Screen
		drawFunc()
		ShowMesssage("Message:","Updating the Image, Please wait...")
		--Create a empty texture.
		App_Graphics.tex = false
		App_Graphics.tex = Texture.new(App_Graphics.width, App_Graphics.height) or false
		if App_Graphics.tex then
			App_Graphics.tex:clear(GL.clr.white)
			--Move the texture to the ram.
			--App_Graphics.tex:toram()
			--set the BlendMode.
			App_Graphics.BlendMode = Graphics.BlendMode.Enum_SourceCopy
			--Draw the image.
			App_Graphics:DrawImage(App_MyBitmap, xr, yr)
			--Set My Bitmap.
			App_MyBitmap = false
			App_MyBitmap = App_Graphics.tex
			--Set Application Data
			App_Graphics.BlendMode = App_BlendMode
			App_FileSaved = false
		else
			--Restore the Graphics Object.
			App_Graphics.tex = App_MyBitmap
			--Show The Error Message.
			ShowMesssage("Out of memory:","Can't move the image.")
			while System.running() do
				--update the controls
				Controls.read()

				if Controls.press() > 0 then
					break
				end
			end
		end
		xr = 0
		yr = 0
	end

	intWidth = nil
	intheight = nil
	x,y = nil,nil

	return Window_Result_Ok
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--Show the RotateFlip Window.
local RotateFlip = function()
	Window_Result_Ok = false
	Window_InputSize = { x = 0, y = 0 }
	Window_OutPutSize = { x = 0, y = 0 }
	Window_CursorLocation = { x = 0, y = 0 }

	local FlipX = false
	local FlipY = false
	local FlipZ = false
	local Rotation = 0

	local intWidth = 9 * 22
	local intheight = 64 + 28
	local x,y = 0,0

	while System.running() do

		--Draw Screen
		drawFunc()

		--Initialize the GU (Note : Any graphical functions MUST be placed AFTER this)
		--System.draw()

		--Draw the Type Window
		x = (480 - intWidth) / 2
		y = (272 - intheight) / 2
		screen.fillRect( x + 2, y + 2, intWidth, intheight, ControlDark)
		screen.fillRect( x, y, intWidth, intheight, InfoText)
		screen.fillRect( x + 1, y + 1, intWidth - 2, intheight - 2, Info)
		screen.print( x + 4, y + 3, "---  RotateFlip:  ---", InfoText)
		screen.print( x + 4, y + intheight - 13, "O : Cancel X : Accept", InfoText)
		if Window_CursorLocation.y == 0 then
			screen.print( x + 4, y + 6 + 14 * 1, string.format(">>  < FlipX:%s >", tostring(FlipX)), InfoText)
			screen.print( x + 4, y + 6 + 14 * 2, string.format("   FlipY:%s", tostring(FlipY)), InfoText)
			screen.print( x + 4, y + 6 + 14 * 3, string.format("   FlipZ:%s", tostring(FlipZ)), InfoText)
			screen.print( x + 4, y + 6 + 14 * 4, string.format("   Angle:%d", Rotation), InfoText)
		elseif Window_CursorLocation.y == 1 then
			screen.print( x + 4, y + 6 + 14 * 1, string.format("   FlipX:%s", tostring(FlipX)), InfoText)
			screen.print( x + 4, y + 6 + 14 * 2, string.format(">>  < FlipY:%s >", tostring(FlipY)), InfoText)
			screen.print( x + 4, y + 6 + 14 * 3, string.format("   FlipZ:%s", tostring(FlipZ)), InfoText)
			screen.print( x + 4, y + 6 + 14 * 4, string.format("   Angle:%d", Rotation), InfoText)
		elseif Window_CursorLocation.y == 2 then
			screen.print( x + 4, y + 6 + 14 * 1, string.format("   FlipX:%s", tostring(FlipX)), InfoText)
			screen.print( x + 4, y + 6 + 14 * 2, string.format("   FlipY:%s", tostring(FlipY)), InfoText)
			screen.print( x + 4, y + 6 + 14 * 3, string.format(">>  < FlipZ:%s >", tostring(FlipZ)), InfoText)
			screen.print( x + 4, y + 6 + 14 * 4, string.format("   Angle:%d", Rotation), InfoText)
		else
			screen.print( x + 4, y + 6 + 14 * 1, string.format("   FlipX:%s", tostring(FlipX)), InfoText)
			screen.print( x + 4, y + 6 + 14 * 2, string.format("   FlipY:%s", tostring(FlipY)), InfoText)
			screen.print( x + 4, y + 6 + 14 * 3, string.format("   FlipZ:%s", tostring(FlipZ)), InfoText)
			screen.print( x + 4, y + 6 + 14 * 4, string.format(">>  < Angle:%d >", Rotation), InfoText)
		end

		--Finish the GU and Sync
		--System.endDraw()

		--Flip buffers
		screen.flip()

		--Slow down the Application if a button is pressed
		if Controls.buttons() > 0 and padcount > 0 then
			padcount = padcount - 1
			screen.waitVblankStart(5)
		elseif padcount ~= 10 and Controls.buttons() == 0 then
			padcount = 10
		end

		--update the controls
		Controls.readPeek()

		if Controls.press(1) or Controls.press(8) or Controls.press(256)
		or Controls.press(512) or Controls.press(8192) then --exit button pressed
			--Break the Type Loop and cancel the input
			Window_Result_Ok = false
			break
		elseif Controls.buttons(16) then --up button pressed
			Window_CursorLocation.y = Window_CursorLocation.y - 1
			if Window_CursorLocation.y < 0 then Window_CursorLocation.y = 3 end
		elseif Controls.buttons(64) then --down button pressed
			Window_CursorLocation.y = Window_CursorLocation.y + 1
			if Window_CursorLocation.y > 3 then Window_CursorLocation.y = 0 end
		elseif Controls.buttons(128) then --left button pressed
			if Window_CursorLocation.y == 0 then
				FlipX = not FlipX
			elseif Window_CursorLocation.y == 1 then
				FlipY = not FlipY
			elseif Window_CursorLocation.y == 2 then
				FlipZ = not FlipZ
			else
				Rotation = Rotation - 90
				if Rotation < 0 then Rotation = 270 end
			end
		elseif Controls.buttons(32) then --right button pressed
			if Window_CursorLocation.y == 0 then
				FlipX = not FlipX
			elseif Window_CursorLocation.y == 1 then
				FlipY = not FlipY
			elseif Window_CursorLocation.y == 2 then
				FlipZ = not FlipZ
			else
				Rotation = Rotation + 90
				if Rotation > 270 then Rotation = 0 end
			end
		elseif Controls.press(16384) then --cross button pressed
			Window_Result_Ok = true
			--Break the Type Loop
			break
		end

		--Move and Zoom Controls
		MoveZoom_Controls()

	end

	if Window_Result_Ok then
		-- Calculate the RotateFlip Style.
		local intStyle = 0
		if FlipX then intStyle = intStyle + 1 end
		if FlipY then intStyle = intStyle + 2 end
		if FlipZ then intStyle = intStyle + 4 end
		if Rotation == 90 then intStyle = intStyle + 8 end
		if Rotation == 180 then intStyle = intStyle + 16 end
		if Rotation == 270 then intStyle = intStyle + 32 end
		--Draw Screen
		drawFunc()
		ShowMesssage("Message:","Updating the Image, Please wait...")
		-- RotateFlip the image.
		App_MyBitmap = Graphics.RotateFlip(App_MyBitmap, intStyle)
		if App_MyBitmap then
			--Dispose the graphics object.
			if App_Graphics then App_Graphics = App_Graphics:free() or false end
			--Create the graphics object.
			App_Graphics = Graphics.FromImage(App_MyBitmap)
			--Set Application Data
			App_Graphics.BlendMode = App_BlendMode
			App_FileSaved = false
			App_MenuMemory = 0
			App_Cursor = {x = 0, y = 0}
			App_Mode = "Normal"
			--App_BlendMode = Graphics.BlendMode.Enum_SourceOver
			App_factor = 1
			--Diselect the image.
				Data_Selection = { Rect = { x = 0, y = 0, w = 0, h = 0},
								   Selected = false }
			--Zoom to screen
			Zoom_screen()
			--Center To Sceen
			Center_screen()
		else
			--Restore the Bitmap.
			App_MyBitmap = App_Graphics.tex
			--Show The Error Message.
			ShowMesssage("Out of memory:",string.format("Can't Apply the RotateFlipType=%s.",tostring(intStyle)))
			while System.running() do
				--update the controls
				Controls.read()

				if Controls.press() > 0 then
					break
				end
			end
		end
		intStyle = nil
	end

	FlipX = nil
	FlipY = nil
	FlipZ = nil
	Rotation = nil

	intWidth = nil
	intheight = nil
	x,y = nil,nil

	return Window_Result_Ok
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--Cut the image.
local CutImage = function()
	if Data_Selection.Selected then
		--Draw Screen
		drawFunc()
		ShowMesssage("Message:","Cutting the Image, Please wait...")
		if Data_Clipboard.Selected then
			--Clear the Clipboard.
			Data_Clipboard.tex = nil
			Data_Clipboard = { Rect = { x = 0, y = 0, w = 0, h = 0},
							   Selected = false,
							   tex = false
							 }
		end
		--Copy the image to the clipboard.
		Data_Clipboard.Rect.x = Data_Selection.Rect.x
		Data_Clipboard.Rect.y = Data_Selection.Rect.y
		Data_Clipboard.Rect.w = Data_Selection.Rect.w
		Data_Clipboard.Rect.h = Data_Selection.Rect.h
		Data_Clipboard.Selected = true
		Data_Clipboard.tex = Texture.new(Data_Selection.Rect.w, Data_Selection.Rect.h) or false
		if Data_Clipboard.tex then
			Data_Clipboard.tex:clear(GL.clr.white)
			--Data_Clipboard.tex:toram()
			--Create the graphics object.
			local objGraphics = Graphics.FromImage(Data_Clipboard.tex)
			--set the BlendMode.
			objGraphics.BlendMode = Graphics.BlendMode.Enum_SourceCopy
			--Draw the image to the clipboard,
			objGraphics:DrawImage( App_MyBitmap, 0, 0, Data_Selection.Rect.x, Data_Selection.Rect.y,
													   Data_Selection.Rect.w, Data_Selection.Rect.h )
			--Dispose the graphics object.
			objGraphics = objGraphics:free()
			--set the BlendMode.
			App_Graphics.BlendMode = Graphics.BlendMode.Enum_SourceCopy
			--Draw the retangle.
			App_Graphics:FillRectangle(Data_Selection.Rect.x, Data_Selection.Rect.y,
									   Data_Selection.Rect.w, Data_Selection.Rect.h,
									  Color.new(0,0,0,0))
			--Set Application Data
			App_Graphics.BlendMode = App_BlendMode
			App_FileSaved = false
		else
			-- Restore the Clipboard.
			Data_Clipboard = { Rect = { x = 0, y = 0, w = 0, h = 0},
							   Selected = false,
							   tex = false
							 }
			--Show The Error Message.
			ShowMesssage("Out of memory:","Can't cut the image.")
			while System.running() do
				--update the controls
				Controls.read()

				if Controls.press() > 0 then
					break
				end
			end
		end
	end
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--Copy the Image.
local CopyImage = function()
	if Data_Selection.Selected then
		--Draw Screen
		drawFunc()
		ShowMesssage("Message:","Copying the Image, Please wait...")
		if Data_Clipboard.Selected then
			--Clear the Clipboard.
			Data_Clipboard.tex = nil
			Data_Clipboard = { Rect = { x = 0, y = 0, w = 0, h = 0},
							   Selected = false,
							   tex = false
							 }
		end
		--Copy the image to the clipboard.
		Data_Clipboard.Rect.x = Data_Selection.Rect.x
		Data_Clipboard.Rect.y = Data_Selection.Rect.y
		Data_Clipboard.Rect.w = Data_Selection.Rect.w
		Data_Clipboard.Rect.h = Data_Selection.Rect.h
		Data_Clipboard.Selected = true
		Data_Clipboard.tex = Texture.new(Data_Selection.Rect.w, Data_Selection.Rect.h) or false
		if Data_Clipboard.tex then
			Data_Clipboard.tex:clear(GL.clr.white)
			--Data_Clipboard.tex:toram()
			--Create the graphics object.
			local objGraphics = Graphics.FromImage(Data_Clipboard.tex)
			--set the BlendMode.
			objGraphics.BlendMode = Graphics.BlendMode.Enum_SourceCopy
			--Draw the image to the clipboard,
			objGraphics:DrawImage( App_MyBitmap, 0, 0, Data_Selection.Rect.x, Data_Selection.Rect.y,
													   Data_Selection.Rect.w, Data_Selection.Rect.h )
			--Dispose the graphics object.
			objGraphics = objGraphics:free()
		else
			-- Restore the Clipboard.
			Data_Clipboard = { Rect = { x = 0, y = 0, w = 0, h = 0},
							   Selected = false,
							   tex = false
							 }
			--Show The Error Message.
			ShowMesssage("Out of memory:","Can't copy the image.")
			while System.running() do
				--update the controls
				Controls.read()

				if Controls.press() > 0 then
					break
				end
			end
		end
	end
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--Paste the Image.
local PasteImage = function()

	ClipboardShow = true

	Window_Result_Ok = false
	Window_InputSize = { x = Data_Clipboard.Rect.x, y = Data_Clipboard.Rect.y }
	Window_OutPutSize = { x = Data_Clipboard.Rect.x, y = Data_Clipboard.Rect.y }
	Window_CursorLocation = { x = 0, y = 0 }

	local intStyle = 1
	local Flip = {}
	Flip[1] = { text = "None", id = 0}
	Flip[2] = { text = "X", id = 1}
	Flip[3] = { text = "Y", id = 2}
	Flip[4] = { text = "Z", id = 4}
	Flip[5] = { text = "XYZ", id = 7}
	Flip[6] = { text = "90", id = 8}
	Flip[7] = { text = "180", id = 16}
	Flip[8] = { text = "270", id = 32}

	local intWidth = 9 * 22
	local intheight = 64 + 14
	local x,y = 0,0

	while System.running() do

		--Draw Screen
		drawFunc()

		--Initialize the GU (Note : Any graphical functions MUST be placed AFTER this)
		--System.draw()

		--Draw the Type Window
		x = (480 - intWidth) / 2
		y = (272 - intheight) / 2
		screen.fillRect( x + 2, y + 2, intWidth, intheight, ControlDark)
		screen.fillRect( x, y, intWidth, intheight, InfoText)
		screen.fillRect( x + 1, y + 1, intWidth - 2, intheight - 2, Info)
		screen.print( x + 9, y + 3, "--- Paste Image: ---", InfoText)
		screen.print( x + 4, y + intheight - 13, "O : Cancel X : Accept", InfoText)
		if Window_CursorLocation.y == 0 then
			screen.print( x + 4, y + 6 + 14 * 1, string.format(">>  < x:%s >", Window_OutPutSize.x), InfoText)
			screen.print( x + 4, y + 6 + 14 * 2, string.format("   y:%s", Window_OutPutSize.y), InfoText)
			screen.print( x + 4, y + 6 + 14 * 3, string.format("   Flip:%s", Flip[intStyle]["text"]), InfoText)
		elseif Window_CursorLocation.y == 1 then
			screen.print( x + 4, y + 6 + 14 * 1, string.format("   x:%s", Window_OutPutSize.x), InfoText)
			screen.print( x + 4, y + 6 + 14 * 2, string.format(">>  < y:%s >", Window_OutPutSize.y), InfoText)
			screen.print( x + 4, y + 6 + 14 * 3, string.format("   Flip:%s", Flip[intStyle]["text"]), InfoText)
		elseif Window_CursorLocation.y == 2 then
			screen.print( x + 4, y + 6 + 14 * 1, string.format("   x:%s", Window_OutPutSize.x), InfoText)
			screen.print( x + 4, y + 6 + 14 * 2, string.format("   y:%s", Window_OutPutSize.y), InfoText)
			screen.print( x + 4, y + 6 + 14 * 3, string.format(">>  < Flip:%s >", Flip[intStyle]["text"]), InfoText)
		end

		--Finish the GU and Sync
		--System.endDraw()

		--Flip buffers
		screen.flip()

		--Slow down the Application if a button is pressed
		if Controls.buttons() > 0 and padcount > 0 then
			padcount = padcount - 1
			screen.waitVblankStart(5)
		elseif padcount ~= 10 and Controls.buttons() == 0 then
			padcount = 10
		end

		--update the controls
		Controls.readPeek()

		if Controls.press(1) or Controls.press(8) or Controls.press(256)
		or Controls.press(512) or Controls.press(8192) then --exit button pressed
			--Break the Type Loop and cancel the input
			Window_Result_Ok = false
			break
		elseif Controls.buttons(16) then --up button pressed
			Window_CursorLocation.y = Window_CursorLocation.y - 1
			if Window_CursorLocation.y < 0 then Window_CursorLocation.y = 2 end
		elseif Controls.buttons(64) then --down button pressed
			Window_CursorLocation.y = Window_CursorLocation.y + 1
			if Window_CursorLocation.y > 2 then Window_CursorLocation.y = 0 end
		elseif Controls.buttons(128) then --left button pressed
			if Window_CursorLocation.y == 0 then
				Window_OutPutSize.x = Window_OutPutSize.x - 1
				Data_Clipboard.Rect.x = Window_OutPutSize.x
			elseif Window_CursorLocation.y == 1 then
				Window_OutPutSize.y = Window_OutPutSize.y - 1
				Data_Clipboard.Rect.y = Window_OutPutSize.y
			else
				intStyle = intStyle - 1
				if intStyle < 1 then intStyle = 8 end
			end
		elseif Controls.buttons(32) then --right button pressed
			if Window_CursorLocation.y == 0 then
				Window_OutPutSize.x = Window_OutPutSize.x + 1
				Data_Clipboard.Rect.x = Window_OutPutSize.x
			elseif Window_CursorLocation.y == 1 then
				Window_OutPutSize.y = Window_OutPutSize.y + 1
				Data_Clipboard.Rect.y = Window_OutPutSize.y
			else
				intStyle = intStyle + 1
				if intStyle > 8 then intStyle = 1 end
			end
		elseif Controls.press(16384) then --cross button pressed
			Window_Result_Ok = true
			--Break the Type Loop
			break
		end

		--Move and Zoom Controls
		MoveZoom_Controls()

	end

	if not Window_Result_Ok then
		Data_Clipboard.Rect.x = Window_InputSize.x
		Data_Clipboard.Rect.y = Window_InputSize.y
	else --Paste the image.
		--Draw Screen
		drawFunc()
		ShowMesssage("Message:","Updating the Image, Please wait...")
		-- Get the Flip Style
		intStyle = Flip[intStyle]["id"]
		-- Paste the image.
		if intStyle > 0 then
			-- RotateFlip the image.
			local TempBitmap = Graphics.RotateFlip(Data_Clipboard.tex, intStyle) or false
			if TempBitmap then
				--Draw the image.
				App_Graphics:DrawImage( TempBitmap, Data_Clipboard.Rect.x, Data_Clipboard.Rect.y )
			else
				--Show The Error Message.
				ShowMesssage("Out of memory:","Can't past the image.")
				while System.running() do
					--update the controls
					Controls.read()

					if Controls.press() > 0 then
						break
					end
				end
			end
			TempBitmap = nil
		else
			--Draw the image.
			App_Graphics:DrawImage( Data_Clipboard.tex, Data_Clipboard.Rect.x, Data_Clipboard.Rect.y )
			--Set Application Data
			App_FileSaved = false
		end
	end

	intStyle = nil
	Flip = nil

	ClipboardShow = false
	intWidth = nil
	intheight = nil
	x,y = nil,nil

	return Window_Result_Ok
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--Delete the selection.
local DeleteImage = function()
	--Draw Screen
	drawFunc()
	ShowMesssage("Message:","Updating the Image, Please wait...")
	--set the BlendMode.
	App_Graphics.BlendMode = Graphics.BlendMode.Enum_SourceCopy
	--Draw the retangle.
	App_Graphics:FillRectangle(Data_Selection.Rect.x, Data_Selection.Rect.y,
							   Data_Selection.Rect.w, Data_Selection.Rect.h,
							   Color.new(0,0,0,0))
	--Set Application Data
	App_Graphics.BlendMode = App_BlendMode
	App_FileSaved = false
	--Diselect the image.
	Data_Selection = { Rect = { x = 0, y = 0, w = 0, h = 0},
					   Selected = false }
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--Show the SelectImage Window.
local SelectImage = function()
	Window_Result_Ok = false
	if not Data_Selection.Selected then
		Data_Selection = { Rect = {x = 0, y = 0 ,w = App_Graphics.width, h = App_Graphics.height},
						   Selected = true }
	end
	Window_InputSize = { x = Data_Selection.Rect.x, y = Data_Selection.Rect.y ,w = Data_Selection.Rect.w, h = Data_Selection.Rect.h}
	Window_OutPutSize = { x = Data_Selection.Rect.x, y = Data_Selection.Rect.y ,w = Data_Selection.Rect.w, h = Data_Selection.Rect.h}
	Window_CursorLocation = { x = 0, y = 0 }

	local intWidth = 9 * 22
	local intheight = 92
	local x,y = 0,0

	while System.running() do

		--Draw Screen
		drawFunc()

		--Initialize the GU (Note : Any graphical functions MUST be placed AFTER this)
		--System.draw()

		--Draw the Type Window
		x = (480 - intWidth) / 2
		y = (272 - intheight) / 2
		screen.fillRect( x + 2, y + 2, intWidth, intheight, ControlDark)
		screen.fillRect( x, y, intWidth, intheight, InfoText)
		screen.fillRect( x + 1, y + 1, intWidth - 2, intheight - 2, Info)
		screen.print( x + 4, y + 3, "--- Select Image: ---", InfoText)
		screen.print( x + 4, y + intheight - 13, "O : Cancel X : Accept", InfoText)
		if Window_CursorLocation.y == 0 then
			screen.print( x + 4, y + 6 + 14 * 1, string.format(">>  < x:%s >", Window_OutPutSize.x), InfoText)
			screen.print( x + 4, y + 6 + 14 * 2, string.format("   y:%s", Window_OutPutSize.y), InfoText)
			screen.print( x + 4, y + 6 + 14 * 3, string.format("   w:%s", Window_OutPutSize.w), InfoText)
			screen.print( x + 4, y + 6 + 14 * 4, string.format("   h:%s", Window_OutPutSize.h), InfoText)
		elseif Window_CursorLocation.y == 1 then
			screen.print( x + 4, y + 6 + 14 * 1, string.format("   x:%s", Window_OutPutSize.x), InfoText)
			screen.print( x + 4, y + 6 + 14 * 2, string.format(">>  < y:%s >", Window_OutPutSize.y), InfoText)
			screen.print( x + 4, y + 6 + 14 * 3, string.format("   w:%s", Window_OutPutSize.w), InfoText)
			screen.print( x + 4, y + 6 + 14 * 4, string.format("   h:%s", Window_OutPutSize.h), InfoText)
		elseif Window_CursorLocation.y == 2 then
			screen.print( x + 4, y + 6 + 14 * 1, string.format("   x:%s", Window_OutPutSize.x), InfoText)
			screen.print( x + 4, y + 6 + 14 * 2, string.format("   y:%s", Window_OutPutSize.y), InfoText)
			screen.print( x + 4, y + 6 + 14 * 3, string.format(">>  < w:%s >", Window_OutPutSize.w), InfoText)
			screen.print( x + 4, y + 6 + 14 * 4, string.format("   h:%s", Window_OutPutSize.h), InfoText)
		elseif Window_CursorLocation.y == 3 then
			screen.print( x + 4, y + 6 + 14 * 1, string.format("   x:%s", Window_OutPutSize.x), InfoText)
			screen.print( x + 4, y + 6 + 14 * 2, string.format("   y:%s", Window_OutPutSize.y), InfoText)
			screen.print( x + 4, y + 6 + 14 * 3, string.format("   w:%s", Window_OutPutSize.w), InfoText)
			screen.print( x + 4, y + 6 + 14 * 4, string.format(">>  < h:%s >", Window_OutPutSize.h), InfoText)
		end

		--Finish the GU and Sync
		--System.endDraw()

		--Flip buffers
		screen.flip()

		--Slow down the Application if a button is pressed
		if Controls.buttons() > 0 and padcount > 0 then
			padcount = padcount - 1
			screen.waitVblankStart(5)
		elseif padcount ~= 10 and Controls.buttons() == 0 then
			padcount = 10
		end

		--update the controls
		Controls.readPeek()

		if Controls.press(1) or Controls.press(8) or Controls.press(256)
		or Controls.press(512) or Controls.press(8192) then --exit button pressed
			--Break the Type Loop and cancel the input
			Window_Result_Ok = false
			break
		elseif Controls.buttons(16) then --up button pressed
			Window_CursorLocation.y = Window_CursorLocation.y - 1
			if Window_CursorLocation.y < 0 then Window_CursorLocation.y = 3 end
		elseif Controls.buttons(64) then --down button pressed
			Window_CursorLocation.y = Window_CursorLocation.y + 1
			if Window_CursorLocation.y > 3 then Window_CursorLocation.y = 0 end
		elseif Controls.buttons(128) then --left button pressed
			if Window_CursorLocation.y == 0 then
				Window_OutPutSize.x = Window_OutPutSize.x - 1
				Data_Selection.Rect.x = Window_OutPutSize.x
			elseif Window_CursorLocation.y == 1 then
				Window_OutPutSize.y = Window_OutPutSize.y - 1
				Data_Selection.Rect.y = Window_OutPutSize.y
			elseif Window_CursorLocation.y == 2 and Window_OutPutSize.w > 1 then
				Window_OutPutSize.w = Window_OutPutSize.w - 1
				Data_Selection.Rect.w = Window_OutPutSize.w
			elseif Window_CursorLocation.y == 3 and Window_OutPutSize.h > 1 then
				Window_OutPutSize.h = Window_OutPutSize.h - 1
				Data_Selection.Rect.h = Window_OutPutSize.h
			end
		elseif Controls.buttons(32) then --right button pressed
			if Window_CursorLocation.y == 0 then
				Window_OutPutSize.x = Window_OutPutSize.x + 1
				Data_Selection.Rect.x = Window_OutPutSize.x
			elseif Window_CursorLocation.y == 1 then
				Window_OutPutSize.y = Window_OutPutSize.y + 1
				Data_Selection.Rect.y = Window_OutPutSize.y
			elseif Window_CursorLocation.y == 2 then
				Window_OutPutSize.w = Window_OutPutSize.w + 1
				Data_Selection.Rect.w = Window_OutPutSize.w
			elseif Window_CursorLocation.y == 3 then
				Window_OutPutSize.h = Window_OutPutSize.h + 1
				Data_Selection.Rect.h = Window_OutPutSize.h
			end
		elseif Controls.press(16384) then --cross button pressed
			Window_Result_Ok = true
			--Break the Type Loop
			break
		end

		--Move and Zoom Controls
		MoveZoom_Controls()

	end

	if not Window_Result_Ok then
		--Diselect the image.
		Data_Selection = { Rect = { x = 0, y = 0, w = 0, h = 0},
						   Selected = false }
	else --Select the image.
		if Data_Selection.Rect.x < App_Graphics.width and
		   Data_Selection.Rect.y < App_Graphics.height and
		   Data_Selection.Rect.x + Data_Selection.Rect.w > 0 and
		   Data_Selection.Rect.y + Data_Selection.Rect.h > 0 then
			--Correct the data.
			if Data_Selection.Rect.x < 0 then
				Data_Selection.Rect.w = Data_Selection.Rect.w + Data_Selection.Rect.x
				Data_Selection.Rect.x = 0
			end
			if Data_Selection.Rect.y < 0 then
				Data_Selection.Rect.h = Data_Selection.Rect.h + Data_Selection.Rect.y
				Data_Selection.Rect.y = 0
			end
			if Data_Selection.Rect.x + Data_Selection.Rect.w > App_Graphics.width then
				Data_Selection.Rect.w = App_Graphics.width - Data_Selection.Rect.x
			end
			if Data_Selection.Rect.y + Data_Selection.Rect.h > App_Graphics.height then
				Data_Selection.Rect.h = App_Graphics.height - Data_Selection.Rect.y
			end
		else
			--Diselect the image.
			Data_Selection = { Rect = { x = 0, y = 0, w = 0, h = 0},
						   Selected = false }
		end
	end

	intWidth = nil
	intheight = nil
	x,y = nil,nil

	return Window_Result_Ok
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--Show the Select BlendMode Window.
local BlendMode_Window = function()
	Window_Result_Ok = false

	local title = "BlendMode:"
	local strArray = {"SourceOver","SourceCopy","SourceAlpha"}
	local text = App_BlendMode + 1
	local intWidth = 0

	while System.running() do

		--Draw Screen
		drawFunc()

		--Initialize the GU (Note : Any graphical functions MUST be placed AFTER this)
		--System.draw()

		intWidth = math.max(string.len(title) * 9 + 16, string.len("<"..strArray[text]..">") * 9 + 16 ,150 + 20)
		screen.fillRect( (480 - intWidth) / 2 + 2, 119, intWidth, 39, ControlDark)
		screen.fillRect( (480 - intWidth) / 2, 117, intWidth, 39, InfoText)
		screen.fillRect( (480 - intWidth) / 2 + 1, 118, intWidth - 2, 37, Info)
		screen.print( math.floor((480 - 9 * string.len(title)) / 2), 122 , title, InfoText)
		screen.print( math.floor((480 - 9 * string.len("<"..strArray[text]..">")) / 2), 139 , "<"..strArray[text]..">", InfoText)

		--Finish the GU and Sync
		--System.endDraw()

		--Flip buffers
		screen.flip()

		--Slow down the Application if a button is pressed
		if Controls.buttons() > 0 and padcount > 0 then
			padcount = padcount - 1
			screen.waitVblankStart(5)
		elseif padcount ~= 10 and Controls.buttons() == 0 then
			padcount = 10
		end

		--update the controls
		Controls.readPeek()

		if Controls.press(1) or Controls.press(8) or Controls.press(256)
		or Controls.press(512) or Controls.press(8192) then --exit button pressed
			Window_Result_Ok = false
			--break the loop
			break
		elseif Controls.buttons(16) or Controls.buttons(128) then--down button pressed.
			text = text - 1
			if text < 1 then text = 3 end
		elseif Controls.buttons(32) or Controls.buttons(64) then --up button pressed.
			text = text + 1
			if text > 3 then text = 1 end
		elseif Controls.press(16384) then --cross button pressed
			Window_Result_Ok = true
			--break the loop
			break
		end

		--Move and Zoom Controls
		MoveZoom_Controls()

	end

	if Window_Result_Ok then --Set the new Blendmode.
		--Set Application Data
		App_BlendMode = text - 1
		App_Graphics.BlendMode = App_BlendMode
	end


	title = nil
	strArray = nil
	text = nil
	intWidth = nil

	return Window_Result_Ok
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--Unswizzle the image.
local Unswizzle = function()
	local Pixelformat = 0
	local title = "Pixelformat:"
	local text = 24
	local intWidth = 0

	while System.running() do

		--Draw Screen
		drawFunc()

		--Initialize the GU (Note : Any graphical functions MUST be placed AFTER this)
		--System.draw()

		intWidth = math.max(string.len(title) * 9 + 16, string.len("<"..text..">") * 9 + 16 ,150 + 20)
		screen.fillRect( (480 - intWidth) / 2 + 2, 119, intWidth, 39, ControlDark)
		screen.fillRect( (480 - intWidth) / 2, 117, intWidth, 39, InfoText)
		screen.fillRect( (480 - intWidth) / 2 + 1, 118, intWidth - 2, 37, Info)
		screen.print( math.floor((480 - 9 * string.len(title)) / 2), 122 , title, InfoText)
		screen.print( math.floor((480 - 9 * string.len("<"..text..">")) / 2), 139 , "<"..text..">", InfoText)

		--Finish the GU and Sync
		--System.endDraw()

		--Flip buffers
		screen.flip()

		--Slow down the Application if a button is pressed
		if Controls.buttons() > 0 and padcount > 0 then
			padcount = padcount - 1
			screen.waitVblankStart(5)
		elseif padcount ~= 10 and Controls.buttons() == 0 then
			padcount = 10
		end

		--update the controls
		Controls.readPeek()

		if Controls.press(1) or Controls.press(8) or Controls.press(256)
		or Controls.press(512) or Controls.press(8192) then --exit button pressed
			--break the loop
			break
		elseif Controls.buttons(16) or Controls.buttons(32)
		or Controls.buttons(64) or Controls.buttons(128) then --next button pressed.
			if text == 24 then text = 32 else text = 24 end
		elseif Controls.press(16384) then --cross button pressed
			Pixelformat = text
			break
		end

		--Move and Zoom Controls
		MoveZoom_Controls()

	end

	if Pixelformat == 24 then --Unswizzle the image as a 24 bit image.
		--Create a empty texture.
		App_Graphics.tex = false
		App_Graphics.tex = Texture.new(App_MyBitmap:width(), App_MyBitmap:height())
		App_Graphics.tex:clear(GL.clr.white)
		-- Set the texture as the render target (instead of the screen).
		if App_Graphics.tex:SetTarget()then
			-- Draw the Image.
			App_MyBitmap:render(0, 0)
			-- End drawing.
			App_Graphics.tex:ResetTarget()
		else
			--error("Can't set the render target.")
		end
		App_MyBitmap = false
		App_MyBitmap = App_Graphics.tex
		App_Swizzled = false
	elseif Pixelformat == 32 then --Unswizzle the image as a 32 bit image.
		--Check the free memory.
		if System.Memory.freeRam() >= 5242880 then --5 mb.
			--Unswizzle the image.
			App_Graphics.tex:unSwizzle()
			App_MyBitmap = App_Graphics.tex
			App_Swizzled = false
		else
			ShowMesssage("Out of memory:","Can't unswizzle the image.")
			while System.running() do
				--update the controls
				Controls.read()

				if Controls.press() > 0 then
					break
				end
			end
		end
	end

	Pixelformat = nil
	title = nil
	text = nil
	intWidth = nil
end
-----------------------------------------------------------------------------------------------------

local R,G,B,S,V,M = 0,0,0,0,0,0

-----------------------------------------------------------------------------------------------------
--Show the Palette Window
local Palette_Window = function()
	Window_Result_Ok = false
	Window_CursorLocation = { x = 1, y = 3 }

	local clrShow = Color.new(App_Color:red(), App_Color:green(), App_Color:blue(), 255)
	local clrTmp = Color.new(255, 255, 255, 255)

	--Set Cursor
	if App_Color:red() == App_Color:blue() and App_Color:blue() == App_Color:green() then --black/white
		Window_CursorLocation.y = 7
		Window_CursorLocation.x = 1 + (App_Color:red() - math.fmod(App_Color:red(),11)) / 11
		if App_Color:red() == 255 then Window_CursorLocation.x = 24 end
	else --Color RGB
		R,G,B = App_Color:red(),App_Color:green(),App_Color:blue()
		S = math.min(R,G,B)
		M = math.max(R,G,B)

		if M <= 118 then
			Window_CursorLocation.y = 6
		elseif M > 118 and M <= 170 then
			Window_CursorLocation.y = 5
		elseif M > 170 and M <= 227 then
			Window_CursorLocation.y = 4
		elseif M > 227 and S <= 36 then
			Window_CursorLocation.y = 3
		elseif M > 227 and S <= 100 then
			Window_CursorLocation.y = 2
		elseif M > 227 and S > 100 then
			Window_CursorLocation.y = 1
		end

		if R == 255 then R = 256 end
		if G == 255 then G = 256 end
		if B == 255 then B = 256 end
		V = (M-S) / 4
		if math.floor(V) ~= V then V = math.floor(V) end

		if R >= G and S == B then -- Red / Yellow
			if G == B then -- Red
				Window_CursorLocation.x = 1
			elseif R ~= G and V ~= 0 then -- Red / Yellow
				Window_CursorLocation.x = 1 + ((G-S)-math.fmod((G-S),V))/V
			else -- Yellow
				Window_CursorLocation.x = 5
			end
		elseif G >= R and S == B then -- Yellow / Green
			if R == G then -- Yellow
				Window_CursorLocation.x = 5
			elseif G ~= R and V ~= 0 then -- Yellow / Green
				Window_CursorLocation.x = 9 - ((R-S)-math.fmod((R-S),V))/V
			else -- Green
				Window_CursorLocation.x = 9
			end
		elseif G >= B and S == R then -- Green / GreenBlue
			if B == R then -- Green
				Window_CursorLocation.x = 9
			elseif G ~= B and V ~= 0 then -- Green / GreenBlue
				Window_CursorLocation.x = 9 + ((B-S)-math.fmod((B-S),V))/V
			else -- GreenBlue
				Window_CursorLocation.x = 13
			end
		elseif B >= G and S == R then -- GreenBlue / Blue
			if R == B then -- GreenBlue
				Window_CursorLocation.x = 13
			elseif B ~= G and V ~= 0 then -- GreenBlue / Blue
				Window_CursorLocation.x = 17 - ((G-S)-math.fmod((G-S),V))/V
			else -- Blue
				Window_CursorLocation.x = 17
			end
		elseif B >= R and S == G then -- Blue / Magenta
			if R == G then -- GreenBlue
				Window_CursorLocation.x = 17
			elseif B ~= R and V ~= 0 then -- Blue / Magenta
				Window_CursorLocation.x = 17 + ((R-S)-math.fmod((R-S),V))/V
			else -- Magenta
				Window_CursorLocation.x = 21
			end
		elseif R >= B and S == G then -- Magenta / Red
			if G == B then -- Magenta
				Window_CursorLocation.x = 21
			elseif R ~= B and V ~= 0 then -- Magenta / Red
				Window_CursorLocation.x = 25 - ((B-S)-math.fmod((B-S),V))/V
			else -- Red
				Window_CursorLocation.x = 1
			end
		end

		if Window_CursorLocation.x == nil then Window_CursorLocation.x = 1 end
		if Window_CursorLocation.x > 24 then Window_CursorLocation.x = 24 end
		if Window_CursorLocation.x < 1 then Window_CursorLocation.x = 1 end

	end

	local intWidth = 270
	local intheight = 142
	local x = (480 - intWidth) / 2
	local y = (272 - intheight) / 2

	while System.running() do

		--Draw Screen
		drawFunc()

		--Initialize the GU (Note : Any graphical functions MUST be placed AFTER this)
		--System.draw()

		--Draw the Type Window
		screen.fillRect( x + 2, y + 2, intWidth, intheight, ControlDark)
		screen.fillRect( x, y, intWidth, intheight, InfoText)
		screen.fillRect( x + 1, y + 1, intWidth - 2, intheight - 2, Info)
		screen.print( x + 4, y + 3, "------- Select Color: -------", InfoText)
		screen.print( x + 4 + 37, y + intheight - 14, "O : Cancel X : Accept", InfoText)

		--Draw Cursor
		if Window_CursorLocation.y < 7 then
			screen.fillRect( x + 3 + (Window_CursorLocation.x - 1) * 11, y + 3 + Window_CursorLocation.y * 11, 12, 12, InfoText )
		else
			screen.print(x + 5 + (Window_CursorLocation.x - 1) * 11, y + 5 + (Window_CursorLocation.y + 1) * 11, "^", InfoText)
			screen.print(x + 5 + (Window_CursorLocation.x - 1) * 11, y + 6 + (Window_CursorLocation.y + 1) * 11, "^", InfoText)
		end

		R,G,B,S,V,M = 0,0,0,0,0,0
		for n_y = 1 , 7 do
			if n_y == 1 then --Paste Color
				S,V,M = 128,32,255
			elseif n_y == 2 then --Paste Color
				S,V,M = 72,46,255
			elseif n_y == 3 then --Normal Color
				S,V,M = 0,64,255
			elseif n_y == 4 then --Dark Color
				S,V,M = 0,50,200
			elseif n_y == 5 then --Gray Color
				S,V,M = 8,33,140
			elseif n_y == 6 then --GrayDark Color
				S,V,M = 8,22,96
			elseif n_y == 7 then --Black/white Color
				S,V,M = 0,11,255
				R,G,B = -11,-11,-11
			end

			for n_x = 1 , 24 do
				if n_y == 7 then
					R,G,B = R + V, B + V, G + V
					if R > M then R,G,B = 255,255,255 end
					if n_x == 24 then R,G,B = 255,255,255 end
				elseif n_x == 1 then --Update Red
					R,G,B = M,S,S
				elseif n_x >= 2 and n_x <= 5 then --increase Green
					G = G + V
					if G > M then G = M end
				elseif n_x >= 6 and n_x <= 9 then --decrease Red
					if R == 255 then R = 256 end
					R = R - V
				elseif n_x >= 10 and n_x <= 13 then --increase Blue
					B = B + V
					if B > M then B = M end
				elseif n_x >= 14 and n_x <= 17 then --decrease Green
					if G == 255 then G = 256 end
					G = G - V
				elseif n_x >= 18 and n_x <= 21 then --increase Red
					R = R + V
					if R > M then R= M end
				elseif n_x >= 22 and n_x <= 24 then --decrease Blue
					if B == 255 then B = 256 end
					B = B - V
				end
				if n_y == 7  then
					--clrTmp = Color.new( (255-R), (255-G), (255-B) )
					clrTmp:red(255-R); clrTmp:green(255-G); clrTmp:blue(255-B);
					screen.fillRect( x + 3 + (n_x - 1) * 11, y + 3 + n_y * 11, 12, 12, clrTmp )
					--clrTmp = Color.new( R, G, B )
					clrTmp:red(R); clrTmp:green(G); clrTmp:blue(B);
					screen.fillRect( x + 4 + (n_x - 1) * 11, y + 4 + n_y * 11, 10, 10, clrTmp )
				else
					--clrTmp = Color.new( R, G, B )
					clrTmp:red(R); clrTmp:green(G); clrTmp:blue(B);
					screen.fillRect( x + 4 + (n_x - 1) * 11, y + 4 + n_y * 11, 10, 10, clrTmp )
				end
				if n_y == Window_CursorLocation.y and n_x == Window_CursorLocation.x then
					--clrShow = Color.new(R,G,B)
					clrShow:red(R); clrShow:green(G); clrShow:blue(B);
					--Draw Color Info
					screen.fillRect( x + 4, y + 4 + 9 * 11, 108, 22, InfoText)
					screen.fillRect( x + 5, y + 5 + 9 * 11, 106, 20, clrShow )
					screen.print( x + 4 + 111, y + 4 + 9 * 11, "Color:", InfoText)
					screen.print( x + 4 + 111, y + 5 + 10 * 11, string.format("R:%s G:%s B:%s",R,G,B), InfoText)
				end
			end
		end

		--Finish the GU and Sync
		--System.endDraw()

		--Flip buffers
		screen.flip()

		--Slow down the Application if a button is pressed
		if Controls.buttons() > 0 and padcount > 0 then
			padcount = padcount - 1
			screen.waitVblankStart(5)
		elseif padcount ~= 10 and Controls.buttons() == 0 then
			padcount = 10
		end

		--update the controls
		Controls.readPeek()

		if Controls.press(1) or Controls.press(8) or Controls.press(256)
		or Controls.press(512) or Controls.press(8192) then --exit button pressed
			--Break the loop and cancel the input.
			Window_Result_Ok = false
			break
		elseif Controls.buttons(16) then --up Button Pressed
			Window_CursorLocation.y = Window_CursorLocation.y - 1
			if Window_CursorLocation.y < 1 then Window_CursorLocation.y = 7 end
		elseif Controls.buttons(32) then --right Button Pressed
			Window_CursorLocation.x = Window_CursorLocation.x + 1
			if Window_CursorLocation.x > 24 then Window_CursorLocation.x = 1 end
		elseif Controls.buttons(64) then --down Button Pressed
			Window_CursorLocation.y = Window_CursorLocation.y + 1
			if Window_CursorLocation.y > 7 then Window_CursorLocation.y = 1 end
		elseif Controls.buttons(128) then --left Button Pressed
			Window_CursorLocation.x = Window_CursorLocation.x - 1
			if Window_CursorLocation.x < 1 then Window_CursorLocation.x = 24 end
		elseif Controls.press(16384)  then --cross Button Pressed
			Window_Result_Ok = true
			--Break the loop
			break
		end

		--Move and Zoom Controls
		MoveZoom_Controls()

	end

	if Window_Result_Ok then
		--Set the new App_Color.
		clrShow:alpha(App_Color:alpha())
		App_Color = clrShow
	end

	clrShow = nil
	clrTmp = nil
	intWidth = nil
	intheight = nil
	x,y = nil,nil

	return Window_Result_Ok
end
-----------------------------------------------------------------------------------------------------

local Select_Window_Menu = {}
	   Select_Window_Menu[1] = {Text = "%sR:%s%s" ,Max = 255 , Min = 0, value = 0}
	   Select_Window_Menu[2] = {Text = "%sG:%s%s" ,Max = 255 , Min = 0, value = 0}
	   Select_Window_Menu[3] = {Text = "%sB:%s%s" ,Max = 255 , Min = 0, value = 0}
	   Select_Window_Menu[4] = {Text = "%sA:%s%s" ,Max = 255 , Min = 0, value = 0}

-----------------------------------------------------------------------------------------------------
--Show the Select Window
local Color_Window = function()
	Window_Result_Ok = false

	--Set Menu
	Select_Window_Menu[1].value = App_Color:red()
	Select_Window_Menu[2].value = App_Color:green()
	Select_Window_Menu[3].value = App_Color:blue()
	Select_Window_Menu[4].value = App_Color:alpha()

	Window_CursorLocation = { x = Select_Window_Menu[1].value, y = 1 }

	local clrShow =  Color.new(Select_Window_Menu[1].value,
								Select_Window_Menu[2].value,
								Select_Window_Menu[3].value,
								Select_Window_Menu[4].value )

	local intWidth = 9 * 22
	local intheight = 92
	local x,y = 0,0

	while System.running() do

		--Draw Screen
		drawFunc()

		--Initialize the GU (Note : Any graphical functions MUST be placed AFTER this)
		--System.draw()

		--Draw the Type Window
		x = (480 - intWidth) / 2
		y = (272 - intheight) / 2
		screen.fillRect( x + 2, y + 2, intWidth, intheight, ControlDark)
		screen.fillRect( x, y, intWidth, intheight, InfoText)
		screen.fillRect( x + 1, y + 1, intWidth - 2, intheight - 2, Info)
		screen.print( x + 4, y + 3, "--- Color Window: ---", InfoText)
		screen.print( x + 4, y + intheight - 14, "O : Cancel X : Accept", InfoText)
		for n = 1 , 4 do
			if n == Window_CursorLocation.y then --Draw Selected Item.
				screen.print(x + 4, y + 6 + 14 * n, string.format( Select_Window_Menu[n].Text, ">>  < ", Select_Window_Menu[n].value, " >"), InfoText)
			else --Draw Item.
				screen.print(x + 4, y + 6 + 14 * n, string.format( Select_Window_Menu[n].Text, "   ", Select_Window_Menu[n].value, ""), InfoText)
			end
		end
		screen.fillRect(x + 9 + 9 * 13, y + 6 + 14 * 1, 62, 54, InfoText)
		clrShow:alpha(255)
		screen.fillRect(x + 10 + 9 * 13, y + 7 + 14 * 1, 60, 26, clrShow)
		clrShow:alpha(Select_Window_Menu[4].value)
		screen.fillRect(x + 10 + 9 * 13, y + 7 + 14 * 1 + 26, 60, 26, clrShow)

		--Finish the GU and Sync
		--System.endDraw()

		--Flip buffers
		screen.flip()

		--Slow down the Application if a button is pressed
		if Controls.buttons() > 0 and padcount > 0 then
			padcount = padcount - 1
			screen.waitVblankStart(5)
		elseif padcount ~= 10 and Controls.buttons() == 0 then
			padcount = 10
		end

		--update the controls
		Controls.readPeek()

		if Controls.press(1) or Controls.press(8) or Controls.press(256)
		or Controls.press(512) or Controls.press(8192) then --exit button pressed
			--Break the loop and cancel the input.
			Window_Result_Ok = false
			break
		elseif Controls.buttons(16) then --up Button Pressed
			Window_CursorLocation.y = Window_CursorLocation.y - 1
			if Window_CursorLocation.y < 1 then Window_CursorLocation.y = 4 end
			Window_CursorLocation.x = Select_Window_Menu[Window_CursorLocation.y].value
		elseif Controls.buttons(64) then --down Button Pressed
			Window_CursorLocation.y = Window_CursorLocation.y + 1
			if Window_CursorLocation.y > 4 then Window_CursorLocation.y = 1 end
			Window_CursorLocation.x = Select_Window_Menu[Window_CursorLocation.y].value
		elseif Controls.buttons(128) then --left Button Pressed
			Window_CursorLocation.x = Window_CursorLocation.x - 1
			if Window_CursorLocation.x < Select_Window_Menu[Window_CursorLocation.y].Min
			   and Select_Window_Menu[Window_CursorLocation.y].Min ~= -1 then
				if Select_Window_Menu[Window_CursorLocation.y].Max == -1 then
					Window_CursorLocation.x = 1
				else
					Window_CursorLocation.x = Select_Window_Menu[Window_CursorLocation.y].Max
				end
			end
			Select_Window_Menu[Window_CursorLocation.y].value = Window_CursorLocation.x
			--Update the color.
			clrShow:red(Select_Window_Menu[1].value)
			clrShow:green(Select_Window_Menu[2].value)
			clrShow:blue(Select_Window_Menu[3].value)
			clrShow:alpha(Select_Window_Menu[4].value)
		elseif Controls.buttons(32) then --right Button Pressed
			Window_CursorLocation.x = Window_CursorLocation.x + 1
			if Window_CursorLocation.x > Select_Window_Menu[Window_CursorLocation.y].Max
			   and Select_Window_Menu[Window_CursorLocation.y].Max ~= -1 then
				Window_CursorLocation.x = Select_Window_Menu[Window_CursorLocation.y].Min
			end
			Select_Window_Menu[Window_CursorLocation.y].value = Window_CursorLocation.x
			--Update the color.
			clrShow:red(Select_Window_Menu[1].value)
			clrShow:green(Select_Window_Menu[2].value)
			clrShow:blue(Select_Window_Menu[3].value)
			clrShow:alpha(Select_Window_Menu[4].value)
		elseif Controls.press(16384) then --cross Button Pressed
			Window_Result_Ok = true
			--Break the loop.
			break
		end

		--Move and Zoom Controls
		MoveZoom_Controls()

	end

	if Window_Result_Ok then
		--Set the new App_Color.
		App_Color = clrShow
	end

	clrShow = nil
	intWidth = nil
	intheight = nil
	x,y = nil,nil

	return Window_Result_Ok
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--Show the Color Picker Window
local ColorPicker = function()
	Window_Result_Ok = false
	App_Mode = "Picker"

	local clrShow = App_MyBitmap:pixel( App_Cursor.x, App_Cursor.y )

	--Set Menu
	Select_Window_Menu[1].value = clrShow:red()
	Select_Window_Menu[2].value = clrShow:green()
	Select_Window_Menu[3].value = clrShow:blue()
	Select_Window_Menu[4].value = clrShow:alpha()

	local intWidth = 9 * 22
	local intheight = 108
	local x,y = 0,0

	while System.running() do

		--Draw Screen
		drawFunc()

		--Initialize the GU (Note : Any graphical functions MUST be placed AFTER this)
		--System.draw()

		--Draw the Type Window
		x = (480 - intWidth) / 2
		y = (272 - intheight) / 2
		screen.fillRect( x + 2, y + 2, intWidth, intheight, ControlDark)
		screen.fillRect( x, y, intWidth, intheight, InfoText)
		screen.fillRect( x + 1, y + 1, intWidth - 2, intheight - 2, Info)
		screen.print( x + 4, y + 3, "--- Color PicKer: ---", InfoText)
		screen.print( x + 4, y + intheight - 14, "O : Cancel X : Accept", InfoText)
		for n = 1 , 4 do
			--Draw Item.
			screen.print(x + 4, y + 6 + 14 * n, string.format( Select_Window_Menu[n].Text, "   ", Select_Window_Menu[n].value, ""), InfoText)
		end

		screen.print(x + 4, y + 8 + 14 * 5, string.format( "x:%s, y:%s", App_Cursor.x, App_Cursor.y ), InfoText)

		screen.fillRect(x + 9 + 9 * 13, y + 6 + 14 * 1, 62, 54, InfoText)
		clrShow:alpha(255)
		screen.fillRect(x + 10 + 9 * 13, y + 7 + 14 * 1, 60, 26, clrShow)
		clrShow:alpha(Select_Window_Menu[4].value)
		screen.fillRect(x + 10 + 9 * 13, y + 7 + 14 * 1 + 26, 60, 26, clrShow)

		--Finish the GU and Sync
		--System.endDraw()

		--Flip buffers
		screen.flip()

		--Slow down the Application if a button is pressed
		if Controls.buttons() > 0 and padcount > 0 then
			padcount = padcount - 1
			screen.waitVblankStart(5)
		elseif padcount ~= 10 and Controls.buttons() == 0 then
			padcount = 10
		end

		--update the controls
		Controls.readPeek()

		if Controls.press(1) or Controls.press(8) or Controls.press(256)
		or Controls.press(512) or Controls.press(8192) then --exit button pressed
			--Break the loop and cancel the input.
			Window_Result_Ok = false
			break
		else
			if Controls.buttons(16) then --up Button Pressed
				App_Cursor.y = App_Cursor.y - 1
				if App_Cursor.y < 0 then App_Cursor.y = 0 end
				--Get the Color
				clrShow = App_MyBitmap:pixel( App_Cursor.x, App_Cursor.y )
				--Set Menu
				Select_Window_Menu[1].value = clrShow:red()
				Select_Window_Menu[2].value = clrShow:green()
				Select_Window_Menu[3].value = clrShow:blue()
				Select_Window_Menu[4].value = clrShow:alpha()
			elseif Controls.buttons(64) then --down Button Pressed
				App_Cursor.y = App_Cursor.y + 1
				if App_Cursor.y > App_Graphics.height - 1 then App_Cursor.y = App_Graphics.height - 1 end
				--Get the Color
				clrShow = App_MyBitmap:pixel( App_Cursor.x, App_Cursor.y )
				--Set Menu
				Select_Window_Menu[1].value = clrShow:red()
				Select_Window_Menu[2].value = clrShow:green()
				Select_Window_Menu[3].value = clrShow:blue()
				Select_Window_Menu[4].value = clrShow:alpha()
			end
			if Controls.buttons(128) then --left Button Pressed
				App_Cursor.x = App_Cursor.x - 1
				if App_Cursor.x < 0 then App_Cursor.x = 0 end
				--Get the Color
				clrShow = App_MyBitmap:pixel( App_Cursor.x, App_Cursor.y )
				--Set Menu
				Select_Window_Menu[1].value = clrShow:red()
				Select_Window_Menu[2].value = clrShow:green()
				Select_Window_Menu[3].value = clrShow:blue()
				Select_Window_Menu[4].value = clrShow:alpha()
			elseif Controls.buttons(32) then --right Button Pressed
				App_Cursor.x = App_Cursor.x + 1
				if App_Cursor.x > App_Graphics.width - 1 then App_Cursor.x = App_Graphics.width - 1 end
				--Get the Color
				clrShow = App_MyBitmap:pixel( App_Cursor.x, App_Cursor.y )
				--Set Menu
				Select_Window_Menu[1].value = clrShow:red()
				Select_Window_Menu[2].value = clrShow:green()
				Select_Window_Menu[3].value = clrShow:blue()
				Select_Window_Menu[4].value = clrShow:alpha()
			end
			if Controls.press(16384) then --cross Button Pressed
				Window_Result_Ok = true
				--Break the loop.
				break
			end
		end

		--Move and Zoom Controls
		MoveZoom_Controls()

	end

	if Window_Result_Ok then
		--Set the new App_Color.
		App_Color = clrShow
	end

	clrShow = nil
	intWidth = nil
	intheight = nil
	x,y = nil,nil

	App_Mode = "Normal"

	return Window_Result_Ok
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--Draw on the bitmap with the pencil.
local Pencil = function()
	Window_Result_Ok = false
	App_Mode = "Pencil"

	local strArray = {"SourceOver","SourceCopy","SourceAlpha"}
	local text = string.format( "%s, x:%d, y:%d",strArray[App_BlendMode+1], App_Cursor.x, App_Cursor.y )
	local intWidth = string.len(text) * 9 + 9
	local intheight = 22
	local x = 480 - 4 - intWidth
	local y = 272 - 18 - intheight
	local heldMove = false
	local heldCross = false
	local DrawPoint = false

	while System.running() do

		--Draw Screen
		drawFunc()

		--Initialize the GU (Note : Any graphical functions MUST be placed AFTER this)
		--System.draw()

		--Draw the Type Window
		screen.fillRect( x + 2, y + 2, intWidth, intheight, ControlDark)
		screen.fillRect( x, y, intWidth, intheight, InfoText)
		screen.fillRect( x + 1, y + 1, intWidth - 2, intheight - 2, Info)
		screen.print(x + 4, y + 5, text, InfoText)

		--Finish the GU and Sync
		--System.endDraw()

		--Flip buffers
		screen.flip()

		if heldCross then
			--Slow down the Application if the cross and move buttons are pressed
			if heldMove and padcount > 0 then
				padcount = padcount - 1
				screen.waitVblankStart(5)
			elseif padcount ~= 10 and not heldMove then
				padcount = 10
			end
		else
			--Slow down the Application if a button is pressed
			if Controls.buttons() > 0 and padcount > 0 then
				padcount = padcount - 1
				screen.waitVblankStart(5)
			elseif padcount ~= 10 and Controls.buttons() == 0 then
				padcount = 10
			end
		end

		--update the controls
		Controls.readPeek()

		heldMove = false
		heldCross = false

		if Controls.press(8) or Controls.press(8192) then --exit button pressed
			--Break the loop and cancel the input.
			Window_Result_Ok = false
			break
		elseif Controls.press(1) then --select Button Pressed
			--Set the Graphics BlendMode.
			if App_BlendMode == 0 then
				App_BlendMode = 1
			else
				App_BlendMode = 0
			end
			App_Graphics.BlendMode = App_BlendMode
			text = string.format( "%s, x:%d, y:%d",strArray[App_BlendMode+1], App_Cursor.x, App_Cursor.y )
			intWidth = string.len(text) * 9 + 9
			x = 480 - 4 - intWidth
		elseif Controls.press(256) then --ltrigger Button Pressed
			--Show the Color window
			Color_Window()
			DrawPoint = false
		elseif Controls.press(512) then --rtrigger Button Pressed
			--Show the Color Palet
			Palette_Window()
			DrawPoint = false
		else
			--Move the Cursor.
			if Controls.buttons(16) then --up Button Pressed
				App_Cursor.y = App_Cursor.y - 1
				if App_Cursor.y < 0 then App_Cursor.y = 0 end
				text = string.format( "%s, x:%d, y:%d",strArray[App_BlendMode+1], App_Cursor.x, App_Cursor.y )
				intWidth = string.len(text) * 9 + 9
				x = 480 - 4 - intWidth
				heldMove = true
				DrawPoint = true
			elseif Controls.buttons(64) then --down Button Pressed
				App_Cursor.y = App_Cursor.y + 1
				if App_Cursor.y > App_Graphics.height - 1 then App_Cursor.y = App_Graphics.height - 1 end
				text = string.format( "%s, x:%d, y:%d",strArray[App_BlendMode+1], App_Cursor.x, App_Cursor.y )
				intWidth = string.len(text) * 9 + 9
				x = 480 - 4 - intWidth
				heldMove = true
				DrawPoint = true
			end
			if Controls.buttons(128) then --left Button Pressed
				App_Cursor.x = App_Cursor.x - 1
				if App_Cursor.x < 0 then App_Cursor.x = 0 end
				text = string.format( "%s, x:%d, y:%d",strArray[App_BlendMode+1], App_Cursor.x, App_Cursor.y )
				intWidth = string.len(text) * 9 + 9
				x = 480 - 4 - intWidth
				heldMove = true
				DrawPoint = true
			elseif Controls.buttons(32) then --right Button Pressed
				App_Cursor.x = App_Cursor.x + 1
				if App_Cursor.x > App_Graphics.width - 1 then App_Cursor.x = App_Graphics.width - 1 end
				text = string.format( "%s, x:%d, y:%d",strArray[App_BlendMode+1], App_Cursor.x, App_Cursor.y )
				intWidth = string.len(text) * 9 + 9
				x = 480 - 4 - intWidth
				heldMove = true
				DrawPoint = true
			end

			if Controls.press(16384) then
				--Draw with the pencil.
				App_Graphics:Pencil(App_Cursor.x, App_Cursor.y, App_Color)
				App_FileSaved = false
				DrawPoint = false
			elseif Controls.buttons(16384) then --cross Button Pressed
				if DrawPoint then --Draw with the pencil.
					App_Graphics:Pencil(App_Cursor.x, App_Cursor.y, App_Color)
					App_FileSaved = false
					DrawPoint = false
				end
				heldCross = true
			end
		end

		--Move and Zoom Controls
		MoveZoom_Controls()

	end

	strArray = nil
	text = nil
	intWidth = nil
	intheight = nil
	x,y = nil,nil
	heldMove = nil
	DrawPoint = nil

	App_Mode = "Normal"

	return Window_Result_Ok
end
-----------------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------------
--Draw a rectangle
local FillRectangle = function()
	Window_Result_Ok = false
	App_Mode = "Rect"

	Window_InputSize = { x = Data_Selection.Rect.x, y = Data_Selection.Rect.y, w = Data_Selection.Rect.w, h = Data_Selection.Rect.h}
	Data_Selection.Rect = { x = App_Cursor.x, y = App_Cursor.y, w = 1, h = 1}
	Window_OutPutSize = { x = App_Cursor.x, y = App_Cursor.y, w = 1, h = 1}
	Window_CursorLocation = { x = 0, y = 0 }

	local intWidth = 9 * 22
	local intheight = 92
	local x,y = 0,0

	while System.running() do

		--Draw Screen
		drawFunc()

		--Initialize the GU (Note : Any graphical functions MUST be placed AFTER this)
		--System.draw()

		--Draw the Type Window
		x = (480 - intWidth) / 2
		y = (272 - intheight) / 2
		screen.fillRect( x + 2, y + 2, intWidth, intheight, ControlDark)
		screen.fillRect( x, y, intWidth, intheight, InfoText)
		screen.fillRect( x + 1, y + 1, intWidth - 2, intheight - 2, Info)
		screen.print( x + 4, y + 3, "-- Draw Rectangle: --", InfoText)
		screen.print( x + 4, y + intheight - 13, "O : Cancel X : Accept", InfoText)
		if Window_CursorLocation.y == 0 then
			screen.print( x + 4, y + 6 + 14 * 1, string.format(">>  < x:%s >", Window_OutPutSize.x), InfoText)
			screen.print( x + 4, y + 6 + 14 * 2, string.format("   y:%s", Window_OutPutSize.y), InfoText)
			screen.print( x + 4, y + 6 + 14 * 3, string.format("   w:%s", Window_OutPutSize.w), InfoText)
			screen.print( x + 4, y + 6 + 14 * 4, string.format("   h:%s", Window_OutPutSize.h), InfoText)
		elseif Window_CursorLocation.y == 1 then
			screen.print( x + 4, y + 6 + 14 * 1, string.format("   x:%s", Window_OutPutSize.x), InfoText)
			screen.print( x + 4, y + 6 + 14 * 2, string.format(">>  < y:%s >", Window_OutPutSize.y), InfoText)
			screen.print( x + 4, y + 6 + 14 * 3, string.format("   w:%s", Window_OutPutSize.w), InfoText)
			screen.print( x + 4, y + 6 + 14 * 4, string.format("   h:%s", Window_OutPutSize.h), InfoText)
		elseif Window_CursorLocation.y == 2 then
			screen.print( x + 4, y + 6 + 14 * 1, string.format("   x:%s", Window_OutPutSize.x), InfoText)
			screen.print( x + 4, y + 6 + 14 * 2, string.format("   y:%s", Window_OutPutSize.y), InfoText)
			screen.print( x + 4, y + 6 + 14 * 3, string.format(">>  < w:%s >", Window_OutPutSize.w), InfoText)
			screen.print( x + 4, y + 6 + 14 * 4, string.format("   h:%s", Window_OutPutSize.h), InfoText)
		elseif Window_CursorLocation.y == 3 then
			screen.print( x + 4, y + 6 + 14 * 1, string.format("   x:%s", Window_OutPutSize.x), InfoText)
			screen.print( x + 4, y + 6 + 14 * 2, string.format("   y:%s", Window_OutPutSize.y), InfoText)
			screen.print( x + 4, y + 6 + 14 * 3, string.format("   w:%s", Window_OutPutSize.w), InfoText)
			screen.print( x + 4, y + 6 + 14 * 4, string.format(">>  < h:%s >", Window_OutPutSize.h), InfoText)
		end

		--Finish the GU and Sync
		--System.endDraw()

		--Flip buffers
		screen.flip()

		--Slow down the Application if a button is pressed
		if Controls.buttons() > 0 and padcount > 0 then
			padcount = padcount - 1
			screen.waitVblankStart(5)
		elseif padcount ~= 10 and Controls.buttons() == 0 then
			padcount = 10
		end

		--update the controls
		Controls.readPeek()

		if Controls.press(1) or Controls.press(8) or Controls.press(256)
		or Controls.press(512) or Controls.press(8192) then --exit button pressed
			--Break the Type Loop and cancel the input
			Window_Result_Ok = false
			break
		elseif Controls.buttons(16) then --up button pressed
			Window_CursorLocation.y = Window_CursorLocation.y - 1
			if Window_CursorLocation.y < 0 then Window_CursorLocation.y = 3 end
		elseif Controls.buttons(64) then --down button pressed
			Window_CursorLocation.y = Window_CursorLocation.y + 1
			if Window_CursorLocation.y > 3 then Window_CursorLocation.y = 0 end
		elseif Controls.buttons(128) then --left button pressed
			if Window_CursorLocation.y == 0 and Data_Selection.Rect.x > 0 then
				Window_OutPutSize.x = Window_OutPutSize.x - 1
				Data_Selection.Rect.x = Window_OutPutSize.x
			elseif Window_CursorLocation.y == 1 and Data_Selection.Rect.y > 0 then
				Window_OutPutSize.y = Window_OutPutSize.y - 1
				Data_Selection.Rect.y = Window_OutPutSize.y
			elseif Window_CursorLocation.y == 2 and Window_OutPutSize.w > 1 then
				Window_OutPutSize.w = Window_OutPutSize.w - 1
				Data_Selection.Rect.w = Window_OutPutSize.w
			elseif Window_CursorLocation.y == 3 and Window_OutPutSize.h > 1 then
				Window_OutPutSize.h = Window_OutPutSize.h - 1
				Data_Selection.Rect.h = Window_OutPutSize.h
			end
		elseif Controls.buttons(32) then --right button pressed
			if Window_CursorLocation.y == 0 and Data_Selection.Rect.x < App_Graphics.width-1 then
				Window_OutPutSize.x = Window_OutPutSize.x + 1
				Data_Selection.Rect.x = Window_OutPutSize.x
			elseif Window_CursorLocation.y == 1 and Data_Selection.Rect.y < App_Graphics.height-1 then
				Window_OutPutSize.y = Window_OutPutSize.y + 1
				Data_Selection.Rect.y = Window_OutPutSize.y
			elseif Window_CursorLocation.y == 2 then
				Window_OutPutSize.w = Window_OutPutSize.w + 1
				Data_Selection.Rect.w = Window_OutPutSize.w
			elseif Window_CursorLocation.y == 3 then
				Window_OutPutSize.h = Window_OutPutSize.h + 1
				Data_Selection.Rect.h = Window_OutPutSize.h
			end
		elseif Controls.press(16384) then --cross button pressed
			Window_Result_Ok = true
			--Break the Type Loop
			break
		end

		--Move and Zoom Controls
		MoveZoom_Controls()

	end

	if Window_Result_Ok then
		--Draw Screen
		drawFunc()
		ShowMesssage("Message:","Updating the Image, Please wait...")
		--Draw the rectangle.
		App_Graphics:FillRectangle(Data_Selection.Rect.x, Data_Selection.Rect.y, Data_Selection.Rect.w, Data_Selection.Rect.h, App_Color)
		App_FileSaved = false
	end

	App_Cursor.x = Data_Selection.Rect.x
	App_Cursor.y = Data_Selection.Rect.y
	--Restore the Data Selection table.
	Data_Selection.Rect = { x = Window_InputSize.x, y = Window_InputSize.y, w = Window_InputSize.w, h = Window_InputSize.h}

	intWidth = nil
	intheight = nil
	x,y = nil,nil

	App_Mode = "Normal"

	return Window_Result_Ok
end
----------------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------------
local PaintBucket = function()
	App_Mode = "Bucket"
	Window_Result_Ok = false

	local strArray = {"SourceOver","SourceCopy","SourceAlpha"}
	local text = string.format( "%s, x:%d, y:%d",strArray[App_BlendMode+1], App_Cursor.x, App_Cursor.y )
	local intWidth = string.len(text) * 9 + 9
	local intheight = 22
	local x = 480 - 4 - intWidth
	local y = 272 - 18 - intheight

	while System.running() do

		--Draw Screen
		drawFunc()

		--Initialize the GU (Note : Any graphical functions MUST be placed AFTER this)
		--System.draw()

		--Draw the Type Window
		screen.fillRect( x + 2, y + 2, intWidth, intheight, ControlDark)
		screen.fillRect( x, y, intWidth, intheight, InfoText)
		screen.fillRect( x + 1, y + 1, intWidth - 2, intheight - 2, Info)
		screen.print(x + 4, y + 5, text, InfoText)

		--Finish the GU and Sync
		--System.endDraw()

		--Flip buffers
		screen.flip()

		--Slow down the Application if a button is pressed
		if Controls.buttons() > 0 and padcount > 0 then
			padcount = padcount - 1
			screen.waitVblankStart(5)
		elseif padcount ~= 10 and Controls.buttons() == 0 then
			padcount = 10
		end

		--update the controls
		Controls.readPeek()

		if Controls.press(8) or Controls.press(8192) then --exit button pressed
			--Break the loop and cancel the input.
			Window_Result_Ok = false
			break
		elseif Controls.press(1) then --select Button Pressed
			--Set the Graphics BlendMode.
			App_BlendMode = App_BlendMode > 1 and 0 or App_BlendMode + 1
			App_Graphics.BlendMode = App_BlendMode
			text = string.format( "%s, x:%d, y:%d",strArray[App_BlendMode+1], App_Cursor.x, App_Cursor.y )
			intWidth = string.len(text) * 9 + 9
			x = 480 - 4 - intWidth
		elseif Controls.press(256) then --ltrigger Button Pressed
			--Show the Color window
			Color_Window()
			DrawPoint = false
		elseif Controls.press(512) then --rtrigger Button Pressed
			--Show the Color Palet
			Palette_Window()
			DrawPoint = false
		else
			--Move the Cursor.
			if Controls.buttons(16) then --up Button Pressed
				App_Cursor.y = App_Cursor.y - 1
				if App_Cursor.y < 0 then App_Cursor.y = 0 end
				text = string.format( "%s, x:%d, y:%d",strArray[App_BlendMode+1], App_Cursor.x, App_Cursor.y )
				intWidth = string.len(text) * 9 + 9
				x = 480 - 4 - intWidth
			elseif Controls.buttons(64) then --down Button Pressed
				App_Cursor.y = App_Cursor.y + 1
				if App_Cursor.y > App_Graphics.height - 1 then App_Cursor.y = App_Graphics.height - 1 end
				text = string.format( "%s, x:%d, y:%d",strArray[App_BlendMode+1], App_Cursor.x, App_Cursor.y )
				intWidth = string.len(text) * 9 + 9
				x = 480 - 4 - intWidth
			end
			if Controls.buttons(128) then --left Button Pressed
				App_Cursor.x = App_Cursor.x - 1
				if App_Cursor.x < 0 then App_Cursor.x = 0 end
				text = string.format( "%s, x:%d, y:%d",strArray[App_BlendMode+1], App_Cursor.x, App_Cursor.y )
				intWidth = string.len(text) * 9 + 9
				x = 480 - 4 - intWidth
			elseif Controls.buttons(32) then --right Button Pressed
				App_Cursor.x = App_Cursor.x + 1
				if App_Cursor.x > App_Graphics.width - 1 then App_Cursor.x = App_Graphics.width - 1 end
				text = string.format( "%s, x:%d, y:%d",strArray[App_BlendMode+1], App_Cursor.x, App_Cursor.y )
				intWidth = string.len(text) * 9 + 9
				x = 480 - 4 - intWidth
			end

			if Controls.press(16384) then
				--Draw Screen
				drawFunc()
				ShowMesssage("Message:","Updating the Image, Please wait...")
				--Use the PaintBucket.
				App_Graphics:Fill( App_Cursor.x, App_Cursor.y, App_Color )
				App_FileSaved = false
			end
		end

		--Move and Zoom Controls
		MoveZoom_Controls()

	end

	strArray = nil
	text = nil
	intWidth = nil
	intheight = nil
	x,y = nil,nil
	heldMove = nil
	DrawPoint = nil

	App_Mode = "Normal"

	return Window_Result_Ok
end
----------------------------------------------------------------------------------------------------

---------------------------------------------------------------------------------------------------
local Recolor = function()
	App_Mode = "Recolor"
	Window_Result_Ok = false

	local strArray = {"SourceOver","SourceCopy","SourceAlpha"}
	local text = string.format( "%s, x:%d, y:%d",strArray[App_BlendMode+1], App_Cursor.x, App_Cursor.y )
	local intWidth = string.len(text) * 9 + 9
	local intheight = 22
	local x = 480 - 4 - intWidth
	local y = 272 - 18 - intheight

	while System.running() do

		--Draw Screen
		drawFunc()

		--Initialize the GU (Note : Any graphical functions MUST be placed AFTER this)
		--System.draw()

		--Draw the Type Window
		screen.fillRect( x + 2, y + 2, intWidth, intheight, ControlDark)
		screen.fillRect( x, y, intWidth, intheight, InfoText)
		screen.fillRect( x + 1, y + 1, intWidth - 2, intheight - 2, Info)
		screen.print(x + 4, y + 5, text, InfoText)

		--Finish the GU and Sync
		--System.endDraw()

		--Flip buffers
		screen.flip()

		--Slow down the Application if a button is pressed
		if Controls.buttons() > 0 and padcount > 0 then
			padcount = padcount - 1
			screen.waitVblankStart(5)
		elseif padcount ~= 10 and Controls.buttons() == 0 then
			padcount = 10
		end

		--update the controls
		Controls.readPeek()

		if Controls.press(8) or Controls.press(8192) then --exit button pressed
			--Break the loop and cancel the input.
			Window_Result_Ok = false
			break
		elseif Controls.press(1) then --select Button Pressed
			--Set the Graphics BlendMode.
			App_BlendMode = App_BlendMode > 1 and 0 or App_BlendMode + 1
			App_Graphics.BlendMode = App_BlendMode
			text = string.format( "%s, x:%d, y:%d",strArray[App_BlendMode+1], App_Cursor.x, App_Cursor.y )
			intWidth = string.len(text) * 9 + 9
			x = 480 - 4 - intWidth
		elseif Controls.press(256) then --ltrigger Button Pressed
			--Show the Color window
			Color_Window()
			DrawPoint = false
		elseif Controls.press(512) then --rtrigger Button Pressed
			--Show the Color Palet
			Palette_Window()
			DrawPoint = false
		else
			--Move the Cursor.
			if Controls.buttons(16) then --up Button Pressed
				App_Cursor.y = App_Cursor.y - 1
				if App_Cursor.y < 0 then App_Cursor.y = 0 end
				text = string.format( "%s, x:%d, y:%d",strArray[App_BlendMode+1], App_Cursor.x, App_Cursor.y )
				intWidth = string.len(text) * 9 + 9
				x = 480 - 4 - intWidth
			elseif Controls.buttons(64) then --down Button Pressed
				App_Cursor.y = App_Cursor.y + 1
				if App_Cursor.y > App_Graphics.height - 1 then App_Cursor.y = App_Graphics.height - 1 end
				text = string.format( "%s, x:%d, y:%d",strArray[App_BlendMode+1], App_Cursor.x, App_Cursor.y )
				intWidth = string.len(text) * 9 + 9
				x = 480 - 4 - intWidth
			end
			if Controls.buttons(128) then --left Button Pressed
				App_Cursor.x = App_Cursor.x - 1
				if App_Cursor.x < 0 then App_Cursor.x = 0 end
				text = string.format( "%s, x:%d, y:%d",strArray[App_BlendMode+1], App_Cursor.x, App_Cursor.y )
				intWidth = string.len(text) * 9 + 9
				x = 480 - 4 - intWidth
			elseif Controls.buttons(32) then --right Button Pressed
				App_Cursor.x = App_Cursor.x + 1
				if App_Cursor.x > App_Graphics.width - 1 then App_Cursor.x = App_Graphics.width - 1 end
				text = string.format( "%s, x:%d, y:%d",strArray[App_BlendMode+1], App_Cursor.x, App_Cursor.y )
				intWidth = string.len(text) * 9 + 9
				x = 480 - 4 - intWidth
			end

			if Controls.press(16384) then
				--Draw Screen
				drawFunc()
				ShowMesssage("Message:","Updating the Image, Please wait...")
				--Recolor the texture.
				App_Graphics:Recolor( App_MyBitmap:pixel(App_Cursor.x,App_Cursor.y), App_Color )
				App_FileSaved = false
			end
		end

		--Move and Zoom Controls
		MoveZoom_Controls()

	end

	strArray = nil
	text = nil
	intWidth = nil
	intheight = nil
	x,y = nil,nil
	heldMove = nil
	DrawPoint = nil

	App_Mode = "Recolor"

	return Window_Result_Ok
end
---------------------------------------------------------------------------------------------------


local Check = true
local strInput = ""

-----------------------------------------------------------------------------------------------------
--Apllication Main Loop
while App_Running do
	-- Exit the application on system request.
	if not System.running() then break end

	--Draw Screen
	drawFunc()

	--Flip buffers
	screen.flip()

	--Slow down the Application if a button is pressed
	if Controls.buttons() > 0 and padcount > 0 then
		padcount = padcount - 1
		screen.waitVblankStart(5)
	elseif padcount ~= 10 and Controls.buttons() == 0 then
		padcount = 10
	end

	--update the controls
	Controls.readPeek()

	if Controls.press(8) then --start Button Pressed
		Check = true
		if App_FileSaved == false then
			Check = false
			if Utility.msgShow(string.format("Warning: (Close Application)\nDo you want to continue without saving the file: %s .", App_FileName),
							   Utility.MSG_BTN_YESMO + Utility.MSG_DEFAULT_BTN_NO) then
				Check = true
			end
		end
		if Check then
			--Break The Main Loop
			App_Running = false
		end
	elseif Controls.press(1) then --select Button Pressed
		App_SelectedMenu = 4
		screen.waitVblankStart(4)
	elseif Controls.buttons(256) or Controls.buttons(128) then --ltrigger or left Button Pressed
		--Hide Menu
		if App_showFileMenu == true then App_showFileMenu = false end
		if App_showViewMenu == true then App_showViewMenu = false end
		if App_showImageMenu == true then App_showImageMenu = false end
		if App_showGraphicsMenu == true then App_showGraphicsMenu = false end
		App_SelectedMenu = App_SelectedMenu - 1
		if App_SelectedMenu < 0 then App_SelectedMenu = 5 end
		screen.waitVblankStart(4)
	elseif Controls.buttons(512) or Controls.buttons(32) then --rtrigger or right Button Pressed
		--Hide Menu
		if App_showFileMenu == true then App_showFileMenu = false end
		if App_showViewMenu == true then App_showViewMenu = false end
		if App_showImageMenu == true then App_showImageMenu = false end
		if App_showGraphicsMenu == true then App_showGraphicsMenu = false end
		App_SelectedMenu = App_SelectedMenu + 1
		if App_SelectedMenu > 5 then App_SelectedMenu = 0 end
		screen.waitVblankStart(4)
	elseif Controls.buttons(16) then --up Button Pressed
		if App_showFileMenu == true or App_showViewMenu == true or App_showImageMenu == true or App_showGraphicsMenu == true then--Menu
			App_MenuIndex = App_MenuIndex - 1
			if App_MenuIndex < 0 then App_MenuIndex = App_MaxMenuIndex end
			if App_showGraphicsMenu == true then
				App_MenuMemory = App_MenuIndex
			end
			screen.waitVblankStart(4)
		end
	elseif Controls.buttons(64) then --down Button Pressed
		if App_showFileMenu == true or App_showViewMenu == true or App_showImageMenu == true or App_showGraphicsMenu == true then --Menu
			App_MenuIndex = App_MenuIndex + 1
			if App_MenuIndex > App_MaxMenuIndex then App_MenuIndex = 0 end
			if App_showGraphicsMenu == true then
				App_MenuMemory = App_MenuIndex
			end
			screen.waitVblankStart(4)
		end
	elseif Controls.press(16384) then --cross Button Pressed
		--Show File Menu if App_SelectedMenu == 0
		if App_SelectedMenu == 0 and App_showFileMenu == false then
			App_showFileMenu = true
			App_MenuIndex = 0
			App_MaxMenuIndex = 5
			--Draw Screen
			drawFunc()
			--Flip buffers
			screen.flip()
			screen.waitVblankStart(9)
		elseif App_showFileMenu == true then
			if App_MenuIndex == 0 then --New File
				Check = true
				if App_FileSaved == false then
					Check = false
					if Utility.msgShow(string.format("Warning: (New File)\nDo you want to continue without saving the file: %s .", App_FileName),
										Utility.MSG_BTN_YESMO + Utility.MSG_DEFAULT_BTN_NO) then
						Check = true
					end
				end
				if Check then
					New_File()
				end
				screen.waitVblankStart(9)
				--Hide File Menu
				App_showFileMenu = false
			elseif App_MenuIndex == 1 then --Open File
			   Check = true
				if App_FileSaved == false then
					Check = false
					if Utility.msgShow(string.format("Warning:(Load File)\nDo you want to continue without saving the file: %s .", App_FileName),
										Utility.MSG_BTN_YESMO + Utility.MSG_DEFAULT_BTN_NO) then
						Check = true
					end
				end
				if Check then
					FileDialog = require "FileDialog"
					FileDialog.Index = 2
					FileDialog.FileName = App_FileName
					FileDialog.Path = App_Path
					FileDialog.Text = "Open File"
					if FileDialog.show() then
						App_FileName = FileDialog.FileName or App_FileName
						App_Path = FileDialog.Path or App_Path
						App_FullFileName = FileDialog.FullFileName or App_FullFileName
						load_File(App_FullFileName)
					end
				end
				screen.waitVblankStart(9)
				--Hide File Menu
				App_showFileMenu = false
			elseif App_MenuIndex == 2 then --Save
				--Save the file to the current directory and fileName
				local bln_error, Result = pcall(save_File, App_FullFileName)
				if not bln_error or not App_FileSaved then --Catch Error
					Utility.msgShow(string.format("Error:\nCan't save the file: %s\nTry the 'Save as' button.", App_FullFileName))
				end
				bln_error = nil
				Result = nil
				screen.waitVblankStart(9)
				--Hide File Menu
				App_showFileMenu = false
			elseif App_MenuIndex == 3 then --Save As
				FileDialog = require "FileDialog"
				FileDialog.Index = 1
				FileDialog.FileName = App_FileName
				FileDialog.Path = App_Path
				FileDialog.Text = "Save File"
				if FileDialog.show() then
					App_FileName = FileDialog.FileName or App_FileName
					App_Path = FileDialog.Path or App_Path
					App_FullFileName = FileDialog.FullFileName or App_FullFileName
					local bln_error, Result = pcall(save_File, App_FullFileName)
					if not bln_error or not App_FileSaved then --Catch Error
						Utility.msgShow(string.format("Error:\nCan't save the file: %s", App_FullFileName))
					end
					bln_error = nil
					Result = nil
				end
				screen.waitVblankStart(9)
				--Hide File Menu
				App_showFileMenu = false
			elseif App_MenuIndex == 4 then --Explorer
				FileDialog = require "FileDialog"
				FileDialog.Index = 2
				FileDialog.FileName = App_FileName
				FileDialog.Path = App_Path
				FileDialog.Text = "Explorer"
				FileDialog.show()
				screen.waitVblankStart(9)
				--Hide File Menu
				App_showFileMenu = false
			elseif App_MenuIndex == 5 then --Exit
				Check = true
				if App_FileSaved == false then
					Check = false
					if Utility.msgShow(string.format("Warning: (Close Application)\nDo you want to continue without saving the file: %s .", App_FileName),
							   Utility.MSG_BTN_YESMO + Utility.MSG_DEFAULT_BTN_NO) then
						Check = true
					end
				end
				if Check then
					--Break The Main Loop
					App_Running = false
				end
				screen.waitVblankStart(9)
				--Hide File Menu
				App_showFileMenu = false
			end
			Update_Menu()
		elseif App_SelectedMenu == 1 and App_showViewMenu == false then  --Show View Menu if App_SelectedMenu == 1
			App_showViewMenu = true
			App_MenuIndex = 0
			App_MaxMenuIndex = 3
			--Draw Screen
			drawFunc()
			--Flip buffers
			screen.flip()
			screen.waitVblankStart(9)
		elseif App_showViewMenu == true then
			if App_MenuIndex == 0 and App_ViewMenu.ZoomIn.enable then --Zoom in
				--Zoom In
				UpdateZoom(0.25)
			elseif App_MenuIndex == 1 and App_ViewMenu.ZoomOut.enable then --Zoom Out
				--Zoom Out
				UpdateZoom(-0.25)
			elseif App_MenuIndex == 2 then --Zoom To Window
				--Zoom to screen
				Zoom_screen()
				--Center To Sceen
				Center_screen()
				--Hide File Menu
				App_showViewMenu = false
				screen.waitVblankStart(9)
			elseif App_MenuIndex == 3 then --Center To Screen
				--Center To Sceen
				Center_screen()
				--Hide File Menu
				App_showViewMenu = false
				screen.waitVblankStart(9)
			end
			Update_Menu()
		elseif App_SelectedMenu == 2 and App_showImageMenu == false then --Show Image Menu if App_SeletedMenu == 2
			App_showImageMenu = true
			App_MenuIndex = 0
			App_MaxMenuIndex = 8
			--Draw Screen
			drawFunc()
			--Flip buffers
			screen.flip()
			screen.waitVblankStart(9)
		elseif App_showImageMenu == true and App_MyBitmap ~= nil then
			if App_MenuIndex == 0 and App_ImageMenu.Unswizzle.enable then --Unswizzle Image
				--Hide File Menu
				App_showImageMenu = false
				Unswizzle()
				screen.waitVblankStart(9)
			elseif App_MenuIndex == 1 and App_ImageMenu.Resize.enable then --Resize Image
				--Hide File Menu
				App_showImageMenu = false
				Resize()
				screen.waitVblankStart(9)
			elseif App_MenuIndex == 2 and App_ImageMenu.MoveImage.enable then --Move Image
				--Hide File Menu
				App_showImageMenu = false
				MoveImage()
				screen.waitVblankStart(9)
			elseif App_MenuIndex == 3 and App_ImageMenu.RotateFlip.enable then --RotateFlip
				--Hide File Menu
				App_showImageMenu = false
				RotateFlip()
				screen.waitVblankStart(9)
			elseif App_MenuIndex == 4 and App_ImageMenu.CutImage.enable then --Cut Image
				--Hide File Menu
				App_showImageMenu = false
				CutImage()
				screen.waitVblankStart(9)
			elseif App_MenuIndex == 5 and App_ImageMenu.CopyImage.enable then --Copy Image
				--Hide File Menu
				App_showImageMenu = false
				CopyImage()
				screen.waitVblankStart(9)
			elseif App_MenuIndex == 6 and App_ImageMenu.PasteImage.enable then --Paste Image
				--Hide File Menu
				App_showImageMenu = false
				PasteImage()
				screen.waitVblankStart(9)
			elseif App_MenuIndex == 7 and App_ImageMenu.DeleteImage.enable then --Delete Image
				--Hide File Menu
				App_showImageMenu = false
				DeleteImage()
				screen.waitVblankStart(9)
			elseif App_MenuIndex == 8 and App_ImageMenu.SelectImage.enable then --Select Image
				--Hide File Menu
				App_showImageMenu = false
				SelectImage()
				screen.waitVblankStart(9)
			end
			Update_Menu()
		elseif App_SelectedMenu == 3 and App_showGraphicsMenu == false then --Show Graphics Menu if App_SeletedMenu == 3
			App_showGraphicsMenu = true
			App_MenuIndex = App_MenuMemory
			App_MaxMenuIndex = 6
			--Draw Screen
			drawFunc()
			--Flip buffers
			screen.flip()
			screen.waitVblankStart(9)
		elseif App_showGraphicsMenu == true then
			if App_MenuIndex == 0 and App_GraphicsMenu.BlendMode.enable then --Select the BlendMode.
				--Hide File Menu
				App_showGraphicsMenu = false
				BlendMode_Window()
				--Show the Menu
				App_showGraphicsMenu = true
			elseif    App_MenuIndex == 1 and App_GraphicsMenu.ColorPicker.enable then --Color Picker.
				--Hide File Menu
				App_showGraphicsMenu = false
				ColorPicker()
				--Show the Menu
				App_showGraphicsMenu = true
			elseif App_MenuIndex == 2 and App_GraphicsMenu.Clear.enable then --Clear.
				-- Ask the use to confirm the current action.
				if Utility.msgShow("Warning: (Clearing image)\nDo you want to clear the image.",
								   Utility.MSG_BTN_YESMO + Utility.MSG_DEFAULT_BTN_NO) then
					--Clear image
					App_FileSaved = false
					if App_BlendMode < 2 then
						--Draw Screen
						drawFunc()
						ShowMesssage("Message:","Updating the Image, Please wait...")
					end
					App_Graphics:clear(App_Color)
				end
			elseif    App_MenuIndex == 3 and App_GraphicsMenu.Pencil.enable then --Pencil.
				--Hide File Menu
				App_showGraphicsMenu = false
				Pencil()
				--Show the Menu
				App_showGraphicsMenu = true
			elseif App_MenuIndex == 4 and App_GraphicsMenu.FillRectangle.enable then --Fill a Rectangle.
				--Hide File Menu
				App_showGraphicsMenu = false
				FillRectangle()
				--Show the Menu
				App_showGraphicsMenu = true
			elseif App_MenuIndex == 5 and App_GraphicsMenu.PaintBucket.enable then
				--Hide File Menu
				App_showGraphicsMenu = false
				PaintBucket()
				--Show the Menu
				App_showGraphicsMenu = true
			elseif App_MenuIndex == 6 and App_GraphicsMenu.Recolor.enable then
				--Hide File Menu
				App_showGraphicsMenu = false
				Recolor()
				--Show the Menu
				App_showGraphicsMenu = true
			end
			Update_Menu()
		elseif App_SelectedMenu == 4 and App_MenuArray.Color.enable then --<Color>
			Color_Window()
		elseif App_SelectedMenu == 5 and App_MenuArray.Palette.enable then --<Palette>
			Palette_Window()
		end
	elseif Controls.press(8192) then --circle Button Pressed
		--Hide File Menu
		if App_showFileMenu == true then
			App_showFileMenu = false
			screen.waitVblankStart(4)
		end
		 --Hide View Menu
		if App_showViewMenu == true then
			App_showViewMenu = false
			screen.waitVblankStart(4)
		end
		--Hide Image Menu
		if App_showImageMenu == true then
			App_showImageMenu = false
			screen.waitVblankStart(4)
		end
		--Hide Rectangle Menu
		if App_showGraphicsMenu == true then
			App_showGraphicsMenu = false
			screen.waitVblankStart(4)
		end
		Check = false
	end

	--Move and Zoom Controls
	MoveZoom_Controls()

end
-----------------------------------------------------------------------------------------------------
Check = nil
strInput = nil

--Dispoe Application Colors
transparent = nil
ContolLightLight = nil
ControlLight = nil
Control = nil
ControlDark = nil
ControlDarkDark = nil
ControlText = nil
ControlBorderLight = nil
ControlBorderDark = nil
CaptionText = nil
GrayText = nil
Window = nil
WindowText = nil
WindowFrame = nil
ButtonFace = nil
ButtonHighlight = nil
ButtonShadow = nil
ButtonBorden = nil
ButtonSelected = nil
Highlight = nil
HighlightText = nil
Info = nil
InfoText = nil
MenuBar = nil
Menu = nil
MenuDark = nil
MenuGrayText = nil
MenuText = nil
ScrollBarLight = nil
ScrollBar = nil
ScrollBarDark = nil
ScrollBarBack = nil
Greentext = nil

--Free All images
ImageMap = nil
ImageNew = nil
ImageOpen = nil
ImageSave = nil
ImageDelete = nil
ImageCut = nil
ImageCopy = nil
ImagePaste = nil
ImageDelete = nil
ImageBackground = nil
ImageCursor = nil

--Dispoe Application Data
App_Running = nil
App_showFileMenu = nil
App_FileMenu = nil
App_showViewMenu = nil
App_ViewMenu = nil
App_showImageMenu = nil
App_ImageMenu = nil
App_showGraphicsMenu = nil
App_RectangleMenu = nil
App_MenuArray = nil
App_SelectedMenu = nil
App_MenuIndex = nil
App_MaxMenuIndex = nil
App_MaxZoom = nil
if  App_Graphics then
	App_Graphics = App_Graphics:free()
else
	App_Graphics = nil
end
App_MyBitmap = nil
App_location = nil
App_factor = nil
App_RectangleColor = nil
App_FileName = nil
App_Path = nil
App_FullFileName = nil
App_FileSaved = nil
App_Size = nil
App_Swizzled = nil
App_BlendMode = nil
App_Mode = nil
App_Color = nil
App_Cursor = nil
App_MenuMemory = nil

--Dispose clipboard data
Data_Selection = nil
if Data_Clipboard.tex then Data_Clipboard.tex = nil end
Data_Clipboard = nil
ClipboardShow = nil

--Dispoe Control Data
padcount = nil

MoveZoom_Controls = nil
UpdateZoom = nil

--Dipose Resize Window
Window_InputSize = nil
Window_OutPutSize = nil
Window_CursorLocation = nil
Window_Result_Ok = nil
R,G,B,S,V,M = nil,nil,nil,nil,nil,nil
Select_Window_Menu = nil

--Dipsose Functions
ShowMesssage = nil
Update_Menu = nil
drawfunc = nil
New_File = nil
load_File = nil
save_File = nil
Unswizzle = nil
Resize = nil
MoveImage = nil
CutImage = nil
CopyImage = nil
PasteImage = nil
DeleteImage = nil
SelectImage = nil
BlendMode_Window = nil
Palette_Window = nil
Color_Window = nil
ColorPicker = nil
Pencil = nil
FillRectangle = nil
PaintBucket = nil
Recolor = nil

--Dipose the FileDialog module
if FileDialog and FileDialog.dispose then
	FileDialog.dispose()
end

--Dipose the Graphics module
if Graphics and Graphics.dispose then
	Graphics.dispose()
end

-- Changelog:
  -- v1.52 ??-??-2013:
  -- v1.53 11-01-2014:
	-- Fixed ColorPicker control function.
	-- Added ask conformation before clearing the image.
