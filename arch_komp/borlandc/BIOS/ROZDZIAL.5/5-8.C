/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 5-8.  Zapisanie sektor¢w dysku twardego
*/ 


/*
   Zapisanie sektor¢w na dyskietce.
   Zwraca 0 lub kod bˆ©du przerwania 13h.
*/

unsigned char writeDisketteSectors( unsigned char drive, 
                                    unsigned char sectorsToWrite, 
                                    unsigned char head, 
                                    unsigned char track, 
                                    unsigned char sector, 
                                    unsigned char far *bufferPtr )
{
union REGS regs;
struct SREGS sregs;

regs.h.ah = 3;                 /* AH = 3 - zapisz sektory */
regs.h.al = sectorsToWrite;    /* AL = liczba sektor¢w do zapisania */
regs.x.bx = FP_OFF(bufferPtr); /* BX = przesuni©cie (offset) bufora */
regs.h.dh = head;              /* DH = gˆowica */
regs.h.dl = drive;             /* DL = numer nap©du (0...3) */
regs.h.ch = track;             /* CH = numer ˜cie¾ki */
regs.h.cl = sector;            /* CL = numer sektora */
sregs.es  = FP_SEG(bufferPtr); /* ES = segment bufora */
int86x( 0x13, &regs, 
        &regs, &sregs );       /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );           /* AH = 0 lub kod bˆ©du */
}
