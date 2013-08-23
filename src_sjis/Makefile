#
#
#

CC = cl.exe
CFLAGS = /nologo /D__WIN32 /O2 /c /Zp /Tp
LD = link.exe
LDFLAGS = 

P_OBJS = alloc.obj common.obj data3d.obj ele_para.obj environment.obj e_field.obj e_field_max.obj e_pml.obj field.obj h_field.obj h_pml.obj input_dipole.obj input_model.obj logger.obj main_p.obj plane_oscillator.obj setting.obj utils.obj
D_OBJS = alloc.obj common.obj data3d.obj dipole_input_model.obj dipole_oscillator.obj ele_para.obj environment.obj e_field.obj e_field_max.obj e_pml.obj field.obj h_field.obj h_pml.obj input_dipole.obj logger.obj main_d.obj setting.obj utils.obj

.c.obj :
	$(CC) $(CFLAGS) $<

fdtd_p : $(P_OBJS)
	$(LD) $(P_OBJS) /OUT:$(*).exe
	
fdtd_d : $(D_OBJS)
	$(LD) $(D_OBJS) /OUT:$(*).exe

all : fdtd_p fdtd_d

clean :
	del *.obj
	del *.exe

