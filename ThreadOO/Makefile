SOURCESDK = /home/andy/SourceMod/Mod2/hl2sdk-ob

ARCH = i486
INCLUDES = -I../Common/includes -I./includes -I$(SOURCESDK)/public

VALVE_CFLAGS = -mtune=i686 -march=pentium3 -mmmx -m32
VALVE_DEFINES = -D_finite=finite -Dstricmp=strcasecmp -D_stricmp=strcasecmp \
	-D_strnicmp=strncasecmp -Dstrnicmp=strncasecmp -D_vsnprintf=vsnprintf -D_alloca=alloca -Dstrcmpi=strcasecmp

CFLAGS = -Wall $(INCLUDES) -D_LINUX -DLINUX $(VALVE_DEFINES) $(VALVE_CFLAGS)

MODE = Debug
LIBDIR = output/$(MODE)/$(ARCH)
OBJDIR = $(LIBDIR)/obj
SOURCEDIR = source

OBJECTS = $(OBJDIR)/CLASS_Event.obj \
			$(OBJDIR)/CLASS_Mutex.obj \
			$(OBJDIR)/CLASS_MutexLocker.obj \
			$(OBJDIR)/CLASS_Thread.obj
			
CC = gcc
MKDIR = mkdir -p
AR = ar
RM = rm -f
RMDIR = rmdir -p

$(LIBDIR)/threadoo.a: $(OBJECTS)
	$(MKDIR) $(LIBDIR)
	$(AR) rcs $@ $(OBJECTS)
	
$(OBJDIR)/%.obj : $(SOURCEDIR)/%.cpp
	$(MKDIR) $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS)
	
clean:
	$(RM)     $(OBJECTS)
	$(RM)     $(LIBDIR)/threadoo.a
	$(RMDIR)  $(OBJDIR)