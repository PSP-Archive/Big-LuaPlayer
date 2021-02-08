# Makefile
TARGET = Player

# Include the Luaplayer file.
OBJS_PLAYER = luaplayer.o
# Include the application objects.
OBJS_RESOURCES =
# Include psp audio files.
OBJS_PSP_AUDIO =
#                Library/Audio/pspaalib.o \
#                Library/Audio/pspaalibeffects.o \
#			     Library/Audio/pspaalibscemp3.o \
#                Library/Audio/pspaalibwav.o \
#			     Library/Audio/pspaalibat3.o \
#			     Library/Audio/pspaalibogg.o
# Include psp sound files.
OBJS_PSP_SOUND = Library/Sound/pspsoundlib.o \
				 Library/Sound/soundlib.o
# Include psp support files.
OBJS_PSP = Common/callbacks.o \
		   Library/glib2d.o \
		   Library/glibplt.o \
		   Library/glibtex.o \
		   Library/glibf.o \
		   Library/utilitylib.o \
		   Library/pspmodlib.o \
		   Library/pspusblib.o \
		   Library/adhoc.o \
		   Library/pspsiolib.o \
		   Library/pspirdalib.o
# Include the Lua modules.
OBJS_LUA = Source/Lua_Library.o \
           Source/Lua_DebugScreen.o \
		   Source/Lua_Controls.o \
		   Source/Lua_System.o \
		   Source/Lua_Utility.o \
		   Source/Lua_Memory.o \
		   Source/Lua_usb.o \
		   Source/Lua_Power.o \
		   Source/Lua_Timer.o \
	       Source/Lua_Color.o \
		   Source/Lua_Palette.o \
		   Source/Lua_Font.o \
		   Source/Lua_BmpFont.o \
		   Source/Lua_Texture.o \
		   Source/Lua_GL.o \
		   Source/Lua_Sound.o
#		   Source/Lua_Aalib.o
# Include psp main application file.
OBJS_MAIN = main.o
# include all objects.
OBJS = $(OBJS_PLAYER) $(OBJS_RESOURCES) $(OBJS_PSP_AUDIO) $(OBJS_PSP_SOUND) $(OBJS_PSP) $(OBJS_LUA) $(OBJS_MAIN)

# Set the flag info. Common Include Library Lua Source
INCDIR = $(PSPSDK)/../include/freetype2 Library Library/Aalib Library/Sound Source
CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

# Add the Lua Library's.
# LIBS_LUA = -llua -llualib -lm
LIBS_LUA = Lua/liblua.a
# Add the Standeart C++ Library's.
LIBS_C++ = -lstdc++
# Add the PSP Library's.
LIBS_PSP = -lpsppower \
           -lpspctrl_driver \
		   -lpsputility \
		   -lpspgum -lpspgu \
		   -lpspusb -lpspusbstor \
		   -lpspvalloc \
		   -lpspaudio \
		   -lpspwlan -lpspnet_adhoc -lpspnet_adhocctl \
		   -lpspsircs
#		   -lvorbisidec -lpspmp3 -lpspatrac3 \
#		   -lpspvram \
# Add the freeType Library's.
LIBS_FT2 = -lfreetype
# Add the Image Library's.
LIBS_IMAGE = -lpng \
             -ljpeg
# Add the Zip Library's.
LIBS_ZIB = -lz
# Add the Math Library's.
#LIBS_MATH = -lpspmath
LIBS_MATH = -lm
# Add the Library's.
LIBDIR =
LIBS = $(LIBS_LUA) $(LIBS_C++) $(LIBS_PSP) $(LIBS_FT2) $(LIBS_IMAGE) $(LIBS_ZIB) $(LIBS_MATH)
LDFLAGS =

# Set The info for the Eboot.pbp file.
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Big LuaPlayer
PSP_EBOOT_SFO =
PSP_EBOOT_ICON = Resources/ICON0.PNG
PSP_EBOOT_ICON1 =
PSP_EBOOT_UNKPNG =
PSP_EBOOT_PIC1 =
PSP_EBOOT_SND0 =
PSP_EBOOT_PSAR =
PSP_EBOOT = EBOOT.PBP

PSP_FW_VERSION = 150

# build the psp eboot.
PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
