# Microsoft Visual C++ generated build script - Do not modify

PROJ = WINIRC
DEBUG = 0
PROGTYPE = 0
CALLER = 
ARGS = 
DLLS = 
ORIGIN = MSVCNT
ORIGIN_VER = 1.00
PROJPATH = C:\USERS\DEFAULT\WINIRC\ 
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC = MAIN.C
FIRSTCPP = 
RC = rc
CFLAGS_D_WEXE32 = /nologo /G3 /W3 /Z7 /YX /Od /D "_DEBUG" /D "_X86_" /D "_WINDOWS" /I "RES" /I "." /I "include" /FR /ML /Fp"WINIRC.PCH"
CFLAGS_R_WEXE32 = /nologo /G3 /W3 /YX /O2 /D "NDEBUG" /D "_X86_" /D "_WINDOWS" /I "RES/" /I "RES" /I "." /I "include" /ML /Fp"WINIRC.PCH"
LFLAGS_D_WEXE32 = /NOLOGO /DEBUG /DEBUGTYPE:both /SUBSYSTEM:windows wsock32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib olecli32.lib olesvr32.lib shell32.lib wsock32.lib
LFLAGS_R_WEXE32 = /NOLOGO /SUBSYSTEM:windows user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib olecli32.lib olesvr32.lib shell32.lib wsock32.lib
LFLAGS_D_LIB32 = /NOLOGO
LFLAGS_R_LIB32 = /NOLOGO
LIBS_D_WEXE32 = 
LIBS_R_WEXE32 = 
RCFLAGS32 = 
D_RCDEFINES32 = /d_DEBUG
R_RCDEFINES32 = /dNDEBUG
OBJS_EXT = 
LIBS_EXT = 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_WEXE32)
LFLAGS = $(LFLAGS_D_WEXE32)
LIBS = $(LIBS_D_WEXE32)
LFLAGS_LIB=$(LFLAGS_D_LIB32)
MAPFILE_OPTION = 
RCDEFINES = $(D_RCDEFINES32)
!else
CFLAGS = $(CFLAGS_R_WEXE32)
LFLAGS = $(LFLAGS_R_WEXE32)
LIBS = $(LIBS_R_WEXE32)
MAPFILE_OPTION = 
LFLAGS_LIB=$(LFLAGS_R_LIB32)
RCDEFINES = $(R_RCDEFINES32)
!endif
SBRS = MAIN.SBR \
		MAINWND.SBR \
		UI.SBR \
		CONFIG.SBR \
		EVENTS.SBR \
		PARSER.SBR \
		APP.SBR \
		HANDLERS.SBR \
		UTIL.SBR \
		LOG.SBR


MAIN_DEP =  \
	C:\USERS\DEFAULT\WINIRC\include\winirc.h \
	C:\USERS\DEFAULT\WINIRC\include\config.h \
	C:\USERS\DEFAULT\WINIRC\include\app.h \
	C:\USERS\DEFAULT\WINIRC\include\parser.h \
	C:\USERS\DEFAULT\WINIRC\include\ui.h \
	C:\USERS\DEFAULT\WINIRC\include\log.h


MAINWND_DEP =  \
	C:\USERS\DEFAULT\WINIRC\include\winirc.h \
	C:\USERS\DEFAULT\WINIRC\include\config.h \
	C:\USERS\DEFAULT\WINIRC\include\app.h \
	C:\USERS\DEFAULT\WINIRC\include\parser.h \
	C:\USERS\DEFAULT\WINIRC\include\ui.h \
	C:\USERS\DEFAULT\WINIRC\include\log.h \
	C:\USERS\DEFAULT\WINIRC\include\events.h \
	C:\USERS\DEFAULT\WINIRC\res\resource.h


UI_DEP =  \
	C:\USERS\DEFAULT\WINIRC\include\winirc.h \
	C:\USERS\DEFAULT\WINIRC\include\config.h \
	C:\USERS\DEFAULT\WINIRC\include\app.h \
	C:\USERS\DEFAULT\WINIRC\include\parser.h \
	C:\USERS\DEFAULT\WINIRC\include\ui.h \
	C:\USERS\DEFAULT\WINIRC\include\log.h \
	C:\USERS\DEFAULT\WINIRC\include\events.h


CONFIG_DEP =  \
	C:\USERS\DEFAULT\WINIRC\include\config.h


EVENTS_DEP =  \
	C:\USERS\DEFAULT\WINIRC\include\winirc.h \
	C:\USERS\DEFAULT\WINIRC\include\config.h \
	C:\USERS\DEFAULT\WINIRC\include\app.h \
	C:\USERS\DEFAULT\WINIRC\include\parser.h \
	C:\USERS\DEFAULT\WINIRC\include\ui.h \
	C:\USERS\DEFAULT\WINIRC\include\log.h \
	C:\USERS\DEFAULT\WINIRC\res\resource.h


MAINMENU_RCDEP =  \
	c:\users\default\winirc\res\resource.h


PARSER_DEP =  \
	C:\USERS\DEFAULT\WINIRC\include\parser.h \
	C:\USERS\DEFAULT\WINIRC\include\app.h


APP_DEP =  \
	C:\USERS\DEFAULT\WINIRC\include\winirc.h \
	C:\USERS\DEFAULT\WINIRC\include\config.h \
	C:\USERS\DEFAULT\WINIRC\include\app.h \
	C:\USERS\DEFAULT\WINIRC\include\parser.h \
	C:\USERS\DEFAULT\WINIRC\include\ui.h \
	C:\USERS\DEFAULT\WINIRC\include\log.h \
	C:\USERS\DEFAULT\WINIRC\include\handlers.h


HANDLERS_DEP =  \
	C:\USERS\DEFAULT\WINIRC\include\winirc.h \
	C:\USERS\DEFAULT\WINIRC\include\config.h \
	C:\USERS\DEFAULT\WINIRC\include\app.h \
	C:\USERS\DEFAULT\WINIRC\include\parser.h \
	C:\USERS\DEFAULT\WINIRC\include\ui.h \
	C:\USERS\DEFAULT\WINIRC\include\log.h \
	C:\USERS\DEFAULT\WINIRC\include\handlers.h


UTIL_DEP = 

LOG_DEP =  \
	C:\USERS\DEFAULT\WINIRC\include\winirc.h \
	C:\USERS\DEFAULT\WINIRC\include\config.h \
	C:\USERS\DEFAULT\WINIRC\include\app.h \
	C:\USERS\DEFAULT\WINIRC\include\parser.h \
	C:\USERS\DEFAULT\WINIRC\include\ui.h \
	C:\USERS\DEFAULT\WINIRC\include\log.h


all:	$(PROJ).EXE

MAIN.OBJ:	SRC\MAIN.C $(MAIN_DEP)
	$(CC) $(CFLAGS) $(CCREATEPCHFLAG) /c SRC\MAIN.C

MAINWND.OBJ:	SRC\UI\MAINWND.C $(MAINWND_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c SRC\UI\MAINWND.C

UI.OBJ:	SRC\UI\UI.C $(UI_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c SRC\UI\UI.C

CONFIG.OBJ:	SRC\CONFIG\CONFIG.C $(CONFIG_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c SRC\CONFIG\CONFIG.C

EVENTS.OBJ:	SRC\UI\EVENTS.C $(EVENTS_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c SRC\UI\EVENTS.C

MAINMENU.RES:	RES\MAINMENU.RC $(MAINMENU_RCDEP)
	$(RC) $(RCFLAGS32) $(RCDEFINES) -r -FoC:\USERS\DEFAULT\WINIRC\MAINMENU.RES RES\MAINMENU.RC

PARSER.OBJ:	SRC\APP\PARSER.C $(PARSER_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c SRC\APP\PARSER.C

APP.OBJ:	SRC\APP\APP.C $(APP_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c SRC\APP\APP.C

HANDLERS.OBJ:	SRC\APP\HANDLERS.C $(HANDLERS_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c SRC\APP\HANDLERS.C

UTIL.OBJ:	SRC\UTIL.C $(UTIL_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c SRC\UTIL.C

LOG.OBJ:	SRC\APP\LOG.C $(LOG_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c SRC\APP\LOG.C


$(PROJ).EXE:	MAINMENU.RES

$(PROJ).EXE:	MAIN.OBJ MAINWND.OBJ UI.OBJ CONFIG.OBJ EVENTS.OBJ PARSER.OBJ APP.OBJ \
	HANDLERS.OBJ UTIL.OBJ LOG.OBJ $(OBJS_EXT) $(LIBS_EXT)
	echo >NUL @<<$(PROJ).CRF
MAIN.OBJ 
MAINWND.OBJ 
UI.OBJ 
CONFIG.OBJ 
EVENTS.OBJ 
PARSER.OBJ 
APP.OBJ 
HANDLERS.OBJ 
UTIL.OBJ 
LOG.OBJ 
$(OBJS_EXT)
-OUT:$(PROJ).EXE
$(MAPFILE_OPTION)
MAINMENU.RES
$(LIBS)
$(LIBS_EXT)
$(DEFFILE_OPTION) -implib:$(PROJ).lib
<<
	link $(LFLAGS) @$(PROJ).CRF

run: $(PROJ).EXE
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
