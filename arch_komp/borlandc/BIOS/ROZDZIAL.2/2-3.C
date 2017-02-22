/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 2-3.  Tworzenie dalekich wska«nik¢w przy u¾yciu
                 makrodefinicji MK_FP
*/


/*
   U¾ycie funkcji 8 przerwania 13h do zademonstrowania
   konwersji adresu w postaci segment:przesuni©cie (offset)
   do zmiennej za pomoc¥ makrodefinicji MK_FP dost©pnej
   w kompilatorach firmy Borland.
*/

unsigned char far *getDriveParameterTable( unsigned char whichDrive )
{
union REGS regs;
struct SREGS sregs;
unsigned char far *tablePtr;

/* Pocz¥tkowo wska«nik = NULL na wypadek niepowodzenia funkcji 8 */
tablePtr = NULL;

regs.h.ah = 8;                          /* AH = funkcja 8 */
regs.h.dl = whichDrive;                 /* DL = numer nap©du */
int86x( 0x13, &regs, &regs, &sregs );   /* Wywoˆanie funkcji BIOS */

/* 
   Je¾eli operacja powiodˆa si©, to sregs.es zawiera
   numer segmentu, a regs.x.di zawiera przesuni©cie (offset).
   MK_FP tworzy na ich podstawie daleki wska«nik. Pierwszym
   argumentem jest numer segmentu, drugim przesuni©cie.
*/

if ( regs.x.cflag == 0 )                /* Je¾eli OK, */
   tablePtr = MK_FP( sregs.es, regs.x.di );

/* Zwracamy wska«nik do funkcji wywoˆuj¥cej (lub NULL
   je¾eli funkcja przerwania 13h nie zostaˆa poprawnie
   wykonana).
*/
return( tablePtr );
}
