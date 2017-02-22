/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 2-1.  U¾ycie FP_SEG i FP_OFF ze zmiennymi wska«nikowymi
*/

char string[] = "Bˆ¥d drukarki!";
char far *pCh = &string[0];
unsigned int segment, offset;

segment = FP_SEG( pCh );
offset  = FP_OFF( pCh );
