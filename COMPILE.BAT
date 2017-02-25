@echo OFF
set arg1=%1
CLS
DEL %arg1%.OBJ
DEL %arg1%.MAP
DEL %arg1%.EXE
DEL %arg1%.COM

C:\ARCH_K~1\BORLANDC\BIN\TASM     %arg1%.ASM
IF "%2" == "COM" GOTO compileCOM 
IF "%2" == "EXE" GOTO compileEXE

:compileCOM
C:\ARCH_K~1\BORLANDC\BIN\TLINK /t %arg1%.OBJ
GOTO END
:compileEXE
C:\ARCH_K~1\BORLANDC\BIN\TLINK /v %arg1%.OBJ
GOTO END

:END
