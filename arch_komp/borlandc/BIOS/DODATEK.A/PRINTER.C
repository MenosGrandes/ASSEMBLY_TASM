/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Dodatek A.  Funkcje obsˆugi drukarki
*/


/* Przerwanie 05h - Obsˆuga klawisza Print Screen */

#define pStat (unsigned char far *)0x00500000
unsigned char printScreen( void )
{
union REGS regs;

int86( 0x05, &regs, &regs );     /* Wywoˆanie funkcji BIOS */
return( *pStat );                /* Zwracamy stan z adresu 0x00500000 */
}



/* Przerwanie 17h / 00h - Drukuj znak */

unsigned char printChar( unsigned int printer, 
                         unsigned char chToPrint )
{
union REGS regs;

regs.h.ah = 0;                /* AH = 0 - drukuj znak */
regs.h.al = chToPrint;        /* AL = znak */
regs.x.dx = printer;          /* DX = numer drukarki */
int86( 0x17, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );          /* AH = stan drukarki */
}



/* Przerwanie 17h / 01h - Inicjalizacja portu drukarki */

unsigned char initPrinterPort( unsigned int port )
{
union REGS regs;

regs.h.ah = 1;                /* AH = 1 - inicjalizacja portu drukarki */
regs.x.dx = port;             /* DX = numer drukarki */
int86( 0x17, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );          /* AH = stan drukarki */
}



/* Przerwanie 17h / 02h - Pobierz sˆowo stanu drukarki */

unsigned char getPrinterStatus( unsigned int printer )
{
union REGS regs;

regs.h.ah = 2;                /* AH = 2 - czytaj sˆowo stanu */
regs.x.dx = printer;          /* DX = numer drukarki */
int86( 0x17, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );          /* AH = stan drukarki */
}

