#
#
#

CC = cl.exe
CFLAGS = /nologo /D__WIN32 /O2 /c /Zp /Tp
LD = link.exe
LDFLAGS = 

OBJS = main.obj mat_diffuse.obj mat_reflection.obj mat_transparent.obj obj_io.obj radiance.obj scene.obj

.c.obj :
	$(CC) $(CFLAGS) $<
	
rt2013 : $(OBJS)
	$(LD) $(D_OBJS) /OUT:$(*).exe

all : rt2013

clean :
	del *.obj
	del *.exe

