/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 5-4.  Odczytaj sektory z dysku twardego
*/ 


/*
   Czytanie sektor¢w z dysku twardego.
   Zwraca 0 lub kod bˆ©du przerwania 13h.
*/
unsigned char readHardDiskSectors( unsigned char drive,
                                   unsigned char sectorsToRead,
                                   unsigned char head,
                                   unsigned int cylinder,
                                   unsigned char sector,
                                   unsigned char far *bufferPtr )

{
union REGS regs;
struct SREGS sregs;
regs.h.ah = 2;                  /* AH = 2 - czytaj sektory */
regs.h.al = sectorsToRead;      /* AL = liczba odczytywanych sektor¢w */
regs.x.bx = FP_OFF(bufferPtr);  /* BX = przesuni©cie (offset) bufora */
regs.h.dh = head;               /* DH = gˆowica */
regs.h.dl = drive;              /* DL = nap©d (0...3 + 80h) */
regs.h.ch = cylinder;           /* CH = mˆodsze 8 bit¢w nr cylindra */
regs.h.cl = ( cylinder >> 8 );  /* Starsze 2 bity nr cylindra */
regs.h.cl <<= 6;                /* Przesuni©cie na bity 7 i 6 */
regs.h.cl |= ( sector & 63 );   /* CL mˆodsze 6 bit¢w = numer sektora */
sregs.es = FP_SEG(bufferPtr);   /* ES = segment bufora */
int86x( 0x13, &regs,
   &regs, &sregs );             /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );            /* AH = 0 lub kod bˆ©du */
}
