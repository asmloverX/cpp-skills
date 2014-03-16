OUT	= skillfwk.exe
CC	= cl -c -nologo
CFLAG4C	= -MDd -Od -W4 -Gm -Zi -Fd"vc.pdb" -D_CRT_SECURE_NO_DEPRECATE
CFLAG4CPP = $(CFLAG4C) -EHsc
LINK	= link -nologo
LINKFLAG = -DEBUG -PDB:"skillfwk.pdb" -manifest\
	-manifestfile:$(OUT).manifest -manifestuac:no kernel32.lib user32.lib
MT	= mt -nologo
CPCH	= -Yc"stdafx.h" -Fp"skillfwk.pch"
UPCH1	= -Yu"../stdafx.h" -Fp"skillfwk.pch"
UPCH2	= -Yu"../../stdafx.h" -Fp"skillfwk.pch"
MKDIRCMD = if not exist ..\build\etc\out mkdir ..\build\etc\out
COPYCMD	= copy $(OUT) ..\build\etc\out
DEL	= del
C_SRCS	= ./src/log.c ./src/time_calc.c
EXAMPLEMINI_SRCS = ./example/example_mini/example_mini.cc\
	./example/example_mini/example_main.cc
PCH_SRC	= stdafx.cc
CPP_SRCS = ./src/syntax.cc ./src/str2bin.cc ./src/philosophereat.cc\
	./src/threadbase.cc ./src/path_tools.cc\
	./example/example_smartptr.cc ./example/example_pathtools.cc\
	./example/example_str2bin.cc ./example/example_staticcheck.cc\
	./example/example_winbase.cc ./example/example_winthunk.cc\
	./example/example_philosophereat.cc ./example/example_timecalc.cc\
	./example/example_log.cc
CPP_SRCS_EX = ./src/win32/win32_mutex.cc
SRCS_OBJS = log.obj time_calc.obj\
	example_mini.obj example_main.obj stdafx.obj syntax.obj str2bin.obj\
	example_smartptr.obj example_str2bin.obj example_staticcheck.obj\
	example_winbase.obj example_winthunk.obj philosophereat.obj\
	threadbase.obj win32_mutex.obj example_philosophereat.obj\
	example_timecalc.obj path_tools.obj example_pathtools.obj example_log.obj

$(OUT): $(SRCS_OBJS)
	$(LINK) -out:$(OUT) $(SRCS_OBJS) $(LINKFLAG)
	$(MT) -manifest $(OUT).manifest -outputresource:$(OUT);1

$(SRCS_OBJS): $(C_SRCS) $(EXAMPLEMINI_SRCS) $(PCH_SRC) $(CPP_SRCS) $(CPP_SRCS_EX)
	$(CC) $(CFLAG4C) $(C_SRCS)
	$(CC) $(CFLAG4CPP) $(EXAMPLEMINI_SRCS)
	$(CC) $(CFLAG4CPP) $(CPCH) $(PCH_SRC)
	$(CC) $(CFLAG4CPP) $(UPCH2) $(CPP_SRCS_EX)
	$(CC) $(CFLAG4CPP) $(UPCH1) $(CPP_SRCS)

copy:
	$(MKDIRCMD)
	$(COPYCMD)

clean:
	$(DEL) *.obj *.pch *.manifest *.exe *.pdb *.ilk *.idb
