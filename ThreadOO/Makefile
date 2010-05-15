ARCH = i486
INCLUDES = -I./includes

CFLAGS = -Wall -DLINUX -DNDEBUG $(INCLUDES)

MODE = Debug
LIBDIR = output/$(MODE)/$(ARCH)
OBJDIR = $(LIBDIR)/obj
SOURCEDIR = source

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