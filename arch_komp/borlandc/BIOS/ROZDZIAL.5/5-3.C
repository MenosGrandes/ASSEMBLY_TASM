/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 5-3.  Odczytaj sektory z dyskietki
*/ 


/*
   Czytanie sektor¢w z dyskietki.
   Zwraca 0 lub kod bˆ©du przerwania 13h.
*/
unsigned char readDisketteSectors( unsigned char drive,
                                   unsigned char sectorsToRead,
                                   unsigned char head,
                                   unsigned char track,
                                   unsigned char sector,
                                   unsigned char far *bufferPtr )
{
union REGS regs;
struct SREGS sregs;
regs.h.ah = 2;                  /* AH = 2 - czytaj sektory */
regs.h.al = sectorsToRead;      /* AL = liczba czytanych sektor¢w */
regs.x.bx = FP_OFF(bufferPtr);  /* BX = przesuni©cie (offset) bufora */
regs.h.dh = head;               /* DH = gˆowica */
regs.h.dl = drive;              /* DL = numer nap©du (0...3) */
regs.h.ch = track;              /* CH = numer ˜cie¾ki */
regs.h.cl = sector;             /* CL = numer sektora */
sregs.es = FP_SEG(bufferPtr);   /* ES = segment bufora */
int86x( 0x13, &regs,
   &regs, &sregs );             /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );            /* AH = 0 lub kod bˆ©du */
}
