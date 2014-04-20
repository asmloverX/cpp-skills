BIN	= test.exe
CC	= cl -c -nologo 
CFLAG	= -W3 -Od -MDd -Gm -Zi -Fd"vc.pdb" -EHsc -D_CRT_SECURE_NO_DEPRECATE
LINK	= link -nologo
LINKFLAG = -DEBUG -PDB:"test.pdb" -manifest -manifestfile:$(BIN).manifest\
	-manifestuac:no
MT	= mt -nologo
MKDIR	= if not exist .\bin mkdir .\bin
COPY	= copy $(BIN) .\bin
DEL	= del

SRCS	= ./unit_mini/unit_main.cc\
	./unit_mini/unit_color.cc\
	./unit_mini/unit_mini.cc\
	test.cc
OBJS = unit_main.obj unit_color.obj unit_mini.obj test.obj


$(BIN): $(OBJS)
	$(LINK) -out:$(BIN) $(OBJS) $(LINKFLAG)
	$(MT) -manifest $(BIN).manifest -outputresource:$(BIN);1

$(OBJS): $(SRCS)
	$(CC) $(CFLAG) $(SRCS)

copy:
	$(MKDIR)
	$(COPY)

clean:
	$(DEL) *.obj *.exe *.manifest *.pdb *.ilk *.idb
