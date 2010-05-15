ARCH = i486
INCLUDES = -I../Common/includes -I./includes -I../LuaOO/includes

CFLAGS = -Wall -DLINUX -DNDEBUG $(INCLUDES)

MODE = Debug
LIBDIR = output/$(MODE)/$(ARCH)
OBJDIR = $(LIBDIR)/obj
SOURCEDIR = source

OBJECTS = $(OBJDIR)/CLASS_Test.obj \
			$(OBJDIR)/GM_Module.obj
			
LIBS = ../LuaOO/$(LIBDIR)/luaoo.a -lstdc++
			
CC = gcc
MKDIR = mkdir -p
AR = ar
RM = rm -f
RMDIR = rmdir -p

all: $(LIBDIR)/gmcl_testoo_$(ARCH).dll $(LIBDIR)/gmsv_testoo_$(ARCH).dll

$(LIBDIR)/gmcl_testoo_$(ARCH).dll: $(OBJECTS)
	$(MKDIR) $(LIBDIR)
	$(CC) -shared -o $@ $(CFLAGS) $(OBJECTS) $(LIBS)
	
$(LIBDIR)/gmsv_testoo_$(ARCH).dll: $(OBJECTS)
	$(MKDIR) $(LIBDIR)
	$(CC) -shared -o $@ $(CFLAGS) $(OBJECTS) $(LIBS)
	
$(OBJDIR)/%.obj : $(SOURCEDIR)/%.cpp
	$(MKDIR) $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS)
	
clean:
	$(RM)     $(OBJECTS)
	$(RM)     $(LIBDIR)/gmsv_testoo_$(ARCH).dll
	$(RM)     $(LIBDIR)/gmcl_testoo_$(ARCH).dll
	$(RMDIR)  $(OBJDIR)