/*
   BIOS dla programujących w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 2-1.  Użycie FP_SEG i FP_OFF ze zmiennymi wskaźnikowymi
*/

char string[] = "Błąd drukarki!";
char far *pCh = &string[0];
unsigned int segment, offset;

segment = FP_SEG( pCh );
offset  = FP_OFF( pCh );
