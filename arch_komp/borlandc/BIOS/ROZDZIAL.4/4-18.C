/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-18.  Zmiana palety karty CGA
*/ 


/* Wyb¢r palety dla tryb¢w 4 i 5 karty CGA */

#define MODE0  0    /* zielony/czerwony/br¥zowy */
#define MODE1  1    /* cjan/koarmazynowy/biaˆy */

void setCgaGraphicsPalette( unsigned char newPalette )
{
union REGS regs;

assert( (newPalette == MODE0) || (newPalette == MODE1) );

regs.h.ah = 0x0b;             /* AH = 0x0B - ustaw palet© barw */
regs.h.bh = 1;                /* BH = podfunkcja 0 */
regs.h.bl = newPalette;       /* BL = nowy kolor lub paleta */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
}                             /* Niczego nie zwracamy */
