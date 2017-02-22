/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 8-2.  Odczytanie stanu portu drukarki
*/


unsigned char getPrinterStatus( int whichPrinter )
{
union REGS regs;

regs.h.ah = 2;                /* AH = 2 - pobranie sˆowa stanu drukarki */
regs.x.dx = whichPrinter;     /* DX = drukarka (0, 1, 2) */
int86( 0x17, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );          /* AH = sˆowo stanu */
}
