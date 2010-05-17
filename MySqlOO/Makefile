MYSQL_LIBS = `mysql_config --libs`
MYSQL_CFLAGS = `mysql_config --cflags`

ARCH = i486
INCLUDES = -I../Common/includes -I./includes -I../LuaOO/includes -I../ThreadOO/includes

CFLAGS = -Wall -DLINUX -DNDEBUG $(INCLUDES) $(MYSQL_CFLAGS)

MODE = Release
LIBDIR = output/$(MODE)/$(ARCH)
OBJDIR = $(LIBDIR)/obj
SOURCEDIR = source

OBJECTS = $(OBJDIR)/CLASS_Database.obj \
			$(OBJDIR)/CLASS_ConnectThread.obj \
			$(OBJDIR)/CLASS_DataRow.obj \
			$(OBJDIR)/CLASS_Query.obj \
			$(OBJDIR)/CLASS_QueryThread.obj \
			$(OBJDIR)/GM_Module.obj
					
LIBS = ../LuaOO/$(LIBDIR)/luaoo.a ../ThreadOO/$(LIBDIR)/threadoo.a $(MYSQL_LIBS) -lstdc++ -lpthread
			
CC = gcc
MKDIR = mkdir -p
AR = ar
RM = rm -f
RMDIR = rmdir -p

all: $(LIBDIR)/gmcl_mysqloo_$(ARCH).dll $(LIBDIR)/gmsv_mysqloo_$(ARCH).dll

$(LIBDIR)/gmcl_mysqloo_$(ARCH).dll: $(OBJECTS)
	$(MKDIR) $(LIBDIR)
	$(CC) -shared -o $@ $(CFLAGS) $(OBJECTS) $(LIBS)
	
$(LIBDIR)/gmsv_mysqloo_$(ARCH).dll: $(OBJECTS)
	$(MKDIR) $(LIBDIR)
	$(CC) -shared -o $@ $(CFLAGS) $(OBJECTS) $(LIBS)
	
$(OBJDIR)/%.obj : $(SOURCEDIR)/%.cpp
	$(MKDIR) $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS)
	
clean:
	$(RM)     $(OBJECTS)
	$(RM)     $(LIBDIR)/gmsv_mysqloo_$(ARCH).dll
	$(RM)     $(LIBDIR)/gmcl_mysqloo_$(ARCH).dll
	$(RMDIR)  $(OBJDIR)