local r = (Gu.PI/180.0)
local red = Color.new(255, 0, 0)
local green = Color.new(0, 255, 0)
local blue = Color.new(0, 0, 255)
local black = Color.new(0, 0, 0)
local white = Color.new(255, 255, 255)
local gray = Color.new(128, 128, 128)
local cyan = Color.new(100, 255, 255)
local clrFog = Color.new(160, 160, 255)

local val = 0

-- Create the logo.
local fnt = Font.load(System.Directory.eboot.."/System/Font/trebuc.ttf", false)
local logo = Texture.new(64, 64, true, GL.PSM_4444)
logo:clear(gray)
logo:fillRect(63, 36, 1, 2, black)
Font.set(fnt)
Font.setSize(13)
Font.setColor(black)
Font.render(logo, 20, 20, "Lua")
Font.render(logo, 10, 40, "Player")
Font.reset()
fnt = nil
collectgarbage("collect")


-- Build the cube vertices list using the new vertices userdata function.
-- This function builds a vertices list from a lua table that can be used be
-- the Gum.verticesDraw([vertices]) for rendering. This is faster then the
-- Gum.drawArray(prim, vtype, tbl) fuction. because that function convert a lua table
-- to a vertices list during rendering time.
local u0, v0, uI,vI = 0, 0, 1.5, 1.5
local xl, yl, zl, xh, yh, zh = -1, -1, -1,  1,  1,  1
local cube = Gum.verticesNew(
	Gum.PRIM_TRIANGLES,
	Gum.VERT_TEXTURE_BITS + Gum.VERT_COLOR_BITS + Gum.VERT_VERTEX_BITS + Gum.VERT_TRANSFORM_3D,
	{
		{u0, v0, red,   xl, yl, zh},  {uI, v0, red,   xl, yh, zh},  {uI, vI, red,   xh, yh, zh},  -- 0, 4, 5
		{u0, v0, red,   xl, yl, zh},  {uI, vI, red,   xh, yh, zh},  {u0, vI, red,   xh, yl, zh},  -- 0, 5, 1
		{u0, v0, red,   xl, yl, zl},  {uI, v0, red,   xh, yl, zl},  {uI, vI, red,   xh, yh, zl},  -- 3, 2, 6
		{u0, v0, red,   xl, yl, zl},  {uI, vI, red,   xh, yh, zl},  {u0, vI, red,   xl, yh, zl},  -- 3, 6, 7
		{u0, v0, green, xh, yl, zl},  {uI, v0, green, xh, yl, zh},  {uI, vI, green, xh, yh, zh},  -- 0, 3, 7
		{u0, v0, green, xh, yl, zl},  {uI, vI, green, xh, yh, zh},  {u0, vI, green, xh, yh, zl},  -- 0, 7, 4
		{u0, v0, green, xl, yl, zl},  {uI, v0, green, xl, yh, zl},  {uI, vI, green, xl, yh, zh},  -- 0, 3, 7
		{u0, v0, green, xl, yl, zl},  {uI, vI, green, xl, yh, zh},  {u0, vI, green, xl, yl, zh},  -- 0, 7, 4
		{u0, v0, blue,  xl, yh, zl},  {uI, v0, blue,  xh, yh, zl},  {uI, vI, blue,  xh, yh, zh},  -- 0, 1, 2
		{u0, v0, blue,  xl, yh, zl},  {uI, vI, blue,  xh, yh, zh},  {u0, vI, blue,  xl, yh, zh},  -- 0 ,2, 3
		{u0, v0, blue,  xl, yl, zl},  {uI, v0, blue,  xl, yl, zh},  {uI, vI, blue,  xh, yl, zh},  -- 4, 7, 6
		{u0, v0, blue,  xl, yl, zl},  {uI, vI, blue,  xh, yl, zh},  {u0, vI, blue,  xh, yl, zl},  -- 4, 6, 5
	}
)

-- Test the Gum.verticesPush function.
--cube = Gum.verticesNew(Gum.verticesNew(cube:push()):push())

-- Test Disable 3D vertex function.
--cube:type3D(false)
-- Test Enable 3D vertex function.
--cube:type3D(true)

-- Test the __tostring() function.
--Utility.msgShow("cube: "..tostring(cube))

-- Test 'get' 'set' vertex function.
--local t = {cube:get(0)}
--t[3] = GL.clr.yellow
--cube:set(0, table.unpack(t))
--t = nil

-- load the plane vertices list from a file.
local plane = Gum.verticesLoad("plane.vrt") or false
-- Build the plane vertices list if the object hasn't been loaded.
if not plane then
	plane = Gum.verticesNew(
		Gum.PRIM_TRIANGLES,
		Gum.VERT_COLOR_BITS + Gum.VERT_VERTEX_BITS + Gum.VERT_TRANSFORM_3D,
		{
			{blue, -8, -3,  0}, {cyan,  8,  3,  0}, {blue,  8, -3,  0},
			{cyan,  8,  3,  0}, {blue, -8, -3,  0}, {cyan, -8,  3,  0},
		}
	)
	-- Save the vertices list to a file.
	plane:save("plane.vrt")
	--Utility.msgShow("file has been saved")
end

while System.running() do
	Gu.start3d()

	-- Setup GU.
	Gu.depthRange(41250, 8340)
	Gu.depthFunc(Gu.FNC_GEQUAL)
	Gu.frontFace(Gu.FF_CW)
	Gu.enable(Gu.STATE_DEPTH_TEST)
	Gu.enable(Gu.STATE_ALPHA_TEST)
	Gu.enable(Gu.STATE_CULL_FACE)
	Gu.enable(Gu.STATE_CLIP_PLANES)

	-- clear screen
	Gu.clearDepth(0);
	Gu.clear(Gu.CLBF_COLOR_BUFFER_BIT + Gu.CLBF_DEPTH_BUFFER_BIT)

	-- setup Fog.
	Gu.fog(3.10, 5, clrFog)
	Gu.enable(Gu.STATE_FOG)

	-- setup projection and view matrices
	Gum.matrixMode(Gum.MTRX_PROJECTION)
	Gum.loadIdentity()
	Gum.perspective(75, 16/9, 0.5, 1000)

	Gum.matrixMode(Gum.MTRX_VIEW)
	Gum.loadIdentity()

	-- setup matrix for triangle
	Gum.matrixMode(Gum.MTRX_MODEL)
	Gum.loadIdentity()
	Gum.translate(0, 0, -3);

	-- draw triangle without texture
	Gu.disable(Gu.STATE_TEXTURE_2D)
	Gum.verticesDraw(plane)

	-- setup texture
	Gu.enable(Gu.STATE_BLEND)
	Gu.blendFunc(Gu.OP_ADD, Gu.BLD_SRC_ALPHA, Gu.BLD_ONE_MINUS_SRC_ALPHA, 0, 0)
	Gu.enable(Gu.STATE_TEXTURE_2D)
	Gu.texImage(logo)
	Gu.texFunc(Gu.TFX_MODULATE, Gu.TCC_RGBA)
	Gu.texEnvColor(white)
	Gu.texFilter(Gu.TFL_LINEAR, Gu.TFL_LINEAR)
	Gu.texScale(1, 1)
	Gu.texOffset(0, 0)
	Gu.ambientColor(white)

	-- setup matrix for cube
	Gum.matrixMode(Gum.MTRX_MODEL)
	Gum.loadIdentity()
	Gum.translate(0, 0, -3.5)
	Gum.rotateXYZ(val * 0.79 * r, val * 0.98 * r, val * 1.32 * r)

	-- draw cube
	Gum.verticesDraw(cube)

	Gu.finish3d()
	screen.flip()

	val = val + 1

	Controls.read()
	if Controls.press(8200) then --start or circle button pressed
		--Close the Application
		break
	end

end
