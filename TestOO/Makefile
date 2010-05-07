SOURCESDK = /home/andy/SourceMod/Mod2/hl2sdk-ob

ARCH = i486
INCLUDES = -I../Common/includes -I./includes -I../LuaOO/includes -I$(SOURCESDK)/public

VALVE_CFLAGS = -mtune=i686 -march=pentium3 -mmmx -m32
VALVE_DEFINES = -D_finite=finite -Dstricmp=strcasecmp -D_stricmp=strcasecmp \
	-D_strnicmp=strncasecmp -Dstrnicmp=strncasecmp -D_vsnprintf=vsnprintf -D_alloca=alloca -Dstrcmpi=strcasecmp
VALVE_LIBS = $(SOURCESDK)/lib/linux/tier1_$(ARCH).a $(SOURCESDK)/lib/linux/tier3_$(ARCH).a

CFLAGS = -Wall $(INCLUDES) -D_LINUX -DLINUX $(VALVE_DEFINES) $(VALVE_CFLAGS)

MODE = Debug
LIBDIR = output/$(MODE)/$(ARCH)
OBJDIR = $(LIBDIR)/obj
SOURCEDIR = source

OBJECTS = $(OBJDIR)/CLASS_Test.obj \
			$(OBJDIR)/GM_Module.obj
			
LIBS = $(VALVE_LIBS) ../LuaOO/$(LIBDIR)/luaoo.a -lstdc++
			
CC = gcc
MKDIR = mkdir -p
AR = ar
RM = rm -f
RMDIR = rmdir -p

$(LIBDIR)/gm_testoo_$(ARCH).so: $(OBJECTS)
	$(MKDIR) $(LIBDIR)
	$(CC) -shared -o $@ $< $(CFLAGS) $(LIBS)
	
$(OBJDIR)/%.obj : $(SOURCEDIR)/%.cpp
	$(MKDIR) $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS)
	
clean:
	$(RM)     $(OBJECTS)
	$(RM)     $(LIBDIR)/threadoo.a
	$(RMDIR)  $(OBJDIR)