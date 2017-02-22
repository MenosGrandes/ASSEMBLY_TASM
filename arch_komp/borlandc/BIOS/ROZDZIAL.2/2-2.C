/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 2-2.  U¾ycie FP_SEG i FP_OFF ze staˆymi adresami
                 (tylko Borland C)
*/

char string[] = "Bˆ¥d drukarki!";
unsigned int segment, offset;

segment = FP_SEG( string );
offset  = FP_OFF( string );
