-- Create a texture, 480x272 in VRAM.
-- Render targets must reside in VRAM!
background = Texture.new(480, 272, true, GL.PSM_8888)
background:clear()

-- Set the texture as the render target (instead of the screen)
if not GL.SetDrawTarget(background) then
	error("Failed to set texture as target.")
end

-- Clear screen
GL.Clear()

local clockOfs = 150
local clockWidth = 100
local clockTextPosition = 85
local clockBigMarkWidth = 7
local clockSmallMarkWidth = 3
local x0 = clockOfs
local y0 = clockOfs - clockWidth
local pi = 4*math.atan(1)
local color = Color.new(0, 255, 0)
local x1,y1 = 0,0
local xv,yv = 0,0
for i=0,60 do
	x1 = math.sin(pi-i/60*2*pi) * clockWidth + clockOfs
	y1 = math.cos(pi-i/60*2*pi) * clockWidth + clockOfs
	screen.drawLine(x0, y0, x1, y1, color)
	xv = (x1 - clockOfs) / clockWidth
	yv = (y1 - clockOfs) / clockWidth
	if math.fmod(i, 5) == 0 then
		xt = xv * clockTextPosition + clockOfs
		yt = yv * clockTextPosition + clockOfs
		value = math.ceil(i / 5)
		if value == 0 then
			value = 12
		end
		screen.print(xt-4, yt-5, value, color)
		xv = xv * (clockWidth - clockBigMarkWidth) + clockOfs
		yv = yv * (clockWidth - clockBigMarkWidth) + clockOfs
		screen.drawLine(x1, y1, xv, yv, color)
	else
		xv = xv * (clockWidth - clockSmallMarkWidth) + clockOfs
		yv = yv * (clockWidth - clockSmallMarkWidth) + clockOfs
		screen.drawLine(x1, y1, xv, yv, color)
	end
	x0 = x1
	y0 = y1
end
screen.print(4, 4, "os.date: ", color)
screen.print(4, 19, "digital: ", color)

-- End drawing
GL.Finish()

-- Move the Texture to the Ram memory.
background:toRam()

-- Swizzle the Texture.
background:swizzle()

local time = 0
local dateString = 0
local dateFields = 0
local hour,min,sec = 0,0,0
local x,y = 0,0

while System.running() do
	screen.clear()
	screen.blit(0, 0, background)
	time = os.time()
	dateString = os.date("%c")
	screen.print(84, 4, dateString, color)
	dateFields = os.date("*t")
	hour = dateFields.hour
	if hour < 10 then
		hour = "0" .. hour
	end
	min = dateFields.min
	if min < 10 then
		min = "0" .. min
	end
	sec = dateFields.sec
	if sec < 10 then
		sec = "0" .. sec
	end
	screen.print(84, 19, hour .. ":" .. min .. ":" .. sec, color)
	hour = dateFields.hour
	if hour > 12 then
		hour = hour - 12
	end
	hour = hour + dateFields.min / 60 + dateFields.sec / 3600
	x = math.sin(pi-hour/12*2*pi) * clockWidth / 3 * 1 + clockOfs
	y = math.cos(pi-hour/12*2*pi) * clockWidth / 3 * 1 + clockOfs
	screen.drawLine(clockOfs, clockOfs, x, y, color)

	min = dateFields.min + dateFields.sec / 60
	x = math.sin(pi-min/60*2*pi) * clockWidth / 3 * 2 + clockOfs
	y = math.cos(pi-min/60*2*pi) * clockWidth / 3 * 2 + clockOfs
	screen.drawLine(clockOfs, clockOfs, x, y, color)

	x = math.sin(pi-dateFields.sec/60*2*pi) * clockWidth + clockOfs
	y = math.cos(pi-dateFields.sec/60*2*pi) * clockWidth + clockOfs
	screen.drawLine(clockOfs, clockOfs, x, y, color)

	screen.flip()

	Controls.read()

	if Controls.press(8200) then --start or circle button pressed
        --Close the Application
		break
	end

end

--Dispose Data
pad = nil
y = nil
x = nil
min = nil
hour = nil
sec = nil
dateFields = nil
dateString = nil
time = nil
x0 = nil
x1 = nil
y0 = nil
y1 = nil
xt = nil
xv = nil
clockTextPosition = nil
clockOfs = nil
yt = nil
yv = nil
value = nil
color = nil
pi = nil
clockWidth = nil
clockBigMarkWidth = nil
clockSmallMarkWidth = nil

background = nil --free the image.
