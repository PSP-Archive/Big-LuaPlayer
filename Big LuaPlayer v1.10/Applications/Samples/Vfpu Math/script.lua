-----------------------------------------------------------------------------------------------------
-- Big LuaPlayer: vfpu math test˜ v 1.00 year (C) company
--
-- Description:
--     Test all lua math and vfpu math functions and calculate the speed.
--
-----------------------------------------------------------------------------------------------------

-- Set the Cpu Clock Frequencies.
Power.cpuSpeed(333)

-- load the screen module.
require "screen"

-- Define the application data.
local App_Running = true
local App_frameTime = 1000/60 -- (~16.67 ms)
local App_Timer = Timer.new()

-- Define the control enumerations.
local CTRL_ENUM_EXIT = Controls.START + Controls.CIRCLE -- (8 + 8192)

-- Define the colors.
local white = Color.new(255, 255, 255)
local black = Color.new(0, 0, 0)

-- Load the bmpFont. 9 pixels in height,
local fntMonaco8 = BmpFont.load(System.Directory.eboot.."/System/Font/monaco.ttf", 9)

-- Load the vfpu math library.
System.vfpuMath(true)

--Store the vfpu math library in a differnt table.
local vfpuMath = math

-- Restore lua math library.
System.vfpuMath(false)

math.op = function(a,b) return (a + (a - (a * (a / b)))) end
vfpuMath.op = function(a,b) return vfpuMath.add(a,vfpuMath.sub(a,vfpuMath.mul(a,vfpuMath.div(a,b)))) end
local seed = os.clock()
math.randomseed(seed)
vfpuMath.randomseed(seed)

-- Define some values.
local timer = Timer.new()
local iterations = 10000
local indx = 0
local result =
{
	{"abs"       , 0, 0, false, 1,     2}, {"acos"      , 0, 0, false, 1,   0.1},
	{"asin"      , 0, 0, false, 1,   0.1}, {"atan2"     , 0, 0, false, 2, 1.337},
	{"atan"      , 0, 0, false, 1,   0.1}, {"ceil"      , 0, 0, false, 1, 5.251},
	{"cosh"      , 0, 0, false, 1,     2}, {"cos"       , 0, 0, false, 1,     2},
	{"deg"       , 0, 0, false, 1,     2}, {"exp"       , 0, 0, false, 1,     2},
	{"floor"     , 0, 0, false, 1,     2}, {"fmod"      , 0, 0, false, 2,     3},
	{"frexp"     , 0, 0, false, 1,     2}, {"ldexp"     , 0, 0, false, 2,     2},
	{"log"       , 0, 0, false, 2,     2}, {"max"       , 0, 0, false, 2,   256},
	{"min"       , 0, 0, false, 2,   256}, {"modf"      , 0, 0, false, 1,     2},
	{"pow"       , 0, 0, false, 2,     2}, {"rad"       , 0, 0, false, 1,     2},
	{"random"    , 0, 0, false, 1,     2}, {"sinh"      , 0, 0, false, 1,     2},
	{"sin"       , 0, 0, false, 1,     2}, {"sqrt"      , 0, 0, false, 1,     2},
	{"tanh"      , 0, 0, false, 1,     2}, {"tan"       , 0, 0, false, 1,     2},
	{"op"        , 0, 0, false, 2,     2},
}
local tblLen = table.getn(result)
-----------------------------------------------------------------------------------------------------
local function doMath()
	if indx >= tblLen then return end
	indx = indx + 1
	local fncA = math[result[indx][1]]
	local fncB = vfpuMath[result[indx][1]]
	if result[indx][5] == 1 then
		local b = result[indx][6] or error("no number")
		result[indx][2] = timer:update()/10
		for n = 1, iterations do fncA(b) end
		result[indx][2] = timer:delta()/10
		for n = 1, iterations do fncB(b) end
		result[indx][3] = timer:delta()/10
	elseif result[indx][5] == 2 then
		local b = result[indx][6] or error("no number")
		result[indx][2] = timer:update()/10
		for n = 1, iterations do fncA(b, b) end
		result[indx][2] = timer:delta()/10
		for n = 1, iterations do fncB(b, b) end
		result[indx][3] = timer:delta()/10
	end
	result[indx][4] = true
end
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------
-- Draw function.
local function drawFunc()

	-- Clear the screen.
	GL.Clear(black)

	-- Print some text using the print function.
	screen.print(0, 0, "Big LuaPlayer: vfpu math test", white)
	screen.print(0, 14, string.format("Hz: %d", 1000/(App_frameTime+0.001)), white)
	screen.print(0, 259, string.format("RunTime: %f", os.clock()), white)

	-- Print the results.
	for idx, tbl in ipairs(result) do
		if tbl[4] then
			fntMonaco8:print(70, 14+9*(idx-1), string.format("%s: ML t = %06.3fns, VFPU t = %06.3fns", tbl[1]..string.rep(" ",6-string.len(tbl[1])), tbl[2], tbl[3]), white)
		end
	end

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

	-- Do some math.
	doMath()

	-- Update the controls.
	Controls.readPeek()

	if Controls.press(CTRL_ENUM_EXIT) then -- start or circle button pressed.
		-- Break the main loop.
		App_Running = false
    end

	-- Store the frame time.
	App_frameTime = App_Timer:delta()
end
-----------------------------------------------------------------------------------------------------

-- Dispose Draw function.
drawFunc = nil

-- Dispose some values.
doMath = nil
tblLen = nil
result = nil
iterations = nil
timer = nil
seed = nil
math.op = nil
vfpuMath = nil

-- Dispose the bmpFont.
fntMonaco8 = nil

-- Dispose colors.
black = nil
white = nil

-- Dispose control enumerations.
CTRL_ENUM_EXIT = nil

-- Dispose application data.
App_Timer = nil
App_Running = nil

-- Quit the application after the main loop breaks.
--System.exit()
