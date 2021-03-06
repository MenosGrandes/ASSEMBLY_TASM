/*
   BIOS dla programuj�cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 2-4.  Tworzenie dalekich wska�nik�w "r�cznie"
*/


/*
   U�ycie funkcji 8 przerwania 13h do zademonstrowania
   "r�cznej" konwersji adresu w postaci
   segment:przesuni�cie (offset) do zmiennej.
*/

unsigned char far *getDriveParameterTable( unsigned char whichDrive )
{
union REGS regs;
struct SREGS sregs;
unsigned char far *tablePtr;

/* Pocz�tkowo wska�nik = NULL na wypadek niepowodzenia funkcji 8 */
tablePtr = NULL;

regs.h.ah = 8;                          /* AH = funkcja 8 */
regs.h.dl = whichDrive;                 /* DL = numer nap�du */
int86x( 0x13, &regs, &regs, &sregs );   /* Wywo�anie funkcji BIOS */

if ( regs.x.cflag == 0 )                /* Je�eli OK, */
{                                       /* ES:DI = adres tablicy */
/* Zapisz numer segmentu do m�odszych 16 bit�w */
   (unsigned long)tablePtr = sregs.es;

/* Przesu� numer segmentu do starszych 16 bit�w */
   (unsigned long)tablePtr <<= 16;

/* Zapisz przesuni�cie (offset) do m�odszych 16 bit�w */
   (unsigned long)tablePtr |= regs.x.di;

/* tablePtr zawiera teraz daleki wska�nik
   zwr�cony przez funkcj� BIOS */
   }

/* Zwracamy wska�nik do funkcji wywo�uj�cej (lub NULL
   je�eli funkcja przerwania 13h nie zosta�a poprawnie
   wykonana).
*/
return( tablePtr );
}
