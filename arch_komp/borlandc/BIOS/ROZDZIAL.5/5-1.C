/* 
   BIOS dla programuj�cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 5-1.  Odczytaj parametry stacji dyskietek
*/


/*
   Odczyt parametr�w nap�du dyskietek.
   Zwraca 0 przy udanej operacji lub kod b��du przerwania 13h.
*/
unsigned char readDisketteParameters( unsigned char drive,
                                      unsigned char *maxTrack,
                                      unsigned char *maxSector,
                                      unsigned char *maxHead )
{
union REGS regs;
regs.h.ah = 8;                  /* AH = 8 - podaj parametry nap�du */
regs.h.dl = drive;              /* DL = numer nap�du (0...3) */
int86(0x13,&regs,&regs );       /* Wywo�anie funkcji BIOS */
if ( regs.x.cflag )             /* Je�eli Carry = 1 */
   return( regs.h.ah );         /* AH = kod b��du */

*maxSector = (regs.h.cl & 63);  /* CL bity 5-0= sektory na �cie�ce */
*maxTrack = regs.h.ch;          /* CH = maksymalny numer �cie�ki */
*maxHead = regs.h.dh;           /* DH = liczba g�owic */
return(0);                      /* Operacja udana */
}
