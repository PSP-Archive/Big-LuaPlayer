-----------------------------------------------------------------------------------------------------
--Phoenix Game Engine: Editer˜ module v1.00 2012 (c) M4MSOFT
-----------------------------------------------------------------------------------------------------

-- Adds to the 'Editer` module
module ("Editer", package.seeall)

_PACKAGE = nil
_M = nil
_NAME = nil
_VERSION = "1.00"

-----------------------------------------------------------------------------------------------------
--Create module settings.
vBar_Size = 201 - 10 + 3
vBar_MinSize = 9
vBar_Location = 39 + 5
hBar_Size = 447
hBar_MinSize = 9
hBar_Location = 11
MaxTextLines = 16
MaxTextWidth = 51
chrTab = string.char(9)
chrMusic = string.char(13)
chrSpace = string.char(32)
chrMin = string.char(45)
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--Define local functions.
local getTextWidth = function(tbl)
	--get the width of the text
	local intWidth = 0
	for idx, line in ipairs(tbl) do
		intWidth = math.max( intWidth, string.len(line))
	end
	return intWidth
end
local updateSelect = function(self)
	local tblSelect = self["selectedText"]
	if self["selecting"] > 0 then
		if tblSelect["startline"]["start"] >= self["pntSelected"]["x"] then
			if tblSelect["startline"]["line"] >= self["pntSelected"]["y"] then
				self["selecting"] = -1
				tblSelect["endline"]["start"] = tblSelect["startline"]["start"]
				tblSelect["endline"]["line"] = tblSelect["startline"]["line"]
				tblSelect["startline"]["start"] = self["pntSelected"]["x"]
				tblSelect["startline"]["line"] = self["pntSelected"]["y"]
			else
				tblSelect["endline"]["start"] = self["pntSelected"]["x"]
				tblSelect["endline"]["line"] = self["pntSelected"]["y"]
			end
		else
			if tblSelect["startline"]["line"] > self["pntSelected"]["y"] then
				self["selecting"] = -1
				tblSelect["endline"]["start"] = tblSelect["startline"]["start"]
				tblSelect["endline"]["line"] = tblSelect["startline"]["line"]
				tblSelect["startline"]["start"] = self["pntSelected"]["x"]
				tblSelect["startline"]["line"] = self["pntSelected"]["y"]
			else
				tblSelect["endline"]["start"] = self["pntSelected"]["x"]
				tblSelect["endline"]["line"] = self["pntSelected"]["y"]
			end
		end
	else
		if tblSelect["endline"]["start"] <= self["pntSelected"]["x"] then
			if tblSelect["endline"]["line"] <= self["pntSelected"]["y"] then
				self["selecting"] = 1
				tblSelect["startline"]["start"] = tblSelect["endline"]["start"]
				tblSelect["startline"]["line"] = tblSelect["endline"]["line"]
				tblSelect["endline"]["start"] = self["pntSelected"]["x"]
				tblSelect["endline"]["line"] = self["pntSelected"]["y"]
			else
				tblSelect["startline"]["start"] = self["pntSelected"]["x"]
				tblSelect["startline"]["line"] = self["pntSelected"]["y"]
			end
		else
			if tblSelect["endline"]["line"] < self["pntSelected"]["y"] then
				self["selecting"] = 1
				tblSelect["startline"]["start"] = tblSelect["endline"]["start"]
				tblSelect["startline"]["line"] = tblSelect["endline"]["line"]
				tblSelect["endline"]["start"] = self["pntSelected"]["x"]
				tblSelect["endline"]["line"] = self["pntSelected"]["y"]
			else
				tblSelect["startline"]["start"] = self["pntSelected"]["x"]
				tblSelect["startline"]["line"] = self["pntSelected"]["y"]
			end
		end
	end
	tblSelect = nil
end
local saveFile = function(self, path)
	local path = path or self.FullFileName
	local file = io.open( path, "w")
	if file then
		local tab = 0
		for idx, line in ipairs(self.text) do
			tab = 0
			line = string.rtrim(line)
			while string.sub(line, 1+tab*4, 4+tab*4) == "    " do tab = tab + 1 end
			if tab > 0 then line = string.rep(chrTab, tab)..(string.sub(line, 1+tab*4, -1) or "") end
			file:write(string.format("%s%s", line, "\n"))
		end
		file:close()
		self.saved = true
		tab = nil
	end
	file = nil
	path = nil
	return "Save Complete"
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func new: build a new file object.
--  @param strName: Name as string
-- @returns
--   @param object as file object
function new(str)
	--build the object
	local tbl = {}

	--Set the Data
	tbl.FileName = str or "Untitled.lua"
	tbl.Path = System.currentDirectory()
	tbl.FullFileName = string.format("%s/%s", tbl.Path, tbl.FileName)
	tbl.saved = true
	tbl.loaded = false
	tbl.pntDraw = { x = 0, y = 0}
	tbl.pntSelected = { x = 1, y = 1}
	tbl.pntCursor = { x = 1, y = 1}
	tbl.text = {""}
	tbl.text_green = {false}
	tbl.textLines = 1
	tbl.textWidth = 0
	tbl.textMaxWidth = 0
	tbl.selecting = 0
	tbl.selected = false
	tbl.selectedText = {
						startline = { line = 0, start = 0, selected = false},
						endline = { line = 0, start = 0, selected = false}
					   }
	tbl.vBar_Size = vBar_Size
	tbl.vBar_StepSize = 0
	tbl.vBar_Location = vBar_Location
	tbl.hBar_Size = hBar_Size
	tbl.hBar_StepSize = 0
	tbl.hBar_Location = hBar_Location

	--Set function handels
	tbl["load"] = Editer["load"]
	tbl["save"] = Editer["save"]
	tbl["saveAs"] = Editer["saveAs"]
	tbl["SetvBar"] = Editer["SetvBar"]
	tbl["SethBar"] = Editer["SethBar"]
	tbl["CursorUp"] = Editer["CursorUp"]
	tbl["CursorDown"] = Editer["CursorDown"]
	tbl["CursorLeft"] = Editer["CursorLeft"]
	tbl["CursorRight"] = Editer["CursorRight"]
	tbl["select"] = Editer["select"]
	tbl["delete"] = Editer["delete"]
	tbl["deleteLine"] = Editer["deleteLine"]
	tbl["newLine"] = Editer["newLine"]
	tbl["write"] = Editer["write"]
	tbl["free"] = Editer["free"]

	--Return the object
	return tbl
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func load: Load the file object.
--  @param : takes no arguments
-- @returns
--   @param object as file object
function load(self)
	--build the object
	local tbl = self

	_G.FileDialog = require "FileDialog"
	FileDialog.Index = 2
	FileDialog.FileName = self.FileName
	FileDialog.Path = self.Path
	FileDialog.Text = "Open File"
	if FileDialog.show() then
		tbl = new(FileDialog.FileName or self.FileName)
		tbl.Path = FileDialog.Path or self.Path
		tbl.FullFileName = FileDialog.FullFileName or self.FullFileName
		--Try to load the file
		local file = io.open( tbl.FullFileName, "r")
		tbl.text = {""}
		tbl.text_green = {false}
		tbl.textLines = 0
		if file then
			for line in file:lines() do
				if line ~= nil then
					tbl["textLines"] = tbl["textLines"] + 1
					if string.sub(line , -1,-1) == chrMusic then line = string.sub( line , 1, -2) end
					--line = string.gsub(line, chrTab.."+(.+)", "    %1")
					line = string.gsub(line, chrTab, "    ")
					tbl["text"][tbl["textLines"]] = line or ""
					tbl.text_green[tbl["textLines"]] = false
					tbl.textMaxWidth = math.max(tbl.textMaxWidth, string.len(line or "") + 1)
				end
			end
			file:close()
			if tbl["textLines"] == 0 then tbl["textLines"] = 1 end
			tbl.textWidth = string.len(tbl.text[tbl.pntSelected.y]) + 1
			SetvBar(tbl, tbl.textLines)
			SethBar(tbl, tbl.textMaxWidth)
			tbl.saved = true
			tbl.loaded = true
		else
			Utility.msgShow(string.format("Error:\nCan't load the file: '%s'.", tbl.FullFileName))
			tbl = tbl:free() or new()
		end
		file = nil
	end
	path = nil

	--Return the object
	return tbl
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func save: Save File Function.
--   @param name: name as string
-- @returns
--   @param nil
function save(self, path)
	if not self.saved then
		local path = path or self.FullFileName
		local bln_error, bln_Result = pcall(saveFile, self, path)
		if not bln_error or not self.saved then --Catch Error
			Utility.msgShow(string.format("Error:\nCan't save the file: '%s'.", path))
		end
		bln_error = nil
		bln_Result = nil
	end
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func saveAs: Save File Function.
--   @param name: name as string
-- @returns
--   @param nil
function saveAs(self)
	_G.FileDialog = require "FileDialog"
	FileDialog.Index = 1
	FileDialog.FileName = self.FileName
	FileDialog.Path = self.Path
	FileDialog.Text = "Save File"
	if FileDialog.show() then
		self.FileName = FileDialog.FileName or self.FileName
		self.Path = FileDialog.Path or self.Path
		self.FullFileName = FileDialog.FullFileName or self.FullFileName
		local bln_saved = self.saved
		self.saved = false
		local bln_error, bln_Result = pcall(saveFile, self, self.FullFileName)
		if not bln_error or not self.saved then --Catch Error
			Utility.msgShow(string.format("Error:\nCan't save the file: '%s.'", self.FullFileName))
		end
		self.saved = self.saved or bln_saved
		bln_saved = nil
		bln_error = nil
		bln_Result = nil
	end
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func SetvBar: Calculate the size and stepSize of the scrollbar.
--  @param Value: Value as number
-- @returns
--   @param nil
function SetvBar(self, Value)
	self.vBar_Size = vBar_Size * (MaxTextLines / math.max( Value or 0, MaxTextLines))
	self.vBar_Size = math.floor( self.vBar_Size )
	self.vBar_Size = math.max( self.vBar_Size, vBar_MinSize)
	if math.max( Value or 0, MaxTextLines) > MaxTextLines then
		self.vBar_StepSize = (vBar_Size - self.vBar_Size) / ( math.max( Value or 0, MaxTextLines) - MaxTextLines)
	else
		self.vBar_StepSize = 0
	end
	Value = nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func SethBar: Calculate the size and stepSize of the scrollbar.
--  @param Value: Value as number
-- @returns
--   @param nil
function SethBar(self, Value)
	self.hBar_Size = hBar_Size * (MaxTextWidth / math.max( Value or 0, MaxTextWidth))
	self.hBar_Size = math.floor( self.hBar_Size )
	self.hBar_Size = math.max( self.hBar_Size, hBar_MinSize)
	if math.max( Value or 0, MaxTextWidth) > MaxTextWidth then
		self.hBar_StepSize = (hBar_Size - self.hBar_Size) / ( math.max( Value or 0, MaxTextWidth) - MaxTextWidth)
	else
		self.hBar_StepSize = 0
	end
	Value = nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func CursorUp: Move the cursor up.
--  @param steps: steps as number
--  @param Yonly: as boolean
-- @returns
--   @param nil
function CursorUp(self, steps, Yonly)
	--Move the Cursor up
	self.pntSelected.y = self.pntSelected.y - steps
	self.pntCursor.y = self.pntCursor.y - steps
	self.pntCursor.y = math.max( self.pntCursor.y, 1)
	if self.pntSelected.y < 1 then
		self.pntSelected.y = self.textLines
		self.pntCursor.y = math.min( self.textLines, MaxTextLines)
		self.pntDraw.y = self.pntSelected.y - math.min( self.textLines, MaxTextLines)
		self.vBar_Location = 44 + self.vBar_StepSize * self.pntDraw.y
	end
	if self.textLines > MaxTextLines and self.pntCursor.y == 1 and self.pntSelected.y >= 1 then
		self.pntDraw.y = self.pntSelected.y - 1
		self.vBar_Location = vBar_Location + self.vBar_StepSize * self.pntDraw.y
	end
	self.textWidth = string.len(self.text[self.pntSelected.y]) + 1
	if Yonly ~= true then
		if self.pntSelected.x > self.textWidth then
			self.pntSelected.x = self.textWidth
			self.pntCursor.x = math.min( self.textWidth, MaxTextWidth)
			self.pntDraw.x = self.pntSelected.x - math.min( self.textWidth, MaxTextWidth)
			self.hBar_Location = hBar_Location + self.hBar_StepSize * self.pntDraw.x
		end
	end
	--select the text.
	if self.selecting ~= 0 then
		updateSelect(self)
	end
	Yonly = nil
	steps = nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func CursorDown: Move the cursor down.
--  @param steps: steps as number
--  @param Yonly: as boolean
-- @returns
--   @param nil
function CursorDown(self, steps, Yonly)
	--Move the Cursor down
	self.pntSelected.y = self.pntSelected.y + steps
	self.pntCursor.y = self.pntCursor.y + steps
	self.pntCursor.y = math.min( self.pntCursor.y, MaxTextLines)
	if self.pntSelected.y > self.textLines then
		self.pntSelected.y = 1
		self.pntCursor.y = 1
		self.pntDraw.y = 0
		self.vBar_Location = vBar_Location + self.vBar_StepSize * self.pntDraw.y
	end
	if self.textLines > MaxTextLines and self.pntCursor.y == MaxTextLines and self.pntSelected.y <= self.textLines then
		self.pntDraw.y = self.pntSelected.y - MaxTextLines
		self.vBar_Location = vBar_Location + self.vBar_StepSize * self.pntDraw.y
	end
	self.textWidth = string.len(self.text[self.pntSelected.y]) + 1
	if Yonly ~= true then
		if self.pntSelected.x > self.textWidth then
			self.pntSelected.x = self.textWidth
			self.pntCursor.x = math.min( self.textWidth, MaxTextWidth)
			self.pntDraw.x = self.pntSelected.x - math.min( self.textWidth, MaxTextWidth)
			self.hBar_Location = hBar_Location + self.hBar_StepSize * self.pntDraw.x
		end
	end
	--select the text.
	if self.selecting ~= 0 then
		updateSelect(self)
	end
	Yonly = nil
	steps = nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func CursorLeft: Move the cursor to the Left.
--  @param steps: steps as number
--  @param Xonly: as boolean
-- @returns
--   @param nil
function CursorLeft(self, steps, Xonly)
	--Move the Cursor left
	self.pntSelected.x = self.pntSelected.x - steps
	self.pntCursor.x = self.pntCursor.x - steps
	self.pntCursor.x = math.max( self.pntCursor.x, 1)
	if self.pntSelected.x < 1 then
		if Xonly ~= true then self:CursorUp(1, true) end
		self.pntSelected.x = self.textWidth
		self.pntCursor.x = math.min( self.textWidth, MaxTextWidth)
		self.pntDraw.x = self.pntSelected.x - math.min( self.textWidth, MaxTextWidth)
		self.hBar_Location = hBar_Location + self.hBar_StepSize * self.pntDraw.x
	end
	if self.pntCursor.x == 1 and self.pntSelected.x >= 1 then
		self.pntDraw.x = self.pntSelected.x - 1
		self.hBar_Location = hBar_Location + self.hBar_StepSize * self.pntDraw.x
	end
	--select the text.
	if self.selecting ~= 0 then
		updateSelect(self)
	end
	steps = nil
	Xonly = nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func CursorRight: Move the cursor to the right.
--  @param steps: steps as number
--  @param Xonly: as boolean
-- @returns
--   @param nil
function CursorRight(self, steps, Xonly)
	--Move the Cursor to the right
	self.pntSelected.x = self.pntSelected.x + steps
	self.pntCursor.x = self.pntCursor.x + steps
	self.pntCursor.x = math.min( self.pntCursor.x, MaxTextWidth)
	if self.pntSelected.x > self.textWidth then
		self.pntSelected.x = 1
		self.pntCursor.x = 1
		self.pntDraw.x = 0
		self.hBar_Location = hBar_Location + self.hBar_StepSize * self.pntDraw.x
		if Xonly ~= true then self:CursorDown(1, true) end
	end
	if self.textWidth > MaxTextWidth and self.pntCursor.x == MaxTextWidth and self.pntSelected.x <= self.textWidth then
		self.pntDraw.x = self.pntSelected.x - MaxTextWidth
		self.hBar_Location = hBar_Location + self.hBar_StepSize * self.pntDraw.x
	end
	--select the text.
	if self.selecting ~= 0 then
		updateSelect(self)
	end
	steps = nil
	Xonly = nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func select:
--  @param bnlQuit: as boolean.
-- @returns
--   @param nil
function select(self, bnlQuit)
	local bnlQuit = bnlQuit or false
	if self.selecting ~= 0 or bnlQuit then
		--Quit selecting the text.
		if self.selecting > 0 then
			self.selectedText.endline.start = self.pntSelected.x
			self.selectedText.endline.line = self.pntSelected.y
		elseif self.selecting < 0 then
			self.selectedText.startline.start = self.pntSelected.x
			self.selectedText.startline.line = self.pntSelected.y
		end
		self.selecting = 0
		self.selected = true
		--check the select text.
		if self.selectedText.startline.line == self.selectedText.endline.line and
		   self.selectedText.startline.start == self.selectedText.endline.start then
			--deselect the selected text.
			self.selecting = 0
			self.selected = false
			self.selectedText.startline.line = 0
			self.selectedText.startline.start = 0
			self.selectedText.startline.selected = false
			self.selectedText.endline.line = 0
			self.selectedText.endline.start = 0
			self.selectedText.endline.selected = false
		end
	elseif self.selected then
		--deselect the selected text.
		self.selecting = 0
		self.selected = false
		self.selectedText.startline.line = 0
		self.selectedText.startline.start = 0
		self.selectedText.startline.selected = false
		self.selectedText.endline.line = 0
		self.selectedText.endline.start = 0
		self.selectedText.endline.selected = false
	else
		--Start selecting the text.
		self.selecting = 1
		self.selectedText.startline.start = self.pntSelected.x
		self.selectedText.startline.line = self.pntSelected.y
		self.selectedText.startline.selected = true
		self.selectedText.endline.start = self.pntSelected.x
		self.selectedText.endline.line = self.pntSelected.y
		self.selectedText.endline.selected = true
	end
	bnlQuit = nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func delete:
--  @param lenght: The size of the part to remove.
-- @returns
--   @param nil
function delete(self, lenght)
	local lenght = lenght or 1
	local str = ""
	local len = 0
	--delete selected text.
	if self.selecting ~= 0 then self:select(true) end
	if self.selected then
		--Move the cursor.
		self:CursorLeft(self.pntSelected.x-1, true)
		local intTmp = self.selectedText.endline.line
		if intTmp < self.pntSelected.y then
			self:CursorUp(self.pntSelected.y-intTmp)
		elseif intTmp > self.pntSelected.y then
			self:CursorDown(intTmp-self.pntSelected.y)
		end
		self:CursorRight(self.selectedText.endline.start-1, true)
		--Calculate the lenght of the text.
		lenght = 0
		for n = self.selectedText.startline.line, self.selectedText.endline.line, 1 do
			lenght = lenght + string.len(self.text[n]) + 1
		end
		lenght = lenght - self.selectedText.startline.start - 1
		lenght = lenght - string.len(self.text[self.selectedText.endline.line]) + self.selectedText.endline.start
		--deselect the selected text.
		self.selecting = 0
		self.selected = false
		self.selectedText.startline.line = 0
		self.selectedText.startline.start = 0
		self.selectedText.startline.selected = false
		self.selectedText.endline.line = 0
		self.selectedText.endline.start = 0
		self.selectedText.endline.selected = false
	end
	while lenght > 0 do
		str = self.text[self.pntSelected.y]
		self.text_green[self.pntSelected.y] = false
		self.textWidth = string.len(str) + 1
		if self.pntSelected.x > lenght then
			str = string.remove(str, self.pntSelected.x - lenght, lenght)
			self.text[self.pntSelected.y] = str
			if self.textWidth == self.textMaxWidth then
				self.textMaxWidth = getTextWidth(self.text) + 1
			end
			self.textWidth = string.len(str) + 1
			self:SetvBar(self.textLines)
			self:SethBar(self.textMaxWidth)
			--Move the Cursor Left
			self:CursorLeft(lenght, false)

			if self.textWidth >= MaxTextWidth and self.pntDraw.x > 0 then
				lenght = math.min(lenght, MaxTextWidth-1)
				self.pntDraw.x = self.pntDraw.x - lenght
				if self.pntDraw.x < 0 then
					lenght = self.pntDraw.x + lenght
					self.pntDraw.x = 0
				end
				self.hBar_Location = hBar_Location + self.hBar_StepSize * self.pntDraw.x
				self.pntCursor.x = math.min(self.pntCursor.x + lenght, MaxTextWidth)
			elseif self.textWidth < MaxTextWidth and self.pntDraw.x ~= 0 then
				self.pntDraw.x = 0
				self.hBar_Location = hBar_Location + self.hBar_StepSize * self.pntDraw.x
				self.pntCursor.x = self.pntSelected.x
			end

			lenght = 0
		else
			len = self.pntSelected.x
			if self.textWidth == len then
				str = ""
			else
				str = string.remove(str, self.pntSelected.x - (len-1), (len-1))
			end
			self.text[self.pntSelected.y] = str
			if self.textWidth == self.textMaxWidth then
				self.textMaxWidth = getTextWidth(self.text) + 1
			end
			self.textWidth = string.len(str) + 1
			self:SetvBar(self.textLines)
			self:SethBar(self.textMaxWidth)
			--Move the Cursor Left
			self:CursorLeft((len-1), false)
			self:deleteLine()
			lenght = lenght - len
		end
	end
	str = nil
	len = nil
	if collectgarbage("step") then collectgarbage("collect") end
	self.saved = false
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func deleteLine: delete a line.
--  @param : takes no arguments
-- @returns
--   @param nil
function deleteLine(self)
	if self.pntSelected.y > 1 then
		local str = self.text[self.pntSelected.y]
		table.remove(self.text, self.pntSelected.y)
		table.remove(self.text_green, self.pntSelected.y)
		self.text_green[self.pntSelected.y-1] = false
		self.textLines = self.textLines - 1
		self.text[self.pntSelected.y-1] = string.format("%s%s", self.text[self.pntSelected.y-1], str)
		self.textWidth = string.len(self.text[self.pntSelected.y-1]) + 1
		self.textMaxWidth = math.max(self.textMaxWidth, self.textWidth)
		self:SetvBar(self.textLines)
		self:SethBar(self.textMaxWidth)
		--Move the Cursor up
		self:CursorUp(1)
		self:CursorRight(self.textWidth - string.len(str) - 1, true)
		str = nil

		if self.textLines > MaxTextLines and self.pntCursor.y ~= MaxTextLines and self.pntSelected.y >= MaxTextLines then
			self.pntDraw.y = self.pntDraw.y - 1
			self.pntCursor.y = self.pntCursor.y + 1
		elseif self.textLines <= MaxTextLines and self.pntDraw.y ~= 0 then
			self.pntDraw.y = 0
			self.pntCursor.y = self.pntSelected.y
		end

		if collectgarbage("step") then collectgarbage("collect") end
	end
	self.saved = false
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func newLine: Create a new line.
--  @param : takes no arguments
-- @returns
--   @param nil
function newLine(self)
	--delete selected text.
	if self.selecting ~= 0 then self:select(true) end
	if self.selected then self:delete(1) end
	local str = self.text[self.pntSelected.y]
	table.insert(self.text, self.pntSelected.y, "")
	table.insert(self.text_green, self.pntSelected.y, 0)
	self.text_green[self.pntSelected.y] = false
	self.text_green[self.pntSelected.y + 1] = false
	self.textLines = self.textLines + 1
	self.text[self.pntSelected.y] = string.sub(str, 1, self.pntSelected.x - 1) or ""
	self.text[self.pntSelected.y + 1] = string.sub(str, self.pntSelected.x, -1) or ""
	self.textWidth = string.len(self.text[self.pntSelected.y]) + 1
	self.textMaxWidth = getTextWidth(self.text) + 1
	self:SetvBar(self.textLines)
	self:SethBar(self.textMaxWidth)
	--Move the Cursor down
	self:CursorRight(self.textWidth, false)
	str = nil
	if collectgarbage("step") then collectgarbage("collect") end
	self.saved = false
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func write: Write text to the file.
--  @param text: text as string or table
--  @param overwrite: as boolean
-- @returns
--   @param nil
function write(self, text, overwrite)
	--delete selected text.
	if self.selecting ~= 0 then self:select(true) end
	if self.selected then self:delete(1) end
	local str = self.text[self.pntSelected.y]
	str = string.insert(str ,self.pntSelected.x, text, overwrite)
	self.text_green[self.pntSelected.y] = false
	self.text[self.pntSelected.y] = str
	self.textWidth = string.len(str) + 1
	self.textMaxWidth = math.max(self.textMaxWidth, self.textWidth)
	self:SethBar(self.textMaxWidth)
	self:CursorRight(string.len(text), false)
	str = nil
	if collectgarbage("step") then collectgarbage("collect") end
	self.saved = false
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func free: free the object
--  @param : takes no arguments
-- @returns
--   @param nil
function free(self)
	--Free the data
	self.FileName = nil
	self.Path = nil
	self.FullFileName = nil
	self.saved = nil
	self.loaded = nil
	self.pntDraw = nil
	self.pntSelected = nil
	self.pntCursor = nil
	self.text = nil
	self.text_green = nil
	self.textLines = nil
	self.textWidth = nil
	self.textMaxWidth = nil
	self.selecting = nil
	self.selected = nil
	self.selectedText = nil
	self.vBar_Size = nil
	self.vBar_StepSize = nil
	self.vBar_Location = nil
	self.hBar_Size = nil
	self.hBar_StepSize = nil
	self.hBar_Location = nil

	--free function handels
	self["load"] = nil
	self["save"] = nil
	self["saveAs"] = nil
	self["SetvBar"] = nil
	self["SethBar"] = nil
	self["CursorUp"] = nil
	self["CursorDown"] = nil
	self["CursorLeft"] = nil
	self["CursorRight"] = nil
	self["select"] = nil
	self["delete"] = nil
	self["deleteLine"] = nil
	self["newLine"] = nil
	self["write"] = nil
	self["free"] = nil

	return nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- @func dispose: remove the module
--  @param : takes no arguments
-- @returns
--   @param nil
function dispose()
	Editer.new = nil
	Editer.load = nil
	Editer.save = nil
	Editer.saveAs = nil
	Editer.SetvBar = nil
	Editer.SethBar = nil
	Editer.CursorUp = nil
	Editer.CursorDown = nil
	Editer.CursorLeft = nil
	Editer.CursorRight = nil
	Editer.select = nil
	Editer.delete = nil
	Editer.deleteLine = nil
	Editer.newLine = nil
	Editer.write = nil
	Editer.free = nil
	Editer.dispose = nil

	--Dispose module settings.
	Editer.vBar_Size = nil
	Editer.vBar_MinSize = nil
	Editer.vBar_Location = nil
	Editer.hBar_Size = nil
	Editer.hBar_MinSize = nil
	Editer.hBar_Location = nil
	Editer.MaxTextLines = nil
	Editer.MaxTextWidth = nil
	Editer.chrTab = nil
	Editer.chrMusic = nil
	Editer.chrSpace = nil
	Editer.chrMin = nil

	--Dispose local functions.
	getTextWidth = nil
	updateSelect = nil
	saveFile = nil

	Editer._PACKAGE = nil
	Editer._M = nil
	Editer._NAME = nil
	Editer._VERSION = nil
	Editer = nil
	package.loaded["Editer"] = nil
	return nil
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
--End Editer˜ module.
-----------------------------------------------------------------------------------------------------
