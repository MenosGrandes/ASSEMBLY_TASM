/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-19.  Zmiana palety i koloru obrze¾a dla kart EGA/VGA
*/


/* Ustawienie koloru w palecie kart EGA/VGA */
void setEgaVgaPaletteEntry( unsigned char entry,
                            unsigned char newColor )
{
union REGS regs;

regs.x.ax = 0x1000;           /* AX = funkcja 10h, podfunkcja 0 */
regs.h.bh = newColor;         /* BH = nowa warto˜† koloru */
regs.h.bl = entry;            /* BL = pozycja palety */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
}                             /* Niczego nie zwracamy */


/* Ustawienie koloru obrze¾a dla kart EGA/VGA */
void setEgaVgaBorderColor( unsigned char newColor )
{
union REGS regs;

regs.x.ax = 0x1001;           /* AX = funkcja 10h, podfunkcja 1 */
regs.h.bh = newColor;         /* BH = nowa warto˜† koloru */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
}                             /* Niczego nie zwracamy */
