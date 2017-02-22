/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 8-4.  drukowanie jednego znaku
*/


unsigned char printChar( unsigned int whichPrinter, 
                         unsigned char chToPrint )
{
union REGS regs;

regs.h.ah = 0;                /* AH = 0 - drukuj znak */
regs.h.al = chToPrint;        /* AL = znak */
regs.x.dx = whichPrinter;     /* DX = drukarka (0, 1, 2) */
int86( 0x17, &regs, &regs );  /* Wywoˆanie funkcji BIOS */

return( regs.h.ah );          /* AH = sˆowo stanu */
}
