/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 5-9.  Reset nap©du i sterownika
*/


/*
   Reset Nap©du i sterownika
*/


unsigned char resetDrive( unsigned char drive )
{
union REGS regs;

regs.h.ah = 0;                /* AH = 0 - reset nap©du i sterownika */
regs.h.dl = drive;            /* DL = numer nap©du */
int86( 0x13, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );          /* AH = 0 i Carry = 0 je¾eli OK */
}                             /* AH = kod bˆ©du i Carry = 1 przy bˆ©dzie */
