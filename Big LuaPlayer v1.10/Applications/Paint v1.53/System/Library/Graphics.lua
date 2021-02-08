-----------------------------------------------------------------------------------------------------
-- Big LuaPlayer: Graphics˜ module v1.15 2013 (c) M4MSOFT
-----------------------------------------------------------------------------------------------------

-- Adds to the 'Graphics' module
module ("Graphics", package.seeall)

_PACKAGE = nil
_M = nil
_NAME = nil
_VERSION = "1.15"

-----------------------------------------------------------------------------------------------------
--Create module enums.
BlendMode = {}
BlendMode.Enum_SourceOver = 0 --Specified that when a color is rendered, it blended with the background color.
BlendMode.Enum_SourceCopy = 1 --Specified that when a color is rendered, it overwrites the background color.
BlendMode.Enum_SourceAlpha = 2 --Specified that when a color is rendered, it Gets the Alhpa value of the background color.
--Create the RotateFlip enums.
RotateFlipType = {}
RotateFlipType.None = 0 --Specified no rotation and no flipping.
RotateFlipType.FlipX = 1 --Specified a horizontal flip.
RotateFlipType.FlipY = 2 --Specified a vertical flip.
RotateFlipType.FlipZ = 4 --Specified a Z flip.
RotateFlipType.Rotata90 = 8 --Specified 90 degree rotation.
RotateFlipType.Rotata180 = 16 --Specified 180 degree rotation.
RotateFlipType.Rotata270 = 32 --Specified 270 degree rotation.
--Define constants
local G2D_STEP = 4096
-----------------------------------------------------------------------------------------------------


-----------------------------------------------------------------------------------------------------
-- Texture mangement.
function Texture.SetTarget(self)
	-- Move the Texture to the Vram.
	if not self:toVram() then
		return false
	end
	-- Start drawing.
	GL.Start()
	-- Set the texture as the render target (instead of the screen).
	if not GL.SetDrawTarget(self) then
		-- Move the Texture to the Ram.
		self:toRam()
		return false
	end
	return true
end
function Texture.ResetTarget(self)
	-- End drawing.
	GL.Finish()
	-- Move the Texture to the Ram.
	self:toRam()
end
-- Add the SetTarget and ResetTarget function to the Texture metateble.
local TexMeta = getmetatable(Texture.object)
TexMeta.SetTarget = Texture.SetTarget
TexMeta.ResetTarget = Texture.ResetTarget
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func FromFile: Create a new Graphics object form the specified bmp file.
--  @param path: path as string.
-- @returns
--   @param objGraphics: Return the Graphics object.
function FromFile(path)
	-- Set the Cpu Clock Frequencies.
	Power.cpuSpeed(333)

	--Build the new graphics object.
    local objGraphics = {}

	--Set the Data
    objGraphics.tex = false
	objGraphics.BlendMode = BlendMode.Enum_SourceOver
	objGraphics.width = 0
	objGraphics.height = 0

    --Bmp Binary file index.
    --2byte File index [66-77]
	--8byte int File size [0-0-0-0-0-0-0-0]
    --8 byte default [54-0-0-0-40-0-0-0]
    --x 4byte int [0-0-0-0]
    --y 4byte int [0-0-0-0]
    --Frames 2byte int [-1-0-]
    --PixelFormat 2 byte int [32-0]
    --8 byte default [0-0-0-0-0-0-0-0]
    --Dpix 4byte int [196-14-0-0]
    --Dpiy 4byte int [196-14-0-0]
    --8 byte default [0-0-0-0-0-0-0-0]
    --Bmp Binary file data.
    --4 byte Color data [bgrA] [255-255-255-255]

    local tex = false

    --Load the Bmp file as a Binary file.
    local file = io.open(path, "rb") or false

    if file then

        --Get the size of the file.
        local file_Bytes = file:seek("end") or 0

        --Rewind back to the start of the file
		file:seek("set")

		--Check the file size.
		if file_Bytes >= 58 then
			local file_data = ""

			-- Read the file Type.
			local file_Type = file:read(2)

			--Check the file Type.
			if file_Type:byte(1) == 66 and file_Type:byte(2) == 77 then
				--read the file  size data.
				local file_Size = file:read(8)
				--Calculate the file size.
				file_Size = file_Size:byte(1)*256^0 + file_Size:byte(2)*256^1 +
							file_Size:byte(3)*256^2 + file_Size:byte(4)*256^3
				--			+ file_Size:byte(5)*256^4 + file_Size:byte(6)*256^5 +
				--			file_Size:byte(7)*256^6 + file_Size:byte(8)*256^7

				--read the default file data.
				file_data = file:read(8)

				--read the Bmp width and heigth.
				local file_x = file:read(4)
				local file_y = file:read(4)
				--Calculate Bmp width and heigth.
				file_x = file_x:byte(1)*256^0 + file_x:byte(2)*256^1 + file_x:byte(3)*256^2 + file_x:byte(4)*256^3
				file_y = file_y:byte(1)*256^0 + file_y:byte(2)*256^1 + file_y:byte(3)*256^2 + file_y:byte(4)*256^3

				--read the file Frames.
				local file_Frames = file:read(2)
				--Calculate the file Frames.
				file_Frames = file_Frames:byte(1)*256^0 + file_Frames:byte(2)*256^1

				--read the PixelFormat.
				local file_Format = file:read(2)
				--Calculate the PixelFormat.
				file_Format = file_Format:byte(1)*256^0 + file_Format:byte(2)*256^1

				--read the default file data.
				file_data = file:read(8)

				--read the Dpi.
				local file_Dpix = file:read(4)
				local file_Dpiy = file:read(4)
				--Calculate the Dpi.
				file_Dpix = file_Dpix:byte(1)*256^0 + file_Dpix:byte(2)*256^1 + file_Dpix:byte(3)*256^2 + file_Dpix:byte(4)*256^3
				file_Dpiy = file_Dpiy:byte(1)*256^0 + file_Dpiy:byte(2)*256^1 + file_Dpiy:byte(3)*256^2 + file_Dpiy:byte(4)*256^3

				--read the default file data.
				file_data = file:read(8)

				--Calculate the file offset.
				file_offset = math.ceil(file_x*file_Format/32)/file_Format*32

				--Check if the file is a correct bmp file.
				if (file_Format / 8 * file_offset * file_y + 54) == file_Bytes then
					--Create the texture.
					objGraphics.tex = Texture.new(file_x, file_y)

					--Set the texture width and height.
					objGraphics.width = file_x
					objGraphics.height = file_y

					--Build the texture.
					local x,y = 0,0
					local bmpData = file:read(file_Bytes-54)
					local Pixelclr = Color.new(255,255,255,255)
					local tmpN = 0
					if file_Format == 24 then --24 bit colors.
						while true do
							if x < file_x then
								tmpN = y*file_offset*3+x*3
								Pixelclr:red(bmpData:byte(tmpN+3) or 255)
								Pixelclr:green(bmpData:byte(tmpN+2) or 255)
								Pixelclr:blue(bmpData:byte(tmpN+1) or 255)
								--Set the pixel.
								objGraphics.tex:pixel(x, file_y-1-y, Pixelclr)
								x = x + 1
							else
								x = 0
								y = y + 1
								if y > file_y - 1 then
									break
								end
							end
						end
					end
					if file_Format == 32 then --32 bit colors.
						while true do --32 bit colors.
							if x < file_x then
								tmpN = y*file_offset*4+x*4
								Pixelclr:red(bmpData:byte(tmpN+3) or 255)
								Pixelclr:green(bmpData:byte(tmpN+2) or 255)
								Pixelclr:blue(bmpData:byte(tmpN+1) or 255)
								Pixelclr:alpha(bmpData:byte(tmpN+4) or 255)
								--Set the pixel.
								objGraphics.tex:pixel(x, file_y-1-y, Pixelclr)
								x = x + 1
							else
								x = 0
								y = y + 1
								if y > file_y - 1 then
									break
								end
							end
						end
					end
					x,y = nil,nil
					bmpData = nil
					Pixelclr = nil
					tmpN = nil
				end

				file_Size = nil
				file_x = nil
				file_y = nil
				file_Frames = nil
				file_Format = nil
				file_offset = nil
			end

			file_Type = nil
			file_data = nil
		end

        file:close()
        file_Bytes = nil
    end
    file = nil

    --Set function handels
	objGraphics["SaveImage"] = Graphics["SaveImage"]
    objGraphics["Pencil"] = Graphics["Pencil"]
	objGraphics["clear"] = Graphics["clear"]
	objGraphics["Fill"] = Graphics["Fill"]
	objGraphics["Recolor"] = Graphics["Recolor"]
	objGraphics["FillRectangle"] = Graphics["FillRectangle"]
	objGraphics["DrawImage"] = Graphics["DrawImage"]
    objGraphics["clone"] = Graphics["clone"]
    objGraphics["free"] = Graphics["free"]

    --Dispose the arguments.
    path = nil

	-- Set the Cpu Clock Frequencies.
	Power.cpuSpeed(200)

    --Return the graphics object.
    return objGraphics
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func FromImage: Create a new Graphics object form the specified texture.
--  @param tex: tex as texture.
-- @returns
--   @param objGraphics: Return the Graphics object.
function FromImage(tex)
    --Build the new graphics object.
    local objGraphics = {}
    --Set the Data
    objGraphics.tex = tex or false
    objGraphics.BlendMode = BlendMode.Enum_SourceOver
    if objGraphics.tex then
        objGraphics.width = tex:width()
        objGraphics.height = tex:height()
    else
        objGraphics.width = 0
        objGraphics.height = 0
    end
    --Set function handels
	objGraphics["SaveImage"] = Graphics["SaveImage"]
    objGraphics["Pencil"] = Graphics["Pencil"]
	objGraphics["clear"] = Graphics["clear"]
	objGraphics["Fill"] = Graphics["Fill"]
	objGraphics["Recolor"] = Graphics["Recolor"]
	objGraphics["FillRectangle"] = Graphics["FillRectangle"]
	objGraphics["DrawImage"] = Graphics["DrawImage"]
    objGraphics["clone"] = Graphics["clone"]
    objGraphics["free"] = Graphics["free"]
    --Return the graphics object.
    return objGraphics
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func SaveImage: Save the image as a bitmap to a specified file.
--  @param path: path as string.
--  @param PixelFormat: PixelFormat as integer.
-- @returns
--   @param objGraphics: Return the Graphics object.
function SaveImage(self, path, PixelFormat)
    local path = path or  error("bad argument #1 path to 'Graphics.SaveImage' (string expected, got nil)")
	local PixelFormat = PixelFormat or 32

	if PixelFormat ~= 24 and PixelFormat ~= 32 then
		error("bad argument #2 PixelFormat to 'Graphics.SaveImage' (Incorrect PixelFormat)")
	end

	-- Set the Cpu Clock Frequencies.
	Power.cpuSpeed(333)

	--Bmp Binary file index.
    --2byte File index [66-77]
	--8byte int File size [0-0-0-0-0-0-0-0]
    --8 byte default [54-0-0-0-40-0-0-0]
    --x 4byte int [0-0-0-0]
    --y 4byte int [0-0-0-0]
    --Frames 2byte int [-1-0-]
    --PixelFormat 2 byte int [32-0]
    --8 byte default [0-0-0-0-0-0-0-0]
    --Dpix 4byte int [196-14-0-0]
    --Dpiy 4byte int [196-14-0-0]
    --8 byte default [0-0-0-0-0-0-0-0]
    --Bmp Binary file data.
    --4 byte Color data [bgrA] [255-255-255-255]

	local Bytesize = 0
	local nSize = 0
	local Frames = 1
	local width = self.width or self.tex:width()
	local height = self.height or self.tex:height()
	local offset = 0
	local Dpix = 3780
	local Dpiy = 3780
	local tmpValue = {}

	local Tobyte = function (intNumber,intBase)
		local result = {0,0,0,0,0,0,0,0}
		local intNumber = intNumber or 0
		local intBase  = intBase or 4
		local n = math.min (intBase,4)

		while n > 0 do
			n = n - 1
			result[n+1] = math.min(math.floor(intNumber / 256 ^ n), 255)
			intNumber = intNumber - result[n+1] * 256 ^ n
		end

		--Dispose arguments.
		intNumber = nil
		intBase = nil

		--Return result
		return result
	end

	--Calculate the file offset.
	offset = math.ceil(width*PixelFormat/32)/PixelFormat*32

	--Calculate the file Bytesize.
	nSize = PixelFormat / 8 * offset * height
	Bytesize = nSize + 54

	--Create a new file.
	file = io.open(path, "wb") or false

	if file then
		--Write the "BM" header.
		file:write(string.char(66, 77))

		--Write the file size.
		tmpValue = Tobyte(Bytesize,8)
		file:write(string.char(tmpValue[1], tmpValue[2], tmpValue[3], tmpValue[4],
		                       tmpValue[5], tmpValue[6], tmpValue[7], tmpValue[8]))

		--write the file max size.
		file:write(string.char(54, 0, 0, 0, 40, 0, 0, 0))

		--Write the width and the height
		tmpValue = Tobyte(width,4)
		file:write(string.char(tmpValue[1], tmpValue[2], tmpValue[3], tmpValue[4]))
		tmpValue = Tobyte(height,4)
		file:write(string.char(tmpValue[1], tmpValue[2], tmpValue[3], tmpValue[4]))

		--Write the Frames count.
		tmpValue = Tobyte(Frames,2)
		file:write(string.char(tmpValue[1], tmpValue[2]))

		--Write the PixelFormat count.
		tmpValue = Tobyte(PixelFormat,2)
		file:write(string.char(tmpValue[1], tmpValue[2]))

		--write the default data.
		file:write(string.char(0, 0, 0, 0, 0, 0, 0, 0))

		--write the Dpi.
		tmpValue = Tobyte(Dpix,4)
		file:write(string.char(tmpValue[1], tmpValue[2], tmpValue[3], tmpValue[4]))
		tmpValue = Tobyte(Dpiy,4)
		file:write(string.char(tmpValue[1], tmpValue[2], tmpValue[3], tmpValue[4]))

		--write the default data.
		file:write(string.char(0, 0, 0, 0, 0, 0, 0, 0))

		--Build the bmpData.
		local x,y = 0,0
		local bmpData = ""
		local Pixelclr = Color.new(255,255,255,255)
		local tmpN = 0
		if PixelFormat == 24 then --24 bit colors.
			while true do
				if x < width then
					Pixelclr = self.tex:pixel(x, height-1-y)
					bmpData = bmpData..string.char(Pixelclr:blue(), Pixelclr:green(), Pixelclr:red())
					x = x + 1
				elseif x < offset then
					bmpData = bmpData..string.char(255)
					x = x + 1
				else
					--Write the bitmap pixel data.
					file:write(bmpData)
					bmpData = ""
					x = 0
					y = y + 1
					if y > height - 1 then
						break
					end
				end
			end
		end
		if PixelFormat == 32 then --32 bit colors.
			while true do --32 bit colors.
				if x < width then
					Pixelclr = self.tex:pixel(x, height-1-y)
					bmpData = bmpData..string.char(Pixelclr:blue(), Pixelclr:green(), Pixelclr:red(), Pixelclr:alpha())
					x = x + 1
				elseif x < offset then
					bmpData = bmpData..string.char(255)
					x = x + 1
				else
					--Write the bitmap pixel data.
					file:write(bmpData)
					bmpData = ""
					x = 0
					y = y + 1
					if y > height - 1 then
						break
					end
				end
			end
		end
		x,y = nil,nil
		Pixelclr = nil
		tmpN = nil

		bmpData = nil
		file:close()
	end
	file = nil

	Bytesize = nil
	nSize = nil
	Frames = nil
	width = nil
	height = nil
	offset = nil
	Dpix = nil
	Dpiy = nil
	tmpValue = nil

	Tobyte = nil

	--Dispose the arguments.
	path = nil
	PixelFormat = nil

	-- Set the Cpu Clock Frequencies.
	Power.cpuSpeed(200)

	--Return nil
    return nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func Pencil: Draw with the pencil on the drawingsurface.
--  @param x: x location as Color.
--  @param y: y location as Color.
--  @param clr: clr as Color.
-- @returns
--   @param nil
function Pencil(self,x,y,clr)
    local x = x or 0
	local y = y or 0
	local clr = clr or Color.new(255, 255, 255, 0)

	--check the x,y loaction.
    if x > -1 and x < self.width and y > -1 and y < self.height then
        if self["BlendMode"] == BlendMode.Enum_SourceOver then
 			--Set the color to the pixel.
			self["tex"]:pixel(x, y, clr, true)
        elseif self["BlendMode"] == BlendMode.Enum_SourceCopy then
			--Set the color to the pixel.
			self["tex"]:pixel(x,y,clr)
        elseif self["BlendMode"] == BlendMode.Enum_SourceAlpha then
			-- Set the texture as the render target (instead of the screen).
			if self["tex"]:SetTarget() then
				--Set the color to the pixel.
				GL.BeginPoints(); GL.AddPoint(x, y, 0, clr); GL.End()
				-- End drawing.
				self["tex"]:ResetTarget()
			else
				--error("Can't set the render target to the texture.")
			end
		end
	end

    --Dispose the arguments.
    x = nil
	y = nil
	clr = nil

    --Return nil
    return nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func clear: Clears the entire drawingsurface and fills it with the specified background color.
--  @param clr: clr as Color.
-- @returns
--   @param nil
function clear(self,clr)
	-- Set the Cpu Clock Frequencies.
	Power.cpuSpeed(333)

    local clr = clr or Color.new(255, 255, 255, 0)

	if self["BlendMode"] == BlendMode.Enum_SourceOver then
		-- Clear the Texture.
		self["tex"]:clear(clr, true)
	elseif self["BlendMode"] == BlendMode.Enum_SourceCopy then
		-- Clear the Texture.
		self["tex"]:clear(clr)
	elseif self["BlendMode"] == BlendMode.Enum_SourceAlpha then
		-- Set the texture as the render target (instead of the screen).
		if self.tex:SetTarget() then
			-- Clear the Texture.
			GL.Clear(clr)
			-- End drawing.
			self.tex:ResetTarget()
		else
			--error("Can't set the render target to the texture.")
		end
	end

    --Dispose the arguments.
    clr = nil

	-- Set the Cpu Clock Frequencies.
	Power.cpuSpeed(200)

    --Return nil
    return nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func Fill: Fill the image form a specified location.
--  @param x: x as number.
--  @param y: y as number.
--  @param clr: clr as Color.
-- @returns
--   @param nil
function Fill(self, x, y, clr)
	-- Set the Cpu Clock Frequencies.
	Power.cpuSpeed(333)

	local x = x or 0
	local y = y or 0
	local clr  = clr or Color.new(255, 255, 255, 0)

	--Check the x and y loaction.
	if x < self.width and y < self.height and x >= 0 and y >= 0 then
		--set data
		local x1,y1 = x,y
		local w = self.width
		local h = self.height
		local intCheckedPixel = {}
		local x0,y0 = 0,0
		for x0 = 1, w do
			intCheckedPixel[x0] = {}
			for y0 = 1, h do
				intCheckedPixel[x0][y0] = 0
			end
		end
		x0,y0 = nil,nil
		local ArrayPoints = {}
		local blnDoLoop = true
		local clrStart = self.tex:pixel(x1, y1)
		local pixel = Color.new(255, 255, 255, 0)

		--Set the Start Point.
		ArrayPoints[1] = {x = x1, y = y1}
		intCheckedPixel[x1+1][y1+1] = 1

		while blnDoLoop do
			--Bekijk of de 4 (+) omringende pixel de zelfde kleur hebben als de huidige pixel.
			x1 = ArrayPoints[1].x
			y1 = ArrayPoints[1].y
			if y1 > 0 then
				pixel = self.tex:pixel(x1, y1-1)
				if pixel == clrStart and intCheckedPixel[x1+1][y1] == 0 then
					intCheckedPixel[x1+1][y1] = 1
					table.insert( ArrayPoints, {x = x1, y = y1-1} )
				elseif intCheckedPixel[x1+1][y1] == 0 then
					intCheckedPixel[x1+1][y1] = 2
				end
			elseif intCheckedPixel[x1+1][y1+1] == 0 then
				intCheckedPixel[x1+1][y1+1] = 2
			end
			if y1 < h-1 then
				pixel = self.tex:pixel(x1, y1+1)
				if pixel == clrStart and intCheckedPixel[x1+1][y1+2] == 0 then
					intCheckedPixel[x1+1][y1+2] = 1
					table.insert( ArrayPoints, {x = x1, y = y1+1} )
				elseif intCheckedPixel[x1+1][y1+2] == 0 then
					intCheckedPixel[x1+1][y1+2] = 2
				end
			elseif intCheckedPixel[x1+1][y1+1] == 0 then
				intCheckedPixel[x1+1][y1+1] = 2
			end
			if x1 > 0 then
				pixel = self.tex:pixel(x1-1, y1)
				if pixel == clrStart and intCheckedPixel[x1][y1+1] == 0 then
					intCheckedPixel[x1][y1+1] = 1
					table.insert( ArrayPoints, {x = x1-1, y = y1} )
				elseif intCheckedPixel[x1][y1+1] == 0 then
					intCheckedPixel[x1][y1+1] = 2
				end
			elseif intCheckedPixel[x1+1][y1+1] == 0 then
				intCheckedPixel[x1+1][y1+1] = 2
			end
			if x1 < w-1 then
				pixel = self.tex:pixel(x1+1, y1)
				if pixel == clrStart and intCheckedPixel[x1+2][y1+1] == 0 then
					intCheckedPixel[x1+2][y1+1] = 1
					table.insert( ArrayPoints, {x = x1+1, y = y1} )
				elseif intCheckedPixel[x1+2][y1+1] == 0 then
					intCheckedPixel[x1+2][y1+1] = 2
				end
			elseif intCheckedPixel[x1+1][y1+1] == 0 then
				intCheckedPixel[x1+1][y1+1] = 2
			end
			--Verwijder Huidige Pixel
			table.remove(ArrayPoints, 1)
			--Stop als er geen pixels meer zijn.
			if table.getn(ArrayPoints) <= 0 then
				blnDoLoop = false
			end
		end
		x1,y1 = nil,nil
		ArrayPoints = nil
		clrStart = nil
		pixel = nil

		if self["BlendMode"] == BlendMode.Enum_SourceOver then
			--Fill the image.
			local nx,ny,nw,nh = 0, 0, w-1, h-1
			for nx = 0, nw do
				for ny = 0, nh do
					if intCheckedPixel[nx+1][ny+1] == 1 then
						--Set the color to the pixel.
						self["tex"]:pixel(nx,ny,clr,true)
						intCheckedPixel[nx+1][ny+1] = nil
					end
				end
				intCheckedPixel[nx+1] = nil
			end
			nx,ny,nw,nh = nil,nil,nil,nil
		elseif self["BlendMode"] == BlendMode.Enum_SourceCopy then
			--Fill the image.
			local nx,ny,nw,nh = 0, 0, w-1, h-1
			for nx = 0, nw do
				for ny = 0, nh do
					if intCheckedPixel[nx+1][ny+1] == 1 then
						--Set the color to the pixel.
						self["tex"]:pixel(nx,ny,clr)
						intCheckedPixel[nx+1][ny+1] = nil
					end
				end
				intCheckedPixel[nx+1] = nil
			end
			nx,ny,nw,nh = nil,nil,nil,nil
		elseif self["BlendMode"] == BlendMode.Enum_SourceAlpha then
			-- Set the texture as the render target (instead of the screen).
			if self["tex"]:SetTarget() then
				--Fill the image.
				GL.BeginPoints();
				local nx,ny,nw,nh = 0, 0, w-1, h-1
				local count = G2D_STEP
				for nx = 0, nw do
					for ny = 0, nh do
						if intCheckedPixel[nx+1][ny+1] == 1 then
							if count == 0 then
								GL.End()
								GL.Finish()
								GL.SetDrawTarget(self["tex"])
								GL.BeginPoints()
								count = G2D_STEP
							end
							--Set the color to the pixel.
							GL.AddPoint(nx, ny, 0, clr)
							intCheckedPixel[nx+1][ny+1] = nil
							count = count - 1
						end
					end
					intCheckedPixel[nx+1] = nil
				end
				nx,ny,nw,nh = nil,nil,nil,nil
				count = nil
				GL.End()
				-- End drawing.
				self["tex"]:ResetTarget()
			else
				--error("Can't set the render target to the texture.")
			end
		end

		--Clear Data
		w,h = nil,nil
		intCheckedPixel = nil
	end

    --Dispose the arguments.
    x,y = nil,nil
	clr = nil

	-- Set the Cpu Clock Frequencies.
	Power.cpuSpeed(200)

    --Return nil
    return nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func Recolor: Replace a color.
--  @param clrStart: clrStart as Color.
--  @param clr: clr as Color.
-- @returns
--   @param nil
function Recolor(self, clrStart, clr)
	-- Set the Cpu Clock Frequencies.
	Power.cpuSpeed(333)

	local clrStart = clrStart or Color.new(255, 255, 255, 0)
	local clr = clr or Color.new(255, 255, 255, 0)

	if self["BlendMode"] == BlendMode.Enum_SourceOver then
		--Recolor the image.
		local nx,ny,nw,nh = 0, 0, self.width-1, self.height-1
		local pixel = clr
		for nx = 0, nw do
			for ny = 0, nh do
				--Get the pixel.
				pixel = self["tex"]:pixel(nx,ny)
				if pixel == clrStart then
					--Set the color to the pixel.
					self["tex"]:pixel(nx,ny,clr,true)
				end
			end
		end
		nx,ny,nw,nh = nil,nil,nil,nil
		pixel = nil
	elseif self["BlendMode"] == BlendMode.Enum_SourceCopy then
		--Recolor the image.
		local nx,ny,nw,nh = 0, 0, self.width-1, self.height-1
		local pixel = clr
		for nx = 0, nw do
			for ny = 0, nh do
				--Get the pixel.
				pixel = self["tex"]:pixel(nx,ny)
				if pixel == clrStart then
					--Set the color to the pixel.
					self["tex"]:pixel(nx,ny,clr)
				end
			end
		end
		nx,ny,nw,nh = nil,nil,nil,nil
		pixel = nil
	elseif self["BlendMode"] == BlendMode.Enum_SourceAlpha then
		-- Set the texture as the render target (instead of the screen).
		if self["tex"]:SetTarget() then
			--Recolor the image.
			GL.BeginPoints();
			local nx,ny,nw,nh = 0, 0, self.width-1, self.height-1
			local pixel = clr
			local count = G2D_STEP
			for nx = 0, nw do
				for ny = 0, nh do
					--Get the pixel.
					pixel = self["tex"]:pixel(nx,ny)
					if pixel == clrStart then
						if count == 0 then
							GL.End()
							GL.Finish()
							GL.SetDrawTarget(self["tex"])
							GL.BeginPoints()
							count = G2D_STEP
						end
						--Set the color to the pixel.
						GL.AddPoint(nx, ny, 0, clr)
						count = count - 1
					end
				end
			end
			nx,ny,nw,nh = nil,nil,nil,nil
			pixel = nil
			count = nil
			GL.End()
			-- End drawing.
			self["tex"]:ResetTarget()
		else
			--error("Can't set the render target to the texture.")
		end
	end

    --Dispose the arguments.
    clrStart = nil
	clr = nil

	-- Set the Cpu Clock Frequencies.
	Power.cpuSpeed(200)

    --Return nil
    return nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func FillRectangle: Fills the interior of a rectangle.
--  @param clr: clr as Color.
--  @param x: x as number.
--  @param y: y as number.
--  @param w: w as number.
--  @param h: h as number.
-- @returns
--   @param nil
function FillRectangle(self, x, y, w, h, clr)
	-- Set the Cpu Clock Frequencies.
	Power.cpuSpeed(333)

	local x = x or 0
	local y = y or 0
	local w = w or 1
	local h = h or 1
	local clr  = clr or Color.new(255, 255, 255, 0)

	--Check the width and the height.
	if x < self.width and y < self.height and x + w > 0 and y + h > 0 then
		--Update the x,y,w,h values.
		if x < 0 then
			w = w + x
			x = 0
		end
		if y < 0 then
			h = h + y
			y = 0
		end
		if x + w > self.width then w = self.width - x end
		if y + h > self.height then h = self.height - y end

		if self["BlendMode"] == BlendMode.Enum_SourceOver then
			-- Draw the rectangle.
			self["tex"]:fillRect(x, y, w, h, clr, true)
		elseif self["BlendMode"] == BlendMode.Enum_SourceCopy then
			-- Draw the rectangle.
			self["tex"]:fillRect(x, y, w, h, clr)
		elseif self["BlendMode"] == BlendMode.Enum_SourceAlpha then
			-- Set the texture as the render target (instead of the screen).
			if self.tex:SetTarget() then
				-- Draw the rectangle.
				GL.BeginRects(); GL.AddRect(x, y, w, h, 0, clr); GL.End()
				-- End drawing.
				self.tex:ResetTarget()
			else
				--error("Can't set the render target to the texture.")
			end
		end

	end

    --Dispose the arguments.
    x,y,w,h = nil,nil,nil,nil
	clr = nil

	-- Set the Cpu Clock Frequencies.
	Power.cpuSpeed(200)

    --Return nil
    return nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func DrawImage: Draws the specified image, using its original physical size, at the specified location.
--  @param tex: tex as texture.
--  @param x: x locaction as number
--  @param y: y locaction as number
--  @param sx: source x locaction as number
--  @param sy: source y locaction as number
--  @param w: width as number
--  @param h: height as number
-- @returns
--   @param nil
function DrawImage(self,tex,x,y,sx,sy,w,h)
    local tex = tex or error("bad argument #1 tex to 'Graphics.DrawImage' (texture expected, got nil)")
	local x = x or 0
	local y = y or 0
	local w = w or tex:width()
	local h = h or tex:height()
	local sx = sx or 0
	local sy = sy or 0

	-- Set the Cpu Clock Frequencies.
	Power.cpuSpeed(333)

	--Update the x,y,w,h values.
	if x < 0 then
		sx = sx - x
		w = w + x
		x = 0
	end
	if y < 0 then
		sy = sy - y
		h = h + y
		y = 0
	end
	if sx+w > tex:width() then w = tex:width() - sx end
	if sy+h > tex:height() then h = tex:height() - sy end

	--Check the width and the height.
	if x < self.width and y < self.height and x + w > 0 and y + h > 0
	and sx >= 0 and sy >= 0 then
		--Update the x,y,w,h values.
		if x + w > self.width then w = self.width - x end
		if y + h > self.height then h = self.height - y end

		if self["BlendMode"] == BlendMode.Enum_SourceOver then
			-- Draw the rectangle.
			self["tex"]:blit(x, y, tex, sx, sy, w, h, true)
		elseif self["BlendMode"] == BlendMode.Enum_SourceCopy then
			-- Draw the rectangle.
			self["tex"]:blit(x, y, tex, sx, sy, w, h)
		elseif self["BlendMode"] == BlendMode.Enum_SourceAlpha then
			-- Set the texture as the render target (instead of the screen).
			if self.tex:SetTarget() then
				-- Draw the texture.
				GL.BeginRects(tex); GL.AddTex(x, y, w, h, sx, sy, w, h); GL.End()
				-- End drawing.
				self.tex:ResetTarget()
			else
				--error("Can't set the render target to the texture.")
			end
		end

	end

    --Dispose the arguments.
    tex = nil
	x,y = nil,nil
	sx,sy = nil,nil
	w,h = nil,nil

	-- Set the Cpu Clock Frequencies.
	Power.cpuSpeed(200)

    --Return nil
    return nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func RotateFlip: Rotates, flips or rotate and flips the Image.
--  @param style: The Rotateflip type as a integer.
-- @returns
--   @param texNew: Return the texture.
function RotateFlip(tex, style)
	local style = style or 0
	if type(style) ~= "number" then error("bad argument #1 style to 'Graphics.RotateFlip' (Number expected, got "..type(style)..")") end
	style =  math.min(math.floor(style),63)

	-- Set the Cpu Clock Frequencies.
	Power.cpuSpeed(333)

	local flipX = false
	local flipY = false
	local flipZ = false
	local Rotate = 0

	--Get The Rotate and Flip Style.
	if style - 32 >= 0 then
		if Rotate == 0 then Rotate = 270 end
		style = style - 32
	end
	if style - 16 >= 0 then
		if Rotate == 0 then Rotate = 180 end
		style = style - 16
	end
	if style - 8 >= 0 then
		if Rotate == 0 then Rotate = 90 end
		style = style - 8
	end
	if style - 4 >= 0 then
		flipZ = true
		style = style - 4
	end
	if style - 2 >= 0 then
		flipY = true
		style = style - 2
	end
	if style - 1 >= 0 then
		flipX = true
		style = style - 1
	end

	local x = x or 0
	local y = y or 0
	local w = tex:width()
	local h = tex:height()

	local tw = 0
	local th = 0

	-- Set the Rotation.
	if Rotate == 90 then
		flipZ = not flipZ
		flipY = not flipY
	elseif Rotate == 180 then
		flipX = not flipX
		flipY = not flipY
	elseif Rotate == 270 then
		flipZ = not flipZ
		flipX = not flipX
	end

	-- Set the Z flip.
	if flipZ then
		tw,th = h,w
	else
		tw,th = w,h
	end

	-- Create the Return texture.
	local texNew = Texture.new(tw, th) or false

	-- Set the horizontal and or the vertical flip.
	if flipX then th = 0 - th end --horizontal flip. <>
	if flipY then tw = 0 - tw end --vertical flip. ~V

	if texNew then
		local nw,nh,nxs,nys = 0, 0, 1, 1
		local pixel = Color.new(255,255,255,255)

		if tw < 0 then
			nxs = -1
			nw = -(tw + 1)
		end
		if th < 0 then
			nys = -1
			nh = -(th + 1)
		end

		--draw the texture.
		if flipZ then
			for nx = 0, tw-nxs, nxs do
				for ny = 0, th-nys, nys do
					--Get the pixel.
					pixel = tex:pixel(ny*nys, nx*nxs)
					--Set the color to the pixel.
					texNew:pixel(nw+nx, nh+ny, pixel)
				end
			end
		else
			for nx = 0, tw-nxs, nxs do
				for ny = 0, th-nys, nys do
					-- Get the pixel.
					pixel = tex:pixel(nx*nxs, ny*nys)
					-- Set the color to the pixel.
					texNew:pixel(nw+nx, nh+ny, pixel)
				end
			end
		end

		nx,ny,nw,nh = nil,nil,nil,nil
		pixel = nil
	end

    --Dispose the arguments.
    tex = nil
	style = nil
	flipX = nil
	flipY = nil
	Rotate = nil
	x,y = nil,nil
	sx,sy = nil,nil
	w,h = nil,nil

	-- Set the Cpu Clock Frequencies.
	Power.cpuSpeed(200)

    --Return texNew
    return texNew
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func clone: Clone the Graphics object.
--  @param : takes no arguments.
-- @returns
--   @param objGraphics: Return the Graphics object.
function clone(self)
    --Build the new graphics object.
    local objGraphics = {}
    --Copy the Data.
    objGraphics["BlendMode"] = self["BlendMode"]
    objGraphics["width"] = self["width"]
    objGraphics["height"] = self["height"]
	objGraphics["tex"] = self["tex"]

    --Copy the function handels.
	objGraphics["SaveImage"] = self["SaveImage"]
	objGraphics["Pencil"] = self["Pencil"]
    objGraphics["clear"] = self["clear"]
	objGraphics["Fill"] = self["Fill"]
	objGraphics["Recolor"] = self["Recolor"]
	objGraphics["FillRectangle"] = self["FillRectangle"]
	objGraphics["DrawImage"] = self["DrawImage"]
    objGraphics["clone"] = self["clone"]
    objGraphics["free"] = self["free"]

    --Return the graphics object.
    return objGraphics
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func free: Free the Graphics object.
--  @param : takes no arguments.
-- @returns
--   @param nil
function free(self)
    --Free the data
    self.tex = nil
    self.BlendMode = nil
    self.width = nil
    self.height = nil
    self = nil

    --Return nil
    return nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func dispose: Remove the module.
--  @param : takes no arguments.
-- @returns
--   @param nil
function dispose()

	-- dispose the swap function.
	TexMeta.SetTarget = nil
	TexMeta.ResetTarget = nil
	Texture.SetTarget = nil
	Texture.ResetTar = nil
	TexMeta = nil

    --dispose the module functions.
    Graphics.FromFile = nil
	Graphics.FromImage = nil
	Graphics.SaveImage = nil
	Graphics.Pencil = nil
    Graphics.clear = nil
	Graphics.Fill = nil
	Graphics.Recolor = nil
	Graphics.FillRectangle = nil
	Graphics.DrawImage = nil
	Graphics.RotateFlip = nil
    Graphics.clone = nil
    Graphics.free = nil
    Graphics.dispose = nil

    --dispose the module Enum Collections.
    Graphics.BlendMode.Enum_SourceOver = nil
    Graphics.BlendMode.Enum_SourceCopy = nil
    Graphics.BlendMode.Enum_SourceAlpha = nil
    Graphics.BlendMode =  nil

	Graphics.RotateFlipType.None = nil
	Graphics.RotateFlipType.FlipX = nil
	Graphics.RotateFlipType.FlipY = nil
	Graphics.RotateFlipType.Rotata90 = nil
	Graphics.RotateFlipType.Rotata180 = nil
	Graphics.RotateFlipType.Rotata270 = nil
	Graphics.RotateFlipType = nil

    Graphics._PACKAGE = nil
    Graphics._M = nil
    Graphics._NAME = nil
    Graphics._VERSION = nil
    Graphics = nil
    package.loaded["Graphics"] = nil
    return nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--End Graphics˜ module.
-----------------------------------------------------------------------------------------------------

-- Changelog:
  -- v1.00 2011:
  -- v1.10 30-12-2012:
    -- Added RotateFlip() function.
	-- Added RotateFlip enums.
  -- v1.15 23-02-2013:
	-- Texture Reder Target support function.
    -- SourceAlpha supprt for each draw function.
