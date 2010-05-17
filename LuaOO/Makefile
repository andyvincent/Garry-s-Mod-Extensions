ARCH = i486
INCLUDES = -I../Common/includes -I./includes

CFLAGS = -Wall -DLINUX -DNDEBUG $(INCLUDES)

MODE = Release
LIBDIR = output/$(MODE)/$(ARCH)
OBJDIR = $(LIBDIR)/obj
SOURCEDIR = source

OBJECTS = $(OBJDIR)/CLASS_LuaBoundFunction.obj \
			$(OBJDIR)/CLASS_LuaClassInfo.obj \
			$(OBJDIR)/CLASS_LuaObjectBase.obj \
			$(OBJDIR)/CLASS_LuaOO.obj
			
CC = gcc
MKDIR = mkdir -p
AR = ar
RM = rm -f
RMDIR = rmdir -p

$(LIBDIR)/luaoo.a: $(OBJECTS)
	$(MKDIR) $(LIBDIR)
	$(AR) rcs $@ $(OBJECTS)
	
$(OBJDIR)/%.obj : $(SOURCEDIR)/%.cpp
	$(MKDIR) $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS)
	
clean:
	$(RM)     $(OBJECTS)
	$(RM)     $(LIBDIR)/luaoo.a
	$(RMDIR)  $(OBJDIR)