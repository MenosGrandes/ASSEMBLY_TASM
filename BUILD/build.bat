echo off 

DEL *.OBJ
DEL *.MAP
DEL *.EXE
DEL *.COM
TASM /zi %1.asm
IF "%2"=="COM" goto COM_FILE
IF "%2"=="EXE" goto EXE_FILE 
:COM_FILE
TLINK /t /l %1.obj 
TD %1.COM
goto END
:EXE_FILE
TLINK /l %1.obj 
TD %1.EXE

:END

