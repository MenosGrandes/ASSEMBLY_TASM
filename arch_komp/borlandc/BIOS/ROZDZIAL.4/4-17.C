/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-17.  Zmiana koloru tˆa/obrze¾a dla karty CGA
*/ 


/*
   Ustawia dla kart CGA:
      - kolor obrze¾a ekranu dla tryb¢w tekstowych
      - kolor tˆa dla tryb¢w graficznych 320x200
      - kolor pi¢ra dla trybu graficznego 640x200
*/

void setCgaColor( unsigned char newColorValue )
{
union REGS regs;

regs.h.ah = 0x0b;             /* AH = 0x0B - ustaw palet© barw */
regs.h.bh = 0;                /* BH = podfunkcja 0 */
regs.h.bl = newColorValue;    /* BL = nowy kolor lub paleta */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
}                             /* Niczego nie zwracamy */
