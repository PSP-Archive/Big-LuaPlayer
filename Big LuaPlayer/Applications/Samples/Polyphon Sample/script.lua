-----------------------------------------------------------------------------------------------------
-- Big LuaPlayer: Polyphon Sample˜ v 1.00 year (C) company
-----------------------------------------------------------------------------------------------------

-- Set the Cpu Clock Frequencies.
Power.cpuSpeed(150)

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

-----------------------------------------------------------------------------------------------------
local function _load(procces)
	--Draw text: Loading: 0%
	screen.clear(black)
	screen.print(0, 259, string.format("loading: %d%s", procces, "%"), white)
	screen.flip()
end
_load(0)
-----------------------------------------------------------------------------------------------------

-- Load the bmpFont.
local fntVerdana14 = BmpFont.load(System.Directory.eboot.."/System/Font/verdana.ttf", 14)


-- Load the polyphonic sounds.
local sndChannel0 = Sound.load("channel0.pol") or false
local sndChannel1 = Sound.load("channel1.pol") or false
_load(10)

-- Create the polyphonic sounds if the sounds can't be loaded.
local saveChannel0 = false
local saveChannel1 = false

if not sndChannel0 then
	-- Create the polyphonic sond object;
	sndChannel0 = Sound.new(Sound.TYPE_POL, 55)
	saveChannel0 = true
	_load(19)

	-- Fill the polyphonic  sond object.
	-- INFO: snd:buffer(pos, note, octave, duration, volume)
	sndChannel0:buffer(0,  Sound.NOTE_D, 4, 7*5512.5,  28627); sndChannel0:buffer(1,  Sound.NOTE_E, 4, 1*5512.5,  28627); sndChannel0:buffer(2,  Sound.NOTE_F, 4, 1*5512.5,  28627); sndChannel0:buffer(3,  Sound.NOTE_E, 4, 1*5512.5,  28627); sndChannel0:buffer(4,  Sound.NOTE_F, 4, 1*5512.5,  28627);
	sndChannel0:buffer(5,  Sound.NOTE_E, 4, 1*5512.5,  28627); sndChannel0:buffer(6,  Sound.NOTE_A, 3, 9*5512.5,  28627); sndChannel0:buffer(7,  Sound.NOTE_B, 3, 2*5512.5,  28627); sndChannel0:buffer(8,  Sound.NOTE_C, 4, 1*5512.5,  28627); sndChannel0:buffer(9,  Sound.NOTE_D, 4, 7*5512.5,  28627);
	sndChannel0:buffer(10, Sound.NOTE_E, 4, 1*5512.5,  28627); sndChannel0:buffer(11, Sound.NOTE_F, 4, 1*5512.5,  28627); sndChannel0:buffer(12, Sound.NOTE_E, 4, 1*5512.5,  28627); sndChannel0:buffer(13, Sound.NOTE_F, 4, 1*5512.5,  28627); sndChannel0:buffer(14, Sound.NOTE_E, 4, 1*5512.5,  28627);
	sndChannel0:buffer(15, Sound.NOTE_A, 3, 9*5512.5,  28627); sndChannel0:buffer(16, Sound.NOTE_G, 3, 3*5512.5,  28627); sndChannel0:buffer(17, Sound.NOTE_A, 3, 7*5512.5,  28627); sndChannel0:buffer(18, Sound.NOTE_B, 3, 1*5512.5,  28627); sndChannel0:buffer(19, Sound.NOTE_C, 4, 1*5512.5,  28627);
	sndChannel0:buffer(20, Sound.NOTE_B, 3, 1*5512.5,  28627); sndChannel0:buffer(21, Sound.NOTE_C, 4, 1*5512.5,  28627); sndChannel0:buffer(22, Sound.NOTE_B, 3, 1*5512.5,  28627); sndChannel0:buffer(23, Sound.NOTE_E, 3, 9*5512.5,  28627); sndChannel0:buffer(24, Sound.NOTE_F, 3, 2*5512.5,  28627);
	_load(28)

	sndChannel0:buffer(25, Sound.NOTE_G, 3, 1*5512.5,  28627); sndChannel0:buffer(26, Sound.NOTE_A, 3, 7*5512.5,  28627); sndChannel0:buffer(27, Sound.NOTE_B, 3, 1*5512.5,  28627); sndChannel0:buffer(28, Sound.NOTE_C, 4, 1*5512.5,  28627); sndChannel0:buffer(29, Sound.NOTE_B, 3, 1*5512.5,  28627);
	sndChannel0:buffer(30, Sound.NOTE_C, 4, 1*5512.5,  28627); sndChannel0:buffer(31, Sound.NOTE_B, 3, 1*5512.5,  28627); sndChannel0:buffer(32, Sound.NOTE_E, 3, 12*5512.5, 28627); sndChannel0:buffer(33, Sound.NOTE_D, 4, 9*5512.5,  28627); sndChannel0:buffer(34, Sound.NOTE_C, 4, 3*5512.5,  28627);
	sndChannel0:buffer(35, Sound.NOTE_B, 3, 6*5512.5,  28627); sndChannel0:buffer(36, Sound.NOTE_A, 3, 6*5512.5,  28627); sndChannel0:buffer(37, Sound.NOTE_D, 4, 7*5512.5,  28627); sndChannel0:buffer(38, Sound.NOTE_E, 4, 1*5512.5,  28627); sndChannel0:buffer(39, Sound.NOTE_E, 4, 1*5512.5,  28627);
	sndChannel0:buffer(40, Sound.NOTE_C, 4, 1*5512.5,  28627); sndChannel0:buffer(41, Sound.NOTE_B, 3, 1*5512.5,  28627); sndChannel0:buffer(42, Sound.NOTE_C, 4, 1*5512.5,  28627); sndChannel0:buffer(43, Sound.NOTE_B, 3, 6*5512.5,  28627); sndChannel0:buffer(44, Sound.NOTE_A, 3, 6*5512.5,  28627);
	sndChannel0:buffer(45, Sound.NOTE_C, 4, 9*5512.5,  28627); sndChannel0:buffer(46, Sound.NOTE_B, 3, 3*5512.5,  28627); sndChannel0:buffer(47, Sound.NOTE_E, 3, 12*5512.5, 28627); sndChannel0:buffer(48, Sound.NOTE_C, 4, 7*5512.5,  28627); sndChannel0:buffer(49, Sound.NOTE_D, 4, 1*5512.5,  28627);
	_load(36)

	sndChannel0:buffer(50, Sound.NOTE_C, 4, 1*5512.5,  28627); sndChannel0:buffer(51, Sound.NOTE_B, 3, 1*5512.5,  28627); sndChannel0:buffer(52, Sound.NOTE_C, 4, 1*5512.5,  28627); sndChannel0:buffer(53, Sound.NOTE_B, 3, 1*5512.5,  28627); sndChannel0:buffer(54, Sound.NOTE_E, 3, 12*5512.5, 28627);
	_load(45)

	-- Rewind the sound object
	sndChannel0:rewind()
end
_load(50)

if not sndChannel1 then
	-- Create the polyphonic sond object;
	sndChannel1 = Sound.new(Sound.TYPE_POL, 192)
	saveChannel1 = true
	_load(55)

	-- Fill the polyphonic  sond object.
	-- INFO: snd:buffer(pos, note, octave, duration, volume)
	sndChannel1:buffer(0,   Sound.NOTE_D, 1, 1*5512.5,  28627); sndChannel1:buffer(1,   Sound.NOTE_A, 1, 1*5512.5,  28627); sndChannel1:buffer(2,   Sound.NOTE_D, 2, 1*5512.5,  28627); sndChannel1:buffer(3,   Sound.NOTE_E, 2, 1*5512.5,  28627); sndChannel1:buffer(4,   Sound.NOTE_F, 2, 1*5512.5,  28627);
	sndChannel1:buffer(5,   Sound.NOTE_A, 2, 1*5512.5,  28627); sndChannel1:buffer(6,   Sound.NOTE_F, 2, 1*5512.5,  28627); sndChannel1:buffer(7,   Sound.NOTE_E, 2, 1*5512.5,  28627); sndChannel1:buffer(8,   Sound.NOTE_D, 2, 1*5512.5,  28627); sndChannel1:buffer(9,   Sound.NOTE_A, 1, 1*5512.5,  28627);
	sndChannel1:buffer(10,  Sound.NOTE_D, 1, 1*5512.5,  28627); sndChannel1:buffer(11,  Sound.NOTE_A, 1, 1*5512.5,  28627); sndChannel1:buffer(12,  Sound.NOTE_A, 0, 1*5512.5,  28627); sndChannel1:buffer(13,  Sound.NOTE_E, 1, 1*5512.5,  28627); sndChannel1:buffer(14,  Sound.NOTE_A, 1, 1*5512.5,  28627);
	sndChannel1:buffer(15,  Sound.NOTE_B, 1, 1*5512.5,  28627); sndChannel1:buffer(16,  Sound.NOTE_C, 2, 1*5512.5,  28627); sndChannel1:buffer(17,  Sound.NOTE_E, 2, 1*5512.5,  28627); sndChannel1:buffer(18,  Sound.NOTE_C, 2, 1*5512.5,  28627); sndChannel1:buffer(19,  Sound.NOTE_B, 1, 1*5512.5,  28627);
	sndChannel1:buffer(20,  Sound.NOTE_A, 1, 1*5512.5,  28627); sndChannel1:buffer(21,  Sound.NOTE_E, 1, 1*5512.5,  28627); sndChannel1:buffer(22,  Sound.NOTE_A, 0, 1*5512.5,  28627); sndChannel1:buffer(23,  Sound.NOTE_E, 1, 1*5512.5,  28627); sndChannel1:buffer(24,  Sound.NOTE_D, 1, 1*5512.5,  28627);
	_load(60)

	sndChannel1:buffer(25,  Sound.NOTE_A, 1, 1*5512.5,  28627); sndChannel1:buffer(26,  Sound.NOTE_D, 2, 1*5512.5,  28627); sndChannel1:buffer(27,  Sound.NOTE_E, 2, 1*5512.5,  28627); sndChannel1:buffer(28,  Sound.NOTE_F, 2, 1*5512.5,  28627); sndChannel1:buffer(29,  Sound.NOTE_A, 2, 1*5512.5,  28627);
	sndChannel1:buffer(30,  Sound.NOTE_F, 2, 1*5512.5,  28627); sndChannel1:buffer(31,  Sound.NOTE_E, 2, 1*5512.5,  28627); sndChannel1:buffer(32,  Sound.NOTE_D, 2, 1*5512.5,  28627); sndChannel1:buffer(33,  Sound.NOTE_A, 1, 1*5512.5,  28627); sndChannel1:buffer(34,  Sound.NOTE_D, 1, 1*5512.5,  28627);
	sndChannel1:buffer(35,  Sound.NOTE_A, 1, 1*5512.5,  28627); sndChannel1:buffer(36,  Sound.NOTE_A, 0, 1*5512.5,  28627); sndChannel1:buffer(37,  Sound.NOTE_E, 1, 1*5512.5,  28627); sndChannel1:buffer(38,  Sound.NOTE_A, 1, 1*5512.5,  28627); sndChannel1:buffer(39,  Sound.NOTE_B, 1, 1*5512.5,  28627);
    sndChannel1:buffer(40,  Sound.NOTE_C, 2, 1*5512.5,  28627); sndChannel1:buffer(41,  Sound.NOTE_E, 2, 1*5512.5,  28627); sndChannel1:buffer(42,  Sound.NOTE_C, 2, 1*5512.5,  28627); sndChannel1:buffer(43,  Sound.NOTE_B, 1, 1*5512.5,  28627); sndChannel1:buffer(44,  Sound.NOTE_A, 1, 1*5512.5,  28627);
	sndChannel1:buffer(45,  Sound.NOTE_E, 1, 1*5512.5,  28627); sndChannel1:buffer(46,  Sound.NOTE_A, 0, 1*5512.5,  28627); sndChannel1:buffer(47,  Sound.NOTE_E, 1, 1*5512.5,  28627); sndChannel1:buffer(48,  Sound.NOTE_F, 1, 1*5512.5,  28627); sndChannel1:buffer(49,  Sound.NOTE_C, 2, 1*5512.5,  28627);
	_load(65)

	sndChannel1:buffer(50,  Sound.NOTE_F, 2, 1*5512.5,  28627); sndChannel1:buffer(51,  Sound.NOTE_G, 2, 1*5512.5,  28627); sndChannel1:buffer(52,  Sound.NOTE_A, 2, 1*5512.5,  28627); sndChannel1:buffer(53,  Sound.NOTE_C, 3, 1*5512.5,  28627); sndChannel1:buffer(54,  Sound.NOTE_A, 2, 1*5512.5,  28627);
	sndChannel1:buffer(55,  Sound.NOTE_G, 2, 1*5512.5,  28627); sndChannel1:buffer(56,  Sound.NOTE_F, 2, 1*5512.5,  28627); sndChannel1:buffer(57,  Sound.NOTE_C, 2, 1*5512.5,  28627); sndChannel1:buffer(58,  Sound.NOTE_F, 1, 1*5512.5,  28627); sndChannel1:buffer(59,  Sound.NOTE_C, 2, 1*5512.5,  28627);
    sndChannel1:buffer(60,  Sound.NOTE_C, 1, 1*5512.5,  28627); sndChannel1:buffer(61,  Sound.NOTE_G, 1, 1*5512.5,  28627); sndChannel1:buffer(62,  Sound.NOTE_C, 2, 1*5512.5,  28627); sndChannel1:buffer(63,  Sound.NOTE_D, 2, 1*5512.5,  28627); sndChannel1:buffer(64,  Sound.NOTE_E, 2, 1*5512.5,  28627);
	sndChannel1:buffer(65,  Sound.NOTE_G, 2, 1*5512.5,  28627); sndChannel1:buffer(66,  Sound.NOTE_E, 2, 1*5512.5,  28627); sndChannel1:buffer(67,  Sound.NOTE_D, 2, 1*5512.5,  28627); sndChannel1:buffer(68,  Sound.NOTE_C, 2, 1*5512.5,  28627); sndChannel1:buffer(69,  Sound.NOTE_G, 1, 1*5512.5,  28627);
	sndChannel1:buffer(70,  Sound.NOTE_C, 1, 1*5512.5,  28627); sndChannel1:buffer(71,  Sound.NOTE_G, 1, 1*5512.5,  28627); sndChannel1:buffer(72,  Sound.NOTE_F, 1, 1*5512.5,  28627); sndChannel1:buffer(73,  Sound.NOTE_C, 2, 1*5512.5,  28627); sndChannel1:buffer(74,  Sound.NOTE_F, 2, 1*5512.5,  28627);
	_load(70)

	sndChannel1:buffer(75,  Sound.NOTE_G, 2, 1*5512.5,  28627); sndChannel1:buffer(76,  Sound.NOTE_A, 2, 1*5512.5,  28627); sndChannel1:buffer(77,  Sound.NOTE_C, 3, 1*5512.5,  28627); sndChannel1:buffer(78,  Sound.NOTE_A, 2, 1*5512.5,  28627); sndChannel1:buffer(79,  Sound.NOTE_G, 2, 1*5512.5,  28627);
	sndChannel1:buffer(80,  Sound.NOTE_F, 2, 1*5512.5,  28627); sndChannel1:buffer(81,  Sound.NOTE_C, 2, 1*5512.5,  28627); sndChannel1:buffer(82,  Sound.NOTE_F, 1, 1*5512.5,  28627); sndChannel1:buffer(83,  Sound.NOTE_C, 2, 1*5512.5,  28627); sndChannel1:buffer(84,  Sound.NOTE_C, 1, 1*5512.5,  28627);
	sndChannel1:buffer(85,  Sound.NOTE_G, 1, 1*5512.5,  28627); sndChannel1:buffer(86,  Sound.NOTE_C, 2, 1*5512.5,  28627); sndChannel1:buffer(87,  Sound.NOTE_D, 2, 1*5512.5,  28627); sndChannel1:buffer(88,  Sound.NOTE_E, 2, 1*5512.5,  28627); sndChannel1:buffer(89,  Sound.NOTE_G, 2, 1*5512.5,  28627);
	sndChannel1:buffer(90,  Sound.NOTE_E, 2, 1*5512.5,  28627); sndChannel1:buffer(91,  Sound.NOTE_D, 2, 1*5512.5,  28627); sndChannel1:buffer(92,  Sound.NOTE_C, 2, 1*5512.5,  28627); sndChannel1:buffer(93,  Sound.NOTE_G, 1, 1*5512.5,  28627); sndChannel1:buffer(94,  Sound.NOTE_C, 1, 1*5512.5,  28627);
	sndChannel1:buffer(95,  Sound.NOTE_G, 1, 1*5512.5,  28627); sndChannel1:buffer(96,  Sound.NOTE_D, 1, 1*5512.5,  28627); sndChannel1:buffer(97,  Sound.NOTE_A, 1, 1*5512.5,  28627); sndChannel1:buffer(98,  Sound.NOTE_D, 2, 1*5512.5,  28627); sndChannel1:buffer(99,  Sound.NOTE_E, 2, 1*5512.5,  28627);
	_load(75)

	sndChannel1:buffer(100, Sound.NOTE_F, 2, 1*5512.5,  28627); sndChannel1:buffer(101, Sound.NOTE_A, 2, 1*5512.5,  28627); sndChannel1:buffer(102, Sound.NOTE_F, 2, 1*5512.5,  28627); sndChannel1:buffer(103, Sound.NOTE_E, 2, 1*5512.5,  28627); sndChannel1:buffer(104, Sound.NOTE_D, 2, 1*5512.5,  28627);
	sndChannel1:buffer(105, Sound.NOTE_A, 1, 1*5512.5,  28627); sndChannel1:buffer(106, Sound.NOTE_D, 1, 1*5512.5,  28627); sndChannel1:buffer(107, Sound.NOTE_A, 1, 1*5512.5,  28627); sndChannel1:buffer(108, Sound.NOTE_A, 0, 1*5512.5,  28627); sndChannel1:buffer(109, Sound.NOTE_E, 1, 1*5512.5,  28627);
	sndChannel1:buffer(110, Sound.NOTE_A, 1, 1*5512.5,  28627); sndChannel1:buffer(111, Sound.NOTE_B, 1, 1*5512.5,  28627); sndChannel1:buffer(112, Sound.NOTE_C, 2, 1*5512.5,  28627); sndChannel1:buffer(113, Sound.NOTE_E, 2, 1*5512.5,  28627); sndChannel1:buffer(114, Sound.NOTE_C, 2, 1*5512.5,  28627);
	sndChannel1:buffer(115, Sound.NOTE_B, 1, 1*5512.5,  28627); sndChannel1:buffer(116, Sound.NOTE_A, 1, 1*5512.5,  28627); sndChannel1:buffer(117, Sound.NOTE_E, 1, 1*5512.5,  28627); sndChannel1:buffer(118, Sound.NOTE_A, 0, 1*5512.5,  28627); sndChannel1:buffer(119, Sound.NOTE_E, 1, 1*5512.5,  28627);
	sndChannel1:buffer(120, Sound.NOTE_D, 1, 1*5512.5,  28627);	sndChannel1:buffer(121, Sound.NOTE_A, 1, 1*5512.5,  28627); sndChannel1:buffer(122, Sound.NOTE_D, 2, 1*5512.5,  28627); sndChannel1:buffer(123, Sound.NOTE_E, 2, 1*5512.5,  28627); sndChannel1:buffer(124, Sound.NOTE_F, 2, 1*5512.5,  28627);
	_load(80)

	sndChannel1:buffer(125, Sound.NOTE_A, 2, 1*5512.5,  28627); sndChannel1:buffer(126, Sound.NOTE_F, 2, 1*5512.5,  28627); sndChannel1:buffer(127, Sound.NOTE_E, 2, 1*5512.5,  28627); sndChannel1:buffer(128, Sound.NOTE_D, 2, 1*5512.5,  28627); sndChannel1:buffer(129, Sound.NOTE_A, 1, 1*5512.5,  28627);
	sndChannel1:buffer(130, Sound.NOTE_D, 1, 1*5512.5,  28627); sndChannel1:buffer(131, Sound.NOTE_A, 1, 1*5512.5,  28627); sndChannel1:buffer(132, Sound.NOTE_A, 0, 1*5512.5,  28627); sndChannel1:buffer(133, Sound.NOTE_E, 1, 1*5512.5,  28627); sndChannel1:buffer(134, Sound.NOTE_A, 1, 1*5512.5,  28627);
	sndChannel1:buffer(135, Sound.NOTE_B, 1, 1*5512.5,  28627); sndChannel1:buffer(136, Sound.NOTE_C, 2, 1*5512.5,  28627); sndChannel1:buffer(137, Sound.NOTE_E, 2, 1*5512.5,  28627); sndChannel1:buffer(138, Sound.NOTE_C, 2, 1*5512.5,  28627); sndChannel1:buffer(139, Sound.NOTE_B, 1, 1*5512.5,  28627);
	sndChannel1:buffer(140, Sound.NOTE_A, 1, 1*5512.5,  28627);	sndChannel1:buffer(141, Sound.NOTE_E, 1, 1*5512.5,  28627); sndChannel1:buffer(142, Sound.NOTE_A, 0, 1*5512.5,  28627); sndChannel1:buffer(143, Sound.NOTE_E, 1, 1*5512.5,  28627); sndChannel1:buffer(144, Sound.NOTE_F, 1, 1*5512.5,  28627);
	sndChannel1:buffer(145, Sound.NOTE_C, 2, 1*5512.5,  28627); sndChannel1:buffer(146, Sound.NOTE_F, 2, 1*5512.5,  28627); sndChannel1:buffer(147, Sound.NOTE_G, 2, 1*5512.5,  28627); sndChannel1:buffer(148, Sound.NOTE_A, 2, 1*5512.5,  28627); sndChannel1:buffer(149, Sound.NOTE_C, 3, 1*5512.5,  28627);
	_load(85)

	sndChannel1:buffer(150, Sound.NOTE_A, 2, 1*5512.5,  28627); sndChannel1:buffer(151, Sound.NOTE_G, 2, 1*5512.5,  28627); sndChannel1:buffer(152, Sound.NOTE_F, 2, 1*5512.5,  28627); sndChannel1:buffer(153, Sound.NOTE_C, 2, 1*5512.5,  28627); sndChannel1:buffer(154, Sound.NOTE_F, 1, 1*5512.5,  28627);
	sndChannel1:buffer(155, Sound.NOTE_C, 2, 1*5512.5,  28627); sndChannel1:buffer(156, Sound.NOTE_C, 1, 1*5512.5,  28627); sndChannel1:buffer(157, Sound.NOTE_G, 1, 1*5512.5,  28627); sndChannel1:buffer(158, Sound.NOTE_C, 2, 1*5512.5,  28627); sndChannel1:buffer(159, Sound.NOTE_D, 2, 1*5512.5,  28627);
	sndChannel1:buffer(160, Sound.NOTE_E, 2, 1*5512.5,  28627); sndChannel1:buffer(161, Sound.NOTE_G, 2, 1*5512.5,  28627); sndChannel1:buffer(162, Sound.NOTE_E, 2, 1*5512.5,  28627); sndChannel1:buffer(163, Sound.NOTE_D, 2, 1*5512.5,  28627); sndChannel1:buffer(164, Sound.NOTE_C, 2, 1*5512.5,  28627);
	sndChannel1:buffer(165, Sound.NOTE_G, 1, 1*5512.5,  28627); sndChannel1:buffer(166, Sound.NOTE_C, 1, 1*5512.5,  28627); sndChannel1:buffer(167, Sound.NOTE_G, 1, 1*5512.5,  28627); sndChannel1:buffer(168, Sound.NOTE_F, 1, 1*5512.5,  28627); sndChannel1:buffer(169, Sound.NOTE_C, 2, 1*5512.5,  28627);
	sndChannel1:buffer(170, Sound.NOTE_F, 2, 1*5512.5,  28627);	sndChannel1:buffer(171, Sound.NOTE_G, 2, 1*5512.5,  28627); sndChannel1:buffer(172, Sound.NOTE_A, 2, 1*5512.5,  28627); sndChannel1:buffer(173, Sound.NOTE_C, 3, 1*5512.5,  28627); sndChannel1:buffer(174, Sound.NOTE_A, 2, 1*5512.5,  28627);
	_load(90)

	sndChannel1:buffer(175, Sound.NOTE_G, 2, 1*5512.5,  28627); sndChannel1:buffer(176, Sound.NOTE_F, 2, 1*5512.5,  28627); sndChannel1:buffer(177, Sound.NOTE_C, 2, 1*5512.5,  28627); sndChannel1:buffer(178, Sound.NOTE_F, 1, 1*5512.5,  28627); sndChannel1:buffer(179, Sound.NOTE_C, 2, 1*5512.5,  28627);
	sndChannel1:buffer(180, Sound.NOTE_C, 1, 1*5512.5,  28627); sndChannel1:buffer(181, Sound.NOTE_G, 1, 1*5512.5,  28627); sndChannel1:buffer(182, Sound.NOTE_C, 2, 1*5512.5,  28627); sndChannel1:buffer(183, Sound.NOTE_D, 2, 1*5512.5,  28627); sndChannel1:buffer(184, Sound.NOTE_E, 2, 1*5512.5,  28627);
	sndChannel1:buffer(185, Sound.NOTE_G, 2, 1*5512.5,  28627);	sndChannel1:buffer(186, Sound.NOTE_E, 2, 1*5512.5,  28627);	sndChannel1:buffer(187, Sound.NOTE_D, 2, 1*5512.5,  28627); sndChannel1:buffer(188, Sound.NOTE_C, 2, 1*5512.5,  28627); sndChannel1:buffer(189, Sound.NOTE_G, 1, 1*5512.5,  28627);
	sndChannel1:buffer(190, Sound.NOTE_C, 1, 1*5512.5,  28627); sndChannel1:buffer(191, Sound.NOTE_G, 1, 1*5512.5,  28627);
	_load(95)

	-- Rewind the sound object
	sndChannel1:rewind()
end
_load(100)
_load = nil

-- Initalize two sound channel.
Sound.init(2)

-- Set the sound amplification.
sndChannel0:amplification(0.75)
sndChannel1:amplification(0.75)

-- Start playing the polyphonic sounds.
sndChannel0:play(false, true)
sndChannel1:play(false, true)

-----------------------------------------------------------------------------------------------------
-- Draw function.
local function drawFunc()

	-- Clear the screen.
	GL.Clear(black)

	-- Print some text using the print function.
	screen.print(0, 0, "Big LuaPlayer: Polyphon Sample", white)
	screen.print(0, 14, string.format("Hz: %d", 1000/(App_frameTime+0.001)), white)
	screen.print(0, 259, string.format("RunTime: %f", os.clock()), white)

	fntVerdana14:print(20, 30, "Soundtrack of the movie:", white)
	fntVerdana14:print(30, 45, "'Le fabuleux destin d'Amelie Poulain'", white)
	fntVerdana14:print(30, 60, "by Yann Tiersen", white)

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

	if Controls.press(CTRL_ENUM_EXIT) then -- start button pressed.
		-- Break the main loop.
		App_Running = false
    end

	-- Store the frame time.
	App_frameTime = App_Timer:delta()
end
-----------------------------------------------------------------------------------------------------

-- Dispose Draw function.
drawFunc = nil

-- Stop playing the polyphonic sounds.
sndChannel0:stop()
sndChannel1:stop()

-- Save the sound objects on requist.
if saveChannel0 then sndChannel0:save("channel0.pol") end
if saveChannel1 then sndChannel1:save("channel1.pol") end

-- Terminate the sound channels.
Sound.term()

-- Dipose the polyphonic sounds.
sndChannel1 = nil
sndChannel1 = nil

-- Dispose the bmpFont.
fntVerdana14 = nil

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
